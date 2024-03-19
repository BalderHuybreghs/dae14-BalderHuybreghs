#pragma once
#include "ScreenManager.h"
#include "Level.h"

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
    Decal,
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

  void ChangeMode();
private:
  Level* m_LevelPtr;
};

