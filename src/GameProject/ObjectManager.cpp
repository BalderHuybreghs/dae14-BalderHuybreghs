#include "pch.h"
#include "ObjectManager.h"
#include <iostream>

ObjectManager* ObjectManager::instance = nullptr;

ObjectManager::~ObjectManager()
{
  std::cout << "Cleaning registered objects" << std::endl;

  // Delete all the textures
  for (const std::pair<int, const GameObject*>& pair : m_ObjectPtrs) {
    std::cout << "Deleting object with ID: '" << pair.first << '\'' << std::endl;
    delete pair.second;
  }

  // Clear out the hashmap
  m_ObjectPtrs.clear();
}

GameObject* ObjectManager::CloneObject(int id)
{
  std::cout << "Cloning object with ID: " << id << std::endl;
  return GetBlueprintObject(id)->Clone();
}

const GameObject* ObjectManager::GetBlueprintObject(int id)
{
  std::unordered_map<int, const GameObject*>::iterator value{ m_ObjectPtrs.find(id) };
  if (value != m_ObjectPtrs.end()) {
    return value->second;
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

bool ObjectManager::ObjectWithIdExists(int id)
{
  std::unordered_map<int, const GameObject*>::iterator value{ m_ObjectPtrs.find(id) };
  return value != m_ObjectPtrs.end();
}

ObjectManager* ObjectManager::GetInstance()
{
  if (instance == nullptr) {
    std::cout << "Creating object manager" << std::endl;
    instance = new ObjectManager();
  }

  return instance;
}

void ObjectManager::DestroyInstance()
{
  std::cout << "Destroying object manager" << std::endl;
  delete instance;
  instance = nullptr;
}