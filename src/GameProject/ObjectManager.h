#pragma once
#include <unordered_map>
#include <memory>
#include "GameObject.h"

// The object manager keeps a list of single instance objects.
class ObjectManager
{
public:
  ~ObjectManager();

  // Gets the object with a given ID
  GameObject* CloneObjectWithId(int id) const;

  // Adds an object to the registry of objects
  int RegisterObject(const GameObject& object);

  static ObjectManager* Instance();

private:
  std::unordered_map<int, const GameObject*> m_ObjectPtrs;

  static ObjectManager* _instance;
};

