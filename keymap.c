/*
 * Keyball39 Logitech Japanese Style Keymap
 * 
 *  Shift単押しの用途 → 基本的にない想定
 *  Shift + 文字 → 大文字
 *  Shift + 数字 → 記号(!@#$%など)
 *  Shift + 矢印 → テキスト選択
 *  Shift + Tab → 逆タブ
 *  
 * 【設計コンセプト】
 * Logicool日本語キーボード風の配置にしたい
 * - 右に「かな」で「かな」を教えている最中にもう一度押すと、再変換できる(これは欲しい)、スペース左に「英数」は仕方がない
 * - Tap-Holdで1キー2役 
 * 
 * 【親指キー配置】Logicool風
 * 左手: [無効] [無効] 英数/Cmd  Alt   ESC/L1  Tab/Ctrl(Cmd)
 * 右手: Space/L2  かな/Shift  [🔴]  [無効]
 * 
 * 【Tap-Hold設計】
 * - 英数/Cmd: 単押し=英数(IME OFF)、長押し=Cmd
 * - ESC/L1: 単押し=ESC(Vim必須)、長押し=Layer1
 * - Tab/Ctrl(Cmd): 単押し=Tab、長押し=Mac:Cmd / Win:Ctrl
 * - Space/L2: 単押し=Space、長押し=Layer2
 * - かな/Shift: 単押し=かな(IME ON)、長押し=Shift
 * 
 * 【IME切り替え】Windows/Mac両対応!
 * - 英数キー: Mac=英数 / Win=無変換 → IME OFF確定
 * - かなキー: Mac=かな / Win=変換   → IME ON確定
 * - 再変換: テキスト選択 → かなキー単押し
 * 
 * 【重要キーの入力方法】
 * - Enter: L1 + 親指右(かな位置) or L位置
 * - Backspace: L1 + P位置 or P+K同時押し
 * - Delete: L2 + P位置
 * 
 * 【スクロールモード】
 * - L1有効時: トラックボールがスクロールモード
 * - /キー長押し: スクロールモード(小指で簡単操作!)
 *   タップ=/ / ホールド=スクロールモード
 * 
 * 【Layer 2の起動】
 * - Space長押し(右手親指) → トラックボール操作しながらクリック、Vim記号
 */

// ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// Tap-Hold設定
// 
// TAPPING_TERM: 150ms(デフォルト200msより短く、反応良好)
// PERMISSIVE_HOLD: 素早い操作でも修飾キーを確実に発動
// ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
#define TAPPING_TERM 150
#define PERMISSIVE_HOLD
#define KEYBALL_SCROLL_DIV_DEFAULT 16

#include QMK_KEYBOARD_H
#include "quantum.h"
#include "os_detection.h"

// ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// カスタムキーコード(Windows/Mac両対応)
// 
// IME_ON:      単押し=かな/変換(Tap専用、Holdは SFT_T で処理)
// IME_OFF:     単押し=英数/無変換(Tap専用、Holdは GUI_T で処理)
// APP_SW:      アプリ切替(Mac=Cmd+Tab / Win=Alt+Tab)
// IME_TOGGLE:  かな/英数トグル
// TAB_CTGUI:   単押し=Tab / 長押し=Ctrl(Win)/Cmd(Mac)
// SLSH_SCRL:   単押し=/ / 長押し=スクロールモード
// ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
enum custom_keycodes {
    IME_ON = SAFE_RANGE,   // かな/変換(タップ専用)
    IME_OFF,               // 英数/無変換(タップ専用)
    APP_SW,                // アプリ切替(OS判定)
    IME_TOGGLE,            // かな/英数トグル
    TAB_CTGUI,             // 単押し=Tab / 長押し=Ctrl(Win)/Cmd(Mac)
    SLSH_SCRL,             // 単押し=/ / 長押し=スクロール
    // JIS/US両対応括弧・記号
    JU_LCBR,               // { (JIS/US両対応)
    JU_RCBR,               // } (JIS/US両対応)
    JU_LBRC,               // [ (JIS/US両対応)
    JU_RBRC,               // ] (JIS/US両対応)
    JU_PIPE,               // | (JIS/US両対応)
    JU_BSLS,               // \ (JIS/US両対応)
    JU_TILD,               // ~ (JIS/US両対応)
    JU_GRV,                // ` (JIS/US両対応)
    JU_AT,                 // @ (JIS/US両対応)
    JU_CIRC,               // ^ (JIS/US両対応)
    // 追加: JIS/US位置が異なる記号
    JU_LPRN,               // ( (JIS/US両対応)
    JU_RPRN,               // ) (JIS/US両対応)
    JU_PLUS,               // + (JIS/US両対応)
    JU_ASTR,               // * (JIS/US両対応)
    JU_EQL,                // = (JIS/US両対応)
    JU_COLN,               // : (JIS/US両対応)
    JU_QUOT,               // ' (JIS/US両対応)
    JU_DQUO,               // " (JIS/US両対応)
    JU_AMPR,               // & (JIS/US両対応)
    JU_UNDS,               // _ (JIS/US両対応)
};

