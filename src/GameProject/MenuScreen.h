#pragma once
#include "ScreenManager.h"

class MenuScreen : public GameScreen
{
public:
  MenuScreen();

  MenuScreen(const MenuScreen& other) = delete;
  MenuScreen(const MenuScreen&& other) = delete;
  MenuScreen& operator=(const MenuScreen& other) = delete;
  MenuScreen& operator=(const MenuScreen&& other) = delete;
  ~MenuScreen();


  void Initialize() override;

  // General game functions
  void Draw() override;
  void Update(float elapsedSec) override;

  // Key events
  void OnKeyDownEvent(const SDL_KeyboardEvent& key) override;
  void OnKeyUpEvent(const SDL_KeyboardEvent& key) override;
  void OnMouseMotionEvent(const SDL_MouseMotionEvent& e) override;
  void OnMouseDownEvent(const SDL_MouseButtonEvent& e) override;
  void OnMouseUpEvent(const SDL_MouseButtonEvent& e) override;
};