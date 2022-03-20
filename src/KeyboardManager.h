#pragma once

#include <cstdint>
#include <mutex>
#include <vector>

#include <Windows.h>

class KeyboardManager {
public:
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
  std::vector<bool> m_logicalKeyState;
  std::mutex m_mtx;
  bool m_dirty = true;
  uint16_t m_lastScanCode = 0;

  LogicalKey WinVKToLogicalKey(WPARAM vkCode);
  HackKey LogicalKeyToHackKey(LogicalKey key, bool isShiftDown);
};


LRESULT KeyboardManager::OnKeyDown(WPARAM wParam, LPARAM lParam) {
  std::lock_guard scopeLock(m_mtx);
  m_logicalKeyState[(size_t)WinVKToLogicalKey(wParam)] = true;
  m_dirty = true;
  return 0;
}

LRESULT KeyboardManager::OnKeyUp(WPARAM wParam, LPARAM lParam) {
  std::lock_guard scopeLock(m_mtx);
  m_logicalKeyState[(size_t)WinVKToLogicalKey(wParam)] = false;
  m_dirty = true;
  return 0;
}

uint16_t KeyboardManager::GetScanCode() {
  size_t whichKey = (size_t)LogicalKey::UNKNOWN;
  bool isShiftDown = false;

  {
    std::lock_guard scopeLock(m_mtx);
    if (!m_dirty) return m_lastScanCode;

    m_dirty = false;

    isShiftDown = m_logicalKeyState[(size_t)LogicalKey::SHIFT];

    for (size_t i = 1; i <= m_logicalKeyState.size(); i++) {
      if (i == (size_t)LogicalKey::SHIFT)
        continue;

      if (m_logicalKeyState[i]) {
        whichKey = i;
        break;
      }
    }
  }

  m_lastScanCode = (uint16_t)LogicalKeyToHackKey((LogicalKey)whichKey, isShiftDown);
  return m_lastScanCode;
}

KeyboardManager::LogicalKey
KeyboardManager::WinVKToLogicalKey(WPARAM vkCode) {
  if (vkCode >= 'A' && vkCode <= 'Z') {
    return (LogicalKey)((unsigned)LogicalKey::A + (vkCode - 'A'));
  }
  if (vkCode >= '0' && vkCode <= '9') {
    return (LogicalKey)((unsigned)LogicalKey::D0 + (vkCode - '0'));
  }
  if (vkCode >= VK_F1 && vkCode <= VK_F12) {
    return (LogicalKey)((unsigned)LogicalKey::F1 + (vkCode - VK_F1));
  }

  switch (vkCode) {
  case (VK_OEM_3):
    return LogicalKey::BACKTICK;
  case (VK_OEM_4):
    return LogicalKey::LBRACKET;
  case (VK_OEM_6):
    return LogicalKey::RBRACKET;
  case (VK_OEM_MINUS):
    return LogicalKey::HYPHEN;
  case (VK_OEM_PLUS):
    return LogicalKey::EQUALS;
  case (VK_OEM_5):
    return LogicalKey::BSLASH;
  case (VK_OEM_1):
    return LogicalKey::SEMICOLON;
  case (VK_OEM_7):
    return LogicalKey::QUOTE;
  case (VK_OEM_COMMA):
    return LogicalKey::COMMA;
  case (VK_OEM_PERIOD):
    return LogicalKey::PERIOD;
  case (VK_OEM_2):
    return LogicalKey::FSLASH;
  case (VK_SHIFT):
    return LogicalKey::SHIFT;
  case (VK_SPACE):
    return LogicalKey::SPACE;
  case (VK_RETURN):
    return LogicalKey::NEWLINE;
  case (VK_BACK):
    return LogicalKey::BACKSPACE;
  case (VK_LEFT):
    return LogicalKey::LARROW;
  case (VK_UP):
    return LogicalKey::UARROW;
  case (VK_RIGHT):
    return LogicalKey::RARROW;
  case (VK_DOWN):
    return LogicalKey::DARROW;
  case (VK_HOME):
    return LogicalKey::HOME;
  case (VK_END):
    return LogicalKey::END;
  case (VK_PRIOR):
    return LogicalKey::PAGEUP;
  case (VK_NEXT):
    return LogicalKey::PAGEDOWN;
  case (VK_INSERT):
    return LogicalKey::INS;
  case (VK_DELETE):
    return LogicalKey::DEL;
  case (VK_ESCAPE):
    return LogicalKey::ESC;

  default:
    return LogicalKey::UNKNOWN;
  }
}

