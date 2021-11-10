#ifndef PTT_H
#define PTT_H

#include QMK_KEYBOARD_H
#define PTT LALT(LGUI(LCTL(LSFT(KC_F22))))

void ptt(uint16_t keycode, keyrecord_t *record);

#endif
