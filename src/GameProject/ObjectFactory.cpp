#include "pch.h"
#include "ObjectFactory.h"
#include "Strawberry.h"
#include "ObjectManager.h"

#include <iostream>

GameObject* ObjectFactory::CreateStrawberry()
{
  return new Strawberry(Point2f{});
}

void ObjectFactory::RegisterGameObjects()
{
  std::cout << "Registering game objects" << std::endl;

  ObjectManager::GetInstance()->RegisterObject(CreateStrawberry());
}
