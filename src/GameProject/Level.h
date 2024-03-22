#pragma once
#include <vector>
#include "GameObject.h"
#include "Player.h"
#include "Tilemap.h"
#include "ObjectBlueprint.h"

class Level
{
public:
  // Level with name loads a level
  Level(const std::string& name, const std::string& foregroundTileResource, const std::string& backgroundTileResource);
  ~Level();

  // Builds the level and all of its objects from the blueprint
  void Build();

  void Draw(bool debug = false) const;
  void Update(Player& player, float elapsedSec);
   
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
};