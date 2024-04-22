#include "RectangleTool.h"
#include "ParallaxBackground.h"

class BackgroundTool final : public RectangleTool
{
public:
  enum class BackgroundLayer
  {
    Middle,
    Front
  };

  BackgroundTool(ParallaxBackground* backgroundPtr, const InputManager* inputManagerPtr);

  void OnMouseWheelEvent(const SDL_MouseWheelEvent& e) override;
protected:
  void OnPaintRect(const Rectf& rect) override;

private:
  ParallaxBackground* m_BackgroundPtr;

  BackgroundLayer m_Layer;
};
