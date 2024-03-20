#pragma once
#include "ScreenManager.h"
#include "Level.h"
#include "Tilemap.h"
#include "ObjectBlueprint.h"
#include "ObjectManager.h"
#include "Camera.h"
#include "Shape.h"

class EditorScreen : public GameScreen
{
public:
  // Level editing has several modes
  // Tileset: Place tiles on the map
  // Object: Place objects on the map
  // Decal Place decals on the map
  enum class Mode
  {
    TilesetFront,
    TilesetBack,
    Object,
    Decal
  };

  EditorScreen();

  void Initialize() override;

  ~EditorScreen();

  // General game functions
  void Draw() override;
  void Update(float elapsedSec) override;

  // Key events
  void OnKeyDownEvent(SDL_KeyboardEvent key) override;
  void OnKeyUpEvent(SDL_KeyboardEvent key) override;
  void OnMouseMotionEvent(const SDL_MouseMotionEvent& e) override;
  void OnMouseDownEvent(const SDL_MouseButtonEvent& e) override;
  void OnMouseUpEvent(const SDL_MouseButtonEvent& e) override;

  Point2f ToWorldPos(const Point2f& position);
private:
  Mode m_EditMode;
  Point2f m_MousePos;

  Level* m_LevelPtr;
  Tilemap* m_CurrentTilemapPtr; // Should not be deleted, holds a reference to the tilemap within the level
  Camera* m_CameraPtr;

  Shape* m_MouseDragBorder;

  int m_CurrentTile;
};

