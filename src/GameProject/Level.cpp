#include "pch.h"
#include "Level.h"
#include "GameDefines.h"
#include "ResourceUtils.h"
#include "utils.h"
#include "RectangleShape.h"
#include "BinaryStream.h"

#include <iostream>
#include <fstream>
#include <climits>
#include <vector>

Level::Level(const std::string& name)
  : m_Name(name), m_PlayerSpawn(Point2f())
{
  m_MusicStream = new SoundStream(ResourceUtils::ResourceToMusicPath(name)); // Load the music for the current level
  m_MusicStream->SetVolume(10);

  // Load both the foreground and background tilemaps
  m_BackgroundTilemapPtr = new Tilemap(Point2f{PIXEL_SCALE, PIXEL_SCALE }, TILE_SIZE, BACKGROUND_TILES, BACKGROUND_TILES_SIZE);
  m_ForegroundTilemapPtr = new Tilemap(Point2f{PIXEL_SCALE, PIXEL_SCALE }, TILE_SIZE, FOREGROUND_TILES, FOREGROUND_TILES_SIZE);

  Shape* emissionZoneShape{ new RectangleShape(Point2f{ SCREEN_EMISSION_ZONE_WIDTH, SCREEN_EMISSION_ZONE_HEIGHT }, Point2f{ WINDOW_WIDTH, 0.f }) };

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
  // Stop the music from playing
  m_MusicStream->Stop();

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

  delete m_MusicStream;
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

  // Queue the music :-)
  if (m_MusicStream->IsLoaded()) {
    m_MusicStream->Play(true);
  }
}

void Level::DrawBackground(Camera& camera, bool debug) const
{
  // Make sure the particle emitter is always on screen
  camera.PushMatrixInverse();
  // Draw the back particle emitter
  m_ParticleEmitterBack->Draw(debug);
  camera.PopMatrix();

  // Draw the background tilemap
  m_BackgroundTilemapPtr->Draw(debug);
}

void Level::DrawForeground(Camera& camera, bool debug) const
{
  camera.PushMatrixInverse();
  m_ParticleEmitterMid->Draw(debug); // Draw the middle particle emitter
  camera.PopMatrix();

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
  camera.PushMatrixInverse();
  m_ParticleEmitterFront->Draw(debug);
  camera.PopMatrix();

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
        m_PlayerSpawn.x,
        m_PlayerSpawn.y
    }, PLAYER_SCALE, PLAYER_SCALE);
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
  const Point2f cameraPosition{ camera.GetPosition() };
  const Point2f particleEmittersPosition{
    cameraPosition.x + WINDOW_WIDTH / 2.f,
    cameraPosition.y - WINDOW_HEIGHT / 2.f
  };

  m_ParticleEmitterBack->Update(elapsedSec);
  m_ParticleEmitterMid->Update(elapsedSec);
  m_ParticleEmitterFront->Update(elapsedSec);

  // Check if the camera rect should be updated
  const Rectf playerCollider{ player.GetCollisionShape()->GetShape() };
  for (const Rectf& rect : m_CameraRects) {
    if (IsOverlapping(playerCollider, rect))
    {
      // A new goal position for the camera to lerp to
      Camera::AnimationGoal goal{
        Point2f{ rect.left, rect.bottom },
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

bool Level::AddCameraRect(const Rectf& rect)
{
  // Check if the rect is not colliding with any other camera rects
  for (const Rectf& other : m_CameraRects) {
    if (IsOverlapping(rect, other)) {
      return false;
    }
  }
  
  m_CameraRects.push_back(rect);
  return true;
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

constexpr int TILEMAP_DELIMITER = MAXINT;

void Level::Load()
{
  std::cout << "Loading level: " << m_Name << std::endl;

  BinaryStream* stream{ new BinaryStream(ResourceUtils::ResourceToLevelPath(m_Name)) };
  stream->Load(); // Load the data

  m_PlayerSpawn = stream->ReadPoint();
  m_CameraRects = stream->ReadRectVec();
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
  stream->Write(m_BackgroundTilemapPtr->ToRawTileData());
  stream->Write(m_ForegroundTilemapPtr->ToRawTileData());
  stream->Write(m_ObjectBlueprints);
  stream->Save();

  delete stream;
}
