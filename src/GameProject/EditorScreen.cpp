#include "pch.h"
#include "EditorScreen.h"
#include "GameDefines.h"
#include <iostream>
#include "utils.h"
#include "RectangleShape.h"

using namespace utils;

EditorScreen::EditorScreen()
  : GameScreen(), m_LevelPtr(nullptr), m_CurrentTilemapPtr(nullptr), 
  m_EditMode(Mode::TilesetFront), m_CurrentTile(0), 
  m_MousePos(Point2f()), m_CameraPtr(nullptr), m_MouseDragBorder(nullptr)
{
  
}

void EditorScreen::Initialize()
{
  m_LevelPtr = new Level(LEVEL1_NAME, "snow", TILEMAP_BG_PREFIX + "dirt");

  m_EditMode = Mode::TilesetFront;
  m_CurrentTilemapPtr = m_LevelPtr->GetFrontTilemap();
  m_CurrentTile = 0;

  m_LevelPtr->Load();

  const Rectf cameraRect{ 0.f, 0.f, WINDOW_WIDTH, WINDOW_HEIGHT };
  m_CameraPtr = new Camera(cameraRect);
  m_MouseDragBorder = new RectangleShape(Point2f{ WINDOW_WIDTH - MOUSE_DRAG_BORDER_MARGIN_HORIZONTAL * 2, WINDOW_HEIGHT - MOUSE_DRAG_BORDER_MARGIN_VERTICAL * 2 }, Point2f{ MOUSE_DRAG_BORDER_MARGIN_HORIZONTAL, MOUSE_DRAG_BORDER_MARGIN_VERTICAL });
}

EditorScreen::~EditorScreen()
{
  delete m_LevelPtr;
  delete m_CameraPtr;
  delete m_MouseDragBorder;
}

void EditorScreen::Draw()
{
  // Everything will be drawn with debug info in the editor by default

  m_CameraPtr->PushMatrix();

  m_LevelPtr->Draw(true);

  // Draw an outline around the tile the player may build
  SetColor(Color4f{ 1.f, 0.f, 1.f, 1.f });
  const Rectf selectedTileRect{ m_CurrentTilemapPtr->GetTileRect(ToWorldPos(m_MousePos)) };

  // Draw red if we are working in negative coordinates
  if (selectedTileRect.left < 0 || selectedTileRect.bottom < 0) {
    SetColor(Color4f{ 1.f, 0.f, 0.f, 1.f });
  }

  DrawRect(selectedTileRect, 2.f);

  m_CameraPtr->PopMatrix();

  m_CurrentTilemapPtr->DrawSingleTile(Point2f{ m_MousePos.x + 10, m_MousePos.y - 60 }, m_CurrentTile);

  // Since finding the mouse with a white background might not always be obvious
  SetColor(Color4f{ 1.f, 0.f, 1.f, 1.f });
  FillEllipse(m_MousePos, 7.f, 7.f);

  m_MouseDragBorder->Draw();
}

void EditorScreen::Update(float elapsedSec)
{
  // The level itself should not be updated in the editor
  m_CameraPtr->Update(elapsedSec);

  // If the mouse is outside of a given area, move the camera over the world (warcraft style)
  if (!m_MouseDragBorder->IsPointInside(m_MousePos)) {
    Rectf cameraRect{ m_CameraPtr->GetRect() };

    const Vector2f mouseFromCenter(
      m_MousePos.x - WINDOW_WIDTH / 2,
      m_MousePos.y - WINDOW_HEIGHT / 2
    );

    const Vector2f direction{ mouseFromCenter.Normalized() };

    cameraRect.left += CAMERA_DRAG_SPEED * direction.x;
    cameraRect.bottom += CAMERA_DRAG_SPEED * direction.y;

    m_CameraPtr->SetRect(cameraRect);
  }
}

void EditorScreen::OnKeyDownEvent(SDL_KeyboardEvent key)
{
  switch (key.keysym.sym)
  {
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
