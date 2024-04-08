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

    Point2f goal;    // The goal for the camera to reach
    float zoomGoal;  // The zoom goal
    float duration;  // The duration in milliseconds
    Type type;       // Motion type, the camera may move in a certain way
  };

  Camera(const Point2f position, float zoom);
  Camera(const Point2f& position);
  virtual ~Camera() = default;

  // This is a function to render on the screen position if we are already rendering on a camera
  void PushMatrixInverse();

  void PushMatrix();
  void PopMatrix();

  // The camera has the ability to follow objects in a certain pattern
  void Update(float elapsedSec);

  void SetPosition(const Point2f& position);
  void SetZoom(float zoom);

  float GetZoom() const;
  Point2f GetPosition() const;
  Point2f GetWorldPosition(const Point2f& screenPosition) const;
private:
  std::queue<AnimationGoal> m_AnimationGoals;

  Point2f m_Position;
  float m_Zoom;
};

