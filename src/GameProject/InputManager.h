#pragma once
#include <unordered_map>
#include <SDL_keyboard.h>
#include <SDL_mouse.h>

// This class keeps track of the input state
// This is useful to check if a key is being held
// or not. It also solves the issue of not being
// able to press multiple keys at a time, which was one
// of my frustrations when working with the default implementation
class InputManager final
{
public:
  // No special constructor needed, rule of 3 also not required

  // Update input state based on SDL events
  void Update(const SDL_Event& event);

  // Check if a key is currently pressed
  bool IsKeyDown(SDL_Keycode key) const;

  // Check if a mouse button is currently pressed
  bool IsMouseDown(Uint32 mouseButton) const;

  // Get the current mouse position
  Point2f GetMousePosition() const;
private:
  // Map to store the current state of each key
  std::unordered_map<SDL_Keycode, bool> m_KeyStates;

  // Map to store the current state of each mouse button
  std::unordered_map<Uint32, bool> m_MouseButtonStates;

  // Current mouse position
  SDL_Point m_MousePosition;
};