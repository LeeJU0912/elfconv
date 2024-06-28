#pragma once

#include <cstdint>

// when '101' result = (PSTATE.N == PSTATE.V); // GE or LT
static inline uint8_t CondGE(uint64_t sr_nzcv);
// when '101' result = (PSTATE.N == PSTATE.V); // GE or LT
static inline uint8_t CondLT(uint64_t sr_nzcv);
// when '000' result = (PSTATE.Z == '1'); // EQ or NE
static inline uint8_t CondEQ(uint64_t sr_nzcv);
// when '000' result = (PSTATE.Z == '1'); // EQ or NE
static inline uint8_t CondNE(uint64_t sr_nzcv);
// when '110' result = (PSTATE.N == PSTATE.V && PSTATE.Z == '0'); // GT or LE
static inline uint8_t CondGT(uint64_t sr_nzcv);
// when '110' result = (PSTATE.N == PSTATE.V && PSTATE.Z == '0'); // GT or LE
static inline uint8_t CondLE(uint64_t sr_nzcv);
// when '001' result = (PSTATE.C == '1'); // CS or CC
static inline uint8_t CondCS(uint64_t sr_nzcv);
// when '001' result = (PSTATE.C == '1'); // CS or CC
static inline uint8_t CondCC(uint64_t sr_nzcv);
// when '010' result = (PSTATE.N == '1'); // MI or PL
static inline uint8_t CondMI(uint64_t sr_nzcv);
// when '010' result = (PSTATE.N == '1'); // MI or PL
static inline uint8_t CondPL(uint64_t sr_nzcv);
// when '011' result = (PSTATE.V == '1'); // VS or VC
static inline uint8_t CondVS(uint64_t sr_nzcv);
// when '011' result = (PSTATE.V == '1'); // VS or VC
static inline uint8_t CondVC(uint64_t sr_nzcv);
// when '100' result = (PSTATE.C == '1' && PSTATE.Z == '0'); // HI or LS
static inline uint8_t CondHI(uint64_t sr_nzcv);
// when '100' result = (PSTATE.C == '1' && PSTATE.Z == '0'); // HI or LS
static inline uint8_t CondLS(uint64_t sr_nzcv);
static inline uint8_t CondAL(uint64_t sr_nzcv);