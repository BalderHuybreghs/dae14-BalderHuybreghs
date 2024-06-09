#pragma once
#include "RectangleTool.h"
#include "Level.h"

// This class is used to determine the area where the game should play different music for the cassette area
class CassetteAreaTool : public RectangleTool
{
public:
  CassetteAreaTool(Level* levelPtr, const InputManager* inputManagerPtr);

  void OnPaintRect(const Rectf& rect) override;
private:
  Level* m_LevelPtr;
};

