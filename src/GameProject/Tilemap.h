#pragma once
#include "GameObject.h"
#include <unordered_map>
#include <vector>

class Tilemap
{
public:
  // Create a tilemap raw data
  Tilemap(const std::string& resource, const Point2f& size, int tileSize);

  // Inherited via GameObject
  void Draw(bool debug = false) const;

  // Draws a single tile at a given position
  void DrawSingleTile(const Point2f& position, int tileId) const;
  
  // This function rounds floats to ints to check if they are within a tile. 
  // There is no need for "complex" collision detection when using a tilemap.
  bool IsTile(const Point2f& point) const;

  // Set a tile at a worldspace
  void SetTile(const Point2f& point, int tileID);

  void SetResource(const std::string& resource);

  // Made to load in raw tile data from a binary file, the formay is just key-value int-int
  // hence the raw tile data vector size should be even
  void LoadRawTileData(const std::vector<int>& rawTileData);

  // Gives the amount of tiles the texture has
  int GetTileCount() const;

  std::string GetResource();

  // Convert the tiles to raw data
  std::vector<int> ToRawTileData() const;
private:
  // Header information
  std::string m_Resource;
  Point2f m_Size;
  int m_TileSize;

  // Internal management
  const Texture* m_TileTexturePtr;

  // A mapping of each coordinate and their texture coordinate.
  // Coordinates are calculated simply by multiplying the x and y values
  // The texture coordinate may never be more than the texture height multiplied by the texture width.
  std::unordered_map<int, int> m_Tiles;

  // Converts a worldpoint to a key point in the tilemap
  int PointToKey(const Point2f& point) const;
  Point2f KeyToPoint(int key) const;
};