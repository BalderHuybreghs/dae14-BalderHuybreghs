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
  m_LogoPtr = new Sprite(Point2f{ WINDOW_WIDTH / 2.f - 1920.f / 4, WINDOW_HEIGHT / 2.f - 1080.f / 4 }, Point2f{ 1920.f / 2, 1080.f / 2 }, Point2f{ 1920.f, 1080.f }, 0, LOGO_RESOURCE);
  m_LoadingPtr = new Sprite(Point2f{ WINDOW_WIDTH - 120.f, 20.f }, Point2f{ 100.f, 100.f }, Point2f{ 200.f, 200.f }, FRAMES_PER_SECOND, LOADING_RESOURCE);
}

void LoadingScreen::Draw()
{
  m_LogoPtr->Draw();
  m_LoadingPtr->Draw();
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
