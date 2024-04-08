#include "pch.h"
#include "EditorScreen.h"
#include "GameDefines.h"
#include <iostream>
#include "utils.h"
#include "RectangleShape.h"
#include "MathUtils.h"
#include "PlayingScreen.h"

using namespace utils;

EditorScreen::EditorScreen(const std::string& levelName, Point2f cameraPos)
  : GameScreen(), m_LevelPtr(nullptr), m_CurrentTilemapPtr(nullptr),
  m_EditMode(Mode::TilesetFront), m_CurrentTile(0), 
  m_LevelName(levelName), m_CameraPtr(new Camera(cameraPos))
{
}

EditorScreen::EditorScreen(const std::string& levelName)
  : EditorScreen(levelName, Point2f{})
{
  
}

void EditorScreen::Initialize()
{
  m_LevelPtr = new Level(m_LevelName);
  m_CurrentTilemapPtr = m_LevelPtr->GetFrontTilemap();
  m_LevelPtr->Load();

  if (m_CameraPtr->GetPosition() == Point2f{}) {
    const Point2f spawn{ m_LevelPtr->GetPlayerSpawn() };
    const Point2f center{ spawn.x - WINDOW_WIDTH / 2.f, spawn.y - WINDOW_HEIGHT / 2.f };
    m_CameraPtr->SetPosition(center);
  }
}

EditorScreen::~EditorScreen()
{
  // Save the level before exiting
  m_LevelPtr->Save();

  delete m_LevelPtr;
  delete m_CameraPtr;
}

void EditorScreen::Draw()
{
  m_CameraPtr->PushMatrix();

  // Everything will be drawn with debug info in the editor by default
  m_LevelPtr->DrawBackground(*m_CameraPtr, true);
  m_LevelPtr->DrawForeground(*m_CameraPtr, true);

  // Draw an outline around the tile the player may build
  SetColor(Color4f{ 1.f, 0.f, 1.f, 1.f });
  const Rectf selectedTileRect{ m_CurrentTilemapPtr->GetTileRect(m_CameraPtr->GetWorldPosition(m_InputManagerPtr->GetMousePosition())) };

  // Draw red if we are working in negative coordinates
  if (selectedTileRect.left < 0 || selectedTileRect.bottom < 0) {
    SetColor(Color4f{ 1.f, 0.f, 0.f, 1.f });
  }

  DrawRect(selectedTileRect, 2.f);

  m_CameraPtr->PopMatrix();

  // Draw a preview tile
  const Point2f mousePos{ m_InputManagerPtr->GetMousePosition() };
  m_CurrentTilemapPtr->DrawSingleTile(Point2f{ mousePos.x + 10, mousePos.y - 60 }, m_CurrentTile);

  // Since finding the mouse with a white background might not always be obvious
  SetColor(Color4f{ 1.f, 0.f, 1.f, 1.f });
  FillEllipse(mousePos, 7.f, 7.f);
}

void EditorScreen::Update(float elapsedSec)
{
  // Do stuff based on the edit mode 
  switch (m_EditMode) {
  case Mode::TilesetFront:
    m_CurrentTilemapPtr = m_LevelPtr->GetFrontTilemap();
    break;
  case Mode::TilesetBack:
    m_CurrentTilemapPtr = m_LevelPtr->GetBackTilemap();
    break;
  }
 
  // The level itself should not be updated in the editor
  m_CameraPtr->Update(elapsedSec);

  const Point2f mousePos{ m_InputManagerPtr->GetMousePosition() };
  if (m_InputManagerPtr->IsMouseDown(SDL_BUTTON_LEFT)) {
    m_CurrentTilemapPtr->SetTile(m_CameraPtr->GetWorldPosition(mousePos), m_CurrentTile);
  } else if (m_InputManagerPtr->IsMouseDown(SDL_BUTTON_RIGHT)) {
    m_CurrentTilemapPtr->RemoveTile(m_CameraPtr->GetWorldPosition(mousePos));
  }

  // Camera movement
  Point2f cameraPosition{ m_CameraPtr->GetPosition() };
  if (m_InputManagerPtr->IsKeyDown(SDLK_d)) {
    cameraPosition.x += 1000 * elapsedSec;
  }

  if (m_InputManagerPtr->IsKeyDown(SDLK_a)) {
    cameraPosition.x -= 1000 * elapsedSec;
  }

  if (m_InputManagerPtr->IsKeyDown(SDLK_w)) {
    cameraPosition.y += 1000 * elapsedSec;
  }

  if (m_InputManagerPtr->IsKeyDown(SDLK_s)) {
    cameraPosition.y -= 1000 * elapsedSec;
  }

  m_CameraPtr->SetPosition(cameraPosition);
}

void EditorScreen::OnKeyDownEvent(const SDL_KeyboardEvent& key)
{
  switch (key.keysym.sym)
  {
  case SDLK_e:
    m_EditMode = EditorScreen::Mode((int)m_EditMode + 1 % 4);
    m_CurrentTile = 0;
    break;
  case SDLK_q:
    m_EditMode = EditorScreen::Mode((int)m_EditMode - 1 % 4);
    m_CurrentTile = 0;
    break;
  case SDLK_SPACE:
    m_LevelPtr->Save();
  	break;
  case SDLK_g:
    m_LevelPtr->SetPlayerSpawn(m_CameraPtr->GetWorldPosition(m_InputManagerPtr->GetMousePosition()));
    break;

  // Switch to playmode
  case SDLK_TAB:
    m_ScreenManagerPtr->SetScreen(new PlayingScreen(m_LevelName));
    break;
  }
}

void EditorScreen::OnKeyUpEvent(const SDL_KeyboardEvent& key)
{
}

void EditorScreen::OnMouseMotionEvent(const SDL_MouseMotionEvent& e)
{
  // Allow the user to pan the camera with middle mouse button
  if (m_InputManagerPtr->IsMouseDown(SDL_BUTTON_MIDDLE)) {
    Point2f cameraPosition{ m_CameraPtr->GetPosition() };

    cameraPosition.x -= e.xrel;
    cameraPosition.y += e.yrel;

    m_CameraPtr->SetPosition(cameraPosition);

  }
}

void EditorScreen::OnMouseDownEvent(const SDL_MouseButtonEvent& e)
{

}

void EditorScreen::OnMouseUpEvent(const SDL_MouseButtonEvent& e)
{
}

void EditorScreen::OnMouseWheelEvent(const SDL_MouseWheelEvent& e)
{
  if (e.y > 0) // Scroll up
  {
    m_CurrentTile = m_CurrentTile > 0 ? m_CurrentTile - 1 : 0;
  } else if (e.y < 0) // Scroll down
  {
    m_CurrentTile = m_CurrentTile < m_CurrentTilemapPtr->GetTileCount() -1 ? m_CurrentTile + 1 : m_CurrentTile;
  }
}