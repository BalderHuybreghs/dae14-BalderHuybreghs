#pragma once
#include "EditTool.h"
#include "ObjectBlueprint.h"

class ObjectTool final : public EditTool
{
public:
  ObjectTool(Level* levelPtr, const InputManager* inputManagerPtr);

  void Draw(const Camera* cameraPtr) const;

  void Update(float elapsedSec, const Rectf& hoveringTile) override;

  void OnMouseWheelEvent(const SDL_MouseWheelEvent& e) override;
  virtual void OnMouseDownEvent(const SDL_MouseButtonEvent& e) override;
private:
  Level* m_LevelPtr;

  ObjectBlueprint* m_CurrentObjectPtr; // The current object the user is placing
};