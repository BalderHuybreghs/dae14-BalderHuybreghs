#include "pch.h"
#include "Level.h"
#include "GameDefines.h"
#include "ResourceUtils.h"
#include "utils.h"
#include "RectangleShape.h"
#include "BinaryStream.h"
#include "ObjectManager.h"

#include <iostream>
#include <fstream>
#include <climits>
#include <vector>

Level::Level(const std::string& name)
  : m_Name(name), m_PlayerSpawn(Point2f())
{
  m_MusicStreamPtr = new SoundStream(ResourceUtils::ResourceToMusicPath(name)); // Load the music for the current level
  m_MusicStreamPtr->SetVolume(10);

  // Load both the foreground and background tilemaps
  m_BackgroundTilemapPtr = new Tilemap(Point2f{PIXEL_SCALE, PIXEL_SCALE }, TILE_SIZE, BACKGROUND_TILES, BACKGROUND_TILES_SIZE);
  m_ForegroundTilemapPtr = new Tilemap(Point2f{PIXEL_SCALE, PIXEL_SCALE }, TILE_SIZE, FOREGROUND_TILES, FOREGROUND_TILES_SIZE);

  m_ParallaxBackgroundPtr = new ParallaxBackground(m_Name); // Set the parralax background

  Shape* emissionZoneShape{ new RectangleShape(Point2f{ SCREEN_EMISSION_ZONE_WIDTH, SCREEN_EMISSION_ZONE_HEIGHT }, Point2f{ WINDOW_WIDTH, 0.f }) };

  Shape* shape1{ new RectangleShape(Point2f{ SNOW_PARTICLE_SIZE, SNOW_PARTICLE_SIZE }, Point2f{}, SNOW_PARTICLE_COLOR1, true) };
  Shape* shape2{ new RectangleShape(Point2f{ SNOW_PARTICLE_SIZE, SNOW_PARTICLE_SIZE }, Point2f{}, SNOW_PARTICLE_COLOR2, true) };
  Shape* shape3{ new RectangleShape(Point2f{ SNOW_PARTICLE_SIZE, SNOW_PARTICLE_SIZE }, Point2f{}, SNOW_PARTICLE_COLOR3, true) };
  Shape* shape4{ new RectangleShape(Point2f{ SNOW_PARTICLE_SIZE, SNOW_PARTICLE_SIZE }, Point2f{}, SNOW_PARTICLE_COLOR4, true) };

  // Create the particle emitters, copy the shapes and remove them afterwards
  // I'm not sure how to properly handle the shapes part since it's polymorphism, might ask the teachers (you who are reading my code), about it after the spring "vacation"
  m_ParticleEmitterBackPtr = new ParticleEmitter(emissionZoneShape->Copy(), SNOW_PARTICLE_INFO_BACK, std::vector<Shape*>{ shape2->Copy(), shape3->Copy(), shape4->Copy() });
  m_ParticleEmitterMidPtr = new ParticleEmitter(emissionZoneShape->Copy(), SNOW_PARTICLE_INFO_MID, std::vector<Shape*>{ shape1->Copy(), shape3->Copy(), shape4->Copy() });
  m_ParticleEmitterFrontPtr = new ParticleEmitter(emissionZoneShape->Copy(), SNOW_PARTICLE_INFO_FRONT, std::vector<Shape*>{ shape1->Copy(), shape2->Copy() });

  // ehhh, this is not particularly good...
  delete emissionZoneShape;
  delete shape1;
  delete shape2;
  delete shape3;
}

Level::~Level()
{
  // Stop the music from playing
  m_MusicStreamPtr->Stop();

  // Clear out the game objects
  for (const GameObject* object : m_ObjectPtrs) {
    delete object;
  }

  m_ObjectPtrs.clear();

  // Delete the particle emitters
  delete m_ParticleEmitterBackPtr;
  delete m_ParticleEmitterMidPtr;
  delete m_ParticleEmitterFrontPtr;

  // Delete the tilemaps
  delete m_ForegroundTilemapPtr;
  delete m_BackgroundTilemapPtr;

  delete m_ParallaxBackgroundPtr;

  delete m_MusicStreamPtr;
}

