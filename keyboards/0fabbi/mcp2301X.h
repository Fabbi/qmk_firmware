#ifndef EXPANDER_H
#define EXPANDER_H

#include <stdint.h>
#include "matrix.h"

#define MCP23017
#define MCP23017_A0 0
#define MCP23017_A1 0
#define MCP23017_A2 0

#ifdef MCP23017
#define EXPANDER_ADDR ((0x20|(MCP23017_A0<<0)|(MCP23017_A1<<1)|(MCP23017_A2<<2)) << 1)
enum EXPANDER_REG_BANK0 {
  EXPANDER_REG_IODIRA = 0,
  EXPANDER_REG_IODIRB, // 1
  EXPANDER_REG_IPOLA,
  EXPANDER_REG_IPOLB,
  EXPANDER_REG_GPINTENA,
  EXPANDER_REG_GPINTENB,
  EXPANDER_REG_DEFVALA,
  EXPANDER_REG_DEFVALB,
  EXPANDER_REG_INTCONA,
  EXPANDER_REG_INTCONB,
  EXPANDER_REG_IOCONA, // A
  EXPANDER_REG_IOCONB,
  EXPANDER_REG_GPPUA,
  EXPANDER_REG_GPPUB,
  EXPANDER_REG_INTFA,
  EXPANDER_REG_INTFB,
  EXPANDER_REG_INTCAPA, // 10
  EXPANDER_REG_INTCAPB,
  EXPANDER_REG_GPIOA,
  EXPANDER_REG_GPIOB,
  EXPANDER_REG_OLATA,
  EXPANDER_REG_OLATB
};
#endif

/* MCP23017 I2C timing */
#define TGPVO  0.5 // 500ns Table 2-2
#define TGPIV  0.450 // 450ns Table 2-2

/********MCP23017 pinout***/
/* GPIOB-COLS  |GPIOA-ROWS*/
/**************|***********/
/* GPB7 ?      |GPA7 ?    */
/* GPB6 col6   |GPA6 ?    */
/* GPB5 col5   |GPA5 ?    */
/* GPB4 col4   |GPA4 ?    */
/* GPB3 col3   |GPA3 row0 */
/* GPB2 col2   |GPA2 row1 */
/* GPB1 col1   |GPA1 row2 */
/* GPB0 col0   |GPA0 row3 */
/**************************/

#define GPBROWS 0b01111111

void expander_init(void);
void expander_scan(void);
void expander_read_cols(void);
uint8_t expander_get_col(uint8_t col);
matrix_row_t expander_read_row(void);
void expander_unselect_rows(void);
void expander_select_row(uint8_t row);

#endif
