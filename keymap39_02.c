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
// わかりやすいキー名定義
// ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

// 言語切替
#define EISU     KC_LNG2      // 英数（Mac）
#define KANA     KC_LNG1      // かな（Mac）
#define HANKAKU  KC_GRV       // 半角/全角（Win）

// モディファイア付きキー
#define CMD_EISU LGUI_T(KC_LNG2)  // 長押しCmd、タップで英数
#define SPC_CMD  LGUI_T(KC_SPC)   // 長押しCmd、タップでSpace（未使用）

// レイヤー切替
#define LOWER    MO(1)        // Layer 1（記号・数字）
#define SETTINGS MO(2)        // Layer 2（設定）

// よく使う記号
#define BRACK_L  KC_LBRC      // [
#define BRACK_R  KC_RBRC      // ]
#define PAREN_L  KC_LPRN      // (
#define PAREN_R  KC_RPRN      // )
#define BRACE_L  KC_LCBR      // {
#define BRACE_R  KC_RCBR      // }
#define EQUAL    KC_EQL       // =
#define MINUS    KC_MINS      // -
#define COLON    KC_COLN      // :
#define UNDER    KC_UNDS      // _
#define QUOTE    KC_QUOT      // '
#define DQUOTE   KC_DQUO      // "
#define BACKSLS  KC_BSLS      // \
#define PIPE     KC_PIPE      // |
#define GRAVE    KC_GRV       // `
#define TILDE    KC_TILD      // ~

// ナビゲーション
#define NAV_L    KC_LEFT      // ←
#define NAV_R    KC_RGHT      // →
#define NAV_U    KC_UP        // ↑
#define NAV_D    KC_DOWN      // ↓

// その他
#define ___      _______      // 透過（下のレイヤーのキー）
#define XXX      XXXXXXX      // 無効

// ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 物理キー位置の定義
// 
// 【指の割り当て】
// L5=左小指, L4=左薬指, L3=左中指, L2=左人差指, L1=左親指
// R1=右親指, R2=右人差指, R3=右中指, R4=右薬指, R5=右小指
// 
// 【段の名前】
// 上段 = Top row (数字・記号)
// 中段 = Home row (ホームポジション)
// 下段 = Bottom row
// 親指 = Thumb cluster
// ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

