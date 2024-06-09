#pragma once
#include "GameObject.h"

class Spike : public GameObject
{
public:
  enum class Orientation
  {
    Left,
    Right,
    Top,
    Bottom
  };

  Spike(const Point2f& position, Orientation orientation);
  Spike(const Spike& other);

  // Apply rule of 5
  ~Spike() = default;
  Spike(const Spike&& other) = delete;
  Spike& operator=(const Spike& other) = delete;
  Spike& operator=(const Spike&& other) = delete;

  void Draw(const Point2f& position, bool debug) const override;
  void Draw(bool debug) const override;

  void Update(Player& player, Camera& camera, float elapsedSec) override;

  void SetPosition(const Point2f& position) override;

  Orientation GetOrientation() const;

  GameObject* Clone() const override;
private:
  void TryKillPlayer(Player& player) const;

  const Texture* m_TexturePtr;
  Orientation m_Orientation;

  Rectf m_Collider;
};

