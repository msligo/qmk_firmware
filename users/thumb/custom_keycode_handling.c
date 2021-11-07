#include "custom_keycode_handling.h"

// This function will do any custom keycode handling after the base keymap
// file is finished
bool process_record_custom(uint16_t keycode, keyrecord_t *record) {

    // Check if the appshift layer needs any special processing. It will return
    // false if it wants to halt keycode processing
    if (!process_appshift_layer(keycode, record))
        return false;

    // If Caps Lock is on, check whether we need to implement any custom
    // caps lock behaviour
    led_t led_state = host_keyboard_led_state();
    if (led_state.caps_lock) {
        // If custom caps lock behaviour is used, we don't want to keep
        // processing the keystroke
        if (custom_caps(keycode, record))
            return false;
    }

    // Custom behaviour for special keys
    switch (keycode) {
        // When swapping to/from Japanese mode, need to adjust some system settings
        case TG(JAPANESE):
            if (record->event.pressed)
                toggle_japanese();
            break;

            // Because the : key is a mod tap with CTRL, it can't send : by
            // default, only ;. This macro sends ; if shift is pressed, and
            // : if shift is not pressed
            // See https://docs.qmk.fm/#/mod_tap?id=intercepting-mod-taps
        case LCTL_T(KC_COLN):
            if (record->tap.count && record->event.pressed) {
                if (get_mods() & MOD_MASK_SHIFT) {
                    uint8_t temp_mod_state = get_mods();
                    del_mods(MOD_MASK_SHIFT);
                    tap_code(KC_SCOLON);
                    set_mods(temp_mod_state);
                }
                else
                    tap_code16(KC_COLN);
                return false;
            }
            break;
    }
    return true;
}

