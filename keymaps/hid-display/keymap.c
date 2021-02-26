#include QMK_KEYBOARD_H

// Add headers for raw hid communication
#include "oled_hid.h"

uint16_t startup_timer;
// static bool finished_logo = false;

// layer Declarations
enum {
    DEFAULT = 0,
    FUNCTION = 1,
    SECRET = 2,
};

// keycode Declarations
enum custom_keycodes { 
    FUNC = SAFE_RANGE,
    LYR_ALT,
    BACKTICK,
    OLED_PAGE
};

// macros
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case FUNC:
            if (record->event.pressed) {
                // when keycode FUNC is pressed
                SEND_STRING("PASSWORD");
            } else {
                // when keycode FUNC is released
            }
            break;
        case LYR_ALT:
            if (record->event.pressed) {
                // when keycode LYR_ALT is pressed
                layer_on(1);                // turn on the FUNCTION layer
                register_code(KC_LALT);     // press the left alt key
            } else {
                // when keycode LYR_ALT is released
                layer_off(1);               // turn off the FUNCTION layer
                unregister_code(KC_LALT);   // release the left alt key
            }
            break;
        case BACKTICK:
            if (record->event.pressed) {
                // when keycode BACKTICK is pressed
                unregister_code(KC_LALT);   // release the left alt key
                SEND_STRING("`");
            } else {
                // when keycode BACKTICK is released
                register_code(KC_LALT);     // press the left alt key
            }
            break;
        case OLED_PAGE:
            if (record->event.pressed) {
                // when keycode OLED_PAGE is pressed
                update_oled();
            } else {
                // when keycode OLED_PAGE is released
            }
            break;
    }
    return true;
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
        [0] = LAYOUT_ansi(
                     KC_ESC, KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, KC_9, KC_0, KC_MINS, KC_EQL, KC_BSPC, KC_DEL, \
            KC_MPLY, KC_TAB, KC_Q, KC_W, KC_E, KC_R, KC_T, KC_Y, KC_U, KC_I, KC_O, KC_P, KC_LBRC, KC_RBRC, KC_BSLS, KC_HOME, \
            KC_F14,  KC_CAPS, KC_A, KC_S, KC_D, KC_F, KC_G, KC_H, KC_J, KC_K, KC_L, KC_SCLN, KC_QUOT, KC_ENT, KC_PGUP, \
            KC_NO,   KC_LSFT, KC_Z, KC_X, KC_C, KC_V, KC_B, KC_N, KC_M, KC_COMM, KC_DOT, KC_SLSH, KC_RSFT, KC_UP, KC_PGDN, \
            KC_NO,   KC_LCTL, KC_LGUI, LYR_ALT,        KC_SPC,        KC_NO, KC_LALT, KC_RGUI, KC_LEFT, KC_DOWN, KC_RGHT \
            ),
        [1] = LAYOUT_ansi(
                       RESET, KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_F6, KC_F7, KC_F8, KC_F9, KC_F10, KC_F11, KC_F12, BACKTICK, MO(SECRET), \
            RGB_TOG,   KC_TRNS, KC_MPRV, KC_MNXT, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, \
            OLED_PAGE, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, \
            KC_TRNS,   KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, \
            KC_TRNS,   KC_TRNS, KC_TRNS, LYR_ALT,        KC_TRNS,        KC_TRNS, KC_TRNS, KC_TRNS, KC_MPRV, KC_MPLY, KC_MNXT \
            ),
        [2] = LAYOUT_ansi(
            KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, FUNC, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO
            )
};

// flip oled
#ifdef OLED_DRIVER_ENABLE
oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    startup_timer = timer_read();
    return OLED_ROTATION_180;
    return rotation;
}
#endif

// RGB config, for changing RGB settings on non-VIA firmwares
void change_RGB(bool clockwise) {
    bool shift = get_mods() & MOD_MASK_SHIFT;
    bool alt = get_mods() & MOD_MASK_ALT;
    bool ctrl = get_mods() & MOD_MASK_CTRL;

    if (clockwise) {
        if (alt) {
            rgblight_increase_hue();
        } else if (ctrl) {
            rgblight_increase_val();
        } else if (shift) {
            rgblight_increase_sat();
        } else {
            rgblight_step();
        }

  } else {
      if (alt) {
            rgblight_decrease_hue();
        } else if (ctrl) {
            rgblight_decrease_val();
        } else if (shift) {
            rgblight_decrease_sat();
        } else {
            rgblight_step_reverse();
        }
    } 
}

void encoder_update_kb(uint8_t index, bool clockwise) {
  if (layer_state_is(1)) {
    //change RGB settings
    change_RGB(clockwise);
  }
  else {
    if (clockwise) {
      tap_code(KC_VOLU);
  } else {
      tap_code(KC_VOLD);
    }  
  }
}

void matrix_init_user(void) {
  // Initialize remote keyboard, if connected (see readme)
  matrix_init_remote_kb();
}

void matrix_scan_user(void) {
  // Scan and parse keystrokes from remote keyboard, if connected (see readme)
  matrix_scan_remote_kb();
}