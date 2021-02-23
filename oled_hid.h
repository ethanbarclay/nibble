#include QMK_KEYBOARD_H
#include "remote_kb.h" 
#include <string.h>
#include "raw_hid.h"

// Define which oled screens you want to see
#define performance true
#define media_status true

// Define which oled screen to start on
// 1: media status   2: performance info
int volatile current_screen = 2;

bool screen_options[2] = {media_status, performance};
uint8_t num_screens = 2;
bool volatile host_connected = false;

// Use this to convert from unicode to a letter
const char unicode_alpha[256] = " ABCDEFGHIJKLMNOPQRSTUVWXYZ";

void performance_to_oled(int * data) {
  oled_write_ln_P(PSTR("VOL:"), false);
  oled_write_ln_P(PSTR("RAM:"), false);
  oled_write_ln_P(PSTR("CPU:"), false);
  oled_write_ln_P(PSTR("SSD:"), false);

  int cur_row;
  for (uint8_t stat = 0; stat < 4; stat++) {
    cur_row = stat * 8 + 1;
    for (uint8_t i = 0; i < 6; i++) {
      for (int j = 25; j < data[stat]; j++) {
        oled_write_pixel(j, cur_row+i, true);
      }
    } 
  }
}

// work in progress
void media_status_to_oled(int * data) {
  oled_write_ln_P(PSTR("VOL:"), false);
  oled_write_ln_P(PSTR("RAM:"), false);
  oled_write_ln_P(PSTR("CPU:"), false);
  oled_write_ln_P(PSTR("SSD:"), false);

  int cur_row;
  for (uint8_t stat = 0; stat < 4; stat++) {
    cur_row = stat * 8 + 1;
    if (stat == 2) {
      oled_write_raw((" test"), false);
    } else {
      for (uint8_t i = 0; i < 6; i++) {
        for (int j = 25; j < data[stat]; j++) {
          oled_write_pixel(j, cur_row+i, true);
        }
      }
    }
  }
}

int arr[400]; // holds data sent from host pc
int arr_pt = 0; // stores current location in arr since we can only recieve 32 bytes at a time
uint8_t prev_screen = 0;
void raw_hid_receive(uint8_t *data, uint8_t length) {

  // Check if this is a new incoming connection
  if (data[0] == 127) {
    arr_pt = 0;
    host_connected = true;
    return;
  }

  // Add data from host pc to our array
  for (uint8_t i = 1; i < 31; i++) {
    arr[arr_pt] = data[i];
    arr_pt++;
  }

  // Check if we've received all of the data
  if (arr_pt >= 390) {

    // Clear the screen before rendering something new
    if (prev_screen != current_screen) {
      oled_clear();
      oled_render();
    }
    prev_screen = current_screen;

    if (current_screen == 1){
      media_status_to_oled(arr);
    } else if (current_screen == 2) {
      performance_to_oled(arr);
    }
    arr_pt = 0; // reset arr_pt
  }
}

void update_oled(void) {
    current_screen++;
    if (current_screen > num_screens) {
        current_screen = 1;
    }
    int i = 0;
    while (i < 3) {
        if (screen_options[current_screen-1]) {
            break;
        } else {
            current_screen++;
        }
        i++;
    }
    uint8_t send_data[32] = {0};
    send_data[0] = current_screen;
    raw_hid_send(send_data, sizeof(send_data)); // let the host know we've switched
                                                // screens -- send new data to kb. 
}