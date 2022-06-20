#include QMK_KEYBOARD_H

// #include "startup_logo.h"

uint16_t startup_timer;
// static bool finished_logo = false;

// Layer Declarations
enum {
    DEFAULT = 0,
    FUNCTION = 1,
    SECRET = 2,
};

// Keycode Declarations
enum custom_keycodes { 
    FUNC = SAFE_RANGE,
    FN,
    FNF1,
    FNF2,
    FNF3,
    FNF4,
    FNF5,
    FNF6,
    FNF7,
    FNF8,
    FNF9,
    FNF10,
    FNF11,
    FNF12,
    BACKTICK,
    SCRNSHT,
    BACK,
    SKIP,
};

bool volatile fn_pressed = false;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case FN:
            if (record->event.pressed) {
                // when keycode FN is pressed
                fn_pressed = true;
                layer_on(1);                // turn on the FUNCTION layer
                register_code(KC_LALT);     // press the left alt key
            } else {
                // when keycode FN is released
                fn_pressed = false;
                layer_off(1);               // turn off the FUNCTION layer
                unregister_code(KC_LALT);   // release the left alt key
            }
            break;
        case FUNC:
            if (record->event.pressed) {
                // when keycode FUNC is pressed
                SEND_STRING("PASSWORD");
            } else {
                // when keycode FUNC is released
            }
            break;
        case FNF1:
            if (record->event.pressed) {
                // when keycode FNF1 is pressed
                unregister_code(KC_LALT);   // release the left alt key
                register_code(KC_F1);       // press the F1 key
            } else {
                // when keycode FNF1 is released
                unregister_code(KC_F1);     // release the F1 key
            }
            break;
        case FNF2:
            if (record->event.pressed) {
                // when keycode FNF2 is pressed
                unregister_code(KC_LALT);   // release the left alt key
                register_code(KC_F2);       // press the F2 key
            } else {
                // when keycode FNF2 is released
                unregister_code(KC_F2);     // release the F2 key
            }
            break;
        case FNF3:
            if (record->event.pressed) {
                // when keycode FNF3 is pressed
                unregister_code(KC_LALT);   // release the left alt key
                register_code(KC_F3);       // press the F3 key
            } else {
                // when keycode FNF3 is released
                unregister_code(KC_F3);     // release the F3 key
            }
            break;
        case FNF4:
            if (record->event.pressed) {
                // when keycode FNF4 is pressed
                unregister_code(KC_LALT);   // release the left alt key
                register_code(KC_F4);       // press the F4 key
            } else {
                // when keycode FNF4 is released
                unregister_code(KC_F4);     // release the F4 key
            }
            break;
        case FNF5:
            if (record->event.pressed) {
                // when keycode FNF5 is pressed
                unregister_code(KC_LALT);   // release the left alt key
                register_code(KC_F5);       // press the F5 key
            } else {
                // when keycode FNF5 is released
                unregister_code(KC_F5);     // release the F5 key
            }
            break;
        case FNF6:
            if (record->event.pressed) {
                // when keycode FNF6 is pressed
                unregister_code(KC_LALT);   // release the left alt key
                register_code(KC_F6);       // press the F6 key
            } else {
                // when keycode FNF6 is released
                unregister_code(KC_F6);     // release the F6 key
            }
            break;
        case FNF7:
            if (record->event.pressed) {
                // when keycode FNF7 is pressed
                unregister_code(KC_LALT);   // release the left alt key
                register_code(KC_F7);       // press the F7 key
            } else {
                // when keycode FNF7 is released
                unregister_code(KC_F7);     // release the F7 key
            }
            break;
        case FNF8:
            if (record->event.pressed) {
                // when keycode FNF8 is pressed
                unregister_code(KC_LALT);   // release the left alt key
                register_code(KC_F5);       // press the F5 key
            } else {
                // when keycode FNF8 is released
                unregister_code(KC_F8);     // release the F8 key
            }
            break;
        case FNF9:
            if (record->event.pressed) {
                // when keycode FNF9 is pressed
                unregister_code(KC_LALT);   // release the left alt key
                register_code(KC_F9);       // press the F9key
            } else {
                // when keycode FNF9 is released
                unregister_code(KC_F9);     // release the F9 key
            }
            break;
        case FNF10:
            if (record->event.pressed) {
                // when keycode FNF10 is pressed
                unregister_code(KC_LALT);   // release the left alt key
                register_code(KC_F10);      // press the F10 key
            } else {
                // when keycode FNF10 is released
                unregister_code(KC_F10);    // release the F10 key
            }
            break;
        case FNF11:
            if (record->event.pressed) {
                // when keycode FNF11 is pressed
                unregister_code(KC_LALT);   // release the left alt key
                register_code(KC_F11);      // press the F11 key
            } else {
                // when keycode FNF11 is released
                unregister_code(KC_F11);    // release the F11 key
            }
            break;
        case FNF12:
            if (record->event.pressed) {
                // when keycode FNF12 is pressed
                unregister_code(KC_LALT);   // release the left alt key
                register_code(KC_F12);      // press the F12 key
            } else {
                // when keycode FNF12 is released
                unregister_code(KC_F12);    // release the F12 key
            }
            break;
        case BACKTICK:
            if (record->event.pressed) {
                // when keycode BACKTICK is pressed
                unregister_code(KC_LALT);   // release the left alt key
                SEND_STRING("`");
            }
            break;
        case SCRNSHT:
            if (record->event.pressed) {
                // when keycode SCRNSHT is pressed
                unregister_code(KC_LALT);   // release the left alt key
                register_code(KC_PSCR);      // press the PSCR key
            } else {
                // when keycode SCRNSHT is released
                unregister_code(KC_PSCR);    // release the PSCR key
            }
            break;
        case BACK:
            if (record->event.pressed) {
                // when keycode BACK is pressed
                unregister_code(KC_LALT);   // release the left alt key
                register_code(KC_MPRV);      // press the MPRV key
            } else {
                // when keycode BACK is released
                unregister_code(KC_MPRV);    // release the MPRV key
            }
            break;
        case SKIP:
            if (record->event.pressed) {
                // when keycode SKIP is pressed
                unregister_code(KC_LALT);   // release the left alt key
                register_code(KC_MNXT);      // press the MNXT key
            } else {
                // when keycode SKIP is released
                unregister_code(KC_MNXT);    // release the MNXT key
            }
            break;
    }
    if (fn_pressed) {
        register_code(KC_LALT);     // press the left alt key   
    } else {
        unregister_code(KC_LALT);   // release the left alt key
    }
    return true;
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
        [0] = LAYOUT_ansi(
                KC_ESC, KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, KC_9, KC_0, KC_MINS, KC_EQL, KC_BSPC, KC_DEL, \
            KC_MPLY, KC_TAB, KC_Q, KC_W, KC_E, KC_R, KC_T, KC_Y, KC_U, KC_I, KC_O, KC_P, KC_LBRC, KC_RBRC, KC_BSLS, KC_HOME, \
            KC_F14,  KC_CAPS, KC_A, KC_S, KC_D, KC_F, KC_G, KC_H, KC_J, KC_K, KC_L, KC_SCLN, KC_QUOT, KC_ENT, KC_PGUP, \
            KC_NO,   KC_LSFT, KC_Z, KC_X, KC_C, KC_V, KC_B, KC_N, KC_M, KC_COMM, KC_DOT, KC_SLSH, KC_RSFT, KC_UP, KC_PGDN, \
            KC_NO,   KC_LCTL, KC_LGUI, FN,          KC_SPC,         KC_NO, KC_RALT, KC_RGUI, KC_LEFT, KC_DOWN, KC_RGHT \
            ),
        [1] = LAYOUT_ansi(
                KC_ESC, FNF1, FNF2, FNF3, FNF4, FNF5, FNF6, FNF7, FNF8, FNF9, FNF10, FNF11, FNF12, BACKTICK, KC_TRNS, \
            RGB_TOG,   KC_TRNS, BACK, SKIP, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, SCRNSHT, \
            KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, \
            KC_TRNS,   KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, \
            KC_TRNS,   KC_TRNS, KC_TRNS, FN,        KC_TRNS,        KC_TRNS, MO(SECRET), KC_TRNS, KC_LEFT, KC_DOWN, KC_RGHT \
            ),
        [2] = LAYOUT_ansi(
                RESET, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, \
            FUNC, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, \
            KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, \
            KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, \
            KC_NO, KC_NO, KC_NO, KC_NO,              KC_NO,         KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO
            )
};

