/*
 * Keyball39 カスタムキーマップ 最適化版
 * 
 * 【親指キー配置（全て二役）】
 * 左親指③：Cmd
 * 左親指④：タップ=Backspace、ホールド=Layer1（記号・ナビ）
 * 左親指⑤：タップ=Space、ホールド=Layer2（数字テンキー）
 * 左親指⑥：タップ=Ctrl、ホールド=Layer3（設定）
 * 右親指①：タップ=Alt、ホールド=Shift
 * 右親指②：Tab
 * 
 * 【特殊配置】
 * R5中（;の位置）：Enter
 * 
 * 【括弧配置（隣り合わせ）】
 * Layer 1中段：[ ] ( ) { }
 * 
 * 【コンボ】
 * D+F → 英数⇔かなトグル
 */

#include QMK_KEYBOARD_H
#include "quantum.h"

// ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// カスタムキーコード
// ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
enum custom_keycodes {
    LANG_TOG = SAFE_RANGE,  // 言語トグル
};

// 言語状態管理（true=かな、false=英数）
static bool is_kana_mode = false;

// ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// わかりやすいキー名定義
// ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

// 言語切替
#define EISU     KC_LNG2      // 英数（Mac）
#define KANA     KC_LNG1      // かな（Mac）

// タップ/ホールド（親指の二役）
#define ALT_SFT  LALT_T(KC_LSFT)  // タップ=Alt、ホールド=Shift
#define SPC_L2   LT(2, KC_SPC)    // タップ=Space、ホールド=Layer2
#define BSP_L1   LT(1, KC_BSPC)   // タップ=Backspace、ホールド=Layer1
#define CTL_L3   LT(3, KC_LCTL)   // タップ=Ctrl、ホールド=Layer3

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
#define ___      _______      // 透過
#define XXX      XXXXXXX      // 無効

// ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// コンボ定義
// ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
enum combos {
    DF_LANG,     // D+F → 言語トグル
    COMBO_LENGTH
};
uint16_t COMBO_LEN = COMBO_LENGTH;

const uint16_t PROGMEM df_combo[] = {KC_D, KC_F, COMBO_END};

combo_t key_combos[] = {
    [DF_LANG]    = COMBO(df_combo, LANG_TOG),
};

// ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// カスタムキー処理
// ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case LANG_TOG:
            if (record->event.pressed) {
                // 状態をトグル
                is_kana_mode = !is_kana_mode;
                
                // トグル後の状態に応じてキーを送信
                if (is_kana_mode) {
                    tap_code(KANA);  // かなモードへ
                } else {
                    tap_code(EISU);  // 英数モードへ
                }
            }
            return false;
    }
    return true;
}

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

  // ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
  // Layer 0: Base（通常入力）
  // 
  // 【親指配置】
  //   左③：Cmd
  //   左④：タップ=Backspace、ホールド=Layer1
  //   左⑤：タップ=Space、ホールド=Layer2
  //   左⑥：タップ=Ctrl、ホールド=Layer3
  //   右①：タップ=Alt、ホールド=Shift
  //   右②：Tab
  // 【特殊配置】
  //   R5中：Enter（セミコロンの位置）
  // 【コンボ】
  //   D+F → 英数⇔かなトグル
  // ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
  [0] = LAYOUT_right_ball(
  //╭────────┬────────┬────────┬────────┬────────╮                    ╭────────┬────────┬────────┬────────┬────────╮
  //│  L5上  │  L4上  │  L3上  │  L2上  │  L2上  │                    │  R2上  │  R2上  │  R3上  │  R4上  │  R5上  │
      KC_Q    , KC_W    , KC_E    , KC_R    , KC_T    ,                       KC_Y    , KC_U    , KC_I    , KC_O    , KC_P    ,
  //├────────┼────────┼────────┼────────┼────────┤                    ├────────┼────────┼────────┼────────┼────────┤
  //│  L5中  │  L4中  │  L3中  │  L2中  │  L2中  │                    │  R2中  │  R2中  │  R3中  │  R4中  │  R5中  │
  //│   A    │   S    │   D    │   F    │   G    │                    │   H    │   J    │   K    │   L    │ Enter  │
      KC_A    , KC_S    , KC_D    , KC_F    , KC_G    ,                       KC_H    , KC_J    , KC_K    , KC_L    , KC_ENT  ,
  //├────────┼────────┼────────┼────────┼────────┤                    ├────────┼────────┼────────┼────────┼────────┤
  //│  L5下  │  L4下  │  L3下  │  L2下  │  L2下  │                    │  R2下  │  R2下  │  R3下  │  R4下  │  R5下  │
      KC_Z    , KC_X    , KC_C    , KC_V    , KC_B    ,                       KC_N    , KC_M    , KC_COMM , KC_DOT  , KC_SLSH ,
  //╰────────┴────────┴────────┼────────┼────────┼────────╮  ╭────────┼────────┼────────┴────────┴────────┴────────╯
  //                            │  L1①  │  L1②  │  L1③  │  │  R1①  │  R1②  │
  //                            │  無効  │  無効  │  Cmd   │  │Alt/Sft│  Tab   │
                                  XXX     , XXX     , KC_LGUI ,          ALT_SFT , KC_TAB  ,
  //                            ├────────┼────────┼────────┤  ├────────┼────────┤
  //                            │  L1④  │  L1⑤  │  L1⑥  │  │        │  R1③  │
  //                            │Bsp/L1  │Spc/L2 │Ctl/L3 │  │        │  無効  │
                                            BSP_L1  , SPC_L2  , CTL_L3  ,          XXX
  //                            ╰────────┴────────┴────────╯  ╰────────┴────────╯
  ),

  // ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
  // Layer 1: 記号・ナビゲーション（左親指④でアクティブ）
  // 
  // 【設計】左手でレイヤー保持、右手で操作
  // - 右手上段：カーソルキー（U=←、I=↑、O=→）
  // - 右手中段：括弧類（隣り合わせ）
  //   H: [  J: ]  K: (  L: )  Enter位置: { R1①: }
  // - 右手下段：補助記号
  // ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
  [1] = LAYOUT_right_ball(
  //╭────────┬────────┬────────┬────────┬────────╮                    ╭────────┬────────┬────────┬────────┬────────╮
  //│   `    │   ~    │   '    │   "    │  Esc   │                    │   -    │   ←    │   ↑    │   →    │   \    │
      GRAVE   , TILDE   , QUOTE   , DQUOTE  , KC_ESC  ,                       MINUS   , NAV_L   , NAV_U   , NAV_R   , BACKSLS ,
  //├────────┼────────┼────────┼────────┼────────┤                    ├────────┼────────┼────────┼────────┼────────┤
  //│   |    │  PgUp  │  PgDn  │        │        │                    │   [    │   ]    │   (    │   )    │   {    │
      PIPE    , KC_PGUP , KC_PGDN , XXX     , XXX     ,                       BRACK_L , BRACK_R , PAREN_L , PAREN_R , BRACE_L ,
  //├────────┼────────┼────────┼────────┼────────┤                    ├────────┼────────┼────────┼────────┼────────┤
  //│        │  Home  │  End   │        │        │                    │   _    │   :    │   =    │   +    │   /    │
      XXX     , KC_HOME , KC_END  , XXX     , XXX     ,                       UNDER   , COLON   , EQUAL   , KC_PLUS , KC_SLSH ,
  //╰────────┴────────┴────────┼────────┼────────┼────────╮  ╭────────┼────────┼────────┴────────┴────────┴────────╯
  //                            │  無効  │  無効  │        │  │   }    │  Del   │
                                  XXX     , XXX     , XXX     ,          BRACE_R , KC_DEL  ,
  //                            ├────────┼────────┼────────┤  ├────────┼────────┤
  //                            │  透過  │        │   ;    │  │        │  無効  │
                                            ___     , XXX     , KC_SCLN ,          XXX
  //                            ╰────────┴────────┴────────╯  ╰────────┴────────╯
  ),

  // ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
  // Layer 2: 数字テンキー（左親指⑤でアクティブ）
  // 
  // 【設計】左手でレイヤー保持、右手でテンキー入力
  // 右手配置：
  //   7 8 9
  //   4 5 6
  //   1 2 3
  //     0
  // R5中：Enter（数字入力後の確定に便利）
  // ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
  [2] = LAYOUT_right_ball(
  //╭────────┬────────┬────────┬────────┬────────╮                    ╭────────┬────────┬────────┬────────┬────────╮
      XXX     , XXX     , XXX     , XXX     , XXX     ,                       XXX     , KC_7    , KC_8    , KC_9    , XXX     ,
  //├────────┼────────┼────────┼────────┼────────┤                    ├────────┼────────┼────────┼────────┼────────┤
      XXX     , XXX     , XXX     , XXX     , XXX     ,                       XXX     , KC_4    , KC_5    , KC_6    , KC_ENT  ,
  //├────────┼────────┼────────┼────────┼────────┤                    ├────────┼────────┼────────┼────────┼────────┤
      XXX     , XXX     , XXX     , XXX     , XXX     ,                       XXX     , KC_1    , KC_2    , KC_3    , XXX     ,
  //╰────────┴────────┴────────┼────────┼────────┼────────╮  ╭────────┼────────┼────────┴────────┴────────┴────────╯
                                  XXX     , XXX     , XXX     ,          KC_0    , XXX     ,
  //                            ├────────┼────────┼────────┤  ├────────┼────────┤
                                            XXX     , ___     , XXX     ,          XXX
  //                            ╰────────┴────────┴────────╯  ╰────────┴────────╯
  ),

  // ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
  // Layer 3: 設定・ファンクション（左親指⑥でアクティブ）
  // 
  // 【設計】左手でレイヤー保持、右手で操作
  // - ファンクションキー：F5、F10のみ
  // - マウスボタン
  // - CPI調整、スクロールモード
  // ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
  [3] = LAYOUT_right_ball(
  //╭────────┬────────┬────────┬────────┬────────╮                    ╭────────┬────────┬────────┬────────┬────────╮
  //│        │        │        │        │   F5   │                    │  F10   │        │        │        │        │
      XXX     , XXX     , XXX     , XXX     , KC_F5   ,                       KC_F10  , XXX     , XXX     , XXX     , XXX     ,
  //├────────┼────────┼────────┼────────┼────────┤                    ├────────┼────────┼────────┼────────┼────────┤
  //│   !    │   @    │   #    │   $    │   %    │                    │  左Clk │  右Clk │  中Clk │        │        │
      KC_EXLM , KC_AT   , KC_HASH , KC_DLR  , KC_PERC ,                       KC_BTN1 , KC_BTN2 , KC_BTN3 , XXX     , XXX     ,
  //├────────┼────────┼────────┼────────┼────────┤                    ├────────┼────────┼────────┼────────┼────────┤
  //│   ^    │   &    │   *    │        │        │                    │ CPI-  │  CPI+  │ スクロ │  自動  │  保存  │
      KC_CIRC , KC_AMPR , KC_ASTR , XXX     , XXX     ,                       CPI_D100, CPI_I100, SCRL_TO , SSNP_FRE, KBC_SAVE,
  //╰────────┴────────┴────────┼────────┼────────┼────────╮  ╭────────┼────────┼────────┴────────┴────────┴────────╯
                                  XXX     , XXX     , XXX     ,          XXX     , XXX     ,
  //                            ├────────┼────────┼────────┤  ├────────┼────────┤
  //                            │ BOOT  │ RESET │  透過  │  │        │  無効  │
                                            QK_BOOT , KBC_RST , ___     ,          XXX
  //                            ╰────────┴────────┴────────╯  ╰────────┴────────╯
  ),
};
// clang-format on

// ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// レイヤー状態管理
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
    keyball_oled_render_keyinfo();
    keyball_oled_render_ballinfo();
    keyball_oled_render_layerinfo();
}
#endif