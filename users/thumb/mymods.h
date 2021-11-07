#ifndef MYMODS_H
#define MYMODS_H

#define LCTL_LGUI  (MOD_BIT(KC_LCTL) | MOD_BIT(KC_LGUI))
#define SWAP_CTL_ALT KC_F24

#include "japanese.h"
#include "my_layers.h"

bool g_suspend_state = false; // For some reason an error is thrown if this line is absent

//This function is called if caps lock is active for certain keys
//If shift is pressed, it sends the standard version of that key
//If shift is not pressed, it sends the shifted version of that key
void caps_send(uint16_t shift_kc, uint16_t unshift_kc, bool *shifted, keyrecord_t *record) {
    if (record->event.pressed) {
        *shifted = get_mods() & MOD_MASK_SHIFT;
        if (*shifted) {
            uint8_t temp_mod_state = get_mods();
            del_mods(MOD_MASK_SHIFT);
            register_code16(shift_kc);
            set_mods(temp_mod_state);
        }
        else
            register_code16(unshift_kc);
    }
    else {
        if (*shifted)
            unregister_code16(shift_kc);
        else
            unregister_code16(unshift_kc);
    }
}

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

    // Custom behaviour for Caps Lock key
    // The symbols on the number row must be reprogrammed to work properly
    // with Caps Lock. We also need to make sure they behave correctly when
    // both Caps Lock and Shift are activated
    led_t led_state = host_keyboard_led_state();
    if (led_state.caps_lock) {
        switch (keycode) {
            case KC_LBRACKET: {
                                  static bool shifted;
                                  caps_send(KC_LBRACKET, KC_7, &shifted, record);
                                  return false;
                              }
            case KC_LCBR: {
                              static bool shifted;
                              caps_send(KC_LCBR, KC_5, &shifted, record);
                              return false;
                          }
            case KC_RCBR: {
                              static bool shifted;
                              caps_send(KC_RCBR, KC_3, &shifted, record);
                              return false;
                          }
            case KC_LPRN: {
                              static bool shifted;
                              caps_send(KC_LPRN, KC_1, &shifted, record);
                              return false;
                          }
            case KC_EQUAL: {
                               static bool shifted;
                               caps_send(KC_EQUAL, KC_9, &shifted, record);
                               return false;
                           }
            case KC_ASTR: {
                              static bool shifted;
                              caps_send(KC_ASTR, KC_0, &shifted, record);
                              return false;
                          }
            case KC_RPRN: {
                              static bool shifted;
                              caps_send(KC_RPRN, KC_2, &shifted, record);
                              return false;
                          }
            case KC_PLUS: {
                              static bool shifted;
                              caps_send(KC_PLUS, KC_4, &shifted, record);
                              return false;
                          }
            case KC_RBRACKET: {
                                  static bool shifted;
                                  caps_send(KC_RBRACKET, KC_6, &shifted, record);
                                  return false;
                              }
            case KC_PERC: {
                              static bool shifted;
                              caps_send(KC_PERC, KC_8, &shifted, record);
                              return false;
                          }
            case KC_MINUS: {
                               static bool shifted;
                               caps_send(KC_MINUS, KC_UNDERSCORE, &shifted, record);
                               return false;
                           }
        }
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


// Shift overrides for the symbols/number row
const key_override_t shift_lbracket = ko_make_basic(MOD_MASK_SHIFT, KC_LBRACKET, KC_7);
const key_override_t shift_lcbr = ko_make_basic(MOD_MASK_SHIFT, KC_LCBR, KC_5);
const key_override_t shift_rcbr = ko_make_basic(MOD_MASK_SHIFT, KC_RCBR, KC_3);
const key_override_t shift_lprn = ko_make_basic(MOD_MASK_SHIFT, KC_LPRN, KC_1);
const key_override_t shift_equal = ko_make_basic(MOD_MASK_SHIFT, KC_EQUAL, KC_9);
const key_override_t shift_astr = ko_make_basic(MOD_MASK_SHIFT, KC_ASTR, KC_0);
const key_override_t shift_rprn = ko_make_basic(MOD_MASK_SHIFT, KC_RPRN, KC_2);
const key_override_t shift_plus = ko_make_basic(MOD_MASK_SHIFT, KC_PLUS, KC_4);
const key_override_t shift_rbracket = ko_make_basic(MOD_MASK_SHIFT, KC_RBRACKET, KC_6);
const key_override_t shift_perc = ko_make_basic(MOD_MASK_SHIFT, KC_PERC, KC_8);
const key_override_t shift_circ = ko_make_basic(MOD_MASK_SHIFT, KC_CIRC, KC_DLR);
const key_override_t shift_kc_coln = ko_make_basic(MOD_MASK_SHIFT, KC_COLN, KC_SCOLON);

// Shift overrides for the numpad, to get hex numbers when shift held
const key_override_t num_hex_a = ko_make_with_layers(MOD_MASK_SHIFT, KC_1, LSFT(KC_A), (1 << SYMBOLS));
const key_override_t num_hex_b = ko_make_with_layers(MOD_MASK_SHIFT, KC_2, LSFT(KC_B), (1 << SYMBOLS));
const key_override_t num_hex_c = ko_make_with_layers(MOD_MASK_SHIFT, KC_3, LSFT(KC_C), (1 << SYMBOLS));
const key_override_t num_hex_d = ko_make_with_layers(MOD_MASK_SHIFT, KC_4, LSFT(KC_D), (1 << SYMBOLS));
const key_override_t num_hex_e = ko_make_with_layers(MOD_MASK_SHIFT, KC_5, LSFT(KC_E), (1 << SYMBOLS));
const key_override_t num_hex_f = ko_make_with_layers(MOD_MASK_SHIFT, KC_6, LSFT(KC_F), (1 << SYMBOLS));

// This globally defines all key overrides to be used
const key_override_t **key_overrides = (const key_override_t *[]){
    &shift_lbracket,
        &shift_lcbr,
        &shift_rcbr,
        &shift_lprn,
        &shift_equal,
        &shift_astr,
        &shift_rprn,
        &shift_plus,
        &shift_rbracket,
        &shift_perc,
        &shift_circ,
        &shift_kc_coln,
        &num_hex_a,
        &num_hex_b,
        &num_hex_c,
        &num_hex_d,
        &num_hex_e,
        &num_hex_f,
        NULL // Null terminate the array of overrides!
};

#endif
