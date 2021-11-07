#ifndef CUSTOM_LAYER_HANDLING_H
#define CUSTOM_LAYER_HANDLING_H

#include QMK_KEYBOARD_H
#include "my_layers.h"

#define LCTL_LGUI  (MOD_BIT(KC_LCTL) | MOD_BIT(KC_LGUI))
#define SWAP_CTL_ALT KC_F24

// This function is called every time a layer change occurs
layer_state_t layer_state_set_user(layer_state_t state);

// Identical to set_mods(mods), except it sends the keyboard report immediately
void only_mod(int8_t mod);

// Makes sure that the layers for app switching are handled correctly
bool process_appshift_layer(uint16_t keycode, keyrecord_t *record);

#endif
