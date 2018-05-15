#ifndef FABBI_CONFIG_H
#define FABBI_CONFIG_H

///// QMK CONFIG
#include "config_common.h"

#define VENDOR_ID 0xFAB1 // awwww yeah!
#define PRODUCT_ID 0xD1EB

#define DEVICE_VER 0x0001
#define MANUFACTURER FABBI
#define PRODUCT SplitBoard  // TODO COME UP WITH A NAME!!
#define DESCRIPTION QMK keyboard firmware for PRODUCT

/* key matrix size */
#define MATRIX_ROWS 4
#define MATRIX_COLS (7*2)

// #define MATRIX_ROW_PINS {F0, F1, F4, F5}
// #define MATRIX_COL_PINS {B0, B1, B2, B3, D3, C6, C7}
// #define UNUSED_PINS {B5, B6, B7, D4, D6, D7}

#define DIODE_DIRECTION COL2ROW

// these are from ergodone config
#define MOUSEKEY_INTERVAL 20
#define MOUSEKEY_DELAY 0
#define MOUSEKEY_TIME_TO_MAX 60
#define MOUSEKEY_MAX_SPEED 7
#define MOUSEKEY_WHEEL_DELAY 0

#define TAPPING_TOGGLE 1

#define PREVENT_STUCK_MODIFIERS

// ???
#ifndef FORCE_NKRO
  #define FORCE_NKRO
#endif

///// OWN CONFIG

// #define WAIT_FOR_USB
#define LED_CONFIG (DDRD |= (1<<6))
#define LED_ON (PORTD SET (1<<6))
#define LED_OFF (PORTD CLEAR (1<<6))
#define LED_TOGGLE (PORTD ^= (1<<6))

#define RLED_CONFIG     (DDRD  |=  (1<<7))
#define RLED_OFF        (PORTD &=~ (1<<7))
#define RLED_ON         (PORTD |=  (1<<7))
#define RLED_TOGGLE     (PORTD ^=  (1<<7))

#define GLED_CONFIG (DDRB |= (1<<5))
#define GLED_OFF (PORTB &=~ (1<<5))
#define GLED_ON (PORTB |= (1<<5))

// #define BLED_CONFIG (DDRB |= (1<<6))
#define BLED_OFF (PORTB &=~ (1<<6))
#define BLED_ON (PORTB |= (1<<6))

// #define NO_DEBUG
// #define NO_PRINT

#define COLS(F,...)                             \
  do {                                          \
    F(__VA_ARGS__,COL0);                          \
    F(__VA_ARGS__,COL1);                          \
    F(__VA_ARGS__,COL2);                          \
    F(__VA_ARGS__,COL3);                          \
    F(__VA_ARGS__,COL4);                          \
    F(__VA_ARGS__,COL5);                          \
    F(__VA_ARGS__,COL6);                          \
  } while(0)

#define ROWS(F,...)                             \
  do {                                          \
    F(__VA_ARGS__,ROW0);                          \
    F(__VA_ARGS__,ROW1);                          \
    F(__VA_ARGS__,ROW2);                          \
    F(__VA_ARGS__,ROW3);                          \
    /* F(__VA_ARGS__,R4); */                    \
  } while(0)

#define ROW3 F, 5
#define ROW2 F, 4
#define ROW1 F, 1
#define ROW0 F, 0

#define COL0 C, 7
#define COL1 C, 6
#define COL2 D, 3
#define COL3 B, 3
#define COL4 B, 2
#define COL5 B, 1
#define COL6 B, 0

#define SET |=
#define CLEAR &=~

#define write_all_col(register, operation)      \
  COLS(write_pin,register,operation)

#define write_all_row(register, operation)      \
  ROWS(write_pin,register,operation)

#define _write_pin(register, operation, letter, number) \
  do {                                                  \
    ((register##letter) operation (1<<(number)));       \
    _delay_us(1);                                       \
  } while(0)

// pin == X, N
#define write_pin(register, operation, pin)     \
  _write_pin(register, operation, pin)

#define _read_pin(letter, number)               \
  ((PIN##letter) & (1<<(number)))

#define read_pin(pin)                           \
  _read_pin(pin)


#define FLOGO                                   \
  0x0, 0xf, 0xc0, 0xf8, 0x0,                    \
    0x0, 0xff, 0xfc, 0x3f, 0x0,                 \
    0x7, 0xff, 0xff, 0xf, 0xc0,                 \
    0xf, 0xff, 0xff, 0xc7, 0xe0,                \
    0x1f, 0xff, 0xff, 0xf1, 0xf0,               \
    0x3f, 0xff, 0xff, 0xf8, 0xf8,               \
    0x7f, 0xff, 0xff, 0xfc, 0x78,               \
    0x7f, 0xff, 0xff, 0xfe, 0x70,               \
    0x7f, 0xff, 0xff, 0xff, 0x34,               \
    0xff, 0xff, 0xc0, 0xf, 0x96,                \
    0xff, 0xff, 0x0, 0x1, 0x86,                 \
    0xff, 0xfe, 0x0, 0x0, 0x4e,                 \
    0x7f, 0xfc, 0x0, 0x0, 0xf,                  \
    0x7f, 0xf8, 0x0, 0x0, 0xf,                  \
    0x3f, 0xf8, 0x0, 0x0, 0xf,                  \
    0x3f, 0xf0, 0x0, 0x0, 0xf,                  \
    0x1f, 0xf0, 0x0, 0x0, 0x7,                  \
    0xf, 0xf0, 0x0, 0x0, 0x7,                   \
    0x3, 0xe0, 0x0, 0x0, 0x3,                   \
    0x0, 0xe0, 0x0, 0x0, 0x3,                   \
    0x0, 0x0, 0x0, 0x0, 0x3,                    \
    0x0, 0x0, 0x0, 0x0, 0x2,                    \
    0x0, 0xf, 0xf0, 0xf, 0xfe,                  \
    0x0, 0x7f, 0xff, 0xff, 0xff,                \
    0x0, 0x78, 0x7, 0xf0, 0x7,                  \
    0x0, 0x30, 0x3, 0xe0, 0x7,                  \
    0x0, 0x10, 0x3, 0x60, 0x6,                  \
    0x0, 0x18, 0x6, 0x60, 0x6,                  \
    0x0, 0x18, 0x6, 0x30, 0x6,                  \
    0x0, 0x1e, 0xe, 0x38, 0xc,                  \
    0x0, 0xf, 0xfc, 0x1f, 0xfc,                 \
    0x0, 0x0, 0xe0, 0x7, 0xe0
#endif // FABBI_CONFIG_H
