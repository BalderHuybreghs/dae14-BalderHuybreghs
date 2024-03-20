#pragma once
#include "ScreenManager.h"
#include "Level.h"
#include "Camera.h"
#include "Player.h"

class PlayingScreen : public GameScreen
{
public:
  PlayingScreen();
  ~PlayingScreen();

  void Initialize() override;

  // General game functions
  void Draw() override;
  void Update(float elapsedSec) override;

  // Key events
  void OnKeyDownEvent(SDL_KeyboardEvent key) override;
  void OnKeyUpEvent(SDL_KeyboardEvent key) override;
  void OnMouseMotionEvent(const SDL_MouseMotionEvent& e) override;
  void OnMouseDownEvent(const SDL_MouseButtonEvent& e) override;
  void OnMouseUpEvent(const SDL_MouseButtonEvent& e) override;

private:
  Level* m_LevelPtr;
  Player* m_PlayerPtr;
  Camera* m_CameraPtr;
};