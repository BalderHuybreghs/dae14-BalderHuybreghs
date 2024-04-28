#pragma once
#include "Level.h"
#include "InputManager.h"

// The base class for editor tools
class EditTool
{
public:
  virtual ~EditTool();

  // An editor tool can obviously draw some information about itself
  // the tool gets the camera so it can handle the matrices right for UI and world stuff
  // The default implementation just draws it's own name
  virtual void Draw(const Camera* cameraPtr) const;

  // Hovering tile is the tile the user is hovering over
  virtual void Update(float elapsedSec, const Rectf& hoveringTile) = 0;

  // Edit tools can listen to the mousewheel
  virtual void OnMouseWheelEvent(const SDL_MouseWheelEvent& e) {};
  virtual void OnMouseDownEvent(const SDL_MouseButtonEvent& e) {};
  virtual void OnMouseUpEvent(const SDL_MouseButtonEvent& e) {};
protected:
  // The name of the tool will be drawn on top of the screen
  EditTool(const std::string& name, const InputManager* inputManagerPtr);

  const InputManager* m_InputManagerPtr;
  const Texture* m_NameTexturePtr;
};