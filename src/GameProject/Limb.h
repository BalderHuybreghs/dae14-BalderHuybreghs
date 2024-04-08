#pragma once
#include <utils.h>
#include "Joint.h"
#include "MathUtils.h"

using namespace utils;

class Limb
{
public:
  Limb(const Point2f& anchor, const Point2f& goal, int joints, float length);
  Limb(const Point2f& anchor, int joints, float length);

  // Application of ro3, how cumbersome to implement for all my classes this late into the game
  // I wish they explained this to us sooner :/
  Limb(const Limb& other);
  Limb& operator=(const Limb& other);
  virtual ~Limb();

  // Draw should only be used in a debug context
  void     Draw();

  /// Returns the distance from the goals
  float    Set(const Point2f& anchor, const Point2f& goal);

  /// Returns the distance from the goal
  float    Follow(const Point2f& goal);

  Point2f GetAnchor() const;
  Point2f GetEnd() const;

  // Crazy move, I know
  void AddJoint(Joint* joint);
  const std::vector<Joint*>* Joints() const;
private:
  std::vector<Joint*> m_Joints;
  Point2f m_Anchor;
};