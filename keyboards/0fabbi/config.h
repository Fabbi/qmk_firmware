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

// #define RLED_CONFIG     (DDRD  |=  (1<<7))
// #define RLED_OFF        (PORTD &=~ (1<<7))
// #define RLED_ON         (PORTD |=  (1<<7))
// #define RLED_TOGGLE     (PORTD ^=  (1<<7))

// #define GLED_CONFIG (DDRB |= (1<<5))
// #define GLED_OFF (PORTB &=~ (1<<5))
// #define GLED_ON (PORTB |= (1<<5))

// #define BLED_CONFIG (DDRB |= (1<<6))
// #define BLED_OFF (PORTB &=~ (1<<6))
// #define BLED_ON (PORTB |= (1<<6))

// #define NO_DEBUG
// #define NO_PRINT

//////////////////////////////////////////////////////////////////

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


//////////////
/// OLED

#define FLOGO \
0b00000000, 0b00000000, 0b00001110, 0b00000000,\
 0b00000000, 0b00000000, 0b00111111, 0b11000000,\
 0b00000000, 0b00000000, 0b11111111, 0b11100000,\
 0b00000000, 0b00000001, 0b11111111, 0b11110000,\
 0b00000000, 0b00000011, 0b11111111, 0b11111000,\
 0b00000000, 0b00000011, 0b11111111, 0b11111100,\
 0b00000000, 0b00000111, 0b11111111, 0b11111100,\
 0b00000000, 0b00000111, 0b11111111, 0b11111100,\
 0b00000000, 0b00001111, 0b11111111, 0b11111110,\
 0b00000001, 0b10001111, 0b11111111, 0b11111110,\
 0b00000011, 0b10001111, 0b11111111, 0b11111110,\
 0b00111111, 0b10000011, 0b11111111, 0b11111110,\
 0b01111001, 0b11000000, 0b01111111, 0b11111111,\
 0b01100000, 0b11000000, 0b00011111, 0b11111111,\
 0b01100000, 0b11000000, 0b00001111, 0b11111111,\
 0b01000000, 0b11000000, 0b00000111, 0b11111111,\
 0b11000000, 0b11000000, 0b00000011, 0b11111111,\
 0b11000000, 0b11000000, 0b00000011, 0b11111111,\
 0b11000000, 0b11000000, 0b00000001, 0b11111110,\
 0b01000000, 0b11000000, 0b00000001, 0b11111110,\
 0b01100000, 0b10000000, 0b00000001, 0b11111110,\
 0b01111001, 0b10000000, 0b00000001, 0b11111110,\
 0b00111111, 0b10000000, 0b00000001, 0b11111100,\
 0b00000111, 0b10000000, 0b00000001, 0b11111100,\
 0b00000011, 0b10000000, 0b00000001, 0b11111001,\
 0b00001111, 0b10000000, 0b00000001, 0b11111001,\
 0b00111111, 0b10000000, 0b00000001, 0b11110011,\
 0b01110001, 0b10000000, 0b00000001, 0b11110011,\
 0b01100000, 0b11000000, 0b00000011, 0b11100111,\
 0b11000000, 0b11000000, 0b00000011, 0b11001110,\
 0b11000000, 0b11000000, 0b00000011, 0b10001110,\
 0b11000000, 0b11000000, 0b00000111, 0b00011110,\
 0b11000000, 0b11000000, 0b00000110, 0b00111100,\
 0b11000000, 0b11000000, 0b00001000, 0b11111100,\
 0b11000000, 0b11000000, 0b00000001, 0b11111000,\
 0b01000000, 0b11000000, 0b00000011, 0b11110000,\
 0b01100000, 0b11000000, 0b11111000, 0b01100000,\
 0b01111111, 0b11000011, 0b11111111, 0b00000000,\
 0b00011111, 0b11111111, 0b11111110, 0b00000000,\
 0b00000011, 0b10011111, 0b11110000, 0b00000000

/////////////////
//// PS2 Mouse
#ifdef PS2_USE_USART
# define PS2_CLOCK_PORT  PORTD
# define PS2_CLOCK_PIN   PIND
# define PS2_CLOCK_DDR   DDRD
# define PS2_CLOCK_BIT   5
# define PS2_DATA_PORT   PORTD
# define PS2_DATA_PIN    PIND
# define PS2_DATA_DDR    DDRD
# define PS2_DATA_BIT    2

/* synchronous, odd parity, 1-bit stop, 8-bit data, sample at falling edge */
/* set DDR of CLOCK as input to be slave */
# define PS2_USART_INIT() do {   \
    PS2_CLOCK_DDR &= ~(1<<PS2_CLOCK_BIT);       \
    PS2_DATA_DDR &= ~(1<<PS2_DATA_BIT);         \
    UCSR1C = ((1 << UMSEL10) |                  \
              (3 << UPM10)   |                  \
              (0 << USBS1)   |                  \
              (3 << UCSZ10)  |                  \
              (0 << UCPOL1));                   \
    UCSR1A = 0;                                 \
    UBRR1H = 0;                                 \
    UBRR1L = 0;                                 \
  } while (0)
# define PS2_USART_RX_INT_ON() do {  \
    UCSR1B = ((1 << RXCIE1) |        \
              (1 << RXEN1));         \
  } while (0)
# define PS2_USART_RX_POLL_ON() do { \
    UCSR1B = (1 << RXEN1);           \
  } while (0)
# define PS2_USART_OFF() do {                   \
    UCSR1C = 0;                                 \
    UCSR1B &= ~((1 << RXEN1) |                  \
                (1 << TXEN1));                  \
  } while (0)
# define PS2_USART_RX_READY      (UCSR1A & (1<<RXC1))
# define PS2_USART_RX_DATA       UDR1
# define PS2_USART_ERROR         (UCSR1A & ((1<<FE1) | (1<<DOR1) | (1<<UPE1)))
# define PS2_USART_RX_VECT       USART1_RX_vect
#endif

#endif // FABBI_CONFIG_H
