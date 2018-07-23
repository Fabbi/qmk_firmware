#include <stdint.h>
#include <stdbool.h>
#if defined(__AVR__)
#include <avr/io.h>
#endif
#include "wait.h"
#include "print.h"
#include "debug.h"
#include "util.h"
#include "matrix.h"
#include "timer.h"
#include "i2cmaster.h"
#include "mcp2301X.h"
#ifdef DEBUG_MATRIX_SCAN_RATE
#include  "timer.h"
#endif


/* Set 0 if debouncing isn't needed */

#ifndef DEBOUNCING_DELAY
#   define DEBOUNCING_DELAY 5
#endif

#if (DEBOUNCING_DELAY > 0)
static uint16_t debouncing_time;
static bool debouncing = false;
#endif
#ifdef DEBUG_MATRIX_SCAN_RATE
uint32_t matrix_timer;
uint32_t matrix_scan_count;
#endif

#if (MATRIX_COLS <= 8)
#    define print_matrix_header()  print("\nr/c 01234567\n")
#    define print_matrix_row(row)  print_bin_reverse8(matrix_get_row(row))
#    define matrix_bitpop(i)       bitpop(matrix[i])
#    define ROW_SHIFTER ((uint8_t)1)
#elif (MATRIX_COLS <= 16)
#    define print_matrix_header()  print("\nr/c 0123456789ABCDEF\n")
#    define print_matrix_row(row)  print_bin_reverse16(matrix_get_row(row))
#    define matrix_bitpop(i)       bitpop16(matrix[i])
#    define ROW_SHIFTER ((uint16_t)1)
#elif (MATRIX_COLS <= 32)
#    define print_matrix_header()  print("\nr/c 0123456789ABCDEF0123456789ABCDEF\n")
#    define print_matrix_row(row)  print_bin_reverse32(matrix_get_row(row))
#    define matrix_bitpop(i)       bitpop32(matrix[i])
#    define ROW_SHIFTER  ((uint32_t)1)
#endif

#ifdef MATRIX_MASKED
extern const matrix_row_t matrix_mask[];
#endif

/* matrix state(1:on, 0:off) */
static matrix_row_t matrix[MATRIX_ROWS];

static matrix_row_t matrix_debouncing[MATRIX_ROWS];


static void init_cols(void);
static bool read_cols_on_row(matrix_row_t current_matrix[], uint8_t current_row);
static void unselect_rows(void);
static void select_row(uint8_t row);
static void unselect_row(uint8_t row);

__attribute__ ((weak))
void matrix_init_quantum(void) {
  matrix_init_kb();
}

__attribute__ ((weak))
void matrix_scan_quantum(void) {
  matrix_scan_kb();
}

__attribute__ ((weak))
void matrix_init_kb(void) {
  matrix_init_user();
}

__attribute__ ((weak))
void matrix_scan_kb(void) {
  matrix_scan_user();
}

__attribute__ ((weak))
void matrix_init_user(void) {
}

__attribute__ ((weak))
void matrix_scan_user(void) {
}

inline
uint8_t matrix_rows(void) {
  return MATRIX_ROWS;
}

inline
uint8_t matrix_cols(void) {
  return MATRIX_COLS;
}

void matrix_init(void) {

  // To use PORTF disable JTAG with writing JTD bit twice within four cycles.
#if  (defined(__AVR_AT90USB1286__) || defined(__AVR_AT90USB1287__) || defined(__AVR_ATmega32U4__))
  MCUCR |= _BV(JTD);
  MCUCR |= _BV(JTD);
#endif

  debug_enable = true;
  LED_CONFIG;
  LED_ON;
  i2c_init();
  wait_ms(1500);
  LED_OFF;

#ifdef DEBUG_MATRIX_SCAN_RATE
  matrix_timer = timer_read32();
  matrix_scan_count = 0;
#endif
  // initialize row and col
  unselect_rows();
  init_cols();

  // initialize matrix state: all keys off
  for (uint8_t i=0; i < MATRIX_ROWS; i++) {
    matrix[i] = 0;
    matrix_debouncing[i] = 0;
  }

  matrix_init_quantum();
}

