#include "japanese.h"

void toggle_japanese(void) {
    // If Japanese mode is being turned on, make sure Caps Lock
    // is off
    if (IS_LAYER_OFF(JAPANESE)) {
        // Make sure caps lock is off
        led_t led_state = host_keyboard_led_state();
        if (led_state.caps_lock)
            tap_code(KC_CAPSLOCK);

        // Tell the OS to turn on the IME
        tap_code16(LALT(KC_LSHIFT));
        // For Windows, tell it to swap to kana mode immediately
        wait_ms(50);
        tap_code(JP_KANA);
    }
    else
        // If we're turning Japanese off, we only need to turn off the IME
        tap_code16(LALT(KC_LSHIFT));
}
