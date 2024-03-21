#include "pch.h"
#include "Tilemap.h"
#include "TextureManager.h"
#include <iostream>
#include "ResourceUtils.h"
#include "utils.h"
#include <queue>

using namespace utils;

Tilemap::Tilemap(const std::string& resource, const Point2f& size, int tileSize)
  : m_Size(size), m_TileSize(tileSize), m_Resource(resource)
{
  SetState(resource);
}

void Tilemap::Draw(bool debug) const
{
  for (const std::pair<std::pair<int, int>, int>& tileInfo : m_Tiles) {
    DrawSingleTile(KeyToPoint(tileInfo.first), tileInfo.second);
  }
}

void Tilemap::DrawSingleTile(const Point2f& position, int tileId) const
{
  // Calculate rects
  const Rectf dstRect{
    position.x,
    position.y,
    m_TileSize * m_Size.x,
    m_TileSize * m_Size.y
  };

  int gridWidth = int(m_TileTexturePtr->GetWidth() / m_TileSize);

  const Rectf srcRect{
    float(tileId % gridWidth) * m_TileSize,
    float(tileId / gridWidth) * m_TileSize,
    float(m_TileSize),
    float(m_TileSize)
  };

  m_TileTexturePtr->Draw(dstRect, srcRect);
}

bool Tilemap::IsTile(const Point2f& point) const
{
  std::pair<int, int> pair{ PointToKey(point) };
  return IsTile(pair.first, pair.second);
}

bool Tilemap::IsTile(int x, int y) const
{
  std::pair<int, int> key{ std::make_pair(x, y) };

  // Search for the coordinates in the map
  // No auto :(
  std::_List_const_iterator<std::_List_val<std::_List_simple_types<std::pair<const std::pair<int, int>, int>>>> it{ m_Tiles.find(key) };

  return it != m_Tiles.end();
}

void Tilemap::SetTile(const Point2f& point, int tileID)
{
  if (tileID > GetTileCount()) {
    std::cout << "Tile ID exceeds tiles" << std::endl;
    return;
  }

  m_Tiles.insert_or_assign(PointToKey(point), tileID);
}

void Tilemap::RemoveTile(const Point2f& point)
{
  // Remove the tile if it exists
  if (IsTile(point)) {
    m_Tiles.erase(PointToKey(point));
  }
}

void Tilemap::SetState(const std::string& resource)
{
  // Grabs tilemaps from the tilemap folder
  m_TileTexturePtr = TextureManager::Instance()->GetTexture(ResourceUtils::ResourceToTilemapPath(resource));

  // The texture is invalid if it is not divisible by the size of each tile
  if ((int)m_TileTexturePtr->GetWidth() % m_TileSize != 0 || (int)m_TileTexturePtr->GetHeight() % m_TileSize != 0) {
    std::cerr << "Tilemap texture is non-divisible by: " << m_TileSize << std::endl;
    exit(-1);
  }
}

void Tilemap::LoadRawTileData(const std::vector<int>& rawTileData)
{
  std::cout << "Loading raw tilemap data for '" << m_Resource << "'" << std::endl;

  if (rawTileData.size() % 3 != 0) {
    std::cerr << "Tile data for '" << m_Resource <<"' is not divisible by 3, concluded that data must be corrupt" << std::endl;
    return;
  }

  // Create key-value pairs and insert into hashmap
  for (size_t index = 0; index < rawTileData.size(); index += 3) {
    int x = rawTileData[index];
    int y = rawTileData[index + 1];
    int value = rawTileData[index + 2];

    if (value > GetTileCount()) {
      std::cerr << "Tile exceeds texture at index (skipping): " << index << std::endl;
      continue;
    }

    m_Tiles.insert(std::make_pair(std::make_pair(x, y), value));
  }
}

int Tilemap::GetTileCount() const
{
  return int(m_TileTexturePtr->GetWidth() / m_TileSize) * int(m_TileTexturePtr->GetHeight() / m_TileSize);
}

int Tilemap::GetTileSize() const
{
  return m_TileSize;
}

std::string Tilemap::GetResource() const
{
  return m_Resource;
}

