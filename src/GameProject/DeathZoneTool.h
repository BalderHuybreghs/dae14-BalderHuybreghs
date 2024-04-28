#pragma once
#include "RectangleTool.h"

class DeathZoneTool final : public RectangleTool
{
public:
  DeathZoneTool(Level* levelPtr, const InputManager* inputManagerPtr);

  void Update(float elapsedSec, const Rectf& hoveringTile) override;
private:
  void OnPaintRect(const Rectf& rect) override;

  Level* m_LevelPtr;
};