uint8_t matrix_scan(void)
{
#ifdef DEBUG_MATRIX_SCAN_RATE
  matrix_scan_count++;

  uint32_t timer_now = timer_read32();
  if (TIMER_DIFF_32(timer_now, matrix_timer)>1000) {
    print("matrix scan frequency: ");
    pdec(matrix_scan_count);
    print("\n");
    // matrix_print();

    matrix_timer = timer_now;
    matrix_scan_count = 0;
  }
#endif

#ifndef NO_EXPANDER
  expander_scan();
#endif

  // Set row, read cols
  for (uint8_t current_row = 0; current_row < MATRIX_ROWS; current_row++) {
#if (DEBOUNCING_DELAY > 0)
    bool matrix_changed = read_cols_on_row(matrix_debouncing, current_row);

    if (matrix_changed) {
      debouncing = true;
      debouncing_time = timer_read();
    }

#else
    read_cols_on_row(matrix, current_row);
#endif

  }

#if (DEBOUNCING_DELAY > 0)
  if (debouncing && (timer_elapsed(debouncing_time) > DEBOUNCING_DELAY)) {
    for (uint8_t i = 0; i < MATRIX_ROWS; i++) {
      matrix[i] = matrix_debouncing[i];
    }
    debouncing = false;
  }
#endif

  matrix_scan_quantum();
  return 1;
}

bool matrix_is_modified(void)
{
#if (DEBOUNCING_DELAY > 0)
  if (debouncing) return false;
#endif
  return true;
}

inline
bool matrix_is_on(uint8_t row, uint8_t col)
{
  return (matrix[row] & ((matrix_row_t)1<col));
}

inline
matrix_row_t matrix_get_row(uint8_t row)
{
  // Matrix mask lets you disable switches in the returned matrix data. For example, if you have a
  // switch blocker installed and the switch is always pressed.
#ifdef MATRIX_MASKED
  return matrix[row] & matrix_mask[row];
#else
  return matrix[row];
#endif
}

void matrix_print(void)
{
  print_matrix_header();

  for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
    phex(row); print(": ");
    print_matrix_row(row);
    print("\n");
  }
}

uint8_t matrix_key_count(void)
{
  uint8_t count = 0;
  for (uint8_t i = 0; i < MATRIX_ROWS; i++) {
    count += matrix_bitpop(i);
  }
  return count;
}


static void init_cols(void)
{
  write_all_col(DDR, CLEAR);
  write_all_col(PORT, SET);
}

static bool read_cols_on_row(matrix_row_t current_matrix[], uint8_t current_row)
{
  // Store last value of row prior to reading
  matrix_row_t last_row_value = current_matrix[current_row];

  // Clear data in matrix row
  current_matrix[current_row] = 0;

  // Select row and wait for row selecton to stabilize
  select_row(current_row);
  wait_us(30);

  current_matrix[current_row] |=
#ifndef NO_EXPANDER
    expander_read_row() |
#endif
    (read_pin(COL0) ? 0 : (1<<6)) |
    (read_pin(COL1) ? 0 : (1<<5)) |
    (read_pin(COL2) ? 0 : (1<<4)) |
    (read_pin(COL3) ? 0 : (1<<3)) |
    (read_pin(COL4) ? 0 : (1<<2)) |
    (read_pin(COL5) ? 0 : (1<<1)) |
    (read_pin(COL6) ? 0 : (1<<0)) ;

  // Unselect row
  unselect_row(current_row);

  return (last_row_value != current_matrix[current_row]);
}

static void select_row(uint8_t row)
{
  // Teensy
  switch (row) {
  case 0:
    write_pin(DDR, SET, ROW0);
    write_pin(PORT, CLEAR, ROW0);
    break;
  case 1:
    write_pin(DDR, SET, ROW1);
    write_pin(PORT, CLEAR, ROW1);
    break;
  case 2:
    write_pin(DDR, SET, ROW2);
    write_pin(PORT, CLEAR, ROW2);
    break;
  case 3:
    write_pin(DDR, SET, ROW3);
    write_pin(PORT, CLEAR, ROW3);
    break;
  }

#ifndef NO_EXPANDER
  expander_select_row(row);
#endif

}

static void unselect_row(uint8_t row)
{
  switch (row) {
  case 0:
    write_pin(DDR, CLEAR, ROW0);
    write_pin(PORT, CLEAR, ROW0);
    break;
  case 1:
    write_pin(DDR, CLEAR, ROW1);
    write_pin(PORT, CLEAR, ROW1);
    break;
  case 2:
    write_pin(DDR, CLEAR, ROW2);
    write_pin(PORT, CLEAR, ROW2);
    break;
  case 3:
    write_pin(DDR, CLEAR, ROW3);
    write_pin(PORT, CLEAR, ROW3);
    break;
  }
#ifndef NO_EXPANDER
  expander_unselect_row(row);
#endif
}

static void unselect_rows(void)
{
  // TEENSY
  write_all_row(DDR, CLEAR);
  write_all_row(PORT, CLEAR);

  // Expander
#ifndef NO_EXPANDER
  expander_unselect_rows();
#endif
}
