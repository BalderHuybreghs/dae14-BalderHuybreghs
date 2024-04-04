#pragma once

namespace MathUtils
{
  // Integer operations
  int     RandInt(int min, int max);

  // FLOATING POINT OPERATIONS
  float   RandFloat(float min, float max, int decimals);
  float   Lerp(float start, float end, float t);
  float   Wave(float amplitude, float period, float phase, float shift, float t);

  // Lerp on a point
  Point2f Lerp(const Point2f& a, const Point2f& b, float t);

  // Random point generation stuff :), this will be used in the particle emitter
  Point2f RandPoint(const Rectf& range, int decimals);
  Point2f RandPoint(const Circlef& range, int decimals);
}