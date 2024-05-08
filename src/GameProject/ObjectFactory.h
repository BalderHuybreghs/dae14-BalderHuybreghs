#pragma once
#include "GameObject.h"

// The object factory simplifies object creation behind functions
namespace ObjectFactory
{
  GameObject* CreateStrawberry();
  GameObject* CreateDashRefill();
  GameObject* CreateCrumbleBlockSmall();
  GameObject* CreateCrumbleBlockMedium();
  GameObject* CreateCrumbleBlockLarge();

  // Registers all the gameobjects with the object manager
  void RegisterGameObjects();
}