#include "pch.h"
#include "ObjectBlueprint.h"
#include "ObjectManager.h"
#include <iostream>

ObjectBlueprint::ObjectBlueprint(int objectId, Point2f position)
  : m_ObjectId(0), m_Position(position)
{
  SetObjectId(objectId);
}

void ObjectBlueprint::Draw() const
{
  // Blueprints will draw with debug info by default
  ObjectManager::Instance()->GetBlueprintObject(m_ObjectId)->Draw(true);
}

GameObject* ObjectBlueprint::Construct() const
{
  return ObjectManager::Instance()->CloneObject(m_ObjectId);
}

void ObjectBlueprint::SetObjectId(int objectId)
{
  if (!ObjectManager::Instance()->ObjectWithIdExists(objectId)) {
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