#ifdef OLED_DRIVER_ENABLE
oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    startup_timer = timer_read();
    return OLED_ROTATION_180;
    return rotation;
}

void oled_task_user(void) {
    // Render logo for 3 seconds at startup
    if (timer_elapsed(startup_timer) < 3000) {
        // render_logo();
        // finished_logo = true;
        return;
    }

    // Host Keyboard Layer Status
    oled_write_P(PSTR("Layer: "), false);

    switch (get_highest_layer(layer_state)) {
        case 0:
            oled_write_P(PSTR("Default\n"), false);
            break;
        case 1:
            oled_write_P(PSTR("Function\n"), false);
            break;
        case 2:
            oled_write_P(PSTR("Secret\n"), false);
            break;
        default:
            oled_write_ln_P(PSTR("Undefined"), false);
    }   

    // Output live WPM
    char wpm_str[5];
	sprintf(wpm_str, "WPM = %i\n", get_current_wpm());
    oled_write(wpm_str, false);

    // Host Keyboard LED Status
    led_t led_state = host_keyboard_led_state();
    // oled_write_ln_P(led_state.num_lock ? PSTR("NUM ") : PSTR("    "), false);
    oled_write_ln_P(led_state.caps_lock ? PSTR("CAP ") : PSTR("    "), false);
    oled_write_ln_P(led_state.scroll_lock ? PSTR("SCR ") : PSTR("    "), false);
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

bool encoder_update_kb(uint8_t index, bool clockwise) {
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
    return true;
}

// void matrix_init_user(void) {
//   // Initialize remote keyboard, if connected (see readme)
//   matrix_init_remote_kb();
// }

// void matrix_scan_user(void) {
//   // Scan and parse keystrokes from remote keyboard, if connected (see readme)
//   matrix_scan_remote_kb();
// }