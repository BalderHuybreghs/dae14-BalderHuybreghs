#include "RectangleTool.h"
#include "ParallaxBackground.h"

class BackgroundTool final : public RectangleTool
{
public:
  enum class BackgroundLayer
  {
    Back,
    Front
  };

  BackgroundTool(ParallaxBackground* backgroundPtr, const InputManager* inputManagerPtr);

  void OnMouseWheelEvent(const SDL_MouseWheelEvent& e) override;

private:
  void OnPaintRect(const Rectf& rect) override;

  ParallaxBackground* m_BackgroundPtr;

  BackgroundLayer m_Layer;
};
