#include "pch.h"
#include "Level.h"
#include "GameDefines.h"
#include "ResourceUtils.h"
#include "utils.h"

#include <iostream>
#include <fstream>

Level::Level(const std::string& name, const std::string& foregroundTileResource, const std::string& backgroundTileResource)
  : m_Name(name)
{
  // Load both the foreground and background tilemaps
  m_ForegroundTilemapPtr = new Tilemap(foregroundTileResource, Point2f{5.f, 5.f}, TILE_SIZE);
  m_BackgroundTilemapPtr = new Tilemap(backgroundTileResource, Point2f{5.f, 5.f}, TILE_SIZE);
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
    utils::SetColor(Color4f{ 1.f, 0.f, 0.f, 1.f });
    utils::DrawPoint(m_PlayerSpawn, 5.f);
  }
}

void Level::Update(Player& player, float elapsedSec)
{
  // Update all the level gameobjects
  for (GameObject* object : m_Objects) {
    object->Update(player, elapsedSec);
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

// Check https://cplusplus.com/doc/tutorial/files/ to see where I found how to implement these

// WARNING: THIS IS A VERY DIRTY AND DANGEROUS IMPLEMENTATION, I AM HIGHLY AWARE OF THAT, HOWEVER
// I JUST WANT A LEVEL EDITOR THAT DOES WHAT IT NEEDS TO DO AND FOR THAT WE WILL ASSUME THAT THE LEVEL
// FILE WILL REMAIN UNTAMPERED WITH
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
  int zeroCount{ 0 };

  int tileValue;
  while (file.read(reinterpret_cast<char*>(&tileValue), sizeof(int)))
  {
    rawTileInfo.push_back(tileValue);
    if (tileValue == 0)
    {
      zeroCount++;
      if (zeroCount == 4)
        break;
    } else
    {
      zeroCount = 0;
    }
  }

  // Assuming BackgroundTilemap and ForegroundTilemap are initialized correctly
  m_BackgroundTilemapPtr->LoadRawTileData(rawTileInfo);

  rawTileInfo.clear();
  zeroCount = 0;

  while (file.read(reinterpret_cast<char*>(&tileValue), sizeof(int)))
  {
    rawTileInfo.push_back(tileValue);
    if (tileValue == 0)
    {
      zeroCount++;
      if (zeroCount == 4)
        break;
    } else
    {
      zeroCount = 0;
    }
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


  // Function to get the raw bytes of any type and add them to a vector<char>
  template<typename T>
  void AddValueToVector(const T& value, std::vector<char>& vec)
  {
    // Treat the value as an array of bytes
    const char* bytes = reinterpret_cast<const char*>(&value);

    // Add each byte to the vector
    vec.insert(vec.end(), bytes, bytes + sizeof(T));
  }

  // AGAIN, DANGEROUS (naive) IMPLEMENTATION
  void Level::Save() const
  {
    std::cout << "Saving level '" << m_Name << "' at: " << ResourceUtils::ResourceToLevelPath(m_Name) << std::endl;

    // Empty value to write
    const int empty{ 0 };

    // Create the memory stream to write
    std::vector<char> data;
  
    AddValueToVector(m_PlayerSpawn, data);

    for (int val : m_BackgroundTilemapPtr->ToRawTileData()) {
      AddValueToVector(val, data);
    }

    // Write empty 4 times for file padding, to know where the tilemap ends
    AddValueToVector(empty, data);
    AddValueToVector(empty, data);
    AddValueToVector(empty, data);
    AddValueToVector(empty, data);

    for (int val : m_ForegroundTilemapPtr->ToRawTileData()) {
      AddValueToVector(val, data);
    }
  
    // Again 4x empty for file padding, this to know where the tilemaps end
    AddValueToVector(empty, data);
    AddValueToVector(empty, data);
    AddValueToVector(empty, data);
    AddValueToVector(empty, data);

    // Write the blueprints to the data
    for (const ObjectBlueprint& bp : m_ObjectBlueprints) {
      AddValueToVector(bp.GetObjectId(), data);
      AddValueToVector(bp.GetPosition(), data);
    }

    // AND FINIT, all we have to do is write the data to the file
    std::ofstream file(ResourceUtils::ResourceToLevelPath(m_Name), std::ios::out | std::ios::binary);
    file.write(data.data(), data.size());
    file.close();
  }