void Level::Build()
{
  std::cout << "Building level '" << m_Name << "'" << std::endl;

  // Destroy any possible game objects
  for (const GameObject* object : m_ObjectPtrs) {
    delete object;
  }

  for (const ObjectBlueprint objectBlueprint : m_ObjectBlueprints) {
    if (!ObjectManager::GetInstance()->ObjectWithIdExists(objectBlueprint.GetObjectId())) {
      std::cerr << "Object with ID " << objectBlueprint.GetObjectId() << " does not exist in registry, skipping" << std::endl;
      continue;
    }

    m_ObjectPtrs.push_back(objectBlueprint.Construct());
  }

  // Queue the music :-)
  if (m_MusicStreamPtr->IsLoaded()) {
    m_MusicStreamPtr->Play(true);
  }
}

void Level::DrawBackground(Camera& camera, bool debug) const
{
  // Make sure the particle emitter is always on screen
  camera.PushMatrixInverse();

  m_ParallaxBackgroundPtr->Draw(camera, debug);

  // Draw the back particle emitter
  m_ParticleEmitterBackPtr->Draw(debug);
  camera.PopMatrix();

  // Draw the background tilemap
  m_BackgroundTilemapPtr->Draw(debug);
}

void Level::DrawForeground(Camera& camera, bool debug) const
{
  camera.PushMatrixInverse();
  m_ParticleEmitterMidPtr->Draw(debug); // Draw the middle particle emitter
  camera.PopMatrix();

  // Draw all the object blueprints in debug mode
  if (debug) {
    for (const ObjectBlueprint objectBlueprint : m_ObjectBlueprints) {
      objectBlueprint.Draw();
    }
  }

  // Draw the objects in between the two tilemaps
  for (const GameObject* object : m_ObjectPtrs) {
    object->Draw(debug);
  }

  // Draw the foreground tilemap
  m_ForegroundTilemapPtr->Draw(debug);

  // Draw the front particle emitter
  camera.PushMatrixInverse();
  m_ParticleEmitterFrontPtr->Draw(debug);
  camera.PopMatrix();

  // Draw the player spawn position on top of everything in debug mode
  if (!debug) {
    return;
  }

  // Draw the player spawn
  utils::SetColor(Color4f{ .8f, 0.2f, 0.2f, 0.8f });
  utils::FillRect(Point2f{
      m_PlayerSpawn.x,
      m_PlayerSpawn.y
  }, PLAYER_SCALE, PLAYER_SCALE);

  // Draw the death zones
  for (const Rectf& rect : m_DeathZones) {
    utils::SetColor(Color4f{ 0.8f, 0.f, 0.f, 0.5f });
    utils::FillRect(rect);
  }

  // Draw all the camera rects
  for (const Rectf& rect : m_CameraRects) {
    utils::SetColor(Color4f{ 0.f, 0.f, 1.f, 1.f });
    utils::DrawRect(rect, 2.f);
  }
}

void Level::Update(Player& player, Camera& camera, float elapsedSec)
{
  // The player dies if below Y 0, this in case the player ever gets out of bounds somehow
  if (player.GetPosition().y < 0) {
    player.Kill();
  }

  // Check if the player hit any death zones
  for (const Rectf& zone : m_DeathZones) {
    if (IsOverlapping(player.GetCollisionShape()->GetShape(), zone)) {
      player.Kill();
      break;
    }
  }

  // Update all the level gameobjects
  for (GameObject* object : m_ObjectPtrs) {
    object->Update(player, camera, elapsedSec);
  }

  // Apply gravity to the player
  if (player.GetState() != Player::State::Climbing && player.GetState() != Player::State::Holding) {
    (&player)->ApplyForce(GRAVITY * elapsedSec);
  }

  // Update the particle emitters, especially the location such that they follow the player
  const Point2f cameraPosition{ camera.GetPosition() };
  const Point2f particleEmittersPosition{
    cameraPosition.x + WINDOW_WIDTH / 2.f,
    cameraPosition.y - WINDOW_HEIGHT / 2.f
  };

  m_ParticleEmitterBackPtr->Update(elapsedSec);
  m_ParticleEmitterMidPtr->Update(elapsedSec);
  m_ParticleEmitterFrontPtr->Update(elapsedSec);

  // Check if the camera rect should be updated
  const Rectf playerCollider{ player.GetCollisionShape()->GetShape() };
  for (const Rectf& rect : m_CameraRects) {
    if (IsOverlapping(playerCollider, rect))
    {
      const Point2f position{ rect.left, rect.bottom };
      const Point2f cameraPosition{ camera.GetPosition() };

      // Move on if the camera is already in the right position
      if ((int)cameraPosition.x == (int)position.x && (int)cameraPosition.y == (int)position.y || camera.GoalCount() > 0) {
        continue;
      }

      // A new goal position for the camera to lerp to
      Camera::AnimationGoal goal{
        position,
        1.f,
        .5f, 
      };

      camera.AddGoal(goal);
      break;
    }
  }
}

