#include "pch.h"
#include "Camera.h"
#include "GameDefines.h"
#include "iostream"
#include "MathUtils.h"

Camera::Camera(const Point2f position, float zoom)
  : m_Position(position), m_Zoom(zoom), m_PreviousPosition(position), m_PreviousZoom(zoom), m_AnimationSeconds(0)
{
  std::cout << "Creating camera at (" << position.x << ", " << position.y << ") with zoom factor " << zoom << std::endl;
}

Camera::Camera(const Point2f& position)
  : Camera(position, 1.f)
{
}

void Camera::PushMatrixInverse() const
{
  glPushMatrix();
  glTranslatef(m_Position.x, m_Position.y, 0);
  glScalef(1, 1, 1);
}

void Camera::PushMatrix() const
{
  glPushMatrix();

  // Center camera around position
  glTranslatef(-m_Position.x, -m_Position.y, 0);
  glScalef(m_Zoom, m_Zoom, 1); // Preserve Z component for proper scaling
}

void Camera::PopMatrix() const
{
  glPopMatrix();
}

void Camera::Update(float elapsedSec)
{
  if (m_AnimationGoals.empty()) {
    return;
  }

  const AnimationGoal goal{ m_AnimationGoals.front() };
  const float delta{ m_AnimationSeconds / goal.duration };
  if (delta >= 1.f) {
    m_AnimationGoals.pop();
    m_AnimationSeconds = 0;
    m_PreviousPosition = m_Position;
    return;
  }

  // Lerp the camera to it's goal position
  m_Position = MathUtils::Lerp(m_PreviousPosition, goal.position, delta);
  m_Zoom = MathUtils::Lerp(m_PreviousZoom, goal.zoom, delta);

  m_AnimationSeconds += elapsedSec;
}

void Camera::AddGoal(const AnimationGoal& goal)
{
  m_AnimationGoals.push(goal);
}

void Camera::SetPosition(const Point2f& position)
{
  m_Position = position;
}

void Camera::SetZoom(float zoom)
{
  m_Zoom = zoom;
}

float Camera::GetZoom() const
{
  return m_Zoom;
}

Point2f Camera::GetPosition() const
{
  return m_Position;
}

Point2f Camera::GetWorldPosition(const Point2f& screenPosition) const
{
  // Calculate world position from screen position, considering zoom
  return Point2f{
      screenPosition.x / m_Zoom + m_Position.x / m_Zoom,
      screenPosition.y / m_Zoom + m_Position.y / m_Zoom
  };
}

int Camera::GoalCount() const
{
  return (int)m_AnimationGoals.size();
}
