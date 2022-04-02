#pragma once

#include <cstdint>
#include <mutex>
#include <vector>

#include <Windows.h>

/**
 * KeyboardManager
 *
 * Translates WM_KEYDOWN and WM_KEYUP messages received by the UI thread and
 * manages the state of the keyboard memory map owned by the simulator thread.
 * All methods are thread-safe.
 */
class KeyboardManager {
public:
  /**
   * KeyboardManager::HackKey
   *
   * Mnemonics for the scan codes recognized by the Hack computer. The backing
   * integer for each mnemonic is equal to its Hack scan code.
   *
   * Hack scan codes differentiate between capital and lowercase letters, for instance.
   */
  enum class HackKey : uint16_t {
    // no key is pressed
    NONE = 0,

    // <space>!"#$%&'()*+,-./
    SPACE = 32, EXCLAM, DQUOTE, POUND, DOLLAR, PERCENT,
    AMPERSAND, SQUOTE, LPAREN, RPAREN, ASTERISK, PLUS,
    COMMA, HYPHEN, PERIOD, FSLASH,

    // 0-9
    D0 = 48, D1, D2, D3, D4, D5, D6, D7, D8, D9,

    // :;<=>?@
    COLON = 58, SEMICOLON, LANGLE, EQUALS, RANGLE, QUESTION, AT,

    // A-Z
    A = 65, B, C, D, E, F, G, H, I, J, K, L, M,
    N, O, P, Q, R, S, T, U, V, W, X, Y, Z,

    // [\]^_`
    LBRACKET = 91, BSLASH, RBRACKET, CARET, USCORE, BACKTICK,

    // a-z
    a = 97, b, c, d, e, f, g, h, i, j, k, l, m,
    n, o, p, q, r, s, t, u, v, w, x, y, z,

    // {|}~
    LBRACE = 123, PIPE, RBRACE, TILDA,

    // return and nonprintable keys
    NEWLINE = 128, BACKSPACE, LARROW, UARROW, RARROW, DARROW,
    HOME, END, PAGEUP, PAGEDOWN, INS, DEL, ESC,
    F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12
  };

  // 1 + 26 + 10 + 12 + 26 = the number of `LogicalKey`s
  KeyboardManager() : m_logicalKeyState(1 + 26 + 10 + 12 + 26, false)
  { }

  /**
   * Records a WM_KEYDOWN message.
   */
  LRESULT OnKeyDown(WPARAM wParam, LPARAM lParam);

  /**
   * Records a WM_KEYUP message.
   */
  LRESULT OnKeyUp(WPARAM wParam, LPARAM lParam);

  /**
   * Gets the current Hack scan code according to the record of keydown
   * and keyup messages.
   */
  uint16_t GetScanCode();

private:
  /*
   * KeyboardManager::LogicalKey
   *
   * Each value represents a key on the standard US keyboard. Serves as an
   * intermediate form in the conversion of Windows virtual key codes to
   * Hack scan codes. Logical keys do not differentiate between capital and
   * lowercase symbols. They only correspond to physical keys.
   *
   * A LogicalKey backing value is a valid index into m_logicalKeyState.
   *
   * For example:
   *   m_logicalKeyState[(size_t)LogicalKey::D8] == true
   * means that the digit-8 key is currently pressed.
   */
  enum class LogicalKey {
    UNKNOWN = 0,
    A, B, C, D, E, F, G, H, I, J, K, L, M,
    N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
    D0, D1, D2, D3, D4, D5, D6, D7, D8, D9,
    F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,
    BACKTICK, LBRACKET, RBRACKET, HYPHEN,
    EQUALS, BSLASH, SEMICOLON, QUOTE, COMMA,
    PERIOD, FSLASH, SHIFT, SPACE,
    NEWLINE, BACKSPACE, LARROW, UARROW, RARROW, DARROW,
    HOME, END, PAGEUP, PAGEDOWN, INS, DEL, ESC
  };

  // Contains the last known state (up/down) of each logical key
  std::vector<bool> m_logicalKeyState;

  // mutex for maintaining thread safety
  std::mutex m_mtx;

  // An optimization allows the KeyboardManager to return the last known scan
  // code if no keyup or keydown messages have been logged since the last time
  // the scan code was computed.
  bool m_dirty = true;
  uint16_t m_lastScanCode = 0;

  /**
   * Translates a Windows virtual key code to its LogicalKey equivalent on the
   * standard US keyboard.
   */
  LogicalKey WinVKToLogicalKey(WPARAM vkCode);

  /**
   * Translates a LogicalKey to a HackKey. Needs to know if the shift key is
   * currently pressed.
   */
  HackKey LogicalKeyToHackKey(LogicalKey key, bool isShiftDown);
};


