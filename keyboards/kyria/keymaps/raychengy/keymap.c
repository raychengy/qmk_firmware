/* Copyright 2019 Thomas Baart <thomas@splitkb.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include QMK_KEYBOARD_H

enum layers { _QWERTY_MAC = 0, _QWERTY_WIN, _LOWER, _RAISE, _ADJUST };
uint16_t alt_gui_tab_timer      = 0;
bool     is_alt_tab_active      = false;
bool     is_gui_tab_active      = false;
bool     is_gui_backtick_active = false;

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /*
     * Base Layer: QWERTY (MacOS)
     *
     * ,-------------------------------------------.                                  ,---------------------------------------------.
     * |  Tab    |   Q  |   W  |   E  |   R  |   T  |                                 |   Y  |   U  |   I  |   O  |   P  |  Bksp    |
     * |---------+------+------+------+------+------|                                 |------+------+------+------+------+----------|
     * |ESC/Lower|   A  |   S  |  D   |   F  |   G  |                                 |   H  |   J  |   K  |   L  | ;  : |' "/Lower |
     * |---------+------+------+------+------+------+--------------.  ,---------------+------+------+------+------+------+----------|
     * | LShift  |   Z  |   X  |   C  |   V  |   B  |Raise [|      |  |       |Raise ]|   N  |   M  | ,  < | . >  | /  ? |  RShift  |
     * `-----------------------+------+------+------+-------|      |  |       |-------+------+------+------+------------------------'
     *                         |DF    | Ctrl | Alt  | Gui   | Enter|  | Space | Gui   | Alt  | Ctrl |TG    |
     *                         |_Q_WIN|      |      |       |      |  |       |       |      |      |Adjust|
     *                         `-----------------------------------'  `------------------------------------'
     */
    [_QWERTY_MAC] = LAYOUT(KC_TAB, KC_Q, KC_W, KC_E, KC_R, KC_T, KC_Y, KC_U, KC_I, KC_O, KC_P, KC_BSPACE, LT(_LOWER, KC_ESC), KC_A, KC_S, KC_D, KC_F, KC_G, KC_H, KC_J, KC_K, KC_L, KC_SCLN, LT(_LOWER, KC_QUOT), KC_LSFT, KC_Z, KC_X, KC_C, KC_V, KC_B, LT(_RAISE, KC_LBRC), XXXXXXX, XXXXXXX, LT(_RAISE, KC_RBRC), KC_N, KC_M, KC_COMM, KC_DOT, KC_SLSH, KC_LSFT, DF(_QWERTY_WIN), KC_LCTRL, KC_LALT, KC_LGUI, KC_ENTER, KC_SPACE, KC_LGUI, KC_RALT, KC_LCTRL, TG(_ADJUST)),
    /*
     * Base Layer: QWERTY (Windows)
     *
     * ,-------------------------------------------.                                  ,---------------------------------------------.
     * |  Tab    |   Q  |   W  |   E  |   R  |   T  |                                 |   Y  |   U  |   I  |   O  |   P  |  Bksp    |
     * |---------+------+------+------+------+------|                                 |------+------+------+------+------+----------|
     * |ESC/Lower|   A  |   S  |  D   |   F  |   G  |                                 |   H  |   J  |   K  |   L  | ;  : |' "/Lower |
     * |---------+------+------+------+------+------+--------------.  ,---------------+------+------+------+------+------+----------|
     * | LShift  |   Z  |   X  |   C  |   V  |   B  |Raise [|      |  |       |Raise ]|   N  |   M  | ,  < | . >  | /  ? |  RShift  |
     * `-----------------------+------+------+------+-------|      |  |       |-------+------+------+------+------------------------'
     *                         |DF    | Gui  | Alt  | Ctrl  | Enter|  | Space | Ctrl  | Alt  | Gui  |TG    |
     *                         |_Q_MAC|      |      |       |      |  |       |       |      |      |Adjust|
     *                         `-----------------------------------'  `------------------------------------'
     */
    [_QWERTY_WIN] = LAYOUT(KC_TAB, KC_Q, KC_W, KC_E, KC_R, KC_T, KC_Y, KC_U, KC_I, KC_O, KC_P, KC_BSPACE, LT(_LOWER, KC_ESC), KC_A, KC_S, KC_D, KC_F, KC_G, KC_H, KC_J, KC_K, KC_L, KC_SCLN, LT(_LOWER, KC_QUOT), KC_LSFT, KC_Z, KC_X, KC_C, KC_V, KC_B, LT(_RAISE, KC_LBRC), XXXXXXX, XXXXXXX, LT(_RAISE, KC_RBRC), KC_N, KC_M, KC_COMM, KC_DOT, KC_SLSH, KC_LSFT, DF(_QWERTY_MAC), KC_LGUI, KC_LALT, KC_LCTRL, KC_ENTER, KC_SPACE, KC_LCTRL, KC_LALT, KC_LGUI, TG(_ADJUST)),
    /*
     * Lower Layer: Symbols
     *
     * ,-------------------------------------------.                              ,-------------------------------------------.
     * |        |  !   |  @   |  #   |  $   | ` ~  |                              |      |      |      |      |      |  Del   |
     * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
     * |        |   %  |  ^   |  &   |  *   | | \  |                              |      | left | down | up   | right|        |
     * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
     * |        |  (   |  )   |  _ - |  + = |      |      |      |  |      |      |      |      |      |      |      |        |
     * `----------------------+------+------+------+------|      |  |      |------+------+------+------+----------------------'
     *                        |      |      |      |      |      |  |      |      |      |      |      |
     *                        |      |      |      |      |      |  |      |      |      |      |      |
     *                        `----------------------------------'  `----------------------------------'
     */
    [_LOWER] = LAYOUT(_______, KC_EXLM, KC_AT, KC_HASH, KC_DLR, KC_GRAVE, _______, _______, _______, _______, _______, _______, _______, KC_PERC, KC_CIRC, KC_AMPR, KC_ASTR, KC_BSLS, _______, KC_LEFT, KC_DOWN, KC_UP, KC_RIGHT, _______, _______, KC_LPRN, KC_LPRN, KC_MINS, KC_EQL, _______, _______, XXXXXXX, XXXXXXX, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______),
    /*
     * Raise Layer: Number keys, media, navigation
     *
     * ,-------------------------------------------.                              ,-------------------------------------------.
     * |        |      |      |      |      |      |                              |   *  |  7   |  8   |  9   |  -   |        |
     * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
     * |        |      | Prev | Play | Next | VolUp|                              |   /  |  4   |  5   |  6   |  +   |        |
     * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
     * |        |      |      |      | Mute | VolDn|      |      |  |      |  .   |   0  |  1   |  2   |  3   |  =   |        |
     * `----------------------+------+------+------+------|      |  |      |------+------+------+------+----------------------'
     *                        |      |      |      |      |      |  |      |      |      |      |      |
     *                        |      |      |      |      |      |  |      |      |      |      |      |
     *                        `----------------------------------'  `----------------------------------'
     */
    [_RAISE] = LAYOUT(_______, _______, _______, _______, _______, _______, KC_PAST, KC_P7, KC_P8, KC_P9, KC_PMNS, _______, _______, _______, KC_MPRV, KC_MPLY, KC_MNXT, KC_VOLU, KC_PSLS, KC_P4, KC_P5, KC_P6, KC_PPLS, _______, _______, _______, _______, _______, KC_MUTE, KC_VOLD, _______, _______, _______, KC_PDOT, KC_P0, KC_P1, KC_P2, KC_P3, KC_PEQL, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______),
    /*
     * Adjust Layer: Function keys, RGB
     *
     * ,-------------------------------------------.                              ,-------------------------------------------.
     * |        |      |      |      |      |      |                              |      | F9   |  F10 | F11  | F12  |        |
     * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
     * |        | TOG  | SAI  | HUI  | VAI  | MOD  |                              |      | F5   |  F6  | F7   | F8   |        |
     * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
     * |        |      | SAD  | HUD  | VAD  | RMOD |      |      |  |      |      |      | F1   |  F2  | F3   | F4   |        |
     * `----------------------+------+------+------+------|      |  |      |------+------+------+------+----------------------'
     *                        |      |      |      |      |      |  |      |      |      |      |      |
     *                        |      |      |      |      |      |  |      |      |      |      |      |
     *                        `----------------------------------'  `----------------------------------'
     */
    [_ADJUST] = LAYOUT(_______, _______, _______, _______, _______, _______, _______, KC_F9, KC_F10, KC_F11, KC_F12, _______, _______, RGB_TOG, RGB_SAI, RGB_HUI, RGB_VAI, RGB_MOD, _______, KC_F5, KC_F6, KC_F7, KC_F8, _______, _______, _______, RGB_SAD, RGB_HUD, RGB_VAD, RGB_RMOD, _______, _______, _______, _______, _______, KC_F1, KC_F2, KC_F3, KC_F4, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______),
    // /*
    //  * Layer template
    //  *
    //  * ,-------------------------------------------.                              ,-------------------------------------------.
    //  * |        |      |      |      |      |      |                              |      |      |      |      |      |        |
    //  * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
    //  * |        |      |      |      |      |      |                              |      |      |      |      |      |        |
    //  * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
    //  * |        |      |      |      |      |      |      |      |  |      |      |      |      |      |      |      |        |
    //  * `----------------------+------+------+------+------|      |  |      |------+------+------+------+----------------------'
    //  *                        |      |      |      |      |      |  |      |      |      |      |      |
    //  *                        |      |      |      |      |      |  |      |      |      |      |      |
    //  *                        `----------------------------------'  `----------------------------------'
    //  */
    //     [_LAYERINDEX] = LAYOUT(
    //       _______, _______, _______, _______, _______, _______,                                     _______, _______, _______, _______, _______, _______,
    //       _______, _______, _______, _______, _______, _______,                                     _______, _______, _______, _______, _______, _______,
    //       _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
    //                                  _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
    //     ),
};
// clang-format on

