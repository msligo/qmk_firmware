#include "shift_caps.h"

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

// Custom behaviour for Caps Lock key
// The symbols on the number row must be reprogrammed to work properly
// with Caps Lock. We also need to make sure they behave correctly when
// both Caps Lock and Shift are activated
// If custom behaviour is used, returns true, otherwise returns false
bool custom_caps(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case KC_LBRACKET:
            {
                static bool shifted;
                caps_send(KC_LBRACKET, KC_7, &shifted, record);
                return true;
            }
        case KC_LCBR:
            {
                static bool shifted;
                caps_send(KC_LCBR, KC_5, &shifted, record);
                return true;
            }
        case KC_RCBR:
            {
                static bool shifted;
                caps_send(KC_RCBR, KC_3, &shifted, record);
                return true;
            }
        case KC_LPRN:
            {
                static bool shifted;
                caps_send(KC_LPRN, KC_1, &shifted, record);
                return true;
            }
        case KC_EQUAL:
            {
                static bool shifted;
                caps_send(KC_EQUAL, KC_9, &shifted, record);
                return true;
            }
        case KC_ASTR:
            {
                static bool shifted;
                caps_send(KC_ASTR, KC_0, &shifted, record);
                return true;
            }
        case KC_RPRN:
            {
                static bool shifted;
                caps_send(KC_RPRN, KC_2, &shifted, record);
                return true;
            }
        case KC_PLUS:
            {
                static bool shifted;
                caps_send(KC_PLUS, KC_4, &shifted, record);
                return true;
            }
        case KC_RBRACKET:
            {
                static bool shifted;
                caps_send(KC_RBRACKET, KC_6, &shifted, record);
                return true;
            }
        case KC_PERC:
            {
                static bool shifted;
                caps_send(KC_PERC, KC_8, &shifted, record);
                return true;
            }
        case KC_MINUS:
            {
                static bool shifted;
                caps_send(KC_MINUS, KC_UNDERSCORE, &shifted, record);
                return true;
            }
        default:
            return false;
    }
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

