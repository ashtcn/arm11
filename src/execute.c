#include <stdbool.h>
#include "instruction.h"
#include "global.h"
#include "system_state.h"
#include "toolbox.h"

void execute(system_state machine, instruction operation);
void execute_dpi(system_state machine, instruction operation);
void execute_mul(system_state machine, instruction operation);

int condition(system_state machine, byte cond) {
  char flags = machine.registers[CPSR] >> (WORD_SIZE - 4); // Want first 4 bits
  switch(cond) {
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
      fprintf(stderr, "Incorrect cond flag, PC: %u", machine.registers[PC]); // How access registers
      exit_program(); // Not sure to exit program here
      return 0;
  }
}

void execute(system_state machine, instruction operation) {
  if (condition(machine, operation.cond)) {
    switch (operation.type) {
      case DPI:
        execute_dpi(machine, operation);
        break;
      case MUL:
        execute_mul(machine, operation);
        break;
      case SDT:
        break;
      case BRA:
        break;
    }
  }
}

void execute_dpi(system_state machine, instruction operation) {
  word op2;
  word shift_ammount;
  bool shifter_carry = 0;

  if (operation.flag_0) {
    op2 = machine.registers[operation.rm];
    if (operation.rs == -1) {
      shift_ammount = operation.immediate_value;
    } else {
      shift_ammount = machine.registers[operation.rs];
    }
  } else {
    op2 = operation.immediate_value;
    shift_ammount = operation.shift_amount;
  }

  shift shifter_out = shifter(op2, operation.shift_type, shift_ammount);
  op2 = shifter_out.value;
  shifter_carry = shifter_out.carry;

  word flags = 0;
  word result;
  switch (operation.operation) {
    case AND:
    case TST:
      result = machine.registers[operation.rn] & op2;
      flags = C * shifter_carry;
      break;
    case EOR:
    case TEQ:
      result = machine.registers[operation.rn] ^ op2;
      flags = C * shifter_carry;
      break;
    case SUB:
    case CMP:
      result = machine.registers[operation.rn] + negate(op2);
      flags = C * !is_negative(result);
      break;
    case RSB:
      result = op2 + negate(machine.registers[operation.rn]);
      flags = C * !is_negative(result);
      break;
    case ADD:
      result = machine.registers[operation.rn] + op2;
      flags = C * (result < op2);
      break;
    case ORR:
      result = machine.registers[operation.rn] | op2;
      flags = C * shifter_carry;
      break;
    case MOV:
      result = op2;
      flags = C * shifter_carry;
      break;
    default:
      fprintf(stderr, "Unknown opcode at PC: %u", machine.registers[PC] - 0x40); // Is this our error message?
      exit_program();
      break;
  }

  flags |= (N * is_negative(result));
  flags |= (Z * (result == 0));

  if (!(operation.operation == TST || operation.operation == TEQ || operation.operation == CMP)) {
    machine.registers[operation.rd] = result;
  }

  // If set flags
  if (operation.flag_1) {
    machine.registers[CPSR] &= MASK_FIRST_4;
    machine.registers[CPSR] |= (flags << (WORD_SIZE - 4)); // Want first 4 bits
  }
}

void execute_mul(system_state machine, instruction operation) {
  word result;
  result = machine.registers[operation.rm] * machine.registers[operation.rs];

  if (operation.flag_0) {
    result += machine.registers[operation.rn];
  }

  machine.registers[operation.rd] = result;
  if (operation.flag_1) {
    machine.registers[CPSR] &= MASK_FIRST_4;
    machine.registers[CPSR] |= (N * is_negative(result)) << (WORD_SIZE - 4);
    machine.registers[CPSR] |= (Z * (result == 0)) << (WORD_SIZE - 4);
  }
}
