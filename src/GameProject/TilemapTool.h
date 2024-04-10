#pragma once
#include "EditTool.h"
#include "Tilemap.h"

// This tool allows for the manipulation of the tilemap
class TilemapTool final : public EditTool
{
public:
  TilemapTool(Tilemap* tilemapPtr, const InputManager* inputManagerPtr);

  void Draw(const Camera* cameraPtr) const;

  void Update(float elapsedSec, const Rectf& hoveringTile) override;

  void OnMouseWheelEvent(const SDL_MouseWheelEvent& e) override;
private:
  Tilemap* m_TilemapPtr;

  int m_CurrentTile; // The current tile for editing
};