// コンボ定義（5個）
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
    [QWE_EISU]   = COMBO(qwe_combo, EISU),
    [ASD_KANA]   = COMBO(asd_combo, KANA),
    [ZXC_HZEN]   = COMBO(zxc_combo, HANKAKU),
    [QW_L2]      = COMBO(qw_combo, SETTINGS),
};

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

  // ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
  // Layer 0: Base（通常入力）
  // 
  // 【LAYOUT_right_ball】右手にトラックボール
  // 左手親指（6個）: Bsp, Alt, Cmd/英数, Ctrl, L1, Tab
  // 右手親指（3個）: Shift, Space, [🔴], Enter
  // ※ Enter は H+J コンボでも入力可能
  // ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
  [0] = LAYOUT_right_ball(
  //╭────────┬────────┬────────┬────────┬────────╮                    ╭────────┬────────┬────────┬────────┬────────╮
  //│  L5上  │  L4上  │  L3上  │  L2上  │  L2上  │                    │  R2上  │  R2上  │  R3上  │  R4上  │  R5上  │
      KC_Q    , KC_W    , KC_E    , KC_R    , KC_T    ,                       KC_Y    , KC_U    , KC_I    , KC_O    , KC_P    ,
  //├────────┼────────┼────────┼────────┼────────┤                    ├────────┼────────┼────────┼────────┼────────┤
  //│  L5中  │  L4中  │  L3中  │  L2中  │  L2中  │                    │  R2中  │  R2中  │  R3中  │  R4中  │  R5中  │
      KC_A    , KC_S    , KC_D    , KC_F    , KC_G    ,                       KC_H    , KC_J    , KC_K    , KC_L    , KC_SCLN ,
  //├────────┼────────┼────────┼────────┼────────┤                    ├────────┼────────┼────────┼────────┼────────┤
  //│  L5下  │  L4下  │  L3下  │  L2下  │  L2下  │                    │  R2下  │  R2下  │  R3下  │  R4下  │  R5下  │
      KC_Z    , KC_X    , KC_C    , KC_V    , KC_B    ,                       KC_N    , KC_M    , KC_COMM , KC_DOT  , KC_SLSH ,
  //╰────────┴────────┴────────┼────────┼────────┼────────╮  ╭────────┼────────┼────────┴────────┴────────┴────────╯
  //                            │  L1①  │  L1②  │  L1③  │  │  R1①  │  R1②  │
                                  KC_BSPC , KC_LALT , CMD_EISU,          KC_LSFT , KC_SPC  ,
  //                            ├────────┼────────┼────────┤  ├────────┼────────┤
  //                            │  L1④  │  L1⑤  │  L1⑥  │  │        │  R1③  │
                                            KC_LCTL , LOWER   , KC_TAB  ,          KC_ENT
  //                            ╰────────┴────────┴────────╯  ╰────────┴────────╯
  ),

  // ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
  // Layer 1: Lower（数字・記号・ナビゲーション）
  // 
  // 【配置戦略】
  // - 数字：左手上段1-5、右手上段6-0
  // - 超頻繁な括弧：人差し指ホーム
  //   {} → L1 + FJ、() → L1 + DK、[] → L1 + SL
  // - ナビゲーション：右手下段（矢印）、左手下段（ページ）
  // ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
  [1] = LAYOUT_right_ball(
  //╭────────┬────────┬────────┬────────┬────────╮                    ╭────────┬────────┬────────┬────────┬────────╮
  //│  L5上  │  L4上  │  L3上  │  L2上  │  L2上  │                    │  R2上  │  R2上  │  R3上  │  R4上  │  R5上  │
  //│   1    │   2    │   3    │   4    │   5    │                    │   6    │   7    │   8    │   9    │   0    │
      KC_1    , KC_2    , KC_3    , KC_4    , KC_5    ,                       KC_6    , KC_7    , KC_8    , KC_9    , KC_0    ,
  //├────────┼────────┼────────┼────────┼────────┤                    ├────────┼────────┼────────┼────────┼────────┤
  //│  L5中  │  L4中  │  L3中  │  L2中  │  L2中  │                    │  R2中  │  R2中  │  R3中  │  R4中  │  R5中  │
  //│   [    │   (    │   {    │   =    │  ESC   │                    │   -    │   }    │   )    │   ]    │   :    │
      BRACK_L , PAREN_L , BRACE_L , EQUAL   , KC_ESC  ,                       MINUS   , BRACE_R , PAREN_R , BRACK_R , COLON   ,
  //├────────┼────────┼────────┼────────┼────────┤                    ├────────┼────────┼────────┼────────┼────────┤
  //│  L5下  │  L4下  │  L3下  │  L2下  │  L2下  │                    │  R2下  │  R2下  │  R3下  │  R4下  │  R5下  │
  //│  Home  │  End   │  PgUp  │  PgDn  │   _    │                    │   ←    │   ↓    │   ↑    │   →    │   /    │
      KC_HOME , KC_END  , KC_PGUP , KC_PGDN , UNDER   ,                       NAV_L   , NAV_D   , NAV_U   , NAV_R   , KC_SLSH ,
  //╰────────┴────────┴────────┼────────┼────────┼────────╮  ╭────────┼────────┼────────┴────────┴────────┴────────╯
  //                            │  L1①  │  L1②  │  L1③  │  │  R1①  │  R1②  │
  //                            │   `    │   ~    │   '    │  │        │   ;    │
                                  GRAVE   , TILDE   , QUOTE   ,          ___     , KC_SCLN ,
  //                            ├────────┼────────┼────────┤  ├────────┼────────┤
  //                            │  L1④  │  L1⑤  │  L1⑥  │  │        │  R1③  │
  //                            │   "    │  透過  │  透過  │  │        │   \    │
                                            DQUOTE  , ___     , ___     ,          BACKSLS
  //                            ╰────────┴────────┴────────╯  ╰────────┴────────╯
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
  //╭────────┬────────┬────────┬────────┬────────╮                    ╭────────┬────────┬────────┬────────┬────────╮
  //│  L5上  │  L4上  │  L3上  │  L2上  │  L2上  │                    │  R2上  │  R2上  │  R3上  │  R4上  │  R5上  │
  //│   F1   │   F2   │   F3   │   F4   │   F5   │                    │   F6   │   F7   │   F8   │   F9   │  F10   │
      KC_F1   , KC_F2   , KC_F3   , KC_F4   , KC_F5   ,                       KC_F6   , KC_F7   , KC_F8   , KC_F9   , KC_F10  ,
  //├────────┼────────┼────────┼────────┼────────┤                    ├────────┼────────┼────────┼────────┼────────┤
  //│  L5中  │  L4中  │  L3中  │  L2中  │  L2中  │                    │  R2中  │  R2中  │  R3中  │  R4中  │  R5中  │
  //│   !    │   @    │   #    │   $    │   %    │                    │  左Clk │  右Clk │  中Clk │  F11   │  F12   │
      KC_EXLM , KC_AT   , KC_HASH , KC_DLR  , KC_PERC ,                       KC_BTN1 , KC_BTN2 , KC_BTN3 , KC_F11  , KC_F12  ,
  //├────────┼────────┼────────┼────────┼────────┤                    ├────────┼────────┼────────┼────────┼────────┤
  //│  L5下  │  L4下  │  L3下  │  L2下  │  L2下  │                    │  R2下  │  R2下  │  R3下  │  R4下  │  R5下  │
  //│   ^    │   &    │   *    │   +    │   |    │                    │ CPI-  │  CPI+  │ スクロ │  自動  │  保存  │
      KC_CIRC , KC_AMPR , KC_ASTR , KC_PLUS , PIPE    ,                       CPI_D100, CPI_I100, SCRL_TO , SSNP_FRE, KBC_SAVE,
  //╰────────┴────────┴────────┼────────┼────────┼────────╮  ╭────────┼────────┼────────┴────────┴────────┴────────╯
  //                            │  L1①  │  L1②  │  L1③  │  │  R1①  │  R1②  │
  //                            │ BOOT  │ RESET │  RGB   │  │  無効  │  無効  │
                                  QK_BOOT , KBC_RST , RGB_TOG ,          XXX     , XXX     ,
  //                            ├────────┼────────┼────────┤  ├────────┼────────┤
  //                            │  L1④  │  L1⑤  │  L1⑥  │  │        │  R1③  │
  //                            │  無効  │  無効  │  無効  │  │        │  無効  │
                                            XXX     , XXX     , XXX     ,          XXX
  //                            ╰────────┴────────┴────────╯  ╰────────┴────────╯
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