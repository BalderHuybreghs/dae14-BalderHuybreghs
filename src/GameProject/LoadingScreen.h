#pragma once
#include "ScreenManager.h"
#include "Sprite.h"
#include "ParticleEmitter.h"

class LoadingScreen final : public GameScreen
{
public:
  LoadingScreen(GameScreen* nextScreenPtr, float loadingTime);

  LoadingScreen(const LoadingScreen& other) = delete;
  LoadingScreen(const LoadingScreen&& other) = delete;
  LoadingScreen& operator=(const LoadingScreen& other) = delete;
  LoadingScreen& operator=(const LoadingScreen&& other) = delete;
  ~LoadingScreen();


  void Initialize() override;

  // General game functions
  void Draw() override;
  void Update(float elapsedSec) override;
private:
  GameScreen* m_NextScreenPtr;
  Sprite* m_LogoPtr;
  Sprite* m_LoadingPtr;

  // A particle emitter for snow particles
  ParticleEmitter* m_ParticleEmitterPtr;

  float m_TimeLeft;
};
