#include "toolbox.h"

/**
 * @brief Loads a binary file into the memory.
 *
 * Writes the contents of the provided binary object code file to the memory,
 * starting at the provided location. Returns an error message and exits if the
 * file cannot be opened or cannot be read.
 * @param fname The filename conataining object code to be loaded.
 * @param memory A pointer to the first byte of memory to be written to.
 */
 void load_file(char *fname, byte_t *memory) {
   // Try to open the file
   FILE *file = fopen(fname, "rb");
   if (file == NULL) {
     perror("Error in opening object code file.");
     exit(EXIT_FAILURE);
   }
   // Try to read all lines into the memory
   fread(memory, NUM_ADDRESSES, 1, file);
   if(ferror(file)) {
     perror("Error in reading from object code file.");
     exit(EXIT_FAILURE);
   }
   // Close the file
   fclose(file);
}

void exit_program(system_state_t *machine) {
  print_system_state(machine);
  free(machine->decoded_instruction);
  free(machine);
  exit(EXIT_FAILURE);
}

void print_array(void *p, size_t bytes_to_print) {
  byte_t *memory = (byte_t *) p;
  for (size_t i = 0; i < bytes_to_print; i++) {
    printf("%x", memory[i]);

    // New line at every 4 bytes
    if (i % 4 == 3) {
      printf("\n");
    }
  }
  printf("\n");
}

word_t get_word(system_state_t *machine, address_t mem_address) {
  word_t value = 0;
  for (size_t i = 0; i < 4; i++) {
    value |= ((word_t) machine->memory[mem_address + i]) << (i * 8);
  }
  return value;
}

void set_word(system_state_t *machine, address_t mem_address, word_t word) {
  for (size_t i = 0; i < 4; i++) {
    machine->memory[mem_address + i] = (byte_t) (word & 0xF);
    word >>= 4;
  }
}

word_t negate(word_t value) {
  return (~value) + 1;
}

bool is_negative(word_t value) {
  return value >> 31;
}

word_t absolute(word_t value) {
  if (is_negative(value)) {
    return negate(value);
  }
  return value;
}


void print_system_state(system_state_t *machine) {
  print_registers(machine);
  print_memory(machine);
}

void print_registers(system_state_t *machine) {
  printf("Register State:\n");
  for (uint8_t i = 0; i < NUM_REGISTERS; ++i) {
    word_t value = machine->registers[i];
    printf("Register no: %d, ", i);
    print_value(value);
  }
}

void print_memory(system_state_t *machine) {
  printf("Memory state:\n");
  for (uint32_t i = 0; i < NUM_ADDRESSES; i += 4) {
    word_t value = get_word(machine, i);
    if (value) {
      printf("Memory address: %d, ", i);
      print_value(value);
    }
  }
}

void print_value(word_t value) {
  printf("Value: ");
  print_binary_value(value);
  printf(" (0x%x) (%ld)\n", value, twos_complement_to_long(value));
}

long twos_complement_to_long(word_t value) {
  long result = absolute(value);
  if (is_negative(value)) {
    result *= -1;
  }
  return result;
}

void print_binary_value(word_t value) {
  for (int i = 0; i < WORD_SIZE; ++i) {
    printf("%u", value >> (WORD_SIZE - 1));
    value <<= 1;
  }
}

value_carry_t *shifter(shift_t type, word_t shift_amount, word_t value) {
  value_carry_t *result = malloc(sizeof(value_carry_t));
  if (!result) {
    fprintf(stderr, "Unable to allocate memory for result of shifter");
    exit(EXIT_FAILURE);
  }
  switch(type) {
    case lsl:
      result->value = value << shift_amount;
      result->carry = (value >> (WORD_SIZE - shift_amount)) & 0x1;
      break;
    case lsr:
      result->value = value >> shift_amount;
      result->carry = value << (WORD_SIZE - shift_amount) & 0x1;
      break;
    case asr:
      result->value = (value >> shift_amount) | ((value >> 31) ? ~((1L << (3L - shift_amount)) - 1L) : 0L);
      result->carry = value << (WORD_SIZE - shift_amount) & 0x1;
      break;
    case ror:
      result->value = (value << (WORD_SIZE - shift_amount)) | (value >> shift_amount);
      result->carry = value << (WORD_SIZE - shift_amount) & 0x1;
      break;
    default:
      fprintf(stderr, "Unknown shift type: %u", type);
      exit(EXIT_FAILURE);
  }
  return result;
}

