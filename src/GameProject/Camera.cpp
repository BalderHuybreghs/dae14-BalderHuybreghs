#include "pch.h"
#include "Camera.h"

Camera::Camera(const Rectf& rect, const Rectf& area)
  : m_Rect(rect), m_Area(area)
{
}

Camera::Camera(const Rectf& rect)
  : Camera(rect, Rectf())
{
}

void Camera::PushMatrix()
{
  glPushMatrix();
  //glScalef(m_Rect.width, m_Rect.height, 0);
  glTranslatef(-m_Rect.left, -m_Rect.bottom, 0);
}

void Camera::PopMatrix()
{
  glPopMatrix();
}

void Camera::Update(float elapsedSec)
{
  // TODO: Implement this
}

void Camera::SetRect(const Rectf& rect)
{
  m_Rect = rect;
}

void Camera::SetArea(const Rectf& area)
{
  m_Area = area;
}

Rectf Camera::GetRect() const
{
  return m_Rect;
}

Rectf Camera::GetArea() const
{
  return m_Area;
}
