#include "pch.h"
#include "ObjectFactory.h"
#include "Strawberry.h"
#include "DashRefill.h"
#include "CrumbleBlock.h"
#include "Spring.h"
#include "decal.h"
#include "Spike.h"
#include "Jumpthru.h"
#include "Cassette.h"
#include "NoteBlock.h"
#include "CheckPoint.h"

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

GameObject* ObjectFactory::CreateDecalSnowA()
{
  return new Decal(Point2f{}, "snow_a", Point2f{ 16.f, 16.f });
}

GameObject* ObjectFactory::CreateDecalSnowB()
{
  return new Decal(Point2f{}, "snow_b", Point2f{ 16.f, 16.f });
}

GameObject* ObjectFactory::CreateDecalSnowC()
{
  return new Decal(Point2f{}, "snow_c", Point2f{ 16.f, 16.f });
}

GameObject* ObjectFactory::CreateDecalSnowD()
{
  return new Decal(Point2f{}, "snow_d", Point2f{ 16.f, 16.f });
}

GameObject* ObjectFactory::CreateDecalSnowE()
{
  return new Decal(Point2f{}, "snow_e", Point2f{ 16.f, 16.f });
}

GameObject* ObjectFactory::CreateDecalSnowF()
{
  return new Decal(Point2f{}, "snow_f", Point2f{ 16.f, 16.f });
}

GameObject* ObjectFactory::CreateDecalSnowG()
{
  return new Decal(Point2f{}, "snow_g", Point2f{ 16.f, 16.f });
}

GameObject* ObjectFactory::CreateDecalSnowH()
{
  return new Decal(Point2f{}, "snow_h", Point2f{ 16.f, 16.f });
}

GameObject* ObjectFactory::CreateDecalSnowI()
{
  return new Decal(Point2f{}, "snow_i", Point2f{ 16.f, 16.f });
}

GameObject* ObjectFactory::CreateDecalSnowJ()
{
  return new Decal(Point2f{}, "snow_j", Point2f{ 16.f, 16.f });
}

GameObject* ObjectFactory::CreateDecalSnowK()
{
  return new Decal(Point2f{}, "snow_k", Point2f{ 16.f, 16.f });
}

GameObject* ObjectFactory::CreateSpikeTop()
{
  return new Spike(Point2f{}, Spike::Orientation::Top);
}

GameObject* ObjectFactory::CreateSpikeBottom()
{
  return new Spike(Point2f{}, Spike::Orientation::Bottom);
}

GameObject* ObjectFactory::CreateSpikeLeft()
{
  return new Spike(Point2f{}, Spike::Orientation::Left);
}

GameObject* ObjectFactory::CreateSpikeRight()
{
  return new Spike(Point2f{}, Spike::Orientation::Right);
}

GameObject* ObjectFactory::CreateJumpthruTopBoth(int size)
{
  return new Jumpthru(Point2f{}, size, Jumpthru::ConnectedSides::TopBoth, JUMPTHRU_DEFAULT_RESOURCE);
}

GameObject* ObjectFactory::CreateJumpthruBottomBoth(int size)
{
  return new Jumpthru(Point2f{}, size, Jumpthru::ConnectedSides::BottomBoth, JUMPTHRU_DEFAULT_RESOURCE);
}

GameObject* ObjectFactory::CreateJumpthruTopLeft(int size)
{
  return new Jumpthru(Point2f{}, size, Jumpthru::ConnectedSides::TopLeft, JUMPTHRU_DEFAULT_RESOURCE);
}

GameObject* ObjectFactory::CreateJumpthruTopRight(int size)
{
  return new Jumpthru(Point2f{}, size, Jumpthru::ConnectedSides::TopRight, JUMPTHRU_DEFAULT_RESOURCE);
}

GameObject* ObjectFactory::CreateJumpthruBottomLeft(int size)
{
  return new Jumpthru(Point2f{}, size, Jumpthru::ConnectedSides::BottomLeft, JUMPTHRU_DEFAULT_RESOURCE);
}

GameObject* ObjectFactory::CreateJumpthruBottomRight(int size)
{
  return new Jumpthru(Point2f{}, size, Jumpthru::ConnectedSides::BottomRight, JUMPTHRU_DEFAULT_RESOURCE);
}

GameObject* ObjectFactory::CreateCassette()
{
  return new Cassette(Point2f{});
}

GameObject* ObjectFactory::CreateNoteBlockBlue4x2()
{
  return new NoteBlock(Point2f{}, NoteBlock::Variant::Blue, 4, 2);
}

GameObject* ObjectFactory::CreateNoteBlockPink4x2()
{
  return new NoteBlock(Point2f{}, NoteBlock::Variant::Pink, 4, 2);
}

GameObject* ObjectFactory::CreateNoteBlockBlue2x6()
{
  return new NoteBlock(Point2f{}, NoteBlock::Variant::Blue, 2, 6);
}

GameObject* ObjectFactory::CreateNoteBlockPink2x6()
{
  return new NoteBlock(Point2f{}, NoteBlock::Variant::Pink, 2, 6);
}

GameObject* ObjectFactory::CreateCheckPoint()
{
  return new CheckPoint(Point2f{});
}

void ObjectFactory::RegisterJumpthrus(ObjectManager* omInstance)
{
  // Register jumpthrus until the size is satisfied
  for (int i = 1; i <= MAX_JUMPTHRU_SIZE; i++)
  {
    omInstance->RegisterObject(CreateJumpthruTopBoth(i));
    omInstance->RegisterObject(CreateJumpthruBottomBoth(i));
    omInstance->RegisterObject(CreateJumpthruTopLeft(i));
    omInstance->RegisterObject(CreateJumpthruTopRight(i));
    omInstance->RegisterObject(CreateJumpthruBottomLeft(i));
    omInstance->RegisterObject(CreateJumpthruBottomRight(i));
  }
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

  // Decals
  omInstance->RegisterObject(CreateDecalSnowA());
  omInstance->RegisterObject(CreateDecalSnowB());
  omInstance->RegisterObject(CreateDecalSnowC());
  omInstance->RegisterObject(CreateDecalSnowD());
  omInstance->RegisterObject(CreateDecalSnowE());
  omInstance->RegisterObject(CreateDecalSnowF());
  omInstance->RegisterObject(CreateDecalSnowG());
  omInstance->RegisterObject(CreateDecalSnowH());
  omInstance->RegisterObject(CreateDecalSnowI());
  omInstance->RegisterObject(CreateDecalSnowJ());
  omInstance->RegisterObject(CreateDecalSnowK());

  // Danger stuffs
  omInstance->RegisterObject(CreateSpikeTop());
  omInstance->RegisterObject(CreateSpikeBottom());
  omInstance->RegisterObject(CreateSpikeLeft());
  omInstance->RegisterObject(CreateSpikeRight());

  RegisterJumpthrus(omInstance);

  omInstance->RegisterObject(CreateCassette());

  omInstance->RegisterObject(CreateNoteBlockBlue4x2());
  omInstance->RegisterObject(CreateNoteBlockPink4x2());
  omInstance->RegisterObject(CreateNoteBlockBlue2x6());
  omInstance->RegisterObject(CreateNoteBlockPink2x6());

  omInstance->RegisterObject(CreateCheckPoint());
}
