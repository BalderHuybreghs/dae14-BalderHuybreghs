#include "pch.h"
#include "Tilemap.h"
#include "TextureManager.h"
#include <iostream>
#include "ResourceUtils.h"

Tilemap::Tilemap(const std::string& resource, const Point2f& size, int tileSize)
  : m_Size(size), m_TileSize(tileSize), m_Resource(resource)
{
  SetResource(resource);
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
  // Search for the coordinates in the map
  // No auto :(
  std::_List_const_iterator<std::_List_val<std::_List_simple_types<std::pair<const std::pair<int, int>, int>>>> tile{ m_Tiles.find(PointToKey(point)) };

  // Return if the given tile is occupied
  return tile != m_Tiles.end();
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

void Tilemap::SetResource(const std::string& resource)
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
    key.first * m_TileSize* m_Size.x,
    key.second * m_TileSize* m_Size.y
  };
}