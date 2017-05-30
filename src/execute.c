#include <stdbool.h>
#include "instruction.h"
#include "global.h"
#include "system_state.h"
#include "toolbox.h"

void execute(system_state *machine);
void execute_dpi(system_state *machine);
void execute_mul(system_state *machine);
void execute_branch(system_state *machine);

int condition(system_state *machine) {
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
      fprintf(stderr, "Incorrect cond flag, PC: %u", machine->registers[PC]); // How access registers
      exit_program(); // Not sure to exit program here
      return 0;
  }
}

void execute(system_state *machine) {
  if (condition(machine)) {
    switch (machine->decoded_instruction->type) {
      case DPI:
        execute_dpi(machine);
        break;
      case MUL:
        execute_mul(machine);
        break;
      case SDT:
        break;
      case BRA:
        execute_branch(machine);
        break;
    }
  }
}

void execute_dpi(system_state *machine) {
  word op2;
  word shift_ammount;
  bool shifter_carry = 0;

  if (machine->decoded_instruction->flag_0) {
    op2 = machine->registers[machine->decoded_instruction->rm];
    if (machine->decoded_instruction->rs == -1) {
      shift_ammount = machine->decoded_instruction->immediate_value;
    } else {
      shift_ammount = machine->registers[machine->decoded_instruction->rs];
    }
  } else {
    op2 = machine->decoded_instruction->immediate_value;
    shift_ammount = machine->decoded_instruction->shift_amount;
  }

  shift shifter_out = shifter(op2, machine->decoded_instruction->shift_type, shift_ammount);
  op2 = shifter_out.value;
  shifter_carry = shifter_out.carry;

  word flags = 0;
  word result;
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
      flags = C * !is_negative(result);
      break;
    case RSB:
      result = op2 + negate(machine->registers[machine->decoded_instruction->rn]);
      flags = C * !is_negative(result);
      break;
    case ADD:
      result = machine->registers[machine->decoded_instruction->rn] + op2;
      flags = C * (result < op2);
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
      exit_program();
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

void execute_mul(system_state *machine) {
  word result;
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

void execute_branch(system_state *machine) {
  word offset = machine->decoded_instruction->immediate_value
  machine->decoded_instruction->type = NUL;
  machine->has_fetched_instruction = 0;
  machine->registers[PC] += twos_complement_to_long(offset);
}
