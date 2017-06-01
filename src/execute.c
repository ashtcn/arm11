/** Functions for the execute cycle. */

#include "execute.h"

/**
 * @brief Returns whether the condition is met.
 *
 * Returns true if and only if the condition required by the current decoded
 * instruction is met by the current state of the flags register (CPSR).
 * @param machine The current system state.
 * @returns Whether condition is met.
 */
int condition(system_state_t *machine) {
  // Want the first 4 bits
  char flags = machine->registers[CPSR] >> (WORD_SIZE - 4);

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

/**
 * @brief Runs one execute cycle.
 *
 * Executes the current decoded instruction if the condition is met, and updates
 * the system state accordingly.
 * A pre-condition is that the instruction must not be type NUL or ZER.
 * @param machine The current system state.
 */
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
        fprintf(stderr, "Incorrect instruction type at PC: %x\n",
                machine->registers[PC] - 8);
        exit_program(machine);
        break;
    }
  }
}

/**
 * @brief Executes a data processing instruction.
 *
 * @param machine The current system state.
 */
void execute_dpi(system_state_t *machine) {
  word_t op2;
  word_t shift_amount;
  bool shifter_carry = 0;
  instruction_t *instruction = machine->decoded_instruction;

  // Get the second operand
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

  // Shift the second operand
  value_carry_t *shifter_out = shifter(instruction->shift_type,
                                       shift_amount, op2);
  op2 = shifter_out->value;
  shifter_carry = shifter_out->carry;
  free(shifter_out);

  word_t flags = 0;
  word_t result;

  // Compute the result and carry flag depending on the opcode
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
      flags = C * ((is_negative(machine->registers[instruction->rn])
              == is_negative(op2)) !=  is_negative(result));
      break;
    case RSB:
      result = op2 + negate(machine->registers[instruction->rn]);
      flags = C * ((is_negative(machine->registers[instruction->rn])
              == is_negative(op2)) !=  is_negative(result));
      break;
    case ADD:
      result = machine->registers[instruction->rn] + op2;
      flags = C * ((is_negative(machine->registers[instruction->rn])
              == is_negative(op2)) !=  is_negative(result));
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
      result = 0;
      fprintf(stderr, "Unknown opcode at PC: %u",
              machine->registers[PC] - 0x40);
      exit_program(machine);
      break;
  }

  // Compute the negative and zero flags
  flags |= (N * is_negative(result));
  flags |= (Z * (result == 0));

  // Update the system state with result, if required
  if (!(instruction->operation == TST
    || instruction->operation == TEQ
    || instruction->operation == CMP)) {
    machine->registers[instruction->rd] = result;
  }

  // Update the system state by setting flags, if required
  if (instruction->flag_1) {
    machine->registers[CPSR] &= MASK_FIRST_4;
    // Only want the first four bits
    machine->registers[CPSR] |= (flags << (WORD_SIZE - 4));
  }
}

/**
 * @brief Executes a multiply instruction.
 *
 * @param machine The current system state.
 */
void execute_mul(system_state_t *machine) {
  word_t result;
  instruction_t *instruction = machine->decoded_instruction;
  // Do the multiplication
  result = machine->registers[instruction->rm]
           * machine->registers[instruction->rs];

  // Accumulate, if is set
  if (instruction->flag_0) {
    result += machine->registers[instruction->rn];
  }

  // Update system state with result
  machine->registers[instruction->rd] = result;

  // Update system state with flags, if required
  if (instruction->flag_1) {
    machine->registers[CPSR] &= MASK_FIRST_4;
    machine->registers[CPSR] |= (N * is_negative(result)) << (WORD_SIZE - 4);
    machine->registers[CPSR] |= (Z * (result == 0)) << (WORD_SIZE - 4);
  }
}

/**
 * @brief Executes a single data transfer instruction.
 *
 * @param machine The current system state.
 */
void execute_sdt(system_state_t *machine) {
  uint32_t address;
  word_t offset;
  word_t shift_amount;
  instruction_t *instruction = machine->decoded_instruction;

  // Immediate offset or not
  if (instruction->flag_0) {
    // Register offset
    if (instruction->rs == -1) {
      shift_amount = instruction->shift_amount;
    } else {
      shift_amount = machine->registers[instruction->rs];
    }
    // Shift the register offset
    value_carry_t *shifter_out = shifter(instruction->shift_type, shift_amount,
                                         machine->registers[instruction->rm]);
    offset = shifter_out->value;
    free(shifter_out);
  } else {
    // Immediate offset
    offset = instruction->immediate_value;
  }

  // Offset added or subtracted
  if (!instruction->flag_2) {
    offset = negate(offset);
  }

  // Use post or pre indexing
  if (!instruction->flag_1) {
    // Post indexing
    address = machine->registers[instruction->rn];
    machine->registers[instruction->rn] = address + offset;
  } else {
    // Pre indexing
    address = machine->registers[instruction->rn] + offset;
  }

  // Load or store - update the system state
  if (instruction->flag_3) {
    // Execute load (gets word from memory)
    machine->registers[instruction->rd] = get_word(machine, address);
  } else {
    // Execute store (sets word in memory)
    set_word(machine, address, machine->registers[instruction->rd]);
  }
}

/**
 * @brief Executes a branch instruction.
 *
 * @param machine The current system state.
 */
void execute_branch(system_state_t *machine) {
  word_t offset = machine->decoded_instruction->immediate_value;
  // Previously fetched instruction not valid, so ignored
  machine->has_fetched_instruction = false;
  // Update system state by changing PC to new address
  machine->registers[PC] += twos_complement_to_long(offset);
}