KeyboardManager::HackKey 
KeyboardManager::LogicalKeyToHackKey(LogicalKey key, bool isShiftDown) {
  constexpr size_t idxA = (size_t)LogicalKey::A;
  constexpr size_t idxZ = (size_t)LogicalKey::Z;
  constexpr size_t idx0 = (size_t)LogicalKey::D0;
  constexpr size_t idx9 = (size_t)LogicalKey::D9;
  constexpr size_t idxF1 = (size_t)LogicalKey::F1;
  constexpr size_t idxF12 = (size_t)LogicalKey::F12;

  uint16_t whichKey = (uint16_t)key;

  if (whichKey >= idxA && whichKey <= idxZ) {
    if (isShiftDown)
      return (HackKey)((uint16_t)HackKey::A + (whichKey - idxA));
    else
      return (HackKey)((uint16_t)HackKey::a + (whichKey - idxA));
  }

  if (whichKey >= idx0 && whichKey <= idx9) {
    if (isShiftDown) {
      switch ((LogicalKey)whichKey) {
        case (LogicalKey::D0): return HackKey::RPAREN;
        case (LogicalKey::D1): return HackKey::EXCLAM;
        case (LogicalKey::D2): return HackKey::AT;
        case (LogicalKey::D3): return HackKey::POUND;
        case (LogicalKey::D4): return HackKey::DOLLAR;
        case (LogicalKey::D5): return HackKey::PERCENT;
        case (LogicalKey::D6): return HackKey::CARET;
        case (LogicalKey::D7): return HackKey::AMPERSAND;
        case (LogicalKey::D8): return HackKey::ASTERISK;
        case (LogicalKey::D9): return HackKey::LPAREN;
        default:               return HackKey::NONE; // impossible
      }
    }
    else {
      return (HackKey)((uint16_t)HackKey::D0 + (whichKey - idx0));
    }
  }

  if (whichKey >= idxF1 && whichKey <= idxF12) {
    return (HackKey)((uint16_t)HackKey::F1 + (whichKey - idxF1));
  }

  // Keys other than a-z, A-Z, 0-9, and !@#$%^&*()

  switch ((LogicalKey)whichKey) {
    case (LogicalKey::BACKTICK):
      return isShiftDown ? HackKey::TILDA    : HackKey::BACKTICK;
    case (LogicalKey::LBRACKET):
      return isShiftDown ? HackKey::LBRACE   : HackKey::LBRACKET;
    case (LogicalKey::RBRACKET):
      return isShiftDown ? HackKey::RBRACE   : HackKey::RBRACKET;
    case (LogicalKey::HYPHEN):
      return isShiftDown ? HackKey::USCORE   : HackKey::HYPHEN;
    case (LogicalKey::EQUALS):
      return isShiftDown ? HackKey::PLUS     : HackKey::EQUALS;
    case (LogicalKey::BSLASH):
      return isShiftDown ? HackKey::PIPE     : HackKey::BSLASH;
    case (LogicalKey::SEMICOLON):
      return isShiftDown ? HackKey::COLON    : HackKey::SEMICOLON;
    case (LogicalKey::QUOTE):
      return isShiftDown ? HackKey::DQUOTE   : HackKey::SQUOTE;
    case (LogicalKey::COMMA):
      return isShiftDown ? HackKey::LANGLE   : HackKey::COMMA;
    case (LogicalKey::PERIOD):
      return isShiftDown ? HackKey::RANGLE   : HackKey::PERIOD;
    case (LogicalKey::FSLASH):
      return isShiftDown ? HackKey::QUESTION : HackKey::FSLASH;

    case (LogicalKey::SPACE):     return HackKey::SPACE;
    case (LogicalKey::NEWLINE):   return HackKey::NEWLINE;
    case (LogicalKey::BACKSPACE): return HackKey::BACKSPACE;
    case (LogicalKey::LARROW):    return HackKey::LARROW;
    case (LogicalKey::UARROW):    return HackKey::UARROW;
    case (LogicalKey::RARROW):    return HackKey::RARROW;
    case (LogicalKey::DARROW):    return HackKey::DARROW;
    case (LogicalKey::HOME):      return HackKey::HOME;
    case (LogicalKey::END):       return HackKey::END;
    case (LogicalKey::PAGEUP):    return HackKey::PAGEUP;
    case (LogicalKey::PAGEDOWN):  return HackKey::PAGEDOWN;
    case (LogicalKey::INS):       return HackKey::INS;
    case (LogicalKey::DEL):       return HackKey::DEL;
    case (LogicalKey::ESC):       return HackKey::ESC;

    default:                      return HackKey::NONE;
  }
}
