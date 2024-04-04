#include "pch.h"
#include "Tilemap.h"
#include "TextureManager.h"
#include <iostream>
#include "ResourceUtils.h"
#include "GameDefines.h"
#include "utils.h"

using namespace utils;

Tilemap::Tilemap(const Point2f& size, int tileSize, const std::string resources[], int resourcesSize)
  : m_Size(size), m_TileSize(tileSize)
{
  // Seed the rng, source is from chatgpt. it works so I won't question it too much
  std::seed_seq seed{ std::random_device{}(), std::random_device{}() };
  m_Rng.seed(seed);

  SetTiles(resources, resourcesSize);
}

void Tilemap::Draw(bool debug) const
{
  for (const std::pair<std::pair<int, int>, int>& tileInfo : m_Tiles) {
    DrawSingleTile(tileInfo.first, tileInfo.second);
  }
}

void Tilemap::DrawSingleTile(Point2f position, int tileId, int x, int y) const
{
  // Calculate rects
  const Rectf dstRect{
    position.x,
    position.y,
    m_TileSize * m_Size.x,
    m_TileSize * m_Size.y
  };

  // Draw a tile with the calculated sourcerect
  m_TileTexturePtrs[tileId]->Draw(dstRect, GetSourceRect(x, y));
}

// To draw a tile, we check the neighbouring tiles to draw the corerct one
void Tilemap::DrawSingleTile(std::pair<int, int> tile, int tileId) const
{
  const Point2f position{ KeyToPoint(tile) };
  DrawSingleTile(position, tileId, tile.first, tile.second);
}

bool Tilemap::IsTile(const Rectf& rect) const
{
  for (const std::pair<std::pair<int, int>, int>& tile : m_Tiles) {
    const Rectf tileRect{ GetTileRect(KeyToPoint({ tile.first.first, tile.first.second })) };

    if (IsOverlapping(rect, tileRect)) {
      return true;
    }
  }

  return false; // No collision detected
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

  const std::pair<int, int> key{ PointToKey(point) };
  m_Tiles.insert_or_assign(key, tileID);
  m_RngMap.insert_or_assign(key, std::abs((int)m_Rng())); // Random shuffling of the tiles
}

void Tilemap::RemoveTile(const Point2f& point)
{
  // Remove the tile if it exists
  if (IsTile(point)) {
    const std::pair<int, int> key{ PointToKey(point) };
    m_Tiles.erase(key);
    m_RngMap.erase(key);
  }
}

void Tilemap::SetTiles(const std::string resources[], int resourcesSize)
{
  // Clear the exising textures, they don't have to be deleted because the manager takes care of that
  m_TileTexturePtrs.clear();

  // For each resource, we load in the texture
  for (int index = 0; index < resourcesSize; ++index)
  {
    // Grabs the tile from the tilemap folder
    const Texture* texturePtr{ TextureManager::Instance()->GetTexture(ResourceUtils::ResourceToTilemapPath(resources[index]))};

    // The texture is invalid if it is not divisible by the size of each tile
    if ((int)texturePtr->GetWidth() % m_TileSize != 0 || (int)texturePtr->GetHeight() % m_TileSize != 0) {
      std::cerr << "Tilemap texture '" << resources[index] << "' is non-divisible by " << m_TileSize << std::endl;
      exit(-1);
    }

    m_TileTexturePtrs.push_back(texturePtr);
  }
}

void Tilemap::LoadRawTileData(const std::vector<int>& rawTileData)
{
  std::cout << "Loading raw tilemap data for tilemap" << std::endl;

  if (rawTileData.size() % 3 != 0) {
    std::cerr << "Tilemap data is not divisible by 3, concluded that data must be corrupt" << std::endl;
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
    m_RngMap.insert(std::make_pair(std::make_pair(x, y), std::abs((int)m_Rng()))); // Insert a random value into the map, for performance reasons, to keep it consistent
  }
}

int Tilemap::GetTileCount() const
{
  return m_TileTexturePtrs.size();
}

int Tilemap::GetTileSize() const
{
  return m_TileSize;
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
  std::cout << "Fetching raw tilemap data " << std::endl;

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

Rectf Tilemap::GetSourceRect(int x, int y) const
{
  // Check all the sides
  const bool topTile{ !IsTile(x, y + 1) };
  const bool leftTile{ !IsTile(x - 1, y) };
  const bool rightTile{ !IsTile(x + 1, y) };
  const bool bottomTile{ !IsTile(x, y - 1) };

  // Convert adjacent tile states to a unique number
  int tileNumber = 0;
  if (topTile) tileNumber |= 1 << 0;
  if (bottomTile) tileNumber |= 1 << 1;
  if (leftTile) tileNumber |= 1 << 2;
  if (rightTile) tileNumber |= 1 << 3;

  // Adjust tile number based on Celeste's tileset format
  switch (tileNumber) {
  case 0b0001:
    tileNumber = 0;
    break;
  case 0b0010:
    tileNumber = 1;
    break;
  case 0b0100:
    tileNumber = 2;
    break;
  case 0b1000:
    tileNumber = 3;
    break;
  case 0b0011:
    tileNumber = 4;
    break;
  case 0b1100:
    tileNumber = 5;
    break;
  case 0b1101:
    tileNumber = 6;
    break;
  case 0b1110:
    tileNumber = 7;
    break;
  case 0b0111:
    tileNumber = 8;
    break;
  case 0b1011:
    tileNumber = 9;
    break;
  case 0b1111:
    tileNumber = 10;
    break;
  case 0b0101:
    tileNumber = 11;
    break;
  case 0b1001:
    tileNumber = 12;
    break;
  case 0b0110:
    tileNumber = 13;
    break;
  case 0b1010:
    tileNumber = 14;
    break;
  case 0b0000:
    tileNumber = 15;
    break;
  }

  int rng = 0; // Initialize rng with some default value
  std::unordered_map<std::pair<int, int>, int>::const_iterator it = m_RngMap.find(std::make_pair(x, y)); // Find the random value for (x, y)
  if (it != m_RngMap.end()) {
    rng = it->second; // Assign the random value if found
  }

  // Calculate the source rectangle for the tile
  Rectf sourceRect{
      tileNumber == 15 ? (TILE_COLUMN_SIZE - 1) * m_TileSize : float((rng % 4) * m_TileSize),
      tileNumber == 15 ? float((rng % 12) * m_TileSize) : float(m_TileSize * tileNumber),
      float(m_TileSize),               // width
      float(m_TileSize)                // height
  };

  return sourceRect;
}