Point2f Tilemap::GetSize() const
{
  return m_Size;
}

Rectf Tilemap::GetTileRect(const Point2f& position) const
{
  // Calculate the position of the tile
  std::pair<int, int> key{ PointToKey(position) };
  const Point2f worldPos{ KeyToPoint(key) };
  
  return Rectf{
    worldPos.x,
    worldPos.y,
    m_TileSize * m_Size.x,
    m_TileSize * m_Size.y
  };
}

std::vector<int> Tilemap::ToRawTileData() const
{
  std::cout << "Fetching raw tilemap data for '" << m_Resource << "'" << std::endl;

  std::vector<int> rawTileData;

  for (const auto& pair : m_Tiles) {
    // Push key
    rawTileData.push_back(pair.first.first);
    rawTileData.push_back(pair.first.second);

    // Push value
    rawTileData.push_back(pair.second);  // Push value
  }

  return rawTileData;
}

int Tilemap::ValueToX(float val) const
{
  return int(val / (m_TileSize * m_Size.x));
}

int Tilemap::ValueToY(float val) const
{
  return int(val / (m_TileSize * m_Size.y));
}

std::pair<int, int> Tilemap::PointToKey(const Point2f& point) const
{
  return std::make_pair(ValueToX(point.x), ValueToY(point.y));
}

Point2f Tilemap::KeyToPoint(std::pair<int, int> key) const
{
  return Point2f{
    key.first * m_TileSize * m_Size.x,
    key.second * m_TileSize * m_Size.y
  };
}

