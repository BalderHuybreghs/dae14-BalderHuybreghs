#include "pch.h"
#include "InputManager.h"
#include "GameDefines.h"

void InputManager::Update(const SDL_Event& event)
{
  // Go through the event types and handle accordingly
  switch (event.type)
  {
  case SDL_KEYDOWN:
    m_KeyStates.insert_or_assign(event.key.keysym.sym, true);
    break;
  case SDL_KEYUP:
    m_KeyStates.insert_or_assign(event.key.keysym.sym, false);
    break;
  case SDL_MOUSEBUTTONDOWN:
    m_MouseButtonStates.insert_or_assign(event.button.button, true);
    break;
  case SDL_MOUSEBUTTONUP:
    m_MouseButtonStates.insert_or_assign(event.button.button, false);
    break;
  case SDL_MOUSEMOTION:
    m_MousePosition.x = event.motion.x;
    m_MousePosition.y = event.motion.y;
    break;
  }
}

bool InputManager::IsKeyDown(SDL_Keycode key) const
{
  std::unordered_map<SDL_Keycode, bool>::const_iterator it = m_KeyStates.find(key);
  if (it != m_KeyStates.end())
  {
    return it->second;
  }

  // If the key has not been found, it has probably not been pressed
  return false;
}

bool InputManager::IsMouseDown(Uint32 mouseButton) const
{
  std::unordered_map<Uint32, bool>::const_iterator it = m_MouseButtonStates.find(mouseButton);
  if (it != m_MouseButtonStates.end())
  {
    return it->second;
  }

  // If the button is not found, it has probably not been pressed
  return false;
}

Point2f InputManager::GetMousePosition() const
{
  return Point2f{ (float)m_MousePosition.x, WINDOW_HEIGHT - (float)m_MousePosition.y };
}