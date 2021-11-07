#ifndef SHIFT_CAPS_H
#define SHIFT_CAPS_H

#include QMK_KEYBOARD_H
#include "my_layers.h"

void caps_send(uint16_t shift_kc, uint16_t unshift_kc, bool *shifted, keyrecord_t *record);
bool custom_caps(uint16_t keycode, keyrecord_t *record);

#endif
