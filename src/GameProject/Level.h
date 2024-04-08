#pragma once
#include <vector>
#include "GameObject.h"
#include "Player.h"
#include "Tilemap.h"
#include "ObjectBlueprint.h"
#include "ParticleEmitter.h"
#include "SoundStream.h"

class Level
{
public:
  // Level with name loads a level
  Level(const std::string& name);

  // Rule of 3 applied, why would we need the ability to copy a level? (I'm too lazy to implement these)
  Level(const Level& other) = delete;
  Level& operator=(const Level& other) = delete;
  virtual ~Level();

  // Builds the level and all of its objects from the blueprint
  void Build();

  void DrawBackground(Camera& camera, bool debug = false) const;
  void DrawForeground(Camera& camera, bool debug = false) const;
  void Update(Player& player, Camera& camera, float elapsedSec);
   
  // Adds an object by pointer
  void AddBlueprint(const ObjectBlueprint& blueprint);

  Tilemap* GetFrontTilemap() const;
  Tilemap* GetBackTilemap() const;

  void SetPlayerSpawn(const Point2f& position);
  Point2f GetPlayerSpawn() const;

  const std::vector<std::vector<Point2f>>& GetCollisionPolygons() const;

  // Save and load the level from and to a file
  void Load();
  void Save() const;
private:
  // Metadata
  std::string m_Name;
  Point2f m_PlayerSpawn;
  std::vector<ObjectBlueprint> m_ObjectBlueprints;

  Tilemap* m_BackgroundTilemapPtr; // The background tilemap for the level
  Tilemap* m_ForegroundTilemapPtr; // The tilemap for the level that contains all general blocks, the tilemap will contain collision information

  std::vector<std::vector<Point2f>> m_CollisionPolygons; // Holds the collision polygons generated from the tilemap
  std::vector<GameObject*> m_Objects; // All interactible objects, objects usually get a hold of the player

  // Every level has 3 possible particle layers
  ParticleEmitter* m_ParticleEmitterBack;
  ParticleEmitter* m_ParticleEmitterMid;
  ParticleEmitter* m_ParticleEmitterFront;

  SoundStream* m_MusicStream;
};