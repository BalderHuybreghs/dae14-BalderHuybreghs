#include "pch.h"
#include "EditorScreen.h"
#include "GameDefines.h"
#include <iostream>
#include "utils.h"

using namespace utils;

EditorScreen::EditorScreen()
  : GameScreen(), m_LevelPtr(nullptr), m_CurrentTilemapPtr(nullptr), 
  m_EditMode(Mode::TilesetFront), m_CurrentTile(0), 
  m_MousePos(Point2f()), m_CameraPtr(nullptr)
{
  
}

void EditorScreen::Initialize()
{
  m_LevelPtr = new Level(LEVEL1_NAME, "snow", TILEMAP_BG_PREFIX + "dirt");

  m_EditMode = Mode::TilesetFront;
  m_CurrentTilemapPtr = m_LevelPtr->GetFrontTilemap();
  m_CurrentTile = 0;

  m_LevelPtr->Load();

  m_CameraPtr = new Camera(Rectf(0.f, 0.f, 480.f, 480.f));
}

EditorScreen::~EditorScreen()
{
  delete m_LevelPtr;
  delete m_CameraPtr;
}

void EditorScreen::Draw()
{
  // Everything will be drawn with debug info in the editor by default

  m_CameraPtr->PushMatrix();

  m_LevelPtr->Draw(true);

  // Draw an outline around the tile the player may build
  SetColor(Color4f{ 1.f, 0.f, 1.f, 1.f });
  DrawRect(m_CurrentTilemapPtr->GetTileRect(ToWorldPos(m_MousePos)), 2.f);

  m_CameraPtr->PopMatrix();

  m_CurrentTilemapPtr->DrawSingleTile(Point2f{ m_MousePos.x + 10, m_MousePos.y - 60 }, m_CurrentTile);

  // Since finding the mouse with a white background might not always be obvious
  FillEllipse(m_MousePos, 7.f, 7.f);
}

void EditorScreen::Update(float elapsedSec)
{
  // The level itself should not be updated in the editor
  m_CameraPtr->Update(elapsedSec);
}

void EditorScreen::OnKeyDownEvent(SDL_KeyboardEvent key)
{
  Rectf cameraRect{ m_CameraPtr->GetRect() };

  switch (key.keysym.sym)
  {
  case SDLK_w:
    cameraRect.bottom += 10;
    break;
  case SDLK_s:
    cameraRect.bottom -= 10;
    break;
  case SDLK_d:
    cameraRect.left += 10;
    break;
  case SDLK_a:
    cameraRect.left -= 10;
    break;
  case SDLK_e:
    m_CurrentTile = m_CurrentTile < m_CurrentTilemapPtr->GetTileCount() ? m_CurrentTile + 1 : m_CurrentTile;
    break;
  case SDLK_q:
    m_CurrentTile = m_CurrentTile > 0 ? m_CurrentTile - 1 : 0;
    break;
  case SDLK_SPACE:
    m_LevelPtr->Save();
  	break;
  case SDLK_g:
    m_LevelPtr->SetPlayerSpawn(ToWorldPos(m_MousePos));
    break;
  }

  m_CameraPtr->SetRect(cameraRect);
}

void EditorScreen::OnKeyUpEvent(SDL_KeyboardEvent key)
{
}

void EditorScreen::OnMouseMotionEvent(const SDL_MouseMotionEvent& e)
{
  m_MousePos = Point2f{ float(e.x), float(e.y) };

  switch(e.state) {
    case SDL_BUTTON_LEFT:
      m_CurrentTilemapPtr->SetTile(ToWorldPos(m_MousePos), m_CurrentTile);
      break;
    case SDL_BUTTON_MIDDLE:
      m_CurrentTilemapPtr->RemoveTile(ToWorldPos(m_MousePos));
      break;
  }
}

void EditorScreen::OnMouseDownEvent(const SDL_MouseButtonEvent& e)
{
}

void EditorScreen::OnMouseUpEvent(const SDL_MouseButtonEvent& e)
{
}

Point2f EditorScreen::ToWorldPos(const Point2f& position)
{
  const Rectf cameraRect{ m_CameraPtr->GetRect() };

  return Point2f{
    position.x + cameraRect.left,
    position.y + cameraRect.bottom
  };
}
