/*
 * Keyball39 カスタムキーマップ 最終版
 * 
 * 【設計方針】
 * 1. 親指+人差し指で超頻繁な括弧を入力
 * 2. ホームポジションから指を離さない設計
 * 3. 使用頻度ベースの最適配置
 * 
 * 【記号使用頻度】
 * 超頻繁：{} () [] : = -
 * 頻繁：_ " ' ; / \
 * やや頻繁：` ~ その他記号
 * 
 * 【親指配置】
 * 左：Tab, 英数, Alt, Ctrl, Shift, L1
 * 右：Backspace, Space, [🔴], Enter
 */

#include QMK_KEYBOARD_H
#include "quantum.h"

// ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// コンボ定義（5個）
// Ctrl+Q、Alt+Qは削除（モディファイアとのコンボは動作しない）
// ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
enum combos {
    HJ_ENT,      // H+J → Enter（トラックボール誤爆防止）
    QWE_EISU,    // Q+W+E → 英数（Mac）
    ASD_KANA,    // A+S+D → かな（Mac）
    ZXC_HZEN,    // Z+X+C → 半角/全角（Win）
    QW_L2,       // Q+W → Layer 2（押している間）
    COMBO_LENGTH
};
uint16_t COMBO_LEN = COMBO_LENGTH;

// コンボキー定義
const uint16_t PROGMEM hj_combo[] = {KC_H, KC_J, COMBO_END};
const uint16_t PROGMEM qwe_combo[] = {KC_Q, KC_W, KC_E, COMBO_END};
const uint16_t PROGMEM asd_combo[] = {KC_A, KC_S, KC_D, COMBO_END};
const uint16_t PROGMEM zxc_combo[] = {KC_Z, KC_X, KC_C, COMBO_END};
const uint16_t PROGMEM qw_combo[] = {KC_Q, KC_W, COMBO_END};

combo_t key_combos[] = {
    [HJ_ENT]     = COMBO(hj_combo, KC_ENT),
    [QWE_EISU]   = COMBO(qwe_combo, KC_LNG2),
    [ASD_KANA]   = COMBO(asd_combo, KC_LNG1),
    [ZXC_HZEN]   = COMBO(zxc_combo, KC_GRV),
    [QW_L2]      = COMBO(qw_combo, MO(2)),
};

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

  // ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
  // Layer 0: Base（通常入力）
  // 
  // 【LAYOUT_right_ball】右手にトラックボール
  // 左手親指（6個）: Ctrl, 英数, Alt, Shift, L1, Tab
  // 右手親指（3個）: Bsp, Space(長押しCmd), Enter
  //                   [Bsp] [Spc/Cmd] [🔴] [Ent]
  // ※ Enter は H+J コンボでも入力可能
  // ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
  [0] = LAYOUT_right_ball(
    KC_Q     , KC_W     , KC_E     , KC_R     , KC_T     ,                            KC_Y     , KC_U     , KC_I     , KC_O     , KC_P     ,
    KC_A     , KC_S     , KC_D     , KC_F     , KC_G     ,                            KC_H     , KC_J     , KC_K     , KC_L     , KC_SCLN  ,
    KC_Z     , KC_X     , KC_C     , KC_V     , KC_B     ,                            KC_N     , KC_M     , KC_COMM  , KC_DOT   , KC_SLSH  ,
    KC_BSPC  , KC_LALT,   LGUI_T(KC_LNG2), KC_LCTL  , MO(1)    , KC_TAB   ,      KC_LSFT   , KC_SPC   ,                           KC_ENT
  ),

  // ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
  // Layer 1: Lower（数字・記号・ナビゲーション）
  // 
  // 【配置戦略】
  // - 数字：左手上段に1-5、右手上段に6-0
  // - 超頻繁な括弧：人差し指ホームで入力
  //   {} → L1 + FJ、() → L1 + DK、[] → L1 + SL
  // - ナビゲーション：右手下段（矢印）、左手下段（ページ）
  //
  // 右手親指: Del, ;, Backslash
  // ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
  [1] = LAYOUT_right_ball(
    KC_1     , KC_2     , KC_3     , KC_4     , KC_5     ,                            KC_6     , KC_7     , KC_8     , KC_9     , KC_0     ,
    KC_LBRC  , KC_LPRN  , KC_LCBR  , KC_EQL   , KC_ESC   ,                            KC_MINS  , KC_RCBR  , KC_RPRN  , KC_RBRC  , KC_COLN  ,
    KC_HOME  , KC_END   , KC_PGUP  , KC_PGDN  , KC_UNDS  ,                            KC_LEFT  , KC_DOWN  , KC_UP    , KC_RGHT  , KC_SLSH  ,
    KC_GRV   , KC_TILD  , KC_QUOT  , KC_DQUO  , _______  , _______  ,      _______  , KC_SCLN  ,                               KC_BSLS
  ),

  // ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
  // Layer 2: Settings（設定・ファンクション・マウス）
  // 
  // Q+Wコンボで起動（押している間有効）
  // - ファンクションキー（F1-F12）
  // - マウスボタン（右手中段）
  // - CPI調整、スクロールモード切替
  // ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
  [2] = LAYOUT_right_ball(
    KC_F1    , KC_F2    , KC_F3    , KC_F4    , KC_F5    ,                            KC_F6    , KC_F7    , KC_F8    , KC_F9    , KC_F10   ,
    KC_EXLM  , KC_AT    , KC_HASH  , KC_DLR   , KC_PERC  ,                            KC_BTN1  , KC_BTN2  , KC_BTN3  , KC_F11   , KC_F12   ,
    KC_CIRC  , KC_AMPR  , KC_ASTR  , KC_PLUS  , KC_PIPE  ,                            CPI_D100 , CPI_I100 , SCRL_TO  , SSNP_FRE , KBC_SAVE ,
    QK_BOOT  , KBC_RST  , RGB_TOG  , XXXXXXX  , XXXXXXX  , XXXXXXX  ,      XXXXXXX  , XXXXXXX  ,                               XXXXXXX
  ),
};
// clang-format on

// ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// レイヤー状態管理
// 
// Layer 1でスクロールモードを有効化
// トラックボールを転がすとスクロールとして動作
// ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
layer_state_t layer_state_set_user(layer_state_t state) {
    keyball_set_scroll_mode(get_highest_layer(state) == 1);
    return state;
}

// ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// OLED表示設定
// ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
#ifdef OLED_ENABLE
#    include "lib/oledkit/oledkit.h"

void oledkit_render_info_user(void) {
    keyball_oled_render_keyinfo();    // キー入力情報
    keyball_oled_render_ballinfo();   // トラックボール情報
    keyball_oled_render_layerinfo();  // レイヤー情報
}
#endif