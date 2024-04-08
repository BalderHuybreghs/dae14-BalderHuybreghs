#include "pch.h"
#include "Level.h"
#include "GameDefines.h"
#include "ResourceUtils.h"
#include "utils.h"
#include "RectangleShape.h"

#include <iostream>
#include <fstream>
#include <climits>
#include <vector>

Level::Level(const std::string& name)
  : m_Name(name), m_PlayerSpawn(Point2f())
{
  // Load both the foreground and background tilemaps
  m_BackgroundTilemapPtr = new Tilemap(Point2f{TILEMAP_SCALE, TILEMAP_SCALE }, TILE_SIZE, BACKGROUND_TILES, BACKGROUND_TILES_SIZE);
  m_ForegroundTilemapPtr = new Tilemap(Point2f{TILEMAP_SCALE, TILEMAP_SCALE }, TILE_SIZE, FOREGROUND_TILES, FOREGROUND_TILES_SIZE);

  Shape* emissionZoneShape{ new RectangleShape(Point2f{ SCREEN_EMISSION_ZONE_WIDTH, SCREEN_EMISSION_ZONE_HEIGHT }, Point2f{}) };

  Shape* shape1{ new RectangleShape(Point2f{ SNOW_PARTICLE_SIZE, SNOW_PARTICLE_SIZE }, Point2f{}, SNOW_PARTICLE_COLOR1, true) };
  Shape* shape2{ new RectangleShape(Point2f{ SNOW_PARTICLE_SIZE, SNOW_PARTICLE_SIZE }, Point2f{}, SNOW_PARTICLE_COLOR2, true) };
  Shape* shape3{ new RectangleShape(Point2f{ SNOW_PARTICLE_SIZE, SNOW_PARTICLE_SIZE }, Point2f{}, SNOW_PARTICLE_COLOR3, true) };

  // Create the particle emitters, copy the shapes and remove them afterwards
  // I'm not sure how to properly handle the shapes part since it's polymorphism, might ask the teachers (you who are reading my code), about it after the spring "vacation"
  m_ParticleEmitterBack = new ParticleEmitter(emissionZoneShape->Copy(), SNOW_PARTICLE_INFO, std::vector<Shape*>{ shape1->Copy(), shape2->Copy(), shape3->Copy() });
  m_ParticleEmitterMid = new ParticleEmitter(emissionZoneShape->Copy(), SNOW_PARTICLE_INFO, std::vector<Shape*>{ shape1->Copy(), shape2->Copy(), shape3->Copy() });
  m_ParticleEmitterFront = new ParticleEmitter(emissionZoneShape->Copy(), SNOW_PARTICLE_INFO, std::vector<Shape*>{ shape1->Copy(), shape2->Copy(), shape3->Copy() });

  // ehhh, this is not particularly good...
  delete emissionZoneShape;
  delete shape1;
  delete shape2;
  delete shape3;
}

Level::~Level()
{
  // Clear out the game objects
  for (const GameObject* object : m_Objects) {
    delete object;
  }

  m_Objects.clear();

  // Delete the particle emitters
  delete m_ParticleEmitterBack;
  delete m_ParticleEmitterMid;
  delete m_ParticleEmitterFront;

  // Delete the tilemaps
  delete m_ForegroundTilemapPtr;
  delete m_BackgroundTilemapPtr;
}

void Level::Build()
{
  std::cout << "Building level '" << m_Name << "'" << std::endl;

  // Clear polys
  m_CollisionPolygons.clear();
  //m_CollisionPolygons = m_ForegroundTilemapPtr->GenCollisionShapes();

  // Destroy any possible game objects
  for (const GameObject* object : m_Objects) {
    delete object;
  }

  for (const ObjectBlueprint objectBlueprint : m_ObjectBlueprints) {
    objectBlueprint.Construct();
  }
}

void Level::DrawBackground(bool debug) const
{
  // Draw the back particle emitter
  m_ParticleEmitterBack->Draw(debug);

  // Draw the background tilemap
  m_BackgroundTilemapPtr->Draw(debug);
}

void Level::DrawForeground(bool debug) const
{
  m_ParticleEmitterMid->Draw(debug); // Draw the middle particle emitter

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

  // Draw the front particle emitter
  m_ParticleEmitterFront->Draw(debug);

  // Draw the player spawn position on top of everything in debug mode
  if (debug) {
    // Draw the collision shapes
    for (const std::vector<Point2f>& polygon : m_CollisionPolygons) {
      utils::SetColor(Color4f{ 0.2f, 0.8f, 0.2f, 0.5f });
      utils::FillPolygon(polygon);

      // Draw the points, OpenGL does NOT like concave polygons
      int count{ 0 };
      for (const Point2f& point : polygon) {
        utils::FillEllipse(point, 5.f, 5.f);
        // Draw count as text
        //Texture* text{ new Texture(std::to_string(count), FONT_FOLDER + FS + SYNE_FONT + FONT_EXTENSION, 20, Color4f(1.f, 0.f, 0.f, 1.f))};
        //text->Draw(point);
        //delete text;

        ++count;
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

void Level::Update(Player& player, Camera& camera, float elapsedSec)
{
  // Update all the level gameobjects
  for (GameObject* object : m_Objects) {
    object->Update(player, camera, elapsedSec);
  }

  // Apply gravity to the player
  if (player.GetState() != Player::State::Climbing && player.GetState() != Player::State::Holding) {
    (&player)->ApplyForce(GRAVITY * elapsedSec);
  }

  // Update the particle emitters, especially the location such that they follow the player
  m_ParticleEmitterBack->SetPosition(m_ParticleEmitterBack->GetPosition() + camera.GetPosition());
  m_ParticleEmitterMid->SetPosition(m_ParticleEmitterMid->GetPosition() + camera.GetPosition());
  m_ParticleEmitterFront->SetPosition(m_ParticleEmitterFront->GetPosition() + camera.GetPosition());

  m_ParticleEmitterBack->Update(elapsedSec);
  m_ParticleEmitterMid->Update(elapsedSec);
  m_ParticleEmitterFront->Update(elapsedSec);
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
  int tileValue{};

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

  int objectId{};
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
