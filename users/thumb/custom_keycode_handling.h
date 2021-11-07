#ifndef CUSTOM_KEYCODE_HANDLING_H
#define CUSTOM_KEYCODE_HANDLING_H

#include QMK_KEYBOARD_H
#include "japanese.h"
#include "shift_caps.h"
#include "custom_layer_handling.h"

bool process_record_custom(uint16_t keycode, keyrecord_t *record);

#endif
