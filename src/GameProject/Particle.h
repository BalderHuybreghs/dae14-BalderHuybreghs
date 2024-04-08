#pragma once
#include "Shape.h"
#include "Vector2f.h"

// Particle will keep track of its own lifetime etc
class Particle
{
public:
  // Particle will take care of deleting shape
  Particle(const Point2f& position, const Vector2f& velocity, Shape* shape, float lifetime);

  // Application of rule of 3
  Particle(const Particle& other);
  Particle& operator=(const Particle& other);
  ~Particle();

  void Draw(bool debug) const;
  void Update(float elapsedSec);

  void ApplyForce(const Vector2f& velocity);

  const Shape* GetShape() const;
  Point2f GetPosition() const;
  Vector2f GetVelocity() const;
  float GetLifetime() const;
private:
  // Position and velocity information
  Vector2f m_Velocity;

  Shape* m_Shape;

  float m_Lifetime;
};