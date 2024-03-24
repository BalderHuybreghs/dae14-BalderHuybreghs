#include "pch.h"
#include "Game.h"
#include "ObjectManager.h"
#include "TextureManager.h"
#include "GameDefines.h"

Game::Game(const Window& window, GameScreen* initialScreenPtr)
	: BaseGame{ window }
{
	Initialize(initialScreenPtr);
}

Game::~Game()
{
	Cleanup();
}

void Game::Initialize(GameScreen* initialScreenPtr)
{
  m_ScreenManagerPtr = new ScreenManager();
  m_ScreenManagerPtr->SetScreen(initialScreenPtr);
}

void Game::Cleanup()
{
  delete m_ScreenManagerPtr;
  m_ScreenManagerPtr = nullptr;

  // Cleanup the managers, as the resources will not be used anymore at this point
  ObjectManager::DestroyInstance();
  TextureManager::DestroyInstance();
}

void Game::Update(float elapsedSec)
{
  m_ScreenManagerPtr->GetScreen()->Update(elapsedSec);
}

void Game::Draw() const
{
	ClearBackground(BACKGROUND_COLOR);

  m_ScreenManagerPtr->GetScreen()->Draw();
}

void Game::ProcessInputEvent(const SDL_Event& e)
{
  m_ScreenManagerPtr->GetScreen()->OnInputEvent(e);
}

void Game::ProcessKeyDownEvent(const SDL_KeyboardEvent& e)
{
  m_ScreenManagerPtr->GetScreen()->OnKeyDownEvent(e);
}

void Game::ProcessKeyUpEvent(const SDL_KeyboardEvent& e)
{
	m_ScreenManagerPtr->GetScreen()->OnKeyUpEvent(e);
}

void Game::ProcessMouseMotionEvent(const SDL_MouseMotionEvent& e)
{
  m_ScreenManagerPtr->GetScreen()->OnMouseMotionEvent(e);
}

void Game::ProcessMouseDownEvent(const SDL_MouseButtonEvent& e)
{
  m_ScreenManagerPtr->GetScreen()->OnMouseDownEvent(e);
}

void Game::ProcessMouseUpEvent(const SDL_MouseButtonEvent& e)
{
  m_ScreenManagerPtr->GetScreen()->OnMouseUpEvent(e);
}

void Game::ProcessMouseWheelEvent(const SDL_MouseWheelEvent& e)
{
  m_ScreenManagerPtr->GetScreen()->OnMouseWheelEvent(e);
}

void Game::ClearBackground(Color4f color) const
{
	glClearColor(color.r, color.g, color.b, color.a);
	glClear( GL_COLOR_BUFFER_BIT );
}
