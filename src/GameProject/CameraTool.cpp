#include "pch.h"
#include "CameraTool.h"
#include "utils.h"

CameraTool::CameraTool(Level* levelPtr, const InputManager* inputManagerPtr)
  : EditTool("Camera", inputManagerPtr), m_LevelPtr(levelPtr), m_IsDrawing(false), m_StartPosition(Point2f()), m_EndPosition(Point2f())
{
}

void CameraTool::Draw(const Camera* cameraPtr) const
{
  EditTool::Draw(cameraPtr);

  // Draw the current rect if the user is drawing
  if (m_IsDrawing) {
    cameraPtr->PushMatrix();
    utils::SetColor(Color4f{ 0.5f, 1.f, 1.f, 1.f }); // Random color lol
    utils::DrawRect(GetRect());
    cameraPtr->PopMatrix();
  }
}

void CameraTool::Update(float elapsedSec, const Rectf& hoveringTile)
{
  // Remove a camera rect
  if (m_InputManagerPtr->IsMouseDown(SDL_BUTTON_RIGHT)) {
    m_LevelPtr->RemoveCameraRect(hoveringTile.Center());
  }

  if (m_InputManagerPtr->IsMouseDown(SDL_BUTTON_LEFT)) {
    if (m_IsDrawing) {
      const Point2f position{
        hoveringTile.left + hoveringTile.width,
        hoveringTile.bottom + hoveringTile.height
      };

      m_EndPosition = position;
      return;
    }

    const Point2f position{
      hoveringTile.left,
      hoveringTile.bottom
    };

    m_StartPosition = position;
    m_IsDrawing = true;
  } else {
    if (m_IsDrawing) {
      m_LevelPtr->AddCameraRect(GetRect());
    }

    m_IsDrawing = false;
  }
}

Rectf CameraTool::GetRect() const
{
  return Rectf(
    m_StartPosition.x,
    m_StartPosition.y,
    m_EndPosition.x - m_StartPosition.x,
    m_EndPosition.y - m_StartPosition.y
  );
}
