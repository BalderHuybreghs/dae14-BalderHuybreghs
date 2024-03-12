#pragma once
#include "Resource.h"
#include "Texture.h"

class Sprite
{
public:
  Sprite(Resource resource);

protected:
  const Texture* m_TexturePtr;
};