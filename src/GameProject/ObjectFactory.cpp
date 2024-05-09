#include "pch.h"
#include "ObjectFactory.h"
#include "ObjectManager.h"
#include "Strawberry.h"
#include "DashRefill.h"
#include "CrumbleBlock.h"
#include "Spring.h"

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

GameObject* ObjectFactory::CreateSpringBottom()
{
  return new Spring(Point2f{}, Vector2f(0.f, 1000.f), Spring::Orientation::Bottom, .5f);
}

GameObject* ObjectFactory::CreateSpringLeft()
{
  return new Spring(Point2f{}, Vector2f(1000.f, 0.f), Spring::Orientation::Left, .5f);
}

GameObject* ObjectFactory::CreateSpringRight()
{
  return new Spring(Point2f{}, Vector2f(-1000.f, 0.f), Spring::Orientation::Right, .5f);
}

GameObject* ObjectFactory::CreateSpringTop()
{
  return new Spring(Point2f{}, Vector2f(0.f, -1000.f), Spring::Orientation::Top, .5f);
}

void ObjectFactory::RegisterGameObjects()
{
  std::cout << "Registering game objects" << std::endl;
  ObjectManager* omInstance{ ObjectManager::GetInstance() };

  // Register the game objects
  omInstance->RegisterObject(CreateStrawberry());
  omInstance->RegisterObject(CreateDashRefill());
  omInstance->RegisterObject(CreateCrumbleBlockSmall());
  omInstance->RegisterObject(CreateCrumbleBlockMedium());
  omInstance->RegisterObject(CreateCrumbleBlockLarge());
  omInstance->RegisterObject(CreateSpringBottom());
  omInstance->RegisterObject(CreateSpringRight());
  omInstance->RegisterObject(CreateSpringTop());
  omInstance->RegisterObject(CreateSpringLeft());
}
