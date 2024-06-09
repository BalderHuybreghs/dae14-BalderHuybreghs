#pragma once
#include "GameObject.h"
#include "Sprite.h"

class Decal : public GameObject
{
public:
  Decal(const Point2f& position, const std::string& resource, const Point2f& frameSize);
  Decal(const Decal& other);
  ~Decal();

  // Rule of 5
  Decal(const Decal&& other) = delete;
  Decal& operator=(const Decal& other) = delete;
  Decal& operator=(const Decal&& other) = delete;

  // Inherited via GameObject
  void Draw(const Point2f& position, bool debug) const override;
  void Draw(bool debug = false) const override;

  void Update(Player& player, Camera& camera, float elapsedSec) override;

  GameObject* Clone() const override;
private:
  const std::string m_Resource;
  Point2f m_FrameSize;

  // Decal makes use of a sprite because decals can be animated
  // they're essentially placeable sprites
  Sprite* m_SpritePtr; 
};