int get_current_layer(void) { return get_highest_layer(layer_state) >= _LOWER ? get_highest_layer(layer_state) : default_layer_state >> 1; }

#ifdef OLED_DRIVER_ENABLE
oled_rotation_t oled_init_user(oled_rotation_t rotation) { return OLED_ROTATION_180; }

static void render_kyria_logo(void) {
    static const char PROGMEM kyria_logo[] = {0,  0,  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   128, 128, 192, 224, 240, 112, 120, 56,  60,  28, 30, 14, 14, 14, 7,  7,   7,   7,   7,  7, 7,   7,   7, 7,  7,   7,   7,   7,  7,  7,  14, 14, 14,  30,  28,  60,  56,  120, 112, 240, 224, 192, 128, 128, 0,   0,  0,  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  0,  0,   0,   0,   0,   192, 224, 240, 124, 62,  31,  15,  7,   3,   1,   128, 192, 224, 240, 120, 56,  60, 28, 30, 14, 14, 7,   7,   135, 231, 127, 31,  255, 255, 31,  127, 231, 135, 7,  7,  14,
                                              14, 30, 28,  60,  56,  120, 240, 224, 192, 128, 1,   3,   7,   15,  31,  62,  124, 240, 224, 192, 0,   0,   0,  0,  0,  0,  0,  0,  0,   0,   0,   0,  0, 0,   0,   0, 0,  0,   0,   0,   0,  0,  0,  0,  0,  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  0,  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   240, 252, 255, 31,  7,   1,  0,   0,   192, 240, 252, 254, 255, 247, 243, 177, 176, 48,  48,  48,  48,  48,  48,  48,  120, 254, 135, 1,   0,   0,   255, 255, 0,   0,   1,   135, 254, 120, 48,  48,  48, 48, 48,  48,  48,  176, 177, 243, 247, 255, 254, 252, 240, 192, 0,   0,   1,   7,   31,  255, 252, 240, 0,  0,  0,  0,  0,  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  0,  0,
                                              0,  0,  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  0,  0,  0,  0,  0,  0,   0,   0,   0,  0, 0,   0,   0, 0,  0,   0,   0,   0,  0,  0,  0,  0,  255, 255, 255, 0,   0,   0,   0,   0,   254, 255, 255, 1,   1,   7,  30, 120, 225, 129, 131, 131, 134, 134, 140, 140, 152, 152, 177, 183, 254, 248, 224, 255, 255, 224, 248, 254, 183, 177, 152, 152, 140, 140, 134, 134, 131, 131, 129, 225, 120, 30, 7,   1,   1,   255, 255, 254, 0,   0,   0,   0,   0,   255, 255, 255, 0,   0,   0,   0,   255, 255, 0,   0,   192, 192, 48,  48,  0,   0,   240, 240, 0,   0,   0,   0,   0,  0,  240, 240, 0,   0,   240, 240, 192, 192, 48,  48,  48,  48,  192, 192, 0,   0,   48,  48,  243, 243, 0,  0,  0,  0,  0,  0,   48,  48,  48,  48,  48,  48,  192, 192, 0,   0,   0,   0,  0,  0,
                                              0,  0,  255, 255, 255, 0,   0,   0,   0,   0,   127, 255, 255, 128, 128, 224, 120, 30,  135, 129, 193, 193, 97, 97, 49, 49, 25, 25, 141, 237, 127, 31, 7, 255, 255, 7, 31, 127, 237, 141, 25, 25, 49, 49, 97, 97,  193, 193, 129, 135, 30,  120, 224, 128, 128, 255, 255, 127, 0,  0,  0,   0,   0,   255, 255, 255, 0,   0,   0,   0,   63,  63,  3,   3,   12,  12,  48,  48,  0,   0,   0,   0,   51,  51,  51,  51,  51,  51,  15,  15,  0,   0,   63,  63,  0,  0,   0,   0,   0,   0,   0,   0,   0,   0,   48,  48,  63,  63,  48,  48,  0,   0,   12,  12,  51,  51,  51,  51,  51,  51,  63,  63,  0,   0,   0,   0,   0,   0,   0,   0,  0,  15,  63,  255, 248, 224, 128, 0,   0,   3,   15,  63,  127, 255, 239, 207, 141, 13,  12,  12,  12,  12, 12, 12, 12, 30, 127, 225, 128, 0,   0,   255, 255, 0,   0,   128, 225, 127, 30, 12, 12,
                                              12, 12, 12,  12,  12,  13,  141, 207, 239, 255, 127, 63,  15,  3,   0,   0,   128, 224, 248, 255, 63,  15,  0,  0,  0,  0,  0,  0,  0,   0,   0,   0,  0, 0,   0,   0, 0,  0,   0,   0,   0,  0,  0,  0,  0,  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  0,  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   3,   7,   15,  62, 124, 248, 240, 224, 192, 128, 1,   3,   7,   15,  30,  28,  60,  56,  120, 112, 112, 224, 224, 225, 231, 254, 248, 255, 255, 248, 254, 231, 225, 224, 224, 112, 112, 120, 56, 60, 28,  30,  15,  7,   3,   1,   128, 192, 224, 240, 248, 124, 62,  15,  7,   3,   0,   0,   0,   0,   0,  0,  0,  0,  0,  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  0,  0,
                                              0,  0,  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  0,  0,  0,  0,  0,  0,   0,   0,   0,  0, 0,   0,   0, 0,  0,   0,   0,   0,  0,  0,  0,  0,  0,   0,   0,   0,   0,   0,   0,   0,   0,   1,   1,   3,   7,   15, 14, 30,  28,  60,  56,  120, 112, 112, 112, 224, 224, 224, 224, 224, 224, 224, 224, 224, 224, 224, 224, 224, 224, 224, 224, 112, 112, 112, 120, 56,  60,  28,  30,  14,  15,  7,  3,   1,   1,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  0,  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  0,  0,  0,  0,  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0};
    oled_write_raw_P(kyria_logo, sizeof(kyria_logo));
}

