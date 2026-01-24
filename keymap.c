/*
 * Keyball39 Logitech Japanese Style Keymap
 * 
 *  Shift単押しの用途 → 基本的にない想定
 *  Shift + 文字 → 大文字
 *  Shift + 数字 → 記号（!@#$%など）
 *  Shift + 矢印 → テキスト選択
 *  Shift + Tab → 逆タブ
 *  
 * 【設計コンセプト】
 * Logicool日本語キーボード風の配置にしたい
 * - 右に「かな」で「かな」を教えている最中にもう一度押すと、再変換できる（これは欲しい）、スペース左に「英数」は仕方がない
 * - Tap-Holdで1キー2役 
 * 
 * 【親指キー配置】Logicool風
 * 左手: [無効] [無効] 英数/Cmd  Alt   ESC/L1  Tab/Ctrl
 * 右手: かな/Shift  Space  [🔴]  [無効]
 * 
 * 【Tap-Hold設計】
 * - 英数/Cmd: 単押し=英数（IME OFF）、長押し=Cmd
 * - ESC/L1: 単押し=ESC（Vim必須）、長押し=Layer1
 * - Tab/Ctrl: 単押し=Tab、長押し=Ctrl
 * - かな/Shift: 単押し=かな（IME ON）、長押し=Shift
 * - Space: 単押し=Space（最頻出なのでシンプルに）
 * 
 * 【IME切り替え】Windows/Mac両対応！
 * - 英数キー: Mac=英数 / Win=無変換 → IME OFF確定
 * - かなキー: Mac=かな / Win=変換   → IME ON確定
 * - 再変換: テキスト選択 → かなキー単押し
 * 
 * 【重要キーの入力方法】
 * - Enter: L1 + 親指右（かな位置）
 * - Backspace: L1 + P位置
 * - Delete: L2 + P位置
 * 
 * 【Layer 2の起動】
 * - Space長押し（右手親指）→ トラックボール操作しながらクリック、Vim記号
 */

// ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// Tap-Hold設定
// 
// TAPPING_TERM: 150ms（デフォルト200msより短く、反応良好）
// PERMISSIVE_HOLD: 素早い操作でも修飾キーを確実に発動
// ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
#define TAPPING_TERM 150
#define PERMISSIVE_HOLD
#define KEYBALL_SCROLL_DIV_DEFAULT 16

#include QMK_KEYBOARD_H
#include "quantum.h"
#include "os_detection.h"

// ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// カスタムキーコード（Windows/Mac両対応）
// 
// IME_OFF_CMD: 単押し=英数/無変換、長押し=Cmd
// IME_ON_SFT:  単押し=かな/変換、長押し=Shift
// APP_SW:      アプリ切替（Mac=Cmd+Tab / Win=Alt+Tab）
// ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
enum custom_keycodes {
    IME_OFF_CMD = SAFE_RANGE,  // 単押し=英数、長押し=Cmd
    IME_ON_SFT,                // 単押し=かな、長押し=Shift
    APP_SW,                    // アプリ切替（OS判定）
    IME_TOGGLE,                // かな/英数トグル
    OS_CTRL_GUI,               // OSでCmd/Ctrl切替
};

// タイマー変数（Tap-Hold判定用）
static uint16_t ime_on_sft_timer;
static bool is_app_sw_active = false;

// ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// コンボ定義（マウスクリック）
// 
// J + K 同時押し → 左クリック
// K + L 同時押し → 右クリック
// ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
const uint16_t PROGMEM combo_ime_toggle[] = {KC_F, KC_G, COMBO_END};

const uint16_t PROGMEM combo_btn1[] = {KC_J, KC_K, COMBO_END};
const uint16_t PROGMEM combo_btn2[] = {KC_K, KC_L, COMBO_END};
const uint16_t PROGMEM combo_bsp[] = {KC_P, KC_L, COMBO_END}; // P+L = Backspace

