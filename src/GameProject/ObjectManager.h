#pragma once
#include <unordered_map>
#include <memory>
#include "GameObject.h"

// The object manager keeps a list of single instance objects.
// This is useful when working with blueprints to create maps
// from predefined objects. While not the most memory efficient solution
// it gets the job done good enough for now
class ObjectManager
{
public:
  ~ObjectManager();

  // Gets the object with a given ID
  GameObject* CloneObject(int id);

  // Gets the blueprint object with the given ID
  const GameObject* GetBlueprintObject(int id);

  // Adds an object to the registry of objects
  int RegisterObject(const GameObject* object);

  bool ObjectWithIdExists(int id);

  // Returns how many objects are registered
  int GetObjectCount();

  static ObjectManager* Instance();
private:
  int m_IdCounter{};
  std::unordered_map<int, const GameObject*> m_ObjectPtrs;

  static ObjectManager* _instance;
};
