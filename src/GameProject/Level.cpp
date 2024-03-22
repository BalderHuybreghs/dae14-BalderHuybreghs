#include "pch.h"
#include "Level.h"
#include "GameDefines.h"
#include "ResourceUtils.h"
#include "utils.h"

#include <iostream>
#include <fstream>
#include <climits>

Level::Level(const std::string& name, const std::string& foregroundTileResource, const std::string& backgroundTileResource)
  : m_Name(name)
{
  // Load both the foreground and background tilemaps
  m_ForegroundTilemapPtr = new Tilemap(foregroundTileResource, Point2f{8.f, 8.f}, TILE_SIZE);
  m_BackgroundTilemapPtr = new Tilemap(backgroundTileResource, Point2f{8.f, 8.f}, TILE_SIZE);
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
  delete m_BackgroundTilemapPtr;
}

void Level::Build()
{
  std::cout << "Building level '" << m_Name << "'" << std::endl;

  // Clear polys
  m_CollisionPolygons.clear();
  m_CollisionPolygons = m_ForegroundTilemapPtr->GenCollisionShapes();

  // Destroy any possible game objects
  for (const GameObject* object : m_Objects) {
    delete object;
  }

  for (const ObjectBlueprint objectBlueprint : m_ObjectBlueprints) {
    objectBlueprint.Construct();
  }
}

void Level::Draw(bool debug) const
{
  // Draw the background tilemap
  m_BackgroundTilemapPtr->Draw(debug);

  // Draw the objects in between the two tilemaps
  for (const GameObject* object : m_Objects) {
    object->Draw(debug);
  }

  // Draw all the object blueprints in debug mode
  if (debug) {
    for (const ObjectBlueprint objectBlueprint : m_ObjectBlueprints) {
      objectBlueprint.Draw();
    }
  }

  // Draw the foreground tilemap
  m_ForegroundTilemapPtr->Draw(debug);

  // Draw the player spawn position on top of everything in debug mode
  if (debug) {
    // Draw the collision shapes
    for (const std::vector<Point2f>& polygon : m_CollisionPolygons) {
      utils::SetColor(Color4f{ 0.2f, 0.8f, 0.2f, 0.5f });
      utils::FillPolygon(polygon);

      // Draw the points, OpenGL does NOT like concave polygons
      for (const Point2f& point : polygon) {
        utils::FillEllipse(point, 5.f, 5.f);
      }
    }

    // Draw the player spawn
    utils::SetColor(Color4f{ .8f, 0.2f, 0.2f, 0.8f });
    utils::FillRect(Point2f{
        m_PlayerSpawn.x - 70.f / 2.f,
        m_PlayerSpawn.y
    }, 70.f, 70.f);
  }
}

void Level::Update(Player& player, float elapsedSec)
{
  // Update all the level gameobjects
  for (GameObject* object : m_Objects) {
    object->Update(player, elapsedSec);
  }

  // Apply gravity to the player
  if (!player.IsGrounded()) {
    (&player)->ApplyForce(GRAVITY * elapsedSec);
  }
}

void Level::AddBlueprint(const ObjectBlueprint& blueprint)
{
  m_ObjectBlueprints.push_back(blueprint);
}

Tilemap* Level::GetFrontTilemap() const
{
  return m_ForegroundTilemapPtr;
}

Tilemap* Level::GetBackTilemap() const
{
  return m_BackgroundTilemapPtr;
}

void Level::SetPlayerSpawn(const Point2f& position)
{
  m_PlayerSpawn = position;
}

Point2f Level::GetPlayerSpawn() const
{
  return m_PlayerSpawn;
}

const std::vector<std::vector<Point2f>>& Level::GetCollisionPolygons() const
{
  return m_CollisionPolygons;
}

// Check https://cplusplus.com/doc/tutorial/files/ to see where I found how to implement these

// WARNING: THIS IS A VERY DIRTY AND DANGEROUS IMPLEMENTATION, I AM HIGHLY AWARE OF THAT, HOWEVER
// I JUST WANT A LEVEL EDITOR THAT DOES WHAT IT NEEDS TO DO AND FOR THAT WE WILL ASSUME THAT THE LEVEL
// FILE WILL REMAIN UNTAMPERED WITH
// Define a delimiter value
// Function to get the raw bytes of any type and add them to a vector<char>
template<typename T>
void AddValueToVector(const T& value, std::vector<char>& vec)
{
  // Treat the value as an array of bytes
  const char* bytes = reinterpret_cast<const char*>(&value);

  // Add each byte to the vector
  vec.insert(vec.end(), bytes, bytes + sizeof(T));
}

constexpr int TILEMAP_DELIMITER = MAXINT;

void Level::Load()
{
  std::cout << "Loading level: " << m_Name << std::endl;

  std::ifstream file(ResourceUtils::ResourceToLevelPath(m_Name), std::ios::in | std::ios::binary);

  if (!file.is_open())
  {
    std::cout << "Unable to load level '" << m_Name << "' at " << ResourceUtils::ResourceToLevelPath(m_Name) << std::endl;
    return;
  };

  file.seekg(0, std::ios::beg);

  // Read PlayerSpawn
  file.read(reinterpret_cast<char*>(&m_PlayerSpawn), sizeof(Point2f));

  std::vector<int> rawTileInfo;
  int tileValue;

  while (file.read(reinterpret_cast<char*>(&tileValue), sizeof(int)))
  {
    if (tileValue == TILEMAP_DELIMITER)
      break;

    rawTileInfo.push_back(tileValue);
  }

  // Assuming BackgroundTilemap and ForegroundTilemap are initialized correctly
  m_BackgroundTilemapPtr->LoadRawTileData(rawTileInfo);

  rawTileInfo.clear();

  while (file.read(reinterpret_cast<char*>(&tileValue), sizeof(int)))
  {
    if (tileValue == TILEMAP_DELIMITER)
      break;

    rawTileInfo.push_back(tileValue);
  }

  m_ForegroundTilemapPtr->LoadRawTileData(rawTileInfo);
  rawTileInfo.clear();

  int objectId;
  Point2f position;
  while (file.read(reinterpret_cast<char*>(&objectId), sizeof(int)) && file.read(reinterpret_cast<char*>(&position), sizeof(Point2f)))
  {
    m_ObjectBlueprints.push_back(ObjectBlueprint(objectId, position));
  }

  file.close();
}

void Level::Save() const
{
  std::cout << "Saving level '" << m_Name << "' at: " << ResourceUtils::ResourceToLevelPath(m_Name) << std::endl;

  std::vector<char> data;

  AddValueToVector(m_PlayerSpawn, data);

  for (int val : m_BackgroundTilemapPtr->ToRawTileData()) {
    AddValueToVector(val, data);
  }

  // Write the delimiter for the background tilemap
  AddValueToVector(TILEMAP_DELIMITER, data);

  for (int val : m_ForegroundTilemapPtr->ToRawTileData()) {
    AddValueToVector(val, data);
  }

  // Write the delimiter for the foreground tilemap
  AddValueToVector(TILEMAP_DELIMITER, data);

  for (const ObjectBlueprint& bp : m_ObjectBlueprints) {
    AddValueToVector(bp.GetObjectId(), data);
    AddValueToVector(bp.GetPosition(), data);
  }

  std::ofstream file(ResourceUtils::ResourceToLevelPath(m_Name), std::ios::out | std::ios::binary);
  file.write(data.data(), data.size());
  file.close();
}
