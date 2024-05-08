#include "pch.h"
#include "ObjectFactory.h"
#include "ObjectManager.h"
#include "Strawberry.h"
#include "DashRefill.h"
#include "CrumbleBlock.h"

#include <iostream>
#include "GameDefines.h"

GameObject* ObjectFactory::CreateStrawberry()
{
  return new Strawberry(Point2f{});
}

GameObject* ObjectFactory::CreateDashRefill()
{
  return new DashRefill(Point2f{});
}

GameObject* ObjectFactory::CreateCrumbleBlockSmall()
{
  return new CrumbleBlock(Point2f{}, 1, 2, CRUMBLE_BLOCK_DEFAULT);
}

GameObject* ObjectFactory::CreateCrumbleBlockMedium()
{
  return new CrumbleBlock(Point2f{}, 2, 2, CRUMBLE_BLOCK_DEFAULT);

}

GameObject* ObjectFactory::CreateCrumbleBlockLarge()
{
  return new CrumbleBlock(Point2f{}, 3, 2, CRUMBLE_BLOCK_DEFAULT);
}

void ObjectFactory::RegisterGameObjects()
{
  std::cout << "Registering game objects" << std::endl;
  ObjectManager* omInstance{ ObjectManager::GetInstance() };

  omInstance->RegisterObject(CreateStrawberry());
  omInstance->RegisterObject(CreateDashRefill());
  omInstance->RegisterObject(CreateCrumbleBlockSmall());
  omInstance->RegisterObject(CreateCrumbleBlockMedium());
  omInstance->RegisterObject(CreateCrumbleBlockLarge());
}
