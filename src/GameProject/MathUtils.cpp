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
