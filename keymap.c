/*
 * Keyball39 カスタムキーマップ
 * 設計方針: 2レイヤー + 設定レイヤー、ホームポジション重視、自然なコンボ
 * RGB無効化でコンボ拡張版
 */

#include QMK_KEYBOARD_H
#include "quantum.h"

// コンボ定義（全9個）
enum combos {
    JK_ESC,
    DF_TAB,
    KL_QUOT,
    UI_MINS,
    IO_UNDS,
    QW_CAPS,
    AS_HZEN,
    JKL_EISU,
    DFG_KANA,
    COMBO_LENGTH
};
uint16_t COMBO_LEN = COMBO_LENGTH;

const uint16_t PROGMEM jk_combo[] = {KC_J, KC_K, COMBO_END};
const uint16_t PROGMEM df_combo[] = {KC_D, KC_F, COMBO_END};
const uint16_t PROGMEM kl_combo[] = {KC_K, KC_L, COMBO_END};
const uint16_t PROGMEM ui_combo[] = {KC_U, KC_I, COMBO_END};
const uint16_t PROGMEM io_combo[] = {KC_I, KC_O, COMBO_END};
const uint16_t PROGMEM qw_combo[] = {KC_Q, KC_W, COMBO_END};
const uint16_t PROGMEM as_combo[] = {KC_A, KC_S, COMBO_END};
const uint16_t PROGMEM jkl_combo[] = {KC_J, KC_K, KC_L, COMBO_END};
const uint16_t PROGMEM dfg_combo[] = {KC_D, KC_F, KC_G, COMBO_END};

combo_t key_combos[] = {
    [JK_ESC]   = COMBO(jk_combo, KC_ESC),
    [DF_TAB]   = COMBO(df_combo, KC_TAB),
    [KL_QUOT]  = COMBO(kl_combo, KC_QUOT),
    [UI_MINS]  = COMBO(ui_combo, KC_MINS),
    [IO_UNDS]  = COMBO(io_combo, S(KC_MINS)),
    [QW_CAPS]  = COMBO(qw_combo, KC_CAPS),
    [AS_HZEN]  = COMBO(as_combo, KC_GRV),
    [JKL_EISU] = COMBO(jkl_combo, KC_LNG2),
    [DFG_KANA] = COMBO(dfg_combo, KC_LNG1),
};

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

  // Layer 0: 通常入力（QWERTY）
  [0] = LAYOUT_universal(
    KC_Q     , KC_W     , KC_E     , KC_R     , KC_T     ,                            KC_Y     , KC_U     , KC_I     , KC_O     , KC_P     ,
    KC_A     , KC_S     , KC_D     , KC_F     , KC_G     ,                            KC_H     , KC_J     , KC_K     , KC_L     , KC_SCLN  ,
    KC_Z     , KC_X     , KC_C     , KC_V     , KC_B     ,                            KC_N     , KC_M     , KC_COMM  , KC_DOT   , KC_SLSH  ,
    KC_LSFT  , KC_LCTL  , KC_LGUI  , KC_LALT  , KC_SPC   , MO(1)    ,      KC_BSPC  , KC_ENT   , XXXXXXX  , XXXXXXX  , XXXXXXX  , XXXXXXX
  ),

  // Layer 1: 記号・数字・ナビ
  [1] = LAYOUT_universal(
    KC_EXLM  , KC_AT    , KC_HASH  , KC_DLR   , KC_PERC  ,                            KC_CIRC  , KC_AMPR  , KC_ASTR  , KC_LPRN  , KC_RPRN  ,
    KC_1     , KC_2     , KC_3     , KC_4     , KC_5     ,                            KC_LEFT  , KC_DOWN  , KC_UP    , KC_RGHT  , KC_COLN  ,
    KC_6     , KC_7     , KC_8     , KC_9     , KC_0     ,                            KC_MINS  , KC_EQL   , KC_LBRC  , KC_RBRC  , KC_BSLS  ,
    KC_GRV   , KC_TILD  , KC_QUOT  , KC_DQUO  , KC_TAB   , _______  ,      KC_DEL   , MO(2)    , XXXXXXX  , XXXXXXX  , XXXXXXX  , XXXXXXX
  ),

  // Layer 2: 設定・ファンクション・マウス
  [2] = LAYOUT_universal(
    KC_F1    , KC_F2    , KC_F3    , KC_F4    , KC_F5    ,                            KC_F6    , KC_F7    , KC_F8    , KC_F9    , KC_F10   ,
    KC_MUTE  , KC_VOLD  , KC_VOLU  , KC_BRID  , KC_BRIU  ,                            KC_BTN1  , KC_BTN2  , KC_BTN3  , KC_F11   , KC_F12   ,
    RGB_TOG  , XXXXXXX  , XXXXXXX  , XXXXXXX  , XXXXXXX  ,                            CPI_D100 , CPI_I100 , SCRL_TO  , SSNP_FRE , KBC_SAVE ,
    QK_BOOT  , KBC_RST  , XXXXXXX  , XXXXXXX  , XXXXXXX  , XXXXXXX  ,      XXXXXXX  , _______  , XXXXXXX  , XXXXXXX  , XXXXXXX  , XXXXXXX
  ),
};
// clang-format on

layer_state_t layer_state_set_user(layer_state_t state) {
    keyball_set_scroll_mode(get_highest_layer(state) == 1);
    return state;
}

#ifdef OLED_ENABLE
#    include "lib/oledkit/oledkit.h"

void oledkit_render_info_user(void) {
    keyball_oled_render_keyinfo();
    keyball_oled_render_ballinfo();
    keyball_oled_render_layerinfo();
}
#endif