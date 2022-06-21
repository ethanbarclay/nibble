#include QMK_KEYBOARD_H
#include "animation_frames.h"

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

#ifdef OLED_ENABLE
#define IDLE_FRAME_DURATION 200 // Idle animation iteration rate in ms

oled_rotation_t oled_init_user(oled_rotation_t rotation) { return OLED_ROTATION_90; }

uint32_t anim_timer         = 0;
uint32_t anim_sleep         = 0;
uint8_t  current_idle_frame = 0;

bool tap_anim        = false;
bool tap_anim_toggle = false;

// Decompress and write a precompressed bitmap frame to the OLED.
// Documentation and python compression script available at:
// https://github.com/nullbitsco/squeez-o
#ifdef USE_OLED_BITMAP_COMPRESSION
static void oled_write_compressed_P(const char* input_block_map, const char* input_block_list) {
    uint16_t block_index = 0;
    for (uint16_t i=0; i<NUM_OLED_BYTES; i++) {
        uint8_t bit = i%8;
        uint8_t map_index = i/8;
        uint8_t _block_map = (uint8_t)pgm_read_byte_near(input_block_map + map_index);
        uint8_t nonzero_byte = (_block_map & (1 << bit));
        if (nonzero_byte) {
            const char data = (const char)pgm_read_byte_near(input_block_list + block_index++);
            oled_write_raw_byte(data, i);
        } else {
            const char data = (const char)0x00;
            oled_write_raw_byte(data, i);
        }
    }
}
#endif

static void render_anim(void) {
    // Idle animation
    void animation_phase(void) {
        if (!tap_anim) {
            current_idle_frame = (current_idle_frame + 1) % NUM_IDLE_FRAMES;
            uint8_t idx = abs((NUM_IDLE_FRAMES - 1) - current_idle_frame);
            #ifdef USE_OLED_BITMAP_COMPRESSION
            oled_write_compressed_P(idle_block_map[idx], idle_frames[idx]);
            #else
            oled_write_raw_P(idle_frames[idx], NUM_OLED_BYTES);
            #endif
        }
    }

    // Idle behaviour
    if (get_current_wpm() != 000) {  // prevent sleep
        oled_on();
        if (timer_elapsed32(anim_timer) > IDLE_FRAME_DURATION) {
            anim_timer = timer_read32();
            animation_phase();
        }
        anim_sleep = timer_read32();
    } else {  // Turn off screen when timer threshold elapsed or reset time since last input
        if (timer_elapsed32(anim_sleep) > OLED_TIMEOUT) {
            oled_off();
        } else {
            if (timer_elapsed32(anim_timer) > IDLE_FRAME_DURATION) {
                anim_timer = timer_read32();
                animation_phase();
            }
        }
    }
}

bool oled_task_user(void) {
    render_anim();
    oled_set_cursor(0, 14);

    uint8_t n = get_current_wpm();
    char wpm_counter[6];
    wpm_counter[5] = '\0';
    wpm_counter[4] = '0' + n % 10;
    wpm_counter[3] = '0' + (n /= 10) % 10;
    wpm_counter[2] = '0' + n / 10 ;
    wpm_counter[1] = '0';
    wpm_counter[0] = '>';
    oled_write_ln(wpm_counter, false);

    return false;
}
#endif

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

    #ifdef OLED_ENABLE
    // Check if non-mod
    if ((keycode >= KC_A && keycode <= KC_0) || (keycode >= KC_TAB && keycode <= KC_SLASH)) {
        if (record->event.pressed) {
            // Display tap frames
            tap_anim_toggle = !tap_anim_toggle;
            #ifdef USE_OLED_BITMAP_COMPRESSION
            oled_write_compressed_P(tap_block_map[tap_anim_toggle], tap_frames[tap_anim_toggle]);
            #else
            oled_write_raw_P(tap_frames[tap_anim_toggle], NUM_OLED_BYTES);
            #endif
        }
    }
    #endif
    
    return true;
};

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