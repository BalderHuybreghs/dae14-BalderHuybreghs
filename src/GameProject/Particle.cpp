#include "pch.h"
#include "Particle.h"
#include "utils.h"

Particle::Particle(const Point2f& position, const Vector2f& velocity, Shape* shape, float lifetime)
  : m_Velocity(velocity), m_Shape(shape), m_Lifetime(lifetime)
{
  m_Shape->SetPosition(position);
}

Particle::Particle(const Particle& other)
{
  m_Lifetime = other.GetLifetime();
  m_Shape = other.GetShape()->Copy();
  m_Velocity = other.GetVelocity();
}

Particle& Particle::operator=(const Particle& other)
{
  if (this == &other)
    return *this;

  m_Lifetime = other.GetLifetime();
  m_Shape = other.GetShape()->Copy();
  m_Velocity = other.GetVelocity();

  return *this;
}

Particle::~Particle()
{
  delete m_Shape;
}

void Particle::Draw(bool debug) const
{
  m_Shape->Draw();

  // Draw the velocity of the particle
  if (debug) {
    const Point2f center{ m_Shape->GetCenter() };
    const Vector2f direction{ m_Velocity.Normalized() };
    const float directionLength{ 100.f };
    const Vector2f drawVector{ direction * directionLength };
    const Point2f endPoint{ center.x + drawVector.x, center.y + drawVector.y };

    utils::DrawLine(center, endPoint);
  }
}

void Particle::Update(float elapsedSec)
{
  const Rectf boundingBox{ m_Shape->GetBoundingBox() };
  const Point2f newPos{ boundingBox.left + m_Velocity.x * elapsedSec, boundingBox.bottom + m_Velocity.y * elapsedSec };
  m_Shape->SetPosition(newPos);

  m_Lifetime -= elapsedSec;
}

void Particle::ApplyForce(const Vector2f& velocity)
{
  m_Velocity += velocity;
}

const Shape* Particle::GetShape() const
{
  return m_Shape;
}

Point2f Particle::GetPosition() const
{
  const Rectf boundingBox{ m_Shape->GetBoundingBox() };
  return Point2f{ boundingBox.left, boundingBox.bottom };
}

Vector2f Particle::GetVelocity() const
{
  return m_Velocity;
}

float Particle::GetLifetime() const
{
  return m_Lifetime;
}
