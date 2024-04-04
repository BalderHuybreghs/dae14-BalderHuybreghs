#include "pch.h"
#include "MathUtils.h"

int MathUtils::RandInt(int min, int max)
{
  int scaledMax{ (max - min) + 1 };
  return rand() % scaledMax + min;
}

float MathUtils::RandFloat(float min, float max, int decimals)
{
  int scale{ (int)pow(10, decimals) };
  int scaledMax{ int((max - min) * scale) };
  return rand() % (scaledMax + 1) / float(scale) + min;
}

float MathUtils::Lerp(float start, float end, float t)
{
  t = std::max(0.0f, std::min(1.0f, t));
  return start + t * (end - start);
}

float MathUtils::Wave(float amplitude, float period, float phase, float shift, float t)
{
  return amplitude * sinf((2 * float(M_PI)) / period * (t + phase)) + shift;
}

Point2f MathUtils::Lerp(const Point2f& a, const Point2f& b, float t)
{
  return Point2f{
    Lerp(a.x, b.x, t),
    Lerp(a.y, b.y, t),
  };
}

Point2f MathUtils::RandPoint(const Rectf& range, int decimals)
{
  float randX = RandFloat(range.left, range.left + range.width, decimals);
  float randY = RandFloat(range.bottom, range.bottom + range.height, decimals);
  return Point2f(randX, randY);
}

Point2f MathUtils::RandPoint(const Circlef& range, int decimals)
{
  float randAngle = RandFloat(0.0f, 2 * float(M_PI), decimals); // A random angle
  float randRadius = RandFloat(0.0f, range.radius, decimals); // Random radius

  // Convert polar to cartesian coordinates
  float randX = range.center.x + randRadius * cosf(randAngle);
  float randY = range.center.y + randRadius * sinf(randAngle);

  return Point2f(randX, randY);
}
