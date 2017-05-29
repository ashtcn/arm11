#include <stdbool.h>
#include "instruction.h"
#include "constants.h"
#include "system_state.c"
#include "toolbox.c"

void execute(system_state machine, instruction operation);
void execute_dpi(system_state machine, instruction operation);
void execute_mul(system_state machine, instruction operation);



int condition(system_state machine, enum cond condition) {
  char flags = machine.registers[CPSR] >> 28; // How access registers
  const unsigned char V = 0x1;
  const unsigned char Z = 0x4;
  const unsigned char N = 0x8;
  switch(condition) {
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
  if (condition(machine, operation.condition)) {
    switch (operation.code) {
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
  unsigned long op2;
  unsigned char shifter_carry = 0;
  unsigned long shift_ammount;

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

  unsigned long flags = 0;
  unsigned char N = 0x8;
  unsigned char Z = 0x4;
  unsigned char C = 0x2;
  unsigned long result;
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
  flags = flags | (N * is_negative(result));
  flags = flags | (Z * (result == 0));
  if (!(operation.operation == TST || operation.operation == TEQ || operation.operation == CMP)) {
    machine.registers[operation.rd] = result;
  }
  if (operation.flag_1) {//If set flags
    machine.registers[CPSR] = flags << 28;
  }
}

void execute_mul(system_state machine, instruction operation) {
  unsigned long result;
  unsigned char N = 0x8;
  unsigned char Z = 0x4;
  result = machine.registers[operation.rm] * machine.registers[operation.rs];
  if (operation.flag_0) {
    result += machine.registers[operation.rn];
  }
  machine.registers[operation.rd] = result;
  if (operation.flag_1) {
    machine.registers[CPSR] =
      (machine.registers[CPSR] & 0x3FFFFFFF)
      | (((N * is_negative(result)) | (Z * (result == 0))) << 28);
  }
}
