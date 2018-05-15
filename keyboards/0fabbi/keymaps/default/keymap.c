#include QMK_KEYBOARD_H
#include "debug.h"
#include "action_layer.h"
#include "version.h"

enum custom_keycodes {
  PLACEHOLDER = SAFE_RANGE,
  MACRO11,
  MACRO12,
  MACRO13
};
#define BASE 0 // default layer

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
[BASE] =
LAYER_PRETTY(
      MACRO11,    KC_TAB,  KC_Q,      KC_W,    KC_E,  KC_R,  KC_T,     KC_Y,     KC_U,  KC_I,    KC_O,   KC_P,    KC_LBRC, KC_RBRC,
      MACRO12,    KC_ESC,  KC_A,      KC_S,    KC_D,  KC_F,  KC_G,     KC_H,     KC_J,  KC_K,    KC_L,   KC_SCLN, KC_QUOT, KC_ENT,
      MACRO13, KC_NO,   KC_Z,      KC_X,    KC_C,  KC_V,  KC_B,     KC_N,     KC_M,  KC_COMM, KC_DOT, KC_SLSH, KC_BSLS, KC_NO,
      // DEBUG,      KC_NO,   KC_Z,      KC_X,      KC_C,  KC_V,  KC_B,     KC_N,     KC_M,  KC_COMM, KC_DOT, KC_SLSH, KC_BSLS, KC_NO,
                        KC_LALT,   KC_LGUI, KC_LSHIFT, KC_LCTL, KC_NO, KC_NO, KC_NO, KC_SPACE, KC_NO, KC_NO
      ),
};
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  LED_TOGGLE;
  matrix_print();
  switch (keycode) {
  case MACRO11:
    dprintf("FREE RAM: %d\n", getFreeSram());
    break;
  case MACRO12:
    break;
  case MACRO13:
    break;
  };
  return true;
}
