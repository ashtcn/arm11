#include "execute.h"

int condition(system_state_t *machine) {
  char flags = machine->registers[CPSR] >> (WORD_SIZE - 4); // Want first 4 bits
  switch(machine->decoded_instruction->cond) {
    case EQ:
      return (flags & Z);
    case NE:
      return !(flags & Z);
    case GE:
      return (flags & V) == ((flags & N) >> 3);
    case LT:
      return (flags & V) != ((flags & N) >> 3);
    case GT:
      return !(flags & Z) && ((flags & V) == ((flags & N) >> 3));
    case LE:
      return (flags & Z) || ((flags & V) != ((flags & N) >> 3));
    case AL:
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
      case NUL:
      default:
        fprintf(stderr, "Incorrect instruction type at PC: %x\n", machine->registers[PC] - 8);
        exit_program(machine);
        break;
    }
  }
}

void execute_dpi(system_state_t *machine) {
  word_t op2;
  word_t shift_amount;
  bool shifter_carry = 0;
  instruction_t *instruction = machine->decoded_instruction;

  if (!instruction->flag_0) {
    op2 = machine->registers[instruction->rm];

    if (instruction->rs == -1) {
      shift_amount = instruction->shift_amount;
    } else {
      shift_amount = machine->registers[instruction->rs];
    }
  } else {
    op2 = instruction->immediate_value;
    shift_amount = instruction->shift_amount;
  }

  value_carry_t *shifter_out = shifter(instruction->shift_type, shift_amount, op2);
  op2 = shifter_out->value;
  shifter_carry = shifter_out->carry;
  free(shifter_out);

  word_t flags = 0;
  word_t result;

  switch (instruction->operation) {
    case AND:
    case TST:
      result = machine->registers[instruction->rn] & op2;
      flags = C * shifter_carry;
      break;
    case EOR:
    case TEQ:
      result = machine->registers[instruction->rn] ^ op2;
      flags = C * shifter_carry;
      break;
    case SUB:
    case CMP:
      result = machine->registers[instruction->rn] + negate(op2);
      flags = C * ((is_negative(machine->registers[instruction->rn]) == is_negative(op2)) !=  is_negative(result));
      break;
    case RSB:
      result = op2 + negate(machine->registers[instruction->rn]);
      flags = C * ((is_negative(machine->registers[instruction->rn]) == is_negative(op2)) !=  is_negative(result));
      break;
    case ADD:
      result = machine->registers[instruction->rn] + op2;
      flags = C * ((is_negative(machine->registers[instruction->rn]) == is_negative(op2)) !=  is_negative(result));
      break;
    case ORR:
      result = machine->registers[instruction->rn] | op2;
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

  if (!(instruction->operation == TST
    || instruction->operation == TEQ
    || instruction->operation == CMP)) {
    machine->registers[instruction->rd] = result;
  }

  // If set flags
  if (instruction->flag_1) {
    machine->registers[CPSR] &= MASK_FIRST_4;
    machine->registers[CPSR] |= (flags << (WORD_SIZE - 4)); // Want first 4 bits
  }
}

void execute_mul(system_state_t *machine) {
  word_t result;
  instruction_t *instruction = machine->decoded_instruction;
  result = machine->registers[instruction->rm] * machine->registers[instruction->rs];

  if (instruction->flag_0) {
    result += machine->registers[instruction->rn];
  }

  machine->registers[instruction->rd] = result;
  if (instruction->flag_1) {
    machine->registers[CPSR] &= MASK_FIRST_4;
    machine->registers[CPSR] |= (N * is_negative(result)) << (WORD_SIZE - 4);
    machine->registers[CPSR] |= (Z * (result == 0)) << (WORD_SIZE - 4);
  }
}

void execute_sdt(system_state_t *machine) {
  uint32_t address;
  word_t offset;
  word_t shift_amount;
  instruction_t *instruction = machine->decoded_instruction;

  // Immediate operand or not
  if (instruction->flag_0) {
    if (instruction->rs == -1) {
      shift_amount = instruction->shift_amount;
    } else {
      shift_amount = machine->registers[instruction->rs];
    }
    value_carry_t *shifter_out = shifter(instruction->shift_type, shift_amount, machine->registers[instruction->rm]);//How do you like my long line Jordan????????
    offset = shifter_out->value;
    free(shifter_out);
  } else {
    offset = instruction->immediate_value;
  }

  // Negative or positive
  if (!instruction->flag_2) {
    offset = negate(offset);
  }

  // Post or pre indexing
  if (!instruction->flag_1) {
    address = machine->registers[instruction->rn];
    machine->registers[instruction->rn] = address + offset;
  } else {
    address = machine->registers[instruction->rn] + offset;
  }

  // Load or save
  if (instruction->flag_3) {
    machine->registers[instruction->rd] = get_word(machine, address);
  } else {
    set_word(machine, address, machine->registers[instruction->rd]);
  }
}

void execute_branch(system_state_t *machine) {
  word_t offset = machine->decoded_instruction->immediate_value;
  machine->has_fetched_instruction = false;
  machine->registers[PC] += twos_complement_to_long(offset);
}