void Tilemap::ExploreIsland(int x, int y, std::unordered_map<std::pair<int, int>, bool, PairHash>& visitedGrid, std::vector<Point2f>& shape, int dx, int dy) const
{
  // Check if the tile is a valid tile and hasn't been visited yet
  if (visitedGrid.find(std::make_pair(x, y)) == visitedGrid.end() && IsTile(x, y)) {
    // Mark the tile as visited
    visitedGrid.insert({ std::make_pair(x, y), true });

    const Rectf tile{ GetTileRect(KeyToPoint({ x, y })) };
    const Point2f bottomLeft{ tile.left, tile.bottom };

    // Count the corners found
    int cornersFound{};

    // Add only the necessary corners, a corner is only a corner if the 3 adjacent connected tiles
    // are non-existent, or 2 adjacent tiles are existent but not the diagonal tile

    // Top-left corner
    if (!IsTile(x - 1, y) && !IsTile(x, y + 1) && !IsTile(x - 1, y + 1)) { // Outside corner
      shape.push_back(Point2f{ bottomLeft.x, bottomLeft.y + tile.height });
      ExploreIsland(x + 1, y, visitedGrid, shape, 1, 0); // Go right
      ++cornersFound;
    } else if (!IsTile(x - 1, y + 1) && IsTile(x, y + 1) && IsTile(x - 1, y)) { // Inside corner
      shape.push_back(Point2f{ bottomLeft.x, bottomLeft.y + tile.height });
      ExploreIsland(x, y + 1, visitedGrid, shape, 0, 1); // Go up
      ++cornersFound;
    } else if(IsTile(x - 1, y + 1) && !IsTile(x, y + 1) && !IsTile(x - 1, y)) { // Edge case (diagonal connection)
      shape.push_back(Point2f{ bottomLeft.x, bottomLeft.y + tile.height }); 
      ExploreIsland(x + 1, y, visitedGrid, shape, 1, 0); // Go right
      ++cornersFound;
    }

    // Top-right corner
    if (!IsTile(x + 1, y) && !IsTile(x, y + 1) && !IsTile(x + 1, y + 1)) { // Outside corner
      shape.push_back(Point2f{ bottomLeft.x + tile.width, bottomLeft.y + tile.height });
      ExploreIsland(x, y - 1, visitedGrid, shape, 0, -1); // Go down
      ++cornersFound;
    } else if (!IsTile(x + 1, y + 1) && IsTile(x + 1, y) && IsTile(x, y + 1)) { // Inside corner
      shape.push_back(Point2f{ bottomLeft.x + tile.width, bottomLeft.y + tile.height });
      ExploreIsland(x + 1, y, visitedGrid, shape, 1, 0);  // Go right
      ++cornersFound;
    } else if (IsTile(x + 1, y + 1) && !IsTile(x + 1, y) && !IsTile(x, y + 1)) { // Edge case (diagonal connection)
      shape.push_back(Point2f{ bottomLeft.x, bottomLeft.y + tile.height }); 
      ExploreIsland(x, y - 1, visitedGrid, shape, 0, -1); // Go down
      ++cornersFound;
    }

    // Bottom-right corner
    if (!IsTile(x + 1, y) && !IsTile(x, y - 1) && !IsTile(x + 1, y - 1)) { // Outside corner
      shape.push_back(Point2f{ bottomLeft.x + tile.width, bottomLeft.y });
      ExploreIsland(x - 1, y, visitedGrid, shape, -1, 0); // Go left
      ++cornersFound;
    } else if (!IsTile(x + 1, y - 1) && IsTile(x + 1, y) && IsTile(x, y - 1)) { // Inside corner
      shape.push_back(Point2f{ bottomLeft.x + tile.width, bottomLeft.y });
      ExploreIsland(x, y - 1, visitedGrid, shape, 0, - 1); // Go down
      ++cornersFound;
    } else if (IsTile(x + 1, y - 1) && !IsTile(x + 1, y) && !IsTile(x, y - 1)) { // Edge case (diagonal connection)
      shape.push_back(Point2f{ bottomLeft.x, bottomLeft.y + tile.height });
      ExploreIsland(x - 1, y, visitedGrid, shape, -1, 0); // Go down
      ++cornersFound;
    }

    // Bottom-left corner
    if (!IsTile(x, y - 1) && !IsTile(x - 1, y) && !IsTile(x - 1, y - 1)) { // Outside corner
      shape.push_back(Point2f{ bottomLeft.x, bottomLeft.y });
      ExploreIsland(x, y + 1, visitedGrid, shape, 0, +1); // Go up
      ++cornersFound;
    } else if (!IsTile(x - 1, y - 1) && IsTile(x, y - 1) && IsTile(x - 1, y)) { // Inside corner
      shape.push_back(Point2f{ bottomLeft.x, bottomLeft.y });
      ExploreIsland(x - 1, y, visitedGrid, shape, -1, 0); // Go left
      ++cornersFound;
    } else if (IsTile(x - 1, y - 1) && !IsTile(x, y - 1) && !IsTile(x - 1, y)) { // Edge case (diagonal connection)
      shape.push_back(Point2f{ bottomLeft.x, bottomLeft.y + tile.height });
      ExploreIsland(x, y + 1, visitedGrid, shape, 0, +1); // Go down
      ++cornersFound;
    }

    // If no adjacent corners have been found and the shape is still empty, 
    // continue exploring the coastline
    if (cornersFound < 1) {
      ExploreIsland(x + dx, y + dy, visitedGrid, shape, dx, dy);
    }
  }
}

std::vector<std::vector<Point2f>> Tilemap::GenCollisionShapes() const
{
  std::cout << "Generating collision shape for tilemap '" << m_Resource << "'" << std::endl;

  std::vector<std::vector<Point2f>> collisionShapes;

  // Grid to mark visited tiles
  std::unordered_map<std::pair<int, int>, bool, PairHash> visitedGrid{};

  for (const std::pair<std::pair<int, int>, int>& tile : m_Tiles) {
    const std::pair<int, int>& key = tile.first;
    int x = key.first;
    int y = key.second;

    auto it = visitedGrid.find(std::make_pair(x, y));
    if (it == visitedGrid.end()) { // If the tile hasn't been visited yet
      std::vector<Point2f> shape; // Initialize a new shape

      // Explore the island using DFS
      ExploreIsland(x, y, visitedGrid, shape, 0, 0);

      // Sort the points of the shape in clockwise order
      // SortPointsClockwise(shape);

      // Add the shape to the list of collision shapes
      if (!shape.empty()) {
        collisionShapes.push_back(shape);
      }
    }
  }

  std::cout << "Found collision islands: " << collisionShapes.size() << std::endl;
  return collisionShapes;
}