static void render_qmk_logo(void) {
    static const char PROGMEM qmk_logo[] = {0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8a, 0x8b, 0x8c, 0x8d, 0x8e, 0x8f, 0x90, 0x91, 0x92, 0x93, 0x94, 0xa0, 0xa1, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7, 0xa8, 0xa9, 0xaa, 0xab, 0xac, 0xad, 0xae, 0xaf, 0xb0, 0xb1, 0xb2, 0xb3, 0xb4, 0xc0, 0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xcb, 0xcc, 0xcd, 0xce, 0xcf, 0xd0, 0xd1, 0xd2, 0xd3, 0xd4, 0};

    oled_write_P(qmk_logo, false);
}

static void render_status(void) {
    // QMK Logo and version information
    render_qmk_logo();
    oled_write_P(PSTR("Kyria rev1.0\n\n"), false);

    // Host Keyboard Layer Status
    oled_write_P(PSTR("Layer: "), false);
    switch (get_current_layer()) {
        case _QWERTY_MAC:
            oled_write_P(PSTR("Default MacOSX\n"), false);
            break;
        case _QWERTY_WIN:
            oled_write_P(PSTR("Default Win\n"), false);
            break;
        case _LOWER:
            oled_write_P(PSTR("Lower\n"), false);
            break;
        case _RAISE:
            oled_write_P(PSTR("Raise\n"), false);
            break;
        case _ADJUST:
            oled_write_P(PSTR("Adjust\n"), false);
            break;
        default:
            oled_write_P(PSTR("Undefined\n"), false);
            break;
    }
}

