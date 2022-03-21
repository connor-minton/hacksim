#pragma once

#include <cstdint>
#include <mutex>
#include <vector>

#include <Windows.h>

class KeyboardManager {
public:
  // Scan codes for Hack computer
  enum class HackKey : uint16_t {
    NONE = 0,

    SPACE = 32, EXCLAM, DQUOTE, POUND, DOLLAR, PERCENT,
    AMPERSAND, SQUOTE, LPAREN, RPAREN, ASTERISK, PLUS,
    COMMA, HYPHEN, PERIOD, FSLASH,

    D0 = 48, D1, D2, D3, D4, D5, D6, D7, D8, D9,

    COLON = 58, SEMICOLON, LANGLE, EQUALS, RANGLE, QUESTION, AT,

    A = 65, B, C, D, E, F, G, H, I, J, K, L, M,
    N, O, P, Q, R, S, T, U, V, W, X, Y, Z,

    LBRACKET = 91, BSLASH, RBRACKET, CARET, USCORE, BACKTICK,

    a = 97, b, c, d, e, f, g, h, i, j, k, l, m,
    n, o, p, q, r, s, t, u, v, w, x, y, z,

    LBRACE = 123, PIPE, RBRACE, TILDA,

    NEWLINE = 128, BACKSPACE, LARROW, UARROW, RARROW, DARROW,
    HOME, END, PAGEUP, PAGEDOWN, INS, DEL, ESC,
    F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12
  };

  KeyboardManager() : m_logicalKeyState(1 + 26 + 10 + 12 + 26, false)
  { }

  LRESULT OnKeyDown(WPARAM wParam, LPARAM lParam);
  LRESULT OnKeyUp(WPARAM wParam, LPARAM lParam);
  uint16_t GetScanCode();

private:
  // Use as indices into m_logicalKeyState
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

  std::vector<bool> m_logicalKeyState;
  std::mutex m_mtx;
  bool m_dirty = true;
  uint16_t m_lastScanCode = 0;

  LogicalKey WinVKToLogicalKey(WPARAM vkCode);
  HackKey LogicalKeyToHackKey(LogicalKey key, bool isShiftDown);
};


