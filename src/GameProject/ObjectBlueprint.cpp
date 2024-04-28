#include "pch.h"
#include "ObjectBlueprint.h"
#include "ObjectManager.h"
#include <iostream>

ObjectBlueprint::ObjectBlueprint(int objectId, Point2f position)
  : m_ObjectId(0), m_Position(position)
{
  std::cout << "Creating Blueprint '" << m_ObjectId << "' at (" << m_Position.x << ', ' << m_Position.y << ")" << std::endl;
  SetObjectId(objectId);
}

void ObjectBlueprint::Draw() const
{
  // Blueprints will draw with debug info by default
  ObjectManager::GetInstance()->GetBlueprintObject(m_ObjectId)->Draw(m_Position, true);
}

GameObject* ObjectBlueprint::Construct() const
{
  std::cout << "Constructing object '" << m_ObjectId << "' at (" << m_Position.x << ", " << m_Position.y << ")" << std::endl;
  GameObject* obj{ ObjectManager::GetInstance()->CloneObject(m_ObjectId) };
  obj->SetPosition(m_Position);
  return obj;
}

void ObjectBlueprint::SetObjectId(int objectId)
{
  if (!ObjectManager::GetInstance()->ObjectWithIdExists(objectId)) {
    std::cout << "Blueprint object does not exist with ID: " << objectId << std::endl;;
    std::exit(-1);
  }

  m_ObjectId = objectId;
}

void ObjectBlueprint::SetPosition(const Point2f& position)
{
  m_Position = position;
}

int ObjectBlueprint::GetObjectId() const
{
  return m_ObjectId;
}

Point2f ObjectBlueprint::GetPosition() const
{
  return m_Position;
}
