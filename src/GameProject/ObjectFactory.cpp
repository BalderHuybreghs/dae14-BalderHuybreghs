#include "pch.h"
#include "ObjectFactory.h"
#include "ObjectManager.h"
#include "Strawberry.h"
#include "DashRefill.h"

#include <iostream>

GameObject* ObjectFactory::CreateStrawberry()
{
  return new Strawberry(Point2f{});
}

GameObject* ObjectFactory::CreateDashRefill()
{
  return new DashRefill(Point2f{});
}

void ObjectFactory::RegisterGameObjects()
{
  std::cout << "Registering game objects" << std::endl;
  ObjectManager* omInstance{ ObjectManager::GetInstance() };

  omInstance->RegisterObject(CreateStrawberry());
  omInstance->RegisterObject(CreateDashRefill());
}
