#include "pch.h"
#include "Camera.h"
#include "GameDefines.h"

Camera::Camera(const Point2f position, float zoom)
  : m_Position(position), m_Zoom(zoom)
{
}

Camera::Camera(const Point2f& position)
  : Camera(position, 1.f)
{
}

void Camera::PushMatrix()
{
  glPushMatrix();

  // Center camera around position
  glTranslatef(-m_Position.x + WINDOW_WIDTH / 2, -m_Position.y + WINDOW_HEIGHT / 2, 0);
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
      (screenPosition.x - (WINDOW_WIDTH / m_Zoom) / 2) + m_Position.x,
      (screenPosition.y - (WINDOW_HEIGHT / m_Zoom) / 2) + m_Position.y
  };
}