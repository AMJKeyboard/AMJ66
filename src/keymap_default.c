#include "keymap_common.h"


// Default
#ifdef KEYMAP_SECTION_ENABLE
const uint8_t keymaps[KEYMAPS_COUNT][MATRIX_ROWS][MATRIX_COLS] __attribute__ ((section (".keymap.keymaps"))) = {
#else
const uint8_t keymaps[][MATRIX_ROWS][MATRIX_COLS] PROGMEM = {
#endif
    /* Keymap 0: Default Layer
     * ,----------------------------------------------------------.  ,---.
     * |Esc|  1|  2|  3|  4|  5|  6|  7|  8|  9|  0|  -|  +|  Bs  |  |INS|
     * |----------------------------------------------------------|  |---|
     * |Tab |  Q|  W|  E|  R|  T|  Y|  U|  I|  O|  P|  [|  ]|   \ |  |DEL|
     * |----------------------------------------------------------|  `---'
     * |Ctrl  |  A|  S|  D|  F|  G|  H|  J|  K|  L|  ;|  '|   Ent |
     * |-------------------------------------------------------------.
     * |Shift   |  Z|  X|  C|  V|  B|  N|  M|  ,|  .|  /|  Shift | UP|
     * |-----------------------------------------------------------------.
     * |Tab |Win |Alt |       Space           |Alt |Win | Fn |LFT|DWN|RIG|
     * `-----------------------------------------------------------------'
     */
    KEYMAP(
        ESC, 1,   2,   3,   4,   5,   6,   7,   8,   9,   0,   MINS,EQL, BSPC, NO,  INS,\
        TAB,    Q,   W,   E,   R,   T,   Y,   U,   I,   O,   P,   LBRC,RBRC,BSLS,   DEL, \
        CAPS,    A,   S,   D,   F,   G,   H,   J,   K,   L,   SCLN,QUOT,     ENT,\
        LSFT,  NO,  Z,   X,   C,   V,   B,   N,   M,   COMM, DOT, SLSH, RSFT,NO, UP,\
        LCTL,LGUI,LALT,NO,      SPC,                     RALT,RCTL, FN0, LEFT, DOWN, RGHT),
    /* Keymap 1: FN Layer
     * ,----------------------------------------------------------.  ,---.
     * |  `| F1| F2| F3| F4| F5| F6| F7| F8| F9|F10|F11|F12|Delete|  |INS|
     * |----------------------------------------------------------|  |---|
     * |     |   |Up |   |   |   |Cal|   |Ins|   |Psc|Slk|Pau|    |  |DEL|
     * |----------------------------------------------------------|  `---'
     * |      |Lef|Dow|Rig|   |   |   |   |   |   |Hom|PgU|       |
     * |-------------------------------------------------------------.
     * |        |   |App|Fn1|Fn2|Fn3|VoD|VoU|Mut|End|PgD|        |   |
     * |-----------------------------------------------------------------.
     * |    |    |    |                        |    |    |   |   |   |   |
     * `-----------------------------------------------------------------'
     */
    KEYMAP(
        GRV, F1,  F2,  F3,  F4,  F5,  F6,  F7,  F8,  F9,  F10, F11, F12, DEL, TRNS, TRNS,\
        TRNS,TRNS,UP,  TRNS,TRNS,TRNS,CALC,TRNS,INS, TRNS,PSCR,SLCK,PAUS,TRNS,      TRNS, \
        TRNS,LEFT,DOWN,RGHT,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,HOME,PGUP,     TRNS, \
        TRNS,NO,  TRNS,APP, FN1, FN2, FN3, VOLD,VOLU,MUTE,END, PGDN,TRNS,NO,  TRNS, \
        TRNS,TRNS,TRNS,TRNS,          TRNS,                    TRNS,TRNS,TRNS,TRNS,TRNS,TRNS)
};

/*
 * Fn action definition
 */
#ifdef KEYMAP_SECTION_ENABLE
const uint16_t fn_actions[FN_ACTIONS_COUNT] __attribute__ ((section (".keymap.fn_actions"))) = {
#else
const uint16_t fn_actions[] PROGMEM = {
#endif
    [0] = ACTION_LAYER_MOMENTARY(1),
    [1] = ACTION_BACKLIGHT_TOGGLE(),
    [2] = ACTION_BACKLIGHT_STEP(),
};

#ifdef KEYMAP_IN_EEPROM_ENABLE
uint16_t keys_count(void) {
    return sizeof(keymaps) / sizeof(keymaps[0]) * MATRIX_ROWS * MATRIX_COLS;
}

uint16_t fn_actions_count(void) {
    return sizeof(fn_actions) / sizeof(fn_actions[0]);
}
#endif

/*
 * Action macro definition
 */
enum macro_id {
    KEYPAD_00 = 0,
};

const macro_t *action_get_macro(keyrecord_t *record, uint8_t id, uint8_t opt)
{
    switch (id) {
        case KEYPAD_00:
            return (record->event.pressed ?
                    MACRO( T(P0), T(P0), END ) :
                    MACRO_NONE );
    }
    return MACRO_NONE;
}
