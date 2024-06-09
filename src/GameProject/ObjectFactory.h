#pragma once
#include "GameObject.h"
#include "ObjectManager.h"

// The object factory simplifies object creation behind functions
namespace ObjectFactory
{
  GameObject* CreateStrawberry();
  GameObject* CreateDashRefill();
  GameObject* CreateCrumbleBlockSmall();
  GameObject* CreateCrumbleBlockMedium();
  GameObject* CreateCrumbleBlockLarge();
  GameObject* CreateSpringBottom();
  GameObject* CreateSpringLeft();
  GameObject* CreateSpringRight();
  GameObject* CreateSpringTop();

  // DECALS
  GameObject* CreateDecalSnowA();
  GameObject* CreateDecalSnowB();
  GameObject* CreateDecalSnowC();
  GameObject* CreateDecalSnowD();
  GameObject* CreateDecalSnowE();
  GameObject* CreateDecalSnowF();
  GameObject* CreateDecalSnowG();
  GameObject* CreateDecalSnowH();
  GameObject* CreateDecalSnowI();
  GameObject* CreateDecalSnowJ();
  GameObject* CreateDecalSnowK();

  // Dangerous thingies
  GameObject* CreateSpikeTop();    // Kinky :)
  GameObject* CreateSpikeBottom(); // ;) :000
  GameObject* CreateSpikeLeft();
  GameObject* CreateSpikeRight();

  GameObject* CreateJumpthruTopBoth(int size);
  GameObject* CreateJumpthruBottomBoth(int size);
  GameObject* CreateJumpthruTopLeft(int size);
  GameObject* CreateJumpthruTopRight(int size);
  GameObject* CreateJumpthruBottomLeft(int size);
  GameObject* CreateJumpthruBottomRight(int size);

  GameObject* CreateCassette();

  GameObject* CreateNoteBlockBlue4x2();
  GameObject* CreateNoteBlockPink4x2();
  GameObject* CreateNoteBlockBlue2x6();
  GameObject* CreateNoteBlockPink2x6();

  // Creates a bunch of jumpthrus with different sizes
  void RegisterJumpthrus(ObjectManager* omInstance);

  // Registers all the gameobjects with the object manager
  void RegisterGameObjects();
}