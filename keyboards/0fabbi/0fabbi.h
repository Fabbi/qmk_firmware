#ifndef FABBI_FABBI_H
#define FABBI_FABBI_H

#include "quantum.h"
#include <stdint.h>
#include <stdbool.h>

#define CPU_PRESCALE(n) (CLKPR = 0x80, CLKPR = (n))
#define CPU_16MHz       0x00

extern unsigned int __bss_end;
extern unsigned int __heap_start;
extern void *__brkval;
extern uint16_t getFreeSram(void);
//extern uint16_t num;

void init_fabbi(void);
void fabbi_lcd_init(void);
void fabbi_expander_init(void);

#define LAYER_PRETTY(k00,k01,k02,k03,k04,k05,k06,k07,k08,k09,k0A,k0B,k0C,k0D,  \
              k10,k11,k12,k13,k14,k15,k16,k17,k18,k19,k1A,k1B,k1C,k1D,  \
              k20,k21,k22,k23,k24,k25,k26,k27,k28,k29,k2A,k2B,k2C,k2D,  \
              k30,k31,k32,k33,k34,k35,k36,k37,k38,k39)                  \
  {                                                                     \
    {k00, k01, k02, k03, k04, k05, k06, k07, k08, k09, k0A, k0B, k0C, k0D}, \
    {k10, k11, k12, k13, k14, k15, k16, k17, k18, k19, k1A, k1B, k1C, k1D}, \
    {k20, k21, k22, k23, k24, k25, k26, k27, k28, k29, k2A, k2B, k2C, k2D}, \
    {KC_NO, KC_NO, k30, k31, k32, k33, k34, k35, k36, k37, k38, k39, KC_NO, KC_NO}   \
  }

#define LAYER( /* left side */                                                     \
              k00,k01,k02,k03,k04,k05,k06,                                         \
              k10,k11,k12,k13,k14,k15,k16,                                         \
              k20,k21,k22,k23,k24,k25,k26,                                         \
              e01,e02,e03,e04,k30,k31,k32,k33,k34,      \
              /* right side */                                                     \
              e05,e06,k07,k08,k09,k0A,k0B,k0C,k0D,                      \
              e07,e08,k17,k18,k19,k1A,k1B,k1C,k1D,                      \
              e09,e10,k27,k28,k29,k2A,k2B,k2C,k2D,                      \
              k35,k36,k37,k38,k39                                                  \
              )                                                                    \
  {                                                                                \
    {k00, k01, k02, k03, k04, k05, k06,   k07, k08, k09, k0A, k0B, k0C, k0D},        \
    {k10, k11, k12, k13, k14, k15, k16,   k17, k18, k19, k1A, k1B, k1C, k1D},        \
    {k20, k21, k22, k23, k24, k25, k26,   k27, k28, k29, k2A, k2B, k2C, k2D},        \
    {KC_NO, KC_NO, k30, k31, k32, k33,   k34, k35, k36, k37, k38, k39, KC_NO, KC_NO} \
  }

#endif