void decode_instruction(system_state_t *machine) {
  // PRE: Instruction is not all 0
  word_t fetched = machine->fetched_instruction;
  instruction_t *instruction = machine->decoded_instruction;
  instruction->cond = fetched >> (WORD_SIZE - 4);

  // No longer consider Cond, so remove it
  fetched &= MASK_FIRST_4;

  if ((fetched >> (WORD_SIZE - 8)) == 0xA) { // Branch iff 1010 (0xA)
    // Branch
    branch(machine);
  } else if ((fetched >> (WORD_SIZE - 6)) == 0x1) {
    // Single Data Transfer
    multiply(machine);
  } else if (!(fetched >> 22) && (((fetched >> 4) & 0xF) == 0x9)) {
    //Multiply
    single_data_transfer(machine);
  } else if (!(fetched >> (WORD_SIZE - 6))) {
    // Data Processing
    data_processing(machine);
  } else {
    // Unknown instruction
    fprintf(stderr, "Unknown instruction, PC: %u", machine->registers[PC]);
    exit_program(machine);
  }
}

void branch(system_state_t *machine) {
  machine->decoded_instruction->immediate_value = machine->fetched_instruction
                                                & 0xFFFFFF; // First 24 bits
}

void multiply(system_state_t *machine) {
  instruction_t *instruction = machine->decoded_instruction;
  word_t fetched = machine->fetched_instruction;

  instruction->type = MUL;
  instruction->flag_0 = (fetched >> 21) & 0x1;
  instruction->flag_1 = (fetched >> 20) & 0x1;
  instruction->rn = (fetched >> 12) & 0x4;
  instruction->rd = (fetched >> 16) & 0x4;
  instruction->rs = (fetched >> 8) & 0x4;
  instruction->rm = fetched & 0x4;
}

void single_data_transfer(system_state_t *machine) {

}

void data_processing(system_state_t *machine) {
  instruction_t *instruction = machine->decoded_instruction;
  word_t fetched = machine->fetched_instruction;

  instruction->type = DPI;
  instruction->flag_0 = (fetched >> 25) & 0x1;
  instruction->flag_1 = (fetched >> 20) & 0x1;
  instruction->operation = (fetched >> 21) & 0x4;
  instruction->rn = (fetched >> 16) & 0x4;
  instruction->rd = (fetched >> 12) & 0x4;
  instruction->immediate_value = fetched & 0xC;
}

//Execute
int condition(system_state_t *machine) {
  char flags = machine->registers[CPSR] >> (WORD_SIZE - 4); // Want first 4 bits
  switch(machine->decoded_instruction->cond) {
    case eq:
      return (flags & Z);
    case ne:
      return !(flags & Z);
    case ge:
      return (flags & V) == ((flags & N) >> 3);
    case lt:
      return (flags & V) != ((flags & N) >> 3);
    case gt:
      return !(flags & Z) && ((flags & V) == ((flags & N) >> 3));
    case le:
      return (flags & Z) || ((flags & V) != ((flags & N) >> 3));
    case al:
      return 1;
    default:
      fprintf(stderr, "Incorrect cond flag, PC: %u", machine->registers[PC]);
      exit_program(machine);
      return 0;
  }
}

void execute(system_state_t *machine) {
  if (condition(machine)) {
    switch (machine->decoded_instruction->type) {
      case DPI:
        execute_dpi(machine);
        break;
      case MUL:
        execute_mul(machine);
        break;
      case SDT:
        execute_sdt(machine);
        break;
      case BRA:
        execute_branch(machine);
        break;
      case ZER:
        break;
      case NUL:
        break;
    }
  }
}

