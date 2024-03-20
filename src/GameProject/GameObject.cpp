#include "pch.h"
#include "GameObject.h"

void GameObject::SetPosition(const Point2f& position)
{
  m_Position = position;
}

Point2f GameObject::GetPosition()
{
    return m_Position;
}
