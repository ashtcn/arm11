#include "execute.h"

void execute(system_state_t *machine);
void execute_dpi(system_state_t *machine);
void execute_mul(system_state_t *machine);
void execute_branch(system_state_t *machine);
void execute_sdt(system_state_t *machine);

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
  machine->has_fetched_instruction = false;
  machine->registers[PC] += twos_complement_to_long(offset);
}
