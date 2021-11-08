#include "custom_layer_handling.h"

bool ctl_alt_swap = false;

// This function is called every time a layer change occurs
layer_state_t layer_state_set_user(layer_state_t state) {
    static bool is_appshift_on = false;
    // If we're currently cycling through apps, make a note of it for later
    if (IS_LAYER_ON_STATE(state, APPSHIFT) || IS_LAYER_ON_STATE(state, VIRTDESK))
        is_appshift_on = true;
    // If we're not shifting apps, but the system thinks we are,
    // then turn off is_appshift_on, turn off ctl_alt_swap, and clear all modifiers
    else if (is_appshift_on) {
        is_appshift_on = false;
        ctl_alt_swap = false;
        clear_mods();
    }
    return state;
}

// Make sure that the layers for app switching are handled correctly
// This function returns false if it wants to halt processing, true otherwise
bool process_appshift_layer(uint16_t keycode, keyrecord_t *record) {
    // Note that in the following section, the "register" style command
    // "only_mod()" is required, since the computer needs to know about the
    // mod change before the actual keypress arrives
    if (IS_LAYER_ON(VIRTDESK)) {
        // If we're currently on virtdesktop shortcuts layer, then
        // activate only LCtl and LGui whenever a key is pressed
        if (record->event.pressed) {
            // If LCtl is still being held from tab shift, we want to register
            // the shift before changing to virtdesk buttons
            if (get_mods() == MOD_BIT(KC_LCTL)) {
                unregister_mods(MOD_BIT(KC_LCTL));
            }
            unregister_mods(~LCTL_LGUI);
            register_mods(LCTL_LGUI);
        }
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

            case MO(5):
                break;

            case MO(6):
                break;

                // We only want to actually register a changed mod state if
                // an app changing key is pressed. If we don't do it like this,
                // then the computer would treat the layer change as an alt
                // press, which causes some effects in the computer
            default:
                if (record->event.pressed) {
                    if (ctl_alt_swap) {
                        unregister_mods(~MOD_BIT(KC_LCTL));
                        register_mods(MOD_BIT(KC_LCTL));
                    }
                    else {
                        unregister_mods(~MOD_BIT(KC_LALT));
                        register_mods(MOD_BIT(KC_LALT));
                    }
                }
        }
    }
    return true;
}
