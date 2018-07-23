#include <stdbool.h>
#include <util/delay.h>
#include "action.h"
#include "i2c_master.h"
#include "mcp2301X.h"
#include "print.h"
#include "debug.h"

static uint8_t expander_status = 0;
static uint8_t expander_input = 0;

void expander_config(void);
uint8_t expander_write(uint8_t reg, uint8_t data);
uint8_t expander_read(uint8_t reg, uint8_t *data);

void expander_init(void)
{
  // i2c_init();
  expander_scan();
  if (expander_status)
    dprint("EXPANDER FOUND\n");
  else
    dprint("NO EXPANDER FOUND\n");
}

void expander_scan(void)
{
  debug_enable = true;
  uint8_t ret = i2c_start(EXPANDER_ADDR | I2C_WRITE);
  if (ret == 0) {
    i2c_stop();
    if (expander_status == 0) {
      dprint("attached\n");
      expander_status = 1;
      expander_config();
      clear_keyboard();
    }
  }
  else {
    if (expander_status == 1) {
      dprint("detached\n");
      expander_status = 0;
      clear_keyboard();
    }
  }
}

void expander_read_cols(void)
{
  // expander_read(EXPANDER_REG_GPIOA, &expander_input);
  expander_read(EXPANDER_REG_GPIOB, &expander_input);
}

uint8_t expander_get_col(uint8_t col)
{
  return expander_input & (1<<col) ? 1 : 0;
}

matrix_row_t expander_read_row(void)
{
  expander_read_cols();

  /* make cols */
  matrix_row_t cols = 0;
  for (uint8_t col = 0; col < MATRIX_COLS; col++) {
    if (expander_get_col(col)) {
      cols |= (1UL << ((MATRIX_COLS/2) + col));
    }
  }

  return cols;
}

void expander_unselect_rows(void)
{
  expander_write(EXPANDER_REG_IODIRA, 0xFF);
  _delay_us(TGPVO); /* tGPOV 500ns */
}

void expander_unselect_row(uint8_t row)
{
  expander_write(EXPANDER_REG_IODIRA, (1<<row));
  _delay_us(TGPVO); /* tGPOV 500ns */
}

void expander_select_row(uint8_t row)
{
  expander_write(EXPANDER_REG_IODIRA, ~(1<<row));
  _delay_us(TGPVO); /* tGPOV 500ns */
}

void expander_config(void)
{
  expander_write(EXPANDER_REG_IPOLB, 0xFF);
  expander_write(EXPANDER_REG_GPPUB, 0xFF);
  expander_write(EXPANDER_REG_IODIRA, 0xFF);
}

uint8_t expander_write(uint8_t reg, uint8_t data)
{
  if (expander_status == 0) {
    return 0;
  }
  uint8_t ret;
  ret = i2c_start(EXPANDER_ADDR | I2C_WRITE);
  if (ret) goto stop;
  ret = i2c_write(reg);
  if (ret) goto stop;
  ret = i2c_write(data);
 stop:
  i2c_stop();
  return ret;
}

uint8_t expander_read(uint8_t reg, uint8_t *data)
{
  if (expander_status == 0) {
    return 0;
  }
  uint8_t ret;
  ret = i2c_start(EXPANDER_ADDR | I2C_WRITE);
  if (ret) goto stop;
  ret = i2c_write(reg);
  if (ret) goto stop;
  ret = i2c_start(EXPANDER_ADDR | I2C_READ);
  if (ret) goto stop;

  _delay_us(TGPIV); /* tGPIV 450ns Table 2-2 page 36*/
  *data = i2c_read_nack();
 stop:
  i2c_stop();
  return ret;
}
