#pragma once
#include "Shape.h"
#include "Vector2f.h"

// Particle will keep track of its own lifetime etc
class Particle
{
public:
  Particle(const Point2f& position, const Vector2f& velocity, const Shape& startShape, const Shape& endShape, float lifetime);
  ~Particle();

  void Draw() const;
  void Update(float elapsedSec);

  void SetPosition(const Point2f& position);
  void SetVelocity(const Vector2f& velocity);
  void SetLifetime(float lifetime);

  Point2f GetPosition() const;
  Vector2f GetVelocity() const;
  float GetLifetime() const;
private:
  // Position and velocity information
  Point2f  m_Position;
  Vector2f m_Velocity;

  // Shapes can lerp into eachother... More or less
  Shape* m_StartShape;
  Shape* m_EndShape;

  float m_Lifetime;
};