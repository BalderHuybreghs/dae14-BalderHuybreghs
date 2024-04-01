#pragma once
#include "Limb.h"
#include "Texture.h"
#include "Vector2f.h"
#include "Sprite.h"

class Hair
{
public:
  // Parts determine how many hair parts there are
  Hair(const Point2f& position, int parts, float startingSize);
  ~Hair();

  void Draw(bool debug = false) const;
  void Update(float elapsedSec); // The update function applies the velocity in an appropriate way (at least it is intended to be this way)

  Point2f GetPosition() const;

  float SetEnd(const Point2f& position); // Returns the distance from the goal position
  void SetGoal(const Point2f& goal);
private:
  // The goal position for the hair
  Point2f m_Position;
  Point2f m_Goal;

  // Limbs for hair sounds funny, but I called limb limb because it can be used in many
  // context for IK
  Limb* m_HairLimb;

  // Since limbs are only there to keep track of the "mathematical" part of simulating hair,
  // we will need a way to draw these limbs. In the case of Celeste, this is a texture of a simple circle
  // in pixel art. Since the hair is only a single texture, only a single texture is required, it will draw in several sizes however
  const Texture* m_HairTexture;

  Sprite* m_Bangs; // The bangs that will be rendered on the player
};