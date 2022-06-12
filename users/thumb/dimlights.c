#include "dimlights.h"

// When this key is pressed, all LEDs are turned off
// When pressed again, all LEDs are turned back on
void dimlights(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        //g_suspend_state = !g_suspend_state;
        //rgb_matrix_mode_noeeprom(RGB_MATRIX_SOLID_COLOR);
        rgblight_toggle();
    }
}
