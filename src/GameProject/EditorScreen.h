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

  EditorScreen(const std::string& levelName, Point2f cameraPos);
  EditorScreen(const std::string& levelName);

  void Initialize() override;

  ~EditorScreen();

  // General game functions
  void Draw() override;
  void Update(float elapsedSec) override;

  // Key events
  void OnKeyDownEvent(const SDL_KeyboardEvent& key) override;
  void OnKeyUpEvent(const SDL_KeyboardEvent& key) override;
  void OnMouseMotionEvent(const SDL_MouseMotionEvent& e) override;
  void OnMouseDownEvent(const SDL_MouseButtonEvent& e) override;
  void OnMouseUpEvent(const SDL_MouseButtonEvent& e) override;
  void OnMouseWheelEvent(const SDL_MouseWheelEvent& e) override;
private:
  std::string m_LevelName;

  Mode m_EditMode;

  Level* m_LevelPtr;
  Tilemap* m_CurrentTilemapPtr; // Should not be deleted, holds a reference to the tilemap within the level
  Camera* m_CameraPtr;

  Shape* m_MouseDragBorder;

  int m_CurrentTile;
};