combo_t key_combos[] = {
  COMBO(combo_ime_toggle, IME_TOGGLE),  // F+G = IMEトグル（かな/英数）
  COMBO(combo_btn1, KC_BTN1),           // J+K = 左クリック
  COMBO(combo_btn2, KC_BTN2),           // K+L = 右クリック
  COMBO(combo_bsp, KC_BSPC),            // P+L = Backspace
};

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

  // ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
  // Layer 0: Base（QWERTY + Logicool風親指）
  // 
  // 【Logicoolの良いところを採用】
  // - 英数キー: IMEを確実にOFFにできる
  // - かなキー: IMEを確実にONにできる、かな
  // - トグル式ではないので迷わない
  // 
  // 【親指Tap-Hold】
  // - GUI_T(IME_OFF): 単押し=英数/無変換、長押し=Cmd
  // - LT(1, KC_ESC): 単押し=ESC、長押し=L1
  // - CTL_T(KC_TAB): 単押し=Tab、長押し=Ctrl
  // - SFT_T(IME_ON): 単押し=かな/変換、長押し=Shift ※再変換も！
  // - LT(2, KC_SPC): 単押し=Space、長押し=L2
  // ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
  [0] = LAYOUT_right_ball(
  //┌────────┬────────┬────────┬────────┬────────┐                          ┌────────┬────────┬────────┬────────┬────────┐
  //│ Q      │ W      │ E      │ R      │ T      │                          │ Y      │ U      │ I      │ O      │ P      │
    KC_Q     , KC_W     , KC_E     , KC_R     , KC_T     ,                            KC_Y     , KC_U     , KC_I     , KC_O     , KC_P     ,
  //├────────┼────────┼────────┼────────┼────────┤                          ├────────┼────────┼────────┼────────┼────────┤
  //│ A      │ S      │ D      │ F      │ G      │                          │ H      │ J      │ K      │ L      │ Enter  │
    KC_A     , KC_S     , KC_D     , KC_F     , KC_G     ,                            KC_H     , KC_J     , KC_K     , KC_L     , KC_ENT   ,
  //├────────┼────────┼────────┼────────┼────────┤                          ├────────┼────────┼────────┼────────┼────────┤
  //│ Z      │ X      │ C      │ V      │ B      │                          │ N      │ M      │ ,      │ .      │ /      │
    KC_Z     , KC_X     , KC_C     , KC_V     , KC_B     ,                            KC_N     , KC_M     , KC_COMM  , KC_DOT   , KC_SLSH  ,
  //┌────────┬────────┬────────────┬────────────┬────────────┬──────────────┐    ┌──────────────┬──────────┐       ┌────────┐
  //│ 無効   │ 無効   │ XXXXXXX   │ Alt        │ ESC/L1     │ Tab/Ctrl     │    │ Space/L2   │ 再変換/Shift │ [🔴]  │ 無効   │
    XXXXXXX  , XXXXXXX  , XXXXXXX, KC_LALT , LT(1,KC_ESC), OS_CTRL_GUI,      LT(2,KC_SPC), IME_ON_SFT,                  XXXXXXX
  //└────────┴────────┴────────────┴────────────┴────────────┴──────────────┘    └──────────────┴────────┘       └────────┘
  ),

  // ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
  // Layer 1: Symbols & Navigation（計算機風）
  // 
  // 【左手】計算機配置
  //        + 7 8 9 -
  //        * 4 5 6 =
  //        0 1 2 3 %
  // 【右手】括弧類 {} [] + Vim矢印 + ' " 
  // 【親指】Enter, _（アンダースコア）
  // ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
  [1] = LAYOUT_right_ball(
  //┌────────┬────────┬────────┬────────┬────────┐                          ┌────────┬────────┬────────┬────────┬────────┐
  //│ +      │ 7      │ 8      │ 9      │ -      │                          │ {      │ }      │ [      │ ]      │ Bsp    │
    KC_PLUS  , KC_7     , KC_8     , KC_9     , KC_MINS  ,                            KC_LCBR  , KC_RCBR  , KC_LBRC  , KC_RBRC  , KC_BSPC  ,
  //├────────┼────────┼────────┼────────┼────────┤                          ├────────┼────────┼────────┼────────┼────────┤
  //│ *      │ 4      │ 5      │ 6      │ =      │                          │ ←      │ ↓      │ ↑      │ →      │ :      │
    KC_ASTR  , KC_4     , KC_5     , KC_6     , KC_EQL   ,                            KC_LEFT  , KC_DOWN  , KC_UP    , KC_RGHT  , KC_COLN  ,
  //├────────┼────────┼────────┼────────┼────────┤                          ├────────┼────────┼────────┼────────┼────────┤
  //│ 0      │ 1      │ 2      │ 3      │ %      │                          │ (      │ )      │ '      │ "      │ |      │
    KC_0     , KC_1     , KC_2     , KC_3     , KC_PERC  ,                            KC_LPRN  , KC_RPRN  , KC_QUOT  , KC_DQUO  , KC_PIPE  ,
  //┌────────┬────────┬────────┬────────┬────────┬────────┐          ┌──────┬────────┐       ┌────────┐
  //│ 無効   │ 無効   │ ___    │ ___    │ ___    │ ___    │          │ ___  │ _      │ [🔴]  │ 無効   │
    XXXXXXX  , XXXXXXX  , _______  , _______  , _______  , _______  ,      _______  , KC_UNDS  ,                               XXXXXXX
  //└────────┴────────┴────────┴────────┴────────┴────────┘          └──────┴────────┘       └────────┘
  ),
  

  // ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
  // Layer 2: Selection & Settings & Shell/Vim記号
  // 
  // 【起動】Space長押し（右手親指）
  // 【マウスクリック】コンボで実装！（J+K=左, K+L=右）
  // 【シェル/Vim記号】
  //   ! → shebang, 履歴展開(!!)
  //   $ → 変数展開, Vim行末
  //   ^ → Vim行頭
  //   @ → SSH(user@host)
  //   & → バックグラウンド実行
  //   ` → コマンド置換
  //   # → コメント, Vim単語検索
  //   \ → エスケープ, パス
  // 【左手上段】スクロール設定
  // 【右手中段】Shift+矢印（選択移動）
  // ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
  [2] = LAYOUT_right_ball(
  //┌────────┬────────┬────────┬────────┬────────┐                          ┌────────┬────────┬────────┬────────┬────────┐
  //│ AppSw  │ SCRL↕  │ SCRL↔  │ F5     │ F10    │                          │ ;      │ Home   │ End    │ Del    │ Bsp    │
    APP_SW   , SSNP_VRT , SSNP_HOR , KC_F5    , KC_F10   ,                            KC_SCLN  , KC_HOME  , KC_END   , KC_DEL   , KC_BSPC  ,
  //├────────┼────────┼────────┼────────┼────────┤                          ├────────┼────────┼────────┼────────┼────────┤
  //│ !      │ $      │ ^      │ @      │ &      │                          │ S+←    │ S+↓    │ S+↑    │ S+→    │ ~      │
    KC_EXLM  , KC_DLR   , KC_CIRC  , KC_AT    , KC_AMPR  ,                         S(KC_LEFT),S(KC_DOWN), S(KC_UP),S(KC_RGHT), KC_TILD  ,
  //├────────┼────────┼────────┼────────┼────────┤                          ├────────┼────────┼────────┼────────┼────────┤
  //│ `      │ #      │ \      │ <      │ >      │                          │ CPI-   │ CPI+   │ PgUp   │ PgDn   │ Ins    │
    KC_GRV   , KC_HASH  , KC_BSLS  , KC_LABK  , KC_RABK  ,                            CPI_D100 , CPI_I100 , KC_PGUP  , KC_PGDN  , KC_INS   ,
  //┌────────┬────────┬────────┬────────┬────────┬────────┐          ┌──────┬────────┐       ┌────────┐
  //│ 無効   │ 無効   │ ___    │ ___    │ ___    │ ___    │          │ ___  │ SCRL   │ [🔴]  │ 無効   │
    XXXXXXX  , XXXXXXX  , _______  , _______  , _______  , _______  ,      _______  , SCRL_TO  ,                               XXXXXXX
  //└────────┴────────┴────────┴────────┴────────┴────────┘          └──────┴────────┘       └────────┘
  ),
};
// clang-format on

// ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// カスタムキー処理（Windows/Mac両対応）
// 
// 【IME_OFF_CMD】単押し=英数/無変換、長押し=Cmd
// 【IME_ON_SFT】単押し=かな/変換、長押し=Shift
// 【APP_SW】アプリ切替（Mac=Cmd+Tab / Win=Alt+Tab）
// ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// IMEトグル状態保持用
static bool ime_toggle_state = false; // false: OFF(英数), true: ON(かな)

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        // OSでCmd/Ctrl切替（親指キー）
        case OS_CTRL_GUI:
          if (record->event.pressed) {
            switch (detected_host_os()) {
              case OS_MACOS:
              case OS_IOS:
                register_code(KC_LGUI); // Cmd
                break;
              default:
                register_code(KC_LCTL); // Ctrl
                break;
            }
          } else {
            switch (detected_host_os()) {
              case OS_MACOS:
              case OS_IOS:
                unregister_code(KC_LGUI);
                break;
              default:
                unregister_code(KC_LCTL);
                break;
            }
          }
          return false;
        // かな/英数トグル
        case IME_TOGGLE:
          if (record->event.pressed) {
            ime_toggle_state = !ime_toggle_state;
            switch (detected_host_os()) {
              case OS_MACOS:
              case OS_IOS:
                if (ime_toggle_state) {
                  tap_code(KC_LNG1); // かな
                } else {
                  tap_code(KC_LNG2); // 英数
                }
                break;
              default:
                if (ime_toggle_state) {
                  tap_code(KC_INT4); // 変換
                } else {
                  tap_code(KC_INT5); // 無変換
                }
                break;
            }
          }
          return false;

        // かな/Shift（カスタムTap-Hold）
        case IME_ON_SFT:
          if (record->event.pressed) {
            ime_on_sft_timer = timer_read();
          } else {
            if (timer_elapsed(ime_on_sft_timer) < TAPPING_TERM) {
              // タップ: 再変換のみ（かなキー送信しない）
              tap_code(KC_INT4);  // 変換
            } else {
              // 長押し: Shiftのみ
              register_code(KC_LSFT);
              unregister_code(KC_LSFT);
            }
          }
          return false;

        // アプリ切替（Mac=Cmd+Tab / Win=Alt+Tab）
        case APP_SW:
            if (record->event.pressed) {
                if (!is_app_sw_active) {
                    is_app_sw_active = true;
                    // OSに応じて修飾キーを切替
                    switch (detected_host_os()) {
                        case OS_MACOS:
                        case OS_IOS:
                            register_code(KC_LGUI);  // Cmd保持
                            break;
                        default:
                            register_code(KC_LALT);  // Alt保持
                            break;
                    }
                }
                tap_code(KC_TAB);  // Tab送信
            }
            return false;
    }
    return true;
}

// ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// レイヤー状態管理
// 
// - Layer 1でスクロールモードを有効化
// - Layer 2を離れたらアプリ切替を解放
// ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
layer_state_t layer_state_set_user(layer_state_t state) {
    // L2を離れたらアプリ切替を解放
    if (!layer_state_cmp(state, 2) && is_app_sw_active) {
        // OSに応じて修飾キーを解放
        switch (detected_host_os()) {
            case OS_MACOS:
            case OS_IOS:
                unregister_code(KC_LGUI);
                break;
            default:
                unregister_code(KC_LALT);
                break;
        }
        is_app_sw_active = false;
    }
    
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
