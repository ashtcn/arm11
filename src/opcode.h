enum opcode {
  AND = 0,
  EOR = 1,
  SUB = 2,
  RSB = 3,
  ADD = 4,
  TST = 0x8,
  TEQ = 0x9,
  CMP = 0xA,
  ORR = 0xC,
  MOV = 0xD,
};

enum cond {
  eq = 0,
  ne = 1,
  ge = 0xA,
  lt = 0xB,
  gt = 0xC,
  le = 0xD,
  al = 0xE,
};

enum shift {
  lsl = 0,
  lsr = 1,
  asr = 2,
  ror = 3,
};
