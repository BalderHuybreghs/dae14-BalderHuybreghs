#include "pch.h"
#include "LoadingScreen.h"
#include "GameDefines.h"

LoadingScreen::LoadingScreen(GameScreen* nextScreenPtr, float loadingTime)
  : m_NextScreenPtr(nextScreenPtr), m_LogoPtr(nullptr), m_LoadingPtr(nullptr), m_TimeLeft(loadingTime)
{
}

LoadingScreen::~LoadingScreen()
{
  delete m_LogoPtr;
  delete m_LoadingPtr;
}

void LoadingScreen::Initialize()
{
  m_LogoPtr = new Sprite(Point2f{ LOGO_WIDTH, LOGO_HEIGHT }, 0, LOGO_RESOURCE);
  m_LoadingPtr = new Sprite(Point2f{ 200.f, 200.f }, FRAMES_PER_SECOND, LOADING_RESOURCE);
}

void LoadingScreen::Draw()
{
  const float scale{ WINDOW_HEIGHT / WINDOW_WIDTH };
  const Point2f logoPos{
    WINDOW_WIDTH  / 2.f - LOGO_WIDTH  * scale / 2.f,
    WINDOW_HEIGHT / 2.f - LOGO_HEIGHT * scale / 2.f
  };

  const Point2f loadingPos{
    WINDOW_WIDTH - 120.f,
    20.f
  };

  m_LogoPtr->Draw(logoPos, scale);
  m_LoadingPtr->Draw(loadingPos, scale);
}

void LoadingScreen::Update(float elapsedSec)
{
  // Logosprite does not have an animation
  m_LoadingPtr->Update(elapsedSec);
  
  // Fake loading lol, it's for the "immersion"
  --m_TimeLeft;

  // Set the screen
  if (m_TimeLeft < 1) {
    m_ScreenManagerPtr->SetScreen(m_NextScreenPtr);
  }
}
