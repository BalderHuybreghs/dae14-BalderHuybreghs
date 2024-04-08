#pragma once
#include "ScreenManager.h"
#include "Sprite.h"

class LoadingScreen final : public GameScreen
{
public:
  LoadingScreen(GameScreen* nextScreenPtr, float loadingTime);

  LoadingScreen(const LoadingScreen& other) = delete;
  LoadingScreen& operator=(const LoadingScreen& other) = delete;
  ~LoadingScreen();

  void Initialize() override;

  // General game functions
  void Draw() override;
  void Update(float elapsedSec) override;
private:
  GameScreen* m_NextScreenPtr;
  Sprite* m_LogoPtr;
  Sprite* m_LoadingPtr;

  float m_TimeLeft;
};
