#pragma once
#include "ScreenManager.h"
#include "Sprite.h"

class LoadingScreen : public GameScreen
{
public:
  LoadingScreen(float loadingTime);
  ~LoadingScreen();

  void Initialize() override;

  // General game functions
  void Draw() override;
  void Update(float elapsedSec) override;
private:
  Sprite* m_LogoSprite;
  Sprite* m_LoadingSprite;

  float m_TimeLeft;
};
