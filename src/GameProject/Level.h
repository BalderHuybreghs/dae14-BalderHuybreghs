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

  void Draw(bool debug = false) const;
  void Update(Player& player, float elapsedSec);
   
  // Adds an object by pointer
  void AddObject(GameObject* object);

  Tilemap* GetFrontTilemap() const;
  Tilemap* GetBackTilemap() const;

  void SetPlayerSpawn(const Point2f& position);
  Point2f GetPlayerSpawn() const;

  // Saves the level to a file with its name
  void Save();
private:
  // Metadata
  std::string m_Name;
  Point2f m_PlayerSpawn;

  std::vector<GameObject*> m_Objects; // All interactible objects, objects usually get a hold of the player

  // Texture* m_BackgroundPtr;

  Tilemap* m_BackgroundTilemapPtr; // The background tilemap for the level
  Tilemap* m_ForegroundTilemapPtr; // The tilemap for the level that contains all general blocks, the tilemap will contain collision information
};