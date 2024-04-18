#pragma once
#include "RectangleTool.h"
#include "Level.h"

// This tools allows for the manipulation of camera rects
class CameraTool final : public RectangleTool
{
public:
  CameraTool(Level* levelPtr, const InputManager* inputManagerPtr);

  void Update(float elapsedSec, const Rectf& hoveringTile) override;

protected:
  void OnPaintRect(const Rectf& rect) override;

private:

  Level* m_LevelPtr;
};