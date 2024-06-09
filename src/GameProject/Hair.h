#pragma once
#include "Limb.h"
#include "Texture.h"
#include "Vector2f.h"
#include "Sprite.h"

class Hair final // Why on earth would we need polymorphism on this simple hair class? x.x
{
public:
  // Parts determine how many hair parts there are
  Hair(const Point2f& goal, int parts, float startingSize);

  // No copying required (totally not being lazy)
  Hair(const Hair& other) = delete;
  Hair& operator=(const Hair& other) = delete;
  ~Hair();

  void DrawBangs(const Point2f& position, const Color4f& color, bool flipped = false, bool debug = false) const;
  void Draw(const Color4f& color, bool flipped = false, bool debug = false) const;
  void Update(float elapsedSec); // The update function applies the velocity in an appropriate way (at least it is intended to be this way)

  Point2f GetPosition() const;

  float SetEnd(const Point2f& position); // Returns the distance from the goal position
  void SetGoal(const Point2f& goal);

  void SetState(const std::string& state);
private:
  const float m_Size;
  // The goal position for the hair
  Point2f m_Position;
  Point2f m_Goal;

  // Limbs for hair sounds funny, but I called limb limb because it can be used in many
  // context for IK
  Limb* m_HairLimbPtr;

  // Since limbs are only there to keep track of the "mathematical" part of simulating hair,
  // we will need a way to draw these limbs. In the case of Celeste, this is a texture of a simple circle
  // in pixel art. Since the hair is only a single texture, only a single texture is required, it will draw in several sizes however
  const Texture* m_HairTexturePtr;

  Sprite* m_BangsSpritePtr; // The bangs that will be rendered on the player
};