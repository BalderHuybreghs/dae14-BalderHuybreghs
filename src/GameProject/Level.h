#pragma once
#include <vector>
#include "GameObject.h"
#include "Player.h"
#include "Tilemap.h"
#include "ObjectBlueprint.h"
#include "ParticleEmitter.h"
#include "SoundStream.h"
#include "ParallaxBackground.h"

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

  // Adds a camera position if it isn't overlapping with an existing one
  // Returns true if the camera managed to add the rect
  void AddCameraRect(const Rectf& rect); 
  bool RemoveCameraRect(const Point2f& point); // Removes a camera rect at a point, returns if it found a rect to remove
  Rectf GetCameraRect(const Player& player); // Gets the camera rect the player is in

  Tilemap* GetFrontTilemap() const;
  Tilemap* GetBackTilemap() const;

  ParallaxBackground* GetBackground() const;

  void SetPlayerSpawn(const Point2f& position);
  Point2f GetPlayerSpawn() const;

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

  std::vector<GameObject*> m_ObjectPtrs; // All interactible objects, objects usually get a hold of the player

  // A list of valid camera rects, when the player enters the rect
  // the camera will lerp over to it's position
  std::vector<Rectf> m_CameraRects;

  // Every level has 3 possible particle layers
  ParticleEmitter* m_ParticleEmitterBackPtr;
  ParticleEmitter* m_ParticleEmitterMidPtr;
  ParticleEmitter* m_ParticleEmitterFrontPtr;

  ParallaxBackground* m_ParallaxBackgroundPtr;

  SoundStream* m_MusicStreamPtr;
};