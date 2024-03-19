#include "pch.h"
#include "ObjectManager.h"
#include <iostream>

ObjectManager* ObjectManager::_instance = nullptr;

ObjectManager::~ObjectManager()
{
  std::cout << "Cleaning registered objects" << std::endl;

  // Delete all the textures
  for (const std::pair<int, const GameObject*>& pair : m_ObjectPtrs) {
    delete pair.second;
  }

  // Clear out the hashmap
  m_ObjectPtrs.clear();
}

GameObject* ObjectManager::CloneObject(int id)
{
  return GetBlueprintObject(id)->clone();
}

const GameObject* ObjectManager::GetBlueprintObject(int id)
{
  std::unordered_map<int, const GameObject*>::iterator value{ m_ObjectPtrs.find(id) };
  if (value != m_ObjectPtrs.end()) {
    return value->second->clone();
  }

  std::cout << "Could not find registered object with ID: " << id << std::endl;
  exit(-1);
}

int ObjectManager::RegisterObject(const GameObject* object)
{
  m_ObjectPtrs.insert({ m_IdCounter, object });
    return m_IdCounter++;
}

int ObjectManager::GetObjectCount()
{
  // Size_t is overkill for this use-case
  return int(m_ObjectPtrs.size());
}

ObjectManager* ObjectManager::Instance()
{
  if (_instance == nullptr) {
    _instance = new ObjectManager();
  }

  return _instance;
}

bool ObjectManager::ObjectWithIdExists(int id)
{
  std::unordered_map<int, const GameObject*>::iterator value{ m_ObjectPtrs.find(id) };
  return value != m_ObjectPtrs.end();
}
