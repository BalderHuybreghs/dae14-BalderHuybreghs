#pragma once
#include "EditTool.h"

class RectangleTool : public EditTool
{
public:
  virtual ~RectangleTool() = default;

  virtual void Draw(const Camera* cameraPtr) const;

  virtual void Update(float elapsedSec, const Rectf& hoveringTile) override;
protected:
  // Rectangletool on its own is useless, thus you cannot instantiate it
  RectangleTool(const std::string& name, const InputManager* inputManagerPtr);

  // Inhereting tools may know of this function, which caleld called when the drawing
  // of a rectangle is done
  virtual void OnPaintRect(const Rectf& rect) = 0; // The rect that gets painted

private:
  // These are specific to the rectangle tool, inheriting tools do not need to know of them
  Rectf GetRect() const; // Gets the rectangle from both positions

  Point2f m_BottomLeft; // The position where the user started drawing
  Point2f m_TopRight; // The position where the user ended drawing
  bool m_IsDrawing; // Determine if the user is rawing
};

