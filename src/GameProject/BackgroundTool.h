#include "RectangleTool.h"

class BackgroundTool final : public RectangleTool
{
public:
  BackgroundTool(Level* levelPtr, const InputManager* inputManagerPtr);

  // void Update(float elapsedSec, const Rectf& hoveringTile) override;
protected:
  void OnPaintRect(const Rectf& rect) override;

private:
  Level* m_LevelPtr;
};
