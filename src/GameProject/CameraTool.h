#pragma once
#include "EditTool.h"
#include "Level.h"

// This tools allows for the manipulation of camera rects
class CameraTool final : public EditTool
{
public:
  CameraTool(Level* levelPtr, const InputManager* inputManagerPtr);

  void Draw(const Camera* cameraPtr) const;

  void Update(float elapsedSec, const Rectf& hoveringTile) override;
private:
  Level* m_LevelPtr;

  Point2f m_StartPosition; // The position where the user started drawing
  Point2f m_EndPosition; // The position where the user ended drawing
  bool m_IsDrawing; // Determine if the user is rawing

  Rectf GetRect() const; // Gets the rectangle from both positions
};