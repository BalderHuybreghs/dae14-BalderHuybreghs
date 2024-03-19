#pragma once
#include <vector>
#include "GameObject.h"

// A simple blueprint class that tells the game where it should
// put which object. This is done to make storing objects to files
// a lot easier.
class ObjectBlueprint
{
public:
  ObjectBlueprint(const std::vector<char>& raw);
  ObjectBlueprint(int objectId, Point2f position);

  // Draws the blueprint as a transparent object, useful for debugging and level editing
  void Draw() const;

  // Constructs the gameobject
  GameObject* Construct() const;

  // Gets the raw binary data of the gameobject
  std::vector<char> GetRawData();

  void SetObjectId(int objectId);
  void SetPosition(const Point2f& position);

  int GetObjectId() const;
  Point2f GetPosition() const;
private:
  // The ID of the object that should be constructed. This ID
  // should reflect the eid stored within the objectmanager
  int m_ObjectId;

  // The position where the object should be constructed
  Point2f m_Position;
};