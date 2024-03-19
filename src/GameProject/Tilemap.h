#pragma once
#include "GameObject.h"
#include <unordered_map>
#include <vector>

// Custom hash function for std::pair<int, int>
// TODO: Clean this up / move this to util
struct PairHash
{
  template <class T1, class T2>
  std::size_t operator () (const std::pair<T1, T2>& p) const
  {
    auto hash1 = std::hash<T1>{}(p.first);
    auto hash2 = std::hash<T2>{}(p.second);
    return hash1 ^ hash2; // Combine the hash values
  }
};

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

  // Remove a tile from the tilemap
  void RemoveTile(const Point2f& point);

  void SetResource(const std::string& resource);

  // Made to load in raw tile data from a binary file, the formay is just key-value int-int
  // hence the raw tile data vector size should be even
  void LoadRawTileData(const std::vector<int>& rawTileData);

  // Gives the amount of tiles the texture has
  int GetTileCount() const;
  int GetTileSize() const;
  std::string GetResource() const;
  Point2f GetSize() const;

  // Gives the rect of the tile in worldspace
  Rectf GetTileRect(const Point2f& position) const;

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
  std::unordered_map<std::pair<int, int>, int, PairHash> m_Tiles;

  // Converts a worldpoint to a key point in the tilemap
  int ValueToX(float val) const;
  int ValueToY(float val) const;
  std::pair<int, int> PointToKey(const Point2f& point) const;
  Point2f KeyToPoint(std::pair<int, int> key) const;
};