void execute_dpi(system_state_t *machine) {
  word_t op2;
  word_t shift_ammount;
  shift_t shift_type;
  bool shifter_carry = 0;
  if (!machine->decoded_instruction->flag_0) {
    op2 = machine->registers[machine->decoded_instruction->rm];
    shift_type = machine->decoded_instruction->shift_type;
    if (machine->decoded_instruction->rs == -1) {
      shift_ammount = machine->decoded_instruction->immediate_value;
    } else {
      shift_ammount = machine->registers[machine->decoded_instruction->rs];
    }
  } else {
    op2 = machine->decoded_instruction->immediate_value;
    shift_ammount = machine->decoded_instruction->shift_amount;
    shift_type = ror;
  }

  value_carry_t *shifter_out = shifter(shift_type, op2, shift_ammount);
  op2 = shifter_out->value;
  shifter_carry = shifter_out->carry;
  free (shifter_out);

  word_t flags = 0;
  word_t result;
  switch (machine->decoded_instruction->operation) {
    case AND:
    case TST:
      result = machine->registers[machine->decoded_instruction->rn] & op2;
      flags = C * shifter_carry;
      break;
    case EOR:
    case TEQ:
      result = machine->registers[machine->decoded_instruction->rn] ^ op2;
      flags = C * shifter_carry;
      break;
    case SUB:
    case CMP:
      result = machine->registers[machine->decoded_instruction->rn] + negate(op2);
      flags = C * (((uint64_t) (machine->registers[machine->decoded_instruction->rn] + negate(op2))) > result);
      break;
    case RSB:
      result = op2 + negate(machine->registers[machine->decoded_instruction->rn]);
      flags = C * (((uint64_t) (negate(machine->registers[machine->decoded_instruction->rn]) + op2)) > result);
      break;
    case ADD:
      result = machine->registers[machine->decoded_instruction->rn] + op2;
      flags = C * (((uint64_t) (machine->registers[machine->decoded_instruction->rn] + op2)) > result);
      break;
    case ORR:
      result = machine->registers[machine->decoded_instruction->rn] | op2;
      flags = C * shifter_carry;
      break;
    case MOV:
      result = op2;
      flags = C * shifter_carry;
      break;
    default:
      fprintf(stderr, "Unknown opcode at PC: %u", machine->registers[PC] - 0x40); // Is this our error message?
      exit_program(machine);
      break;
  }

  flags |= (N * is_negative(result));
  flags |= (Z * (result == 0));

  if (!(machine->decoded_instruction->operation == TST || machine->decoded_instruction->operation == TEQ || machine->decoded_instruction->operation == CMP)) {
    machine->registers[machine->decoded_instruction->rd] = result;
  }

  // If set flags
  if (machine->decoded_instruction->flag_1) {
    machine->registers[CPSR] &= MASK_FIRST_4;
    machine->registers[CPSR] |= (flags << (WORD_SIZE - 4)); // Want first 4 bits
  }
}

void execute_mul(system_state_t *machine) {
  word_t result;
  result = machine->registers[machine->decoded_instruction->rm] * machine->registers[machine->decoded_instruction->rs];

  if (machine->decoded_instruction->flag_0) {
    result += machine->registers[machine->decoded_instruction->rn];
  }

  machine->registers[machine->decoded_instruction->rd] = result;
  if (machine->decoded_instruction->flag_1) {
    machine->registers[CPSR] &= MASK_FIRST_4;
    machine->registers[CPSR] |= (N * is_negative(result)) << (WORD_SIZE - 4);
    machine->registers[CPSR] |= (Z * (result == 0)) << (WORD_SIZE - 4);
  }
}

void execute_sdt(system_state_t *machine) {
  address_t address;
  word_t offset;
  word_t shift_ammount;
  if (machine->decoded_instruction->flag_0) {//Immediate or not
    if (machine->decoded_instruction->rs == -1) {
      shift_ammount = machine->decoded_instruction->immediate_value;
    } else {
      shift_ammount = machine->registers[machine->decoded_instruction->rs];
    }
    value_carry_t *shifter_out = shifter(machine->decoded_instruction->shift_type, shift_ammount, machine->registers[machine->decoded_instruction->rm]);//How do you like my long line Jordan????????
    offset = shifter_out->value;
  } else {
    offset = machine->decoded_instruction->immediate_value;
  }

  if (!machine->decoded_instruction->flag_2) {//Negative or positive
    offset = negate(offset);
  }

  if (machine->decoded_instruction->flag_1) {//Post or pre indexing
    address = machine->registers[machine->decoded_instruction->rn];
    machine->registers[machine->decoded_instruction->rn] = address + offset;
  } else {
    address = machine->registers[machine->decoded_instruction->rn] + offset;
  }

  if (machine->decoded_instruction->flag_3) {//Load or save
    machine->registers[machine->decoded_instruction->rd] = get_word(machine, address);
  } else {
    set_word(machine, address, machine->registers[machine->decoded_instruction->rd]);
  }
}

void execute_branch(system_state_t *machine) {
  word_t offset = machine->decoded_instruction->immediate_value;
  machine->decoded_instruction->type = NUL;
  machine->has_fetched_instruction = 0;
  machine->registers[PC] += twos_complement_to_long(offset);
}
