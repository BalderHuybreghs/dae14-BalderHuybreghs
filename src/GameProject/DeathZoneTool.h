#pragma once
#include "RectangleTool.h"

class DeathZoneTool final : public RectangleTool
{
public:

private:
  void OnPaintRect(const Rectf& rect) override;
};