// アプリ切替状態保持
static bool is_app_sw_active = false;
// スクロールモード状態保持
static bool is_slash_scroll_active = false;
static uint16_t slash_scroll_timer;
// Tab/Ctrl(Cmd)状態保持
static bool is_tab_ctgui_active = false;
static uint16_t tab_ctgui_timer;

// ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// コンボ定義(マウスクリック)
// 
// F + G 同時押し → IMEトグル
// J + K 同時押し → 左クリック
// K + L 同時押し → 右クリック
// P + K 同時押し → Backspace
// ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
const uint16_t PROGMEM combo_ime_toggle[] = {KC_F, KC_G, COMBO_END};
const uint16_t PROGMEM combo_btn1[] = {KC_J, KC_K, COMBO_END};
const uint16_t PROGMEM combo_btn2[] = {KC_K, KC_L, COMBO_END};
const uint16_t PROGMEM combo_bsp[] = {KC_P, KC_K, COMBO_END};

combo_t key_combos[] = {
  COMBO(combo_ime_toggle, IME_TOGGLE),  // F+G = IMEトグル(かな/英数)
  COMBO(combo_btn1, KC_BTN1),           // J+K = 左クリック
  COMBO(combo_btn2, KC_BTN2),           // K+L = 右クリック
  COMBO(combo_bsp, KC_BSPC),            // P+K = Backspace
};

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

  // ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
  // Layer 0: Base(QWERTY + Logicool風親指)
  // 
  // 【Logicoolの良いところを採用】
  // - 英数キー: IMEを確実にOFFにできる
  // - かなキー: IMEを確実にONにできる、かな
  // - トグル式ではないので迷わない
  // 
  // 【親指Tap-Hold】
  // - GUI_T(IME_OFF): 単押し=英数/無変換、長押し=Cmd
  // - LT(1, KC_ESC): 単押し=ESC、長押し=L1
  // - TAB_CTGUI: 単押し=Tab、長押し=Mac:Cmd / Win:Ctrl
  // - LT(2, KC_SPC): 単押し=Space、長押し=L2
  // - SFT_T(IME_ON): 単押し=かな/変換、長押し=Shift
  // 
  // 【小指でスクロール】
  // - /キー(SLSH_SCRL): タップ=/ / ホールド=スクロールモード
  // ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
  [0] = LAYOUT_right_ball(
  //┌────────┬────────┬────────┬────────┬────────┐                          ┌────────┬────────┬────────┬────────┬────────┐
  //│ Q      │ W      │ E      │ R      │ T      │                          │ Y      │ U      │ I      │ O      │ P      │
    KC_Q     , KC_W     , KC_E     , KC_R     , KC_T     ,                            KC_Y     , KC_U     , KC_I     , KC_O     , KC_P     ,
  //├────────┼────────┼────────┼────────┼────────┤                          ├────────┼────────┼────────┼────────┼────────┤
  //│ A      │ S      │ D      │ F      │ G      │                          │ H      │ J      │ K      │ L      │ Enter  │
    KC_A     , KC_S     , KC_D     , KC_F     , KC_G     ,                            KC_H     , KC_J     , KC_K     , KC_L     , KC_ENT   ,
  //├────────┼────────┼────────┼────────┼────────┤                          ├────────┼────────┼────────┼────────┼────────┤
  //│ Z      │ X      │ C      │ V      │ B      │                          │ N      │ M      │ ,      │ .      │ /=SCRL │
    KC_Z     , KC_X     , KC_C     , KC_V     , KC_B     ,                            KC_N     , KC_M     , KC_COMM  , KC_DOT   , SLSH_SCRL,
  //┌────────┬────────┬────────────┬────────────┬────────────┬──────────────┐    ┌──────────────┬──────────┐       ┌────────┐
  //│ 無効   │ 無効   │ 英数/Cmd  │ Alt        │ ESC/L1     │ Tab/CtrlCmd  │    │ Space/L2     │ かな/Shift│ [🔴]  │ 無効   │
    XXXXXXX  , XXXXXXX  , GUI_T(IME_OFF), KC_LALT, LT(1,KC_ESC), TAB_CTGUI,        LT(2,KC_SPC), SFT_T(IME_ON),                XXXXXXX
  //└────────┴────────┴────────────┴────────────┴────────────┴──────────────┘    └──────────────┴──────────┘       └────────┘
  ),

  // ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
  // Layer 1: Symbols & Navigation(計算機風)
  // 
  // 【左手】計算機配置
  //        + 7 8 9 -
  //        * 4 5 6 =
  //        0 1 2 3 %
  // 【右手】括弧類 {} [] + Vim矢印 + ' " 
  // 【親指】Enter
  // ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
  [1] = LAYOUT_right_ball(
  //┌────────┬────────┬────────┬────────┬────────┐                          ┌────────┬────────┬────────┬────────┬────────┐
  //│ +      │ 7      │ 8      │ 9      │ -      │                          │ {      │ }      │ [      │ ]      │ Bsp    │
    JU_PLUS  , KC_7     , KC_8     , KC_9     , KC_MINS  ,                            JU_LCBR  , JU_RCBR  , JU_LBRC  , JU_RBRC  , KC_BSPC  ,
  //├────────┼────────┼────────┼────────┼────────┤                          ├────────┼────────┼────────┼────────┼────────┤
  //│ *      │ 4      │ 5      │ 6      │ =      │                          │ ←      │ ↓      │ ↑      │ →      │ :      │
    JU_ASTR  , KC_4     , KC_5     , KC_6     , JU_EQL   ,                            KC_LEFT  , KC_DOWN  , KC_UP    , KC_RGHT  , JU_COLN  ,
  //├────────┼────────┼────────┼────────┼────────┤                          ├────────┼────────┼────────┼────────┼────────┤
  //│ 0      │ 1      │ 2      │ 3      │ %      │                          │ (      │ )      │ '      │ "      │ |      │
    KC_0     , KC_1     , KC_2     , KC_3     , KC_PERC  ,                            JU_LPRN  , JU_RPRN  , JU_QUOT  , JU_DQUO  , JU_PIPE  ,
  //┌────────┬────────┬────────┬────────┬────────┬────────┐          ┌──────┬────────┐       ┌────────┐
  //│ 無効   │ 無効   │ ___    │ ___    │ ___    │ ___    │          │ ___  │ Enter  │ [🔴]  │ 無効   │
    XXXXXXX  , XXXXXXX  , _______  , _______  , _______  , _______  ,      _______  , KC_ENT   ,                               XXXXXXX
  //└────────┴────────┴────────┴────────┴────────┴────────┘          └──────┴────────┘       └────────┘
  ),
  

  // ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
  // Layer 2: Selection & Settings & Shell/Vim記号
  // 
  // 【起動】Space長押し(右手親指)
  // 【マウスクリック】コンボで実装!(J+K=左, K+L=右)
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
  // 【右手中段】Shift+矢印(選択移動)
  // ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
  [2] = LAYOUT_right_ball(
  //┌────────┬────────┬────────┬────────┬────────┐                          ┌────────┬────────┬────────┬────────┬────────┐
  //│ AppSw  │ SCRL↕  │ SCRL↔  │ F5     │ F10    │                          │ ;      │ Home   │ End    │ Del    │ Bsp    │
    APP_SW   , SSNP_VRT , SSNP_HOR , KC_F5    , KC_F10   ,                            KC_SCLN  , KC_HOME  , KC_END   , KC_DEL   , KC_BSPC  ,
  //├────────┼────────┼────────┼────────┼────────┤                          ├────────┼────────┼────────┼────────┼────────┤
  //│ !      │ $      │ ^      │ @      │ &      │                          │ S+←    │ S+↓    │ S+↑    │ S+→    │ ~      │
    KC_EXLM  , KC_DLR   , JU_CIRC  , JU_AT    , JU_AMPR  ,                         S(KC_LEFT),S(KC_DOWN), S(KC_UP),S(KC_RGHT), JU_TILD  ,
  //├────────┼────────┼────────┼────────┼────────┤                          ├────────┼────────┼────────┼────────┼────────┤
  //│ `      │ #      │ \      │ <      │ >      │                          │ CPI-   │ CPI+   │ PgUp   │ PgDn   │ _      │
    JU_GRV   , KC_HASH  , JU_BSLS  , KC_LABK  , KC_RABK  ,                            CPI_D100 , CPI_I100 , KC_PGUP  , KC_PGDN  , JU_UNDS  ,
  //┌────────┬────────┬────────┬────────┬────────┬────────┐          ┌──────┬────────┐       ┌────────┐
  //│ 無効   │ 無効   │ ___    │ ___    │ ___    │ ___    │          │ ___  │ SCRL   │ [🔴]  │ 無効   │
    XXXXXXX  , XXXXXXX  , _______  , _______  , _______  , _______  ,      _______  , SCRL_TO  ,                               XXXXXXX
  //└────────┴────────┴────────┴────────┴────────┴────────┘          └──────┴────────┘       └────────┘
  ),
};
// clang-format on

// ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// カスタムキー処理(Windows/Mac両対応)
// 
// 【IME_ON】単押し=かな/変換(長押しShiftはSFT_Tで処理)
// 【IME_OFF】単押し=英数/無変換(長押しCmdはGUI_Tで処理)
// 【APP_SW】アプリ切替(Mac=Cmd+Tab / Win=Alt+Tab)
// 【IME_TOGGLE】かな/英数トグル
// 【OS_CTRL_GUI】OSに応じてCtrl/Cmd切替
// 【SLSH_SCRL】単押し=/ / 長押し=スクロールモード
// ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// IMEトグル状態保持用
static bool ime_toggle_state = false; // false: OFF(英数), true: ON(かな)

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        // かな/変換(タップ専用、ホールドはSFT_Tで処理)
        case IME_ON:
            if (record->event.pressed) {
                switch (detected_host_os()) {
                    case OS_MACOS:
                    case OS_IOS:
                        tap_code(KC_LNG1); // かな
                        break;
                    default:
                        // Windows: 変換キー
                        tap_code(KC_INT4);
                        break;
                }
            }
            return false;

        // 英数/無変換(タップ専用、ホールドはGUI_Tで処理)
        case IME_OFF:
            if (record->event.pressed) {
                switch (detected_host_os()) {
                    case OS_MACOS:
                    case OS_IOS:
                        tap_code(KC_LNG2); // 英数
                        break;
                    default:
                        // Windows: 無変換キー
                        tap_code(KC_INT5);
                        break;
                }
            }
            return false;

        // OSでCmd/Ctrl切替(親指キー) - タップ=Tab / ホールド=Ctrl(Win)/Cmd(Mac)
        case TAB_CTGUI:
            if (record->event.pressed) {
                tab_ctgui_timer = timer_read();
                is_tab_ctgui_active = true;
                // 長押し開始時点で修飾キーを有効化
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
                // キーを離した時
                switch (detected_host_os()) {
                    case OS_MACOS:
                    case OS_IOS:
                        unregister_code(KC_LGUI);
                        break;
                    default:
                        unregister_code(KC_LCTL);
                        break;
                }
                // タップ判定: Tabキーを送信
                if (timer_elapsed(tab_ctgui_timer) < TAPPING_TERM) {
                    tap_code(KC_TAB);
                }
                is_tab_ctgui_active = false;
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

        // アプリ切替(Mac=Cmd+Tab / Win=Alt+Tab)
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

        // /キー: タップ=/ / ホールド=スクロールモード
        case SLSH_SCRL:
            if (record->event.pressed) {
                slash_scroll_timer = timer_read();
                is_slash_scroll_active = true;
                // 長押し開始時点でスクロールモード有効化
                keyball_set_scroll_mode(true);
            } else {
                // キーを離した時
                if (timer_elapsed(slash_scroll_timer) < TAPPING_TERM) {
                    // タップ判定: /キーを送信
                    tap_code(KC_SLSH);
                }
                // スクロールモード無効化(L1が有効ならそちらを優先)
                is_slash_scroll_active = false;
                keyball_set_scroll_mode(layer_state_cmp(layer_state, 1));
            }
            return false;

        // ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
        // JIS/US両対応キーコード
        // Mac(US配列想定) / Windows(JIS配列想定)
        // ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
        
        // { 左波括弧
        case JU_LCBR:
            if (record->event.pressed) {
                switch (detected_host_os()) {
                    case OS_MACOS:
                    case OS_IOS:
                        tap_code16(KC_LCBR);  // US: Shift + [
                        break;
                    default:
                        tap_code16(S(KC_RBRC));  // JIS: Shift + ]キー位置
                        break;
                }
            }
            return false;

        // } 右波括弧
        case JU_RCBR:
            if (record->event.pressed) {
                switch (detected_host_os()) {
                    case OS_MACOS:
                    case OS_IOS:
                        tap_code16(KC_RCBR);  // US: Shift + ]
                        break;
                    default:
                        tap_code16(S(KC_BSLS));  // JIS: Shift + \キー位置
                        break;
                }
            }
            return false;

        // [ 左角括弧
        case JU_LBRC:
            if (record->event.pressed) {
                switch (detected_host_os()) {
                    case OS_MACOS:
                    case OS_IOS:
                        tap_code(KC_LBRC);  // US: [
                        break;
                    default:
                        tap_code(KC_RBRC);  // JIS: ]キー位置
                        break;
                }
            }
            return false;

        // ] 右角括弧
        case JU_RBRC:
            if (record->event.pressed) {
                switch (detected_host_os()) {
                    case OS_MACOS:
                    case OS_IOS:
                        tap_code(KC_RBRC);  // US: ]
                        break;
                    default:
                        tap_code(KC_BSLS);  // JIS: \キー位置
                        break;
                }
            }
            return false;

        // | パイプ
        case JU_PIPE:
            if (record->event.pressed) {
                switch (detected_host_os()) {
                    case OS_MACOS:
                    case OS_IOS:
                        tap_code16(KC_PIPE);  // US: Shift + backslash
                        break;
                    default:
                        tap_code16(S(KC_INT3));  // JIS: Shift + yen key
                        break;
                }
            }
            return false;

        // \ バックスラッシュ
        case JU_BSLS:
            if (record->event.pressed) {
                switch (detected_host_os()) {
                    case OS_MACOS:
                    case OS_IOS:
                        tap_code(KC_BSLS);  // US: backslash
                        break;
                    default:
                        tap_code(KC_INT3);  // JIS: yen key (backslash)
                        break;
                }
            }
            return false;

        // ~ チルダ
        case JU_TILD:
            if (record->event.pressed) {
                switch (detected_host_os()) {
                    case OS_MACOS:
                    case OS_IOS:
                        tap_code16(KC_TILD);  // US: Shift + `
                        break;
                    default:
                        tap_code16(S(KC_EQL));  // JIS: Shift + ^キー位置
                        break;
                }
            }
            return false;

        // ` バッククォート
        case JU_GRV:
            if (record->event.pressed) {
                switch (detected_host_os()) {
                    case OS_MACOS:
                    case OS_IOS:
                        tap_code(KC_GRV);  // US: `
                        break;
                    default:
                        tap_code16(S(KC_LBRC));  // JIS: Shift + @キー位置
                        break;
                }
            }
            return false;

        // @ アットマーク
        case JU_AT:
            if (record->event.pressed) {
                switch (detected_host_os()) {
                    case OS_MACOS:
                    case OS_IOS:
                        tap_code16(KC_AT);  // US: Shift + 2
                        break;
                    default:
                        tap_code(KC_LBRC);  // JIS: @キー(Shiftなし)
                        break;
                }
            }
            return false;

        // ^ キャレット
        case JU_CIRC:
            if (record->event.pressed) {
                switch (detected_host_os()) {
                    case OS_MACOS:
                    case OS_IOS:
                        tap_code16(KC_CIRC);  // US: Shift + 6
                        break;
                    default:
                        tap_code(KC_EQL);  // JIS: ^キー(Shiftなし)
                        break;
                }
            }
            return false;

        // ( 左丸括弧
        case JU_LPRN:
            if (record->event.pressed) {
                switch (detected_host_os()) {
                    case OS_MACOS:
                    case OS_IOS:
                        tap_code16(KC_LPRN);  // US: Shift + 9
                        break;
                    default:
                        tap_code16(S(KC_8));  // JIS: Shift + 8
                        break;
                }
            }
            return false;

        // ) 右丸括弧
        case JU_RPRN:
            if (record->event.pressed) {
                switch (detected_host_os()) {
                    case OS_MACOS:
                    case OS_IOS:
                        tap_code16(KC_RPRN);  // US: Shift + 0
                        break;
                    default:
                        tap_code16(S(KC_9));  // JIS: Shift + 9
                        break;
                }
            }
            return false;

        // + プラス
        case JU_PLUS:
            if (record->event.pressed) {
                switch (detected_host_os()) {
                    case OS_MACOS:
                    case OS_IOS:
                        tap_code16(KC_PLUS);  // US: Shift + =
                        break;
                    default:
                        tap_code16(S(KC_SCLN));  // JIS: Shift + ;
                        break;
                }
            }
            return false;

        // * アスタリスク
        case JU_ASTR:
            if (record->event.pressed) {
                switch (detected_host_os()) {
                    case OS_MACOS:
                    case OS_IOS:
                        tap_code16(KC_ASTR);  // US: Shift + 8
                        break;
                    default:
                        tap_code16(S(KC_QUOT));  // JIS: Shift + :
                        break;
                }
            }
            return false;

        // = イコール
        case JU_EQL:
            if (record->event.pressed) {
                switch (detected_host_os()) {
                    case OS_MACOS:
                    case OS_IOS:
                        tap_code(KC_EQL);  // US: =
                        break;
                    default:
                        tap_code16(S(KC_MINS));  // JIS: Shift + -
                        break;
                }
            }
            return false;

        // : コロン
        case JU_COLN:
            if (record->event.pressed) {
                switch (detected_host_os()) {
                    case OS_MACOS:
                    case OS_IOS:
                        tap_code16(KC_COLN);  // US: Shift + ;
                        break;
                    default:
                        tap_code(KC_QUOT);  // JIS: : (Shiftなし)
                        break;
                }
            }
            return false;

        // ' シングルクォート
        case JU_QUOT:
            if (record->event.pressed) {
                switch (detected_host_os()) {
                    case OS_MACOS:
                    case OS_IOS:
                        tap_code(KC_QUOT);  // US: '
                        break;
                    default:
                        tap_code16(S(KC_7));  // JIS: Shift + 7
                        break;
                }
            }
            return false;

        // " ダブルクォート
        case JU_DQUO:
            if (record->event.pressed) {
                switch (detected_host_os()) {
                    case OS_MACOS:
                    case OS_IOS:
                        tap_code16(KC_DQUO);  // US: Shift + '
                        break;
                    default:
                        tap_code16(S(KC_2));  // JIS: Shift + 2
                        break;
                }
            }
            return false;

        // & アンパサンド
        case JU_AMPR:
            if (record->event.pressed) {
                switch (detected_host_os()) {
                    case OS_MACOS:
                    case OS_IOS:
                        tap_code16(KC_AMPR);  // US: Shift + 7
                        break;
                    default:
                        tap_code16(S(KC_6));  // JIS: Shift + 6
                        break;
                }
            }
            return false;

        // _ アンダースコア
        case JU_UNDS:
            if (record->event.pressed) {
                switch (detected_host_os()) {
                    case OS_MACOS:
                    case OS_IOS:
                        tap_code16(KC_UNDS);  // US: Shift + -
                        break;
                    default:
                        tap_code16(S(KC_INT1));  // JIS: Shift + ろ
                        break;
                }
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
// - SLSH_SCRLとの競合を考慮
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
    
    // スクロールモード制御: SLSH_SCRL優先、次にL1
    if (!is_slash_scroll_active) {
        keyball_set_scroll_mode(layer_state_cmp(state, 1));
    }
    
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
#kj
