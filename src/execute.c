void execute(system_state machine, instruction operation) {
  if (condition(operation.cond)) {
    switch (operation.code) {
      case DPI:
        execute_dpi(machine, operation);
        break;
      case MUL:
        break;
      case SDT:
        break;
      case BRA:
        break;

    }
  }
}

void execute_dpi(system_state machine, instruction_code) {
  unsigned long op2;
  unsigned long flags = 0;
  unsigned char shiftercarry = 0;
  unsigned char N = 0x8;
  unsigned char Z = 0x4;
  unsigned char C = 0x2;
  unsigned long result;
  switch (operation.opcode) {
    case AND:
    case TST:
      result = machine.registers[operation.rn] & op2;
      flags = C * shiftercarry;
      break;
    case EOR:
    case TEQ:
      result = machine.registers[operation.rn] ^ op2;
      flags = C * shiftercarry;
      break;
    case SUB:
    case CMP:
      result = machine.registers[operation.rn] - op2;
      flags = C * (machine.registers[operation.rn] < op2);
      break;
    case RSB:
      result = op2 - machine.registers[operation.rn];
      flags = C * (machine.registers[operation.rn] > op2);
      break;
    case ADD:
      result = machine.registers[operation.rn] + op2;
      flags = C * (result < op2);
      break;
    case ORR:
      result = machine.registers[operation.rn] | op2;
      flags = C * shiftercarry
      break;
    case MOV:
      result = op2;
      flags = C * shiftercarry;
      break;
    default:
      fprintf(stderr, "Unknown opcode at PC: %u", machine.registers[PC] - 0x40); // Is this our error message?
      exit_program();
      break;
  }
  flags = flags | (N * (result >> 31));
  flags = flags | (Z * (result == 0));
  if (!(operation.opcode == TST || operation.opcode == TEQ || operation.opcode == CMP)) {
    machine.registers[operation.rd] = result;
  }
  if (operation.flag_1) {//If set flags
    machine.registers[CSPR] = flags << 28;
  }
}
