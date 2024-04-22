#include "pch.h"
#include "EditorScreen.h"
#include "GameDefines.h"
#include <iostream>
#include "utils.h"
#include "RectangleShape.h"
#include "MathUtils.h"
#include "PlayingScreen.h"
#include "TilemapTool.h"
#include "CameraTool.h"
#include "BackgroundTool.h"

using namespace utils;

EditorScreen::EditorScreen(const std::string& levelName, Point2f cameraPos)
  : GameScreen(), m_LevelPtr(nullptr), m_CurrentTilemapPtr(nullptr),
  m_Tool(nullptr), m_LevelName(levelName), m_CameraPtr(new Camera(cameraPos)), 
  m_Tools(std::vector<EditTool*>()), m_ToolId(0)
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

  // Create the tools
  TilemapTool* frontTilemapTool{ new TilemapTool(m_LevelPtr->GetFrontTilemap(), m_InputManagerPtr) };
  TilemapTool* backTilemapTool{ new TilemapTool(m_LevelPtr->GetBackTilemap(), m_InputManagerPtr) };
  CameraTool* cameraTool{ new CameraTool(m_LevelPtr, m_InputManagerPtr) };
  BackgroundTool* backgroundTool{ new BackgroundTool(m_LevelPtr->GetBackground(), m_InputManagerPtr)};

  m_Tools.push_back(frontTilemapTool);
  m_Tools.push_back(backTilemapTool);
  m_Tools.push_back(cameraTool);
  m_Tools.push_back(backgroundTool);

  m_Tool = m_Tools.front();
}

EditorScreen::~EditorScreen()
{
  // Save the level before exiting
  m_LevelPtr->Save();

  delete m_LevelPtr;
  delete m_CameraPtr;

  m_Tool = nullptr;
  for (EditTool* tool : m_Tools) {
    delete tool;
    tool = nullptr;
  }
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

  // Draw the current tool
  if (m_Tool != nullptr) {
    m_Tool->Draw(m_CameraPtr);
  }
}

void EditorScreen::Update(float elapsedSec)
{ 
  if (m_Tool != nullptr) {
    m_Tool->Update(elapsedSec, m_CurrentTilemapPtr->GetTileRect(m_CameraPtr->GetWorldPosition(m_InputManagerPtr->GetMousePosition())));
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

  // Camera zoom
  const float zoomFactor{ 1.0f + (0.2f * elapsedSec) };
  if (m_InputManagerPtr->IsKeyDown(SDLK_z)) {
    // Zoom out
    m_CameraPtr->SetZoom(m_CameraPtr->GetZoom() / zoomFactor);

    // Adjust camera position to keep the center fixed
    cameraPosition.x /= zoomFactor;
    cameraPosition.y /= zoomFactor;
  } else if (m_InputManagerPtr->IsKeyDown(SDLK_x)) {
    // Zoom in
    m_CameraPtr->SetZoom(m_CameraPtr->GetZoom() * zoomFactor);

    // Adjust camera position to keep the center fixed
    cameraPosition.x *= zoomFactor;
    cameraPosition.y *= zoomFactor;
  }

  m_CameraPtr->SetPosition(cameraPosition);
}

void EditorScreen::OnKeyDownEvent(const SDL_KeyboardEvent& key)
{
  switch (key.keysym.sym)
  {
  case SDLK_e:
    if (m_ToolId >= m_Tools.size() -1) {
      break;
    }

    m_Tool = m_Tools.at(++m_ToolId);
    break;
  case SDLK_q:
    if (m_ToolId < 1) {
      break;
    }

    m_Tool = m_Tools.at(--m_ToolId);
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
  if (m_Tool != nullptr) {
    m_Tool->OnMouseWheelEvent(e);
  }
}