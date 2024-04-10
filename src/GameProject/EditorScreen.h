#pragma once
#include "ScreenManager.h"
#include "Level.h"
#include "Tilemap.h"
#include "Camera.h"
#include "Shape.h"
#include "EditTool.h"

class EditorScreen final : public GameScreen
{
public:
  EditorScreen(const std::string& levelName, Point2f cameraPos);
  EditorScreen(const std::string& levelName);

  void Initialize() override;

  EditorScreen(const EditorScreen& other) = delete;
  EditorScreen& operator=(const EditorScreen& other) = delete;
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

  // A list of all the tools the user can switch to, this so that we do not loose tool information when switching
  std::vector<EditTool*> m_Tools;
  size_t m_ToolId; // The ID of the current tool
  EditTool* m_Tool;

  Level* m_LevelPtr;
  Tilemap* m_CurrentTilemapPtr; // Should not be deleted, holds a reference to the tilemap within the level
  Camera* m_CameraPtr;
};

