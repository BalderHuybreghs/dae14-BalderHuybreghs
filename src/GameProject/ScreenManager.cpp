#include "pch.h"
#include "ScreenManager.h"

ScreenManager::ScreenManager()
  : m_ParentPtr(nullptr), m_PreviousScreenPtr(nullptr), m_CurrentScreenPtr(nullptr)
{
}

ScreenManager::~ScreenManager()
{
  delete m_CurrentScreenPtr;
}

void ScreenManager::SetScreen(GameScreen* newScreenPtr)
{
  delete m_CurrentScreenPtr;
  m_CurrentScreenPtr = newScreenPtr;
  m_CurrentScreenPtr->SetScreenManager(this);
  m_CurrentScreenPtr->Initialize();
}

void ScreenManager::SetParent(ScreenManager* parentPtr)
{
  delete m_ParentPtr;
  m_ParentPtr = parentPtr;
}

GameScreen* ScreenManager::GetScreen()
{
  return m_CurrentScreenPtr;
}

ScreenManager* ScreenManager::GetParent()
{
  return m_ParentPtr == nullptr ? this : m_ParentPtr; // Return self if parent is null
}

GameScreen::GameScreen()
  : m_ScreenManagerPtr(nullptr)
{
}

void GameScreen::SetScreenManager(ScreenManager* screenManagerPtr)
{
  m_ScreenManagerPtr = screenManagerPtr;
}
