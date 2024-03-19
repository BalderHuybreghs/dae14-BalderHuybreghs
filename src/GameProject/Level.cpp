#include "pch.h"
#include "Level.h"
#include "GameDefines.h"

Level::Level(const std::string& name, const std::string& foregroundTileResource, const std::string& backgroundTileResource)
  : m_Name(name)
{
  // Load both the foreground and background tilemaps
  m_ForegroundTilemapPtr = new Tilemap(foregroundTileResource, Point2f{1.f, 1.f}, TILE_SIZE);
  m_BackgroundTilemapPtr = new Tilemap(backgroundTileResource, Point2f{1.f, 1.f}, TILE_SIZE);
}

Level::~Level()
{
  // Delete all the textures
  for (const GameObject* object : m_Objects) {
    delete object;
  }

  // Clear out the game objects
  m_Objects.clear();
  delete m_ForegroundTilemapPtr;
}

void Level::Draw(bool debug) const
{
  // Draw the background tilemap
  m_BackgroundTilemapPtr->Draw(debug);

  // Draw the objects in between the two tilemaps
  for (const GameObject* object : m_Objects) {
    object->Draw(debug);
  }

  // Draw the foreground tilemap
  m_ForegroundTilemapPtr->Draw(debug);
}

void Level::Update(Player& player, float elapsedSec)
{
  // Update all the level gameobjects
  for (GameObject* object : m_Objects) {
    object->Update(player, elapsedSec);
  }
}

void Level::AddObject(GameObject& object)
{
  //m_Objects.insert(object);
}

Tilemap* Level::GetFrontTilemap() const
{
  return m_ForegroundTilemapPtr;
}

Tilemap* Level::GetBackTilemap() const
{
  return m_BackgroundTilemapPtr;
}

void Level::Save()
{
}