void oled_task_user(void) {
    if (is_keyboard_master()) {
        render_status();  // Renders the current keyboard state (layer, lock, caps, scroll, etc)
    } else {
        render_kyria_logo();
    }
}
#endif

#ifdef ENCODER_ENABLE
void volume_control(bool clockwise) {
    if (clockwise) {
        tap_code(KC_VOLU);
    } else {
        tap_code(KC_VOLD);
    }
}

void scroll_pgup_pgdn(bool clockwise) {
    if (clockwise) {
        tap_code(KC_PGDN);
    } else {
        tap_code(KC_PGUP);
    }
}

void scroll_horizontal_by_words(bool clockwise, bool is_mac) {
    if (is_mac) {
        if (clockwise) {
            tap_code16(A(KC_RIGHT));
        } else {
            tap_code16(A(KC_LEFT));
        }
    } else {
        if (clockwise) {
            tap_code16(C(KC_RIGHT));
        } else {
            tap_code16(C(KC_LEFT));
        }
    }
}

void window_tabbing(bool clockwise, bool is_mac) {
    if (is_mac) {
        if (clockwise) {
            if (!is_gui_tab_active) {
                is_gui_tab_active = true;
                register_code(KC_LGUI);
            }
            alt_gui_tab_timer = timer_read();
            tap_code16(KC_TAB);
        } else {
            if (!is_gui_tab_active) {
                is_gui_tab_active = true;
                register_code(KC_LGUI);
            }
            alt_gui_tab_timer = timer_read();
            tap_code16(S(KC_TAB));
        }
    } else {
        if (clockwise) {
            if (!is_alt_tab_active) {
                is_alt_tab_active = true;
                register_code(KC_LALT);
            }
            alt_gui_tab_timer = timer_read();
            tap_code16(KC_TAB);
        } else {
            if (!is_alt_tab_active) {
                is_alt_tab_active = true;
                register_code(KC_LALT);
            }
            alt_gui_tab_timer = timer_read();
            tap_code16(S(KC_TAB));
        }
    }
}