void Level::AddBlueprint(const ObjectBlueprint& blueprint)
{
  m_ObjectBlueprints.push_back(blueprint);
}

bool Level::RemoveBlueprint(const Point2f& position)
{
  for (std::vector<ObjectBlueprint>::iterator it = m_ObjectBlueprints.begin(); it != m_ObjectBlueprints.end(); ++it) {
    const ObjectBlueprint& blueprint = *it;

    // Get the tilemap position of the blueprint
    const Rectf blueprintRect = m_ForegroundTilemapPtr->GetTileRect(blueprint.GetPosition());

    if (IsPointInRect(position, blueprintRect)) {
      m_ObjectBlueprints.erase(it);
      return true;
    }
  }

  return false;
}

void Level::AddCameraRect(const Rectf& rect)
{
  // Check if the rect is not colliding with any other camera rects
  m_CameraRects.push_back(rect);
}

bool Level::RemoveCameraRect(const Point2f& position)
{
  for (std::vector<Rectf>::iterator it = m_CameraRects.begin(); it != m_CameraRects.end(); ++it) {
    const Rectf& rect = *it;
    if (IsPointInRect(position, rect)) {
      // Remove this value from camerarects
      m_CameraRects.erase(it);
      return true;
    }
  }

  return false;
}

Rectf Level::GetCameraRect(const Player& player)
{
  const Rectf playerCollider{ player.GetCollisionShape()->GetShape() };
  for (const Rectf& rect : m_CameraRects) {
    if (IsOverlapping(playerCollider, rect))
    {
      return rect;
    }
  }

  return player.GetCollisionShape()->GetShape();
}

void Level::AddDeathZone(const Rectf& rect)
{
  m_DeathZones.push_back(rect);
}

bool Level::RemoveDeathZone(const Point2f position)
{
  for (std::vector<Rectf>::iterator it = m_DeathZones.begin(); it != m_DeathZones.end(); ++it) {
    const Rectf& rect = *it;
    if (IsPointInRect(position, rect)) {
      // Remove this value from the deathzones
      m_DeathZones.erase(it);
      return true;
    }
  }

  return false;
}

Tilemap* Level::GetFrontTilemap() const
{
  return m_ForegroundTilemapPtr;
}

Tilemap* Level::GetBackTilemap() const
{
  return m_BackgroundTilemapPtr;
}

ParallaxBackground* Level::GetBackground() const
{
  return m_ParallaxBackgroundPtr;
}

void Level::SetPlayerSpawn(const Point2f& position)
{
  m_PlayerSpawn = position;
}

Point2f Level::GetPlayerSpawn() const
{
  return m_PlayerSpawn;
}

constexpr int TILEMAP_DELIMITER = MAXINT;

void Level::Load()
{
  std::cout << "Loading level: " << m_Name << std::endl;

  BinaryStream* stream{ new BinaryStream(ResourceUtils::ResourceToLevelPath(m_Name)) };
  stream->Load(); // Load the data

  m_PlayerSpawn = stream->ReadPoint();
  m_CameraRects = stream->ReadRectVec();
  m_DeathZones = stream->ReadRectVec();
  m_ParallaxBackgroundPtr->SetMidRect(stream->ReadRect());
  m_ParallaxBackgroundPtr->SetFrontRect(stream->ReadRect());
  m_BackgroundTilemapPtr->LoadRawTileData(stream->ReadIntVec());
  m_ForegroundTilemapPtr->LoadRawTileData(stream->ReadIntVec());
  m_ObjectBlueprints = stream->ReadBlueprintVec();

  delete stream;
}

void Level::Save() const
{
  std::cout << "Saving level '" << m_Name << "' at: " << ResourceUtils::ResourceToLevelPath(m_Name) << std::endl;

  BinaryStream* stream{ new BinaryStream(ResourceUtils::ResourceToLevelPath(m_Name)) };

  stream->Write(m_PlayerSpawn);
  stream->Write(m_CameraRects);
  stream->Write(m_DeathZones);
  stream->Write(m_ParallaxBackgroundPtr->GetMidRect());
  stream->Write(m_ParallaxBackgroundPtr->GetFrontRect());
  stream->Write(m_BackgroundTilemapPtr->ToRawTileData());
  stream->Write(m_ForegroundTilemapPtr->ToRawTileData());
  stream->Write(m_ObjectBlueprints);
  stream->Save();

  delete stream;
}
