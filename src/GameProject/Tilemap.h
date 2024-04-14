#pragma once
#include "AlgoUtils.h"
#include <unordered_map>
#include <vector>
#include "Texture.h"
#include <random>

class Tilemap final
{
public:
  enum class TileSide
  {
    None,
    Top,
    Bottom,
    Left,
    Right
  };

  // Create a tilemap raw data, a tilemap exists of multiple tilemap images, each representing 1 tile that
  // is automatically rotated based on the placement and neighbours
  Tilemap(const Point2f& size, int tileSize, const std::string resources[], int resourcesSize);

  void Draw(bool debug = false) const;

  // Draw a tile at a given tile position, this function accounts for adjacent tiles
  // the x and y coordinates are to determine the type of type, they are optional
  void DrawSingleTile(Point2f position, int tileID, int rng = 0, int x = 0, int y = 0) const;

  // Check if a rectangle is on a tile
  bool IsTile(const Rectf& rect) const;

  // This function rounds floats to ints to check if they are within a tile. 
  // There is no need for "complex" collision detection when using a tilemap.
  bool IsTile(const Point2f& point) const;

  // Checks for a tile on the grid
  bool IsTile(int x, int y) const;

  // Set a tile at a worldspace
  void SetTile(const Point2f& point, int tileID);

  // Remove a tile from the tilemap
  void RemoveTile(const Point2f& point);

  // Set the tiles for the tilemap
  void SetTiles(const std::string resources[], int resourcesSize);

  // Made to load in raw tile data from a binary file, the formay is just key-value int-int
  // hence the raw tile data vector size should be even
  void LoadRawTileData(const std::vector<int>& rawTileData);

  // Gives the amount of tiles the texture has
  int GetTileCount() const;
  int GetTileSize() const;
  Point2f GetSize() const;

  // Gives the rect of the tile in worldspace
  Rectf GetTileRect(const Point2f& position) const;

  // Convert the tiles to raw data
  std::vector<int> ToRawTileData() const;
private:
  int ValueToX(float val) const;
  int ValueToY(float val) const;

  // Converts a worldpoint to a key point in the tilemap
  std::pair<int, int> PointToKey(const Point2f& point) const;
  Point2f KeyToPoint(std::pair<int, int> key) const;

  // Gets a tile source rect for the tilemap based on a tile position
  // it does this by checking the adjacent tiles
  Rectf GetSourceRect(int x, int y, int rng) const;

  void DrawSingleTile(std::pair<int, int> position, int tileID, int rng) const;

  // Header information
  Point2f m_Size;
  int m_TileSize;

  // Internal management of the tilemap textures
  std::vector<const Texture*> m_TileTexturePtrs;

  // A mapping of each coordinate and their texture coordinate. Also updated to have the rng value of each tile for optimization reasons
  std::unordered_map<std::pair<int, int>, std::pair<int, int>, AlgoUtils::PairHash> m_Tiles;

};