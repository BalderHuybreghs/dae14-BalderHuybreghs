#include "pch.h"
#include "Camera.h"
#include "GameDefines.h"
#include "iostream"

Camera::Camera(const Point2f position, float zoom)
  : m_Position(position), m_Zoom(zoom)
{
  std::cout << "Creating camera at (" << position.x << ", " << position.y << ") with zoom factor " << zoom << std::endl;
}

Camera::Camera(const Point2f& position)
  : Camera(position, 1.f)
{
}

void Camera::PushMatrixInverse()
{
  glPushMatrix();
  glTranslatef(m_Position.x, m_Position.y, 0);
}

void Camera::PushMatrix()
{
  glPushMatrix();

  // Center camera around position
  glTranslatef(-m_Position.x, -m_Position.y, 0);
  glScalef(m_Zoom, m_Zoom, 1); // Preserve Z component for proper scaling
}

void Camera::PopMatrix()
{
  glPopMatrix();
}

void Camera::Update(float elapsedSec)
{
  // TODO: Implement this
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
      screenPosition.x * m_Zoom + m_Position.x,
      screenPosition.y * m_Zoom + m_Position.y
  };
}