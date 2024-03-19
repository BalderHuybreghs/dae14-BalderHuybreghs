#pragma once
#include <queue>

class Camera
{
public:
  // A goal for this camera to follow
  struct AnimationGoal
  {
    enum class Type
    {
      None,
      Linear
    };

    Rectf goal;      // The goal for the camera to reach
    float duration;  // The duration in milliseconds
    Type type;       // Motion type, the camera may move in a certain way
  };

  Camera(const Rectf& rect, const Rectf& area);
  Camera(const Rectf& rect);

  void PushMatrix();
  void PopMatrix();

  // The camera has the ability to follow objects in a certain pattern
  void Update(float elapsedSec);

  void SetRect(const Rectf& rect);
  void SetArea(const Rectf& area);
  Rectf GetRect() const;
  Rectf GetArea() const;
private:
  std::queue<AnimationGoal> m_AnimationGoals;

  Rectf m_Rect;

  // A limited area of control for the camera
  Rectf m_Area;
};

