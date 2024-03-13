#include "pch.h"
#include "Joint.h"
#include "MathUtils.h"
#include "Vector2f.h"

Joint::Joint(const Point2f& anchor, float length)
  : m_Position(anchor), m_Length(length), m_Angle(0)
{
  Calculate();
}

void Joint::Calculate()
{
  m_End = Point2f(
    m_Position.x + m_Length * cosf(m_Angle),
    m_Position.y + m_Length * sinf(m_Angle)
  );
}

void Joint::Draw() const
{
  DrawLine(m_Position, m_End);

  SetColor(Color4f(1.f, 1.f, 1.f, 1.f));
  FillEllipse(m_Position, 2.f, 2.f);

  SetColor(Color4f(1.f, 1.f, .1f, 1.f));
  FillEllipse(m_End, 2.f, 2.f);

  SetColor(Color4f(1.f, 0.f, 1.f, 1.f));
  DrawLine(m_Position, m_End, 0.5f);
}

void Joint::Follow(const Point2f& goal)
{
  Vector2f vector{ goal.x - m_Position.x, goal.y - m_Position.y };
  vector = vector.Normalized();

  // Calculate the angle
  m_Angle = atan2f(vector.y, vector.x);

  // Set the position based on the goal and direction vector
  m_Position.x = goal.x - vector.x * m_Length;
  m_Position.y = goal.y - vector.y * m_Length;

  Calculate();
}

void Joint::SetPosition(const Point2f& position)
{
  m_Position = position;
  Calculate();
}

float Joint::GetLength() const
{
  return m_Length;
}

float Joint::GetAngle() const
{
  return m_Angle;
}

Point2f Joint::GetPosition() const
{
  return m_Position;
}

Point2f Joint::GetEnd() const
{
  return m_End;
}

Point2f Joint::GetMiddle() const
{
  // To calculate the middle, just lerp to 50% of the start- and endpoints
  return MathUtils::Lerp(GetPosition(), GetEnd(), 0.5f);
}
