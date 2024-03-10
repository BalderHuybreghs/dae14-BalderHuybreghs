#pragma once
#include "ScreenManager.h"

class EditorScreen : public GameScreen
{
  EditorScreen();
  ~EditorScreen();

  // General game functions
  void Draw();
  void Update(float elapsedSec);

  // Key events
  void OnKeyDownEvent(SDL_KeyboardEvent key);
  void OnKeyUpEvent(SDL_KeyboardEvent key);
  void OnMouseMotionEvent(const SDL_MouseMotionEvent& e);
  void OnMouseDownEvent(const SDL_MouseButtonEvent& e);
  void OnMouseUpEvent(const SDL_MouseButtonEvent& e);
};

