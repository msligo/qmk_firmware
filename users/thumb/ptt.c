#include "ptt.h"

// If the PTT button is held, we want to send the unmute shortcut once
// when it's pressed down, and once when it's released. If the button is
// just tapped, we want to toggle into talk mode, so we only send the shortcut
// once. We then send it again when the key is pressed again. The colour
// change turns the whole keyboard breathing red whenever the system is
// unmuted
void ptt(uint16_t keycode, keyrecord_t *record) {
    static bool ptt_on = false;
    static uint16_t timer = 0;

    if (record->event.pressed) {
        timer = timer_read();
        tap_code16(PTT);
        // This line disables the colour code in the keymap.c file
        keyboard_config.disable_layer_led = true;
        rgb_matrix_set_color_all(RGB_RED);
        rgb_matrix_mode_noeeprom(RGB_MATRIX_BREATHING);
    }
    else {
        if (timer_elapsed(timer) > 200) {
            tap_code16(PTT);
            keyboard_config.disable_layer_led = false;
        }
        else {
            if (ptt_on) {
                ptt_on = false;
                keyboard_config.disable_layer_led = false;
            }
            else
                ptt_on = true;
        }
    }
}