void cycle_open_windows(bool clockwise) {
    if (clockwise) {
        if (!is_gui_backtick_active) {
            is_gui_backtick_active = true;
            register_code(KC_LGUI);
        }
        alt_gui_tab_timer = timer_read();
        tap_code16(KC_GRAVE);
    } else {
        if (!is_gui_backtick_active) {
            is_gui_backtick_active = true;
            register_code(KC_LGUI);
        }
        alt_gui_tab_timer = timer_read();
        tap_code16(S(KC_GRAVE));
    }
}

void encoder_update_user(uint8_t index, bool clockwise) {
    if (index == 0) {
        switch (default_layer_state >> 1) {
            case _QWERTY_MAC:
                switch (get_current_layer()) {
                    case _LOWER:
                        cycle_open_windows(clockwise);
                        break;
                    case _RAISE:
                        break;
                    case _ADJUST:
                        volume_control(clockwise);
                        break;
                    default:
                        window_tabbing(clockwise, true);
                        break;
                }
                break;
            case _QWERTY_WIN:
                switch (get_current_layer()) {
                    case _LOWER:
                        break;
                    case _RAISE:
                        break;
                    case _ADJUST:
                        volume_control(clockwise);
                        break;
                    default:
                        window_tabbing(clockwise, false);
                        break;
                }
                break;
        }
    } else if (index == 1) {
        switch (default_layer_state >> 1) {
            case _QWERTY_MAC:
                switch (get_current_layer()) {
                    case _LOWER:
                        scroll_horizontal_by_words(clockwise, true);
                        break;
                    case _RAISE:
                        break;
                    case _ADJUST:
                        break;
                    default:
                        scroll_pgup_pgdn(clockwise);
                        break;
                }
                break;
            case _QWERTY_WIN:
                switch (get_current_layer()) {
                    case _LOWER:
                        scroll_horizontal_by_words(clockwise, false);
                        break;
                    case _RAISE:
                        break;
                    case _ADJUST:
                        break;
                    default:
                        scroll_pgup_pgdn(clockwise);
                        break;
                }
                break;
        }
    }
}
#endif

void matrix_scan_user(void) {
    if (is_alt_tab_active) {
        if (timer_elapsed(alt_gui_tab_timer) > 1250) {
            unregister_code(KC_LALT);
            is_alt_tab_active = false;
        }
    }

    if (is_gui_tab_active) {
        if (timer_elapsed(alt_gui_tab_timer) > 1250) {
            unregister_code(KC_LGUI);
            is_gui_tab_active = false;
        }
    }

    if (is_gui_backtick_active) {
        if (timer_elapsed(alt_gui_tab_timer) > 1250) {
            unregister_code(KC_LGUI);
            is_gui_backtick_active = false;
        }
    }
}
