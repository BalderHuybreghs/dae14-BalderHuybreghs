#pragma once
#include <utils.h>

using namespace utils;

class Joint
{
public:
  Joint(const Point2f& anchor, float length);
  virtual ~Joint() = default;

  // Joins will only be drawn in a debug scenario
  void    Draw() const;
  void    Calculate();

  Point2f GetPosition()	const;
  Point2f GetEnd()	const;
  Point2f GetMiddle() const;
  float   GetLength() const;
  float   GetAngle() const;

  void    Follow(const Point2f& goal);
  void    SetPosition(const Point2f& position);
private:
  Point2f m_Position;
  Point2f m_End;

  float   m_Length;
  float   m_Angle;
};