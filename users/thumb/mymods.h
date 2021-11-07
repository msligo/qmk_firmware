#ifndef MYMODS_H
#define MYMODS_H

#define LCTL_LGUI  (MOD_BIT(KC_LCTL) | MOD_BIT(KC_LGUI))
#define SWAP_CTL_ALT KC_F24

#include "japanese.h"
#include "my_layers.h"
#include "shift_caps.h"

bool g_suspend_state = false; // For some reason an error is thrown if this line is absent


// Identical to set_mods(mods), except it sends the keyboard report immediately
void only_mod(int8_t mod) {
    clear_mods();
    register_mods(mod);
}

// Custom behaviour for certain keycodes
bool process_record_custom(uint16_t keycode, keyrecord_t *record) {

    //Make sure that the layers for app switching are handled correctly
    static bool ctl_alt_swap = false;

    // Note that in the following section, the "register" style command
    // "only_mod()" is required, since the computer needs to know about the
    // mod change before the actual keypress arrives
    if (IS_LAYER_ON(VIRTDESK)) {
        // If we're currently on virtdesktop shortcuts layer, then
        // activate only LCtl and LGui whenever a key is pressed
        if (record->event.pressed)
            only_mod(LCTL_LGUI);
    }
    else if (IS_LAYER_ON(APPSHIFT)) {
        switch (keycode) {
            // If the Ctl Alt swap key is pressed, we only want to take
            // note of the change. We don't actually want to register it
            // until a key is actually pressed
            case SWAP_CTL_ALT:
                if (record->event.pressed)
                    ctl_alt_swap = true;
                else
                    ctl_alt_swap = false;

                return false;

                // If we exit out of the appshift layer, make sure to set
                // ctl_alt_swap back to false, since this variable isn't cleaned
                // up by the automatic code that runs when the layer changes
            case MO(5):
                ctl_alt_swap = false;

            case MO(6):
                break;

                // We only want to actually register a changed mod state if
                // an app changing key is pressed. If we don't do it like this,
                // then the computer would treat the layer change as an alt
                // press, which causes some effects in the computer
            default:
                if (record->event.pressed) {
                    if (ctl_alt_swap)
                        only_mod(MOD_BIT(KC_LCTL));
                    else
                        only_mod(MOD_BIT(KC_LALT));
                }
        }
    }

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

// This function is called every time a layer change occurs
layer_state_t layer_state_set_user(layer_state_t state) {
    static bool is_appshift_on = false;
    // If we're currently cycling through apps, make a note of it for later
    if (IS_LAYER_ON_STATE(state, APPSHIFT) || IS_LAYER_ON_STATE(state, VIRTDESK))
        is_appshift_on = true;
    // If we're not shifting apps, but the system thinks we are,
    // then turn off is_appshift_on, and clear all modifiers
    else if (is_appshift_on) {
        is_appshift_on = false;
        clear_mods();
    }
    return state;
}

#endif
