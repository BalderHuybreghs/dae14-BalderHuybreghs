#include "pch.h"
#include "Tilemap.h"
#include "TextureManager.h"
#include <iostream>

Tilemap::Tilemap(const std::string& resource, const Point2f& size, int tileSize)
  : m_Size(size), m_TileSize(tileSize)
{
  SetResource(resource);
}

void Tilemap::Draw(bool debug) const
{
  for (const auto& tileInfo : m_Tiles) {
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
    tileId % gridWidth,
    tileId / gridWidth,
    m_TileSize,
    m_TileSize
  };

  m_TileTexturePtr->Draw(dstRect, srcRect);
}

bool Tilemap::IsTile(const Point2f& point) const
{
  // Search for the coordinates in the map
  auto tile{ m_Tiles.find(PointToKey(point)) };

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

void Tilemap::SetResource(const std::string& resource)
{
  m_TileTexturePtr = TextureManager::Instance()->GetTexture(resource);

  // The texture is invalid if it is not divisible by the size of each tile
  if ((int)m_TileTexturePtr->GetWidth() % m_TileSize != 0 || (int)m_TileTexturePtr->GetHeight() % m_TileSize != 0) {
    std::cerr << "Tilemap texture is non-divisible by: " << m_TileSize << std::endl;
    std::exit(-1);
  }
}

void Tilemap::LoadRawTileData(const std::vector<int>& rawTileData)
{
  if (rawTileData.size() % 2 != 0) {
    std::cerr << "Tile data is not even, concluded that data must be corrupt" << std::endl;
    return;
  }
  // Create key-value pairs and insert into hashmap
  for (size_t index = 0; index < rawTileData.size(); index += 2) {
    int key = rawTileData[index];
    int value = rawTileData[index + 1];

    if (value > GetTileCount()) {
      std::cerr << "Tile exceeds texture at index (skipping): " << index << std::endl;
      continue;
    }

    m_Tiles.insert(std::make_pair(key, value));
  }
}

int Tilemap::GetTileCount() const
{
  return int(m_TileTexturePtr->GetWidth() / m_TileSize) * int(m_TileTexturePtr->GetHeight() / m_TileSize);
}

std::vector<int> Tilemap::ToRawTileData() const
{
  std::vector<int> rawTileData;
  rawTileData.reserve(m_Tiles.size() * 2); // Reserve space for keys and values

  for (const auto& pair : m_Tiles) {
    rawTileData.push_back(pair.first);   // Push key
    rawTileData.push_back(pair.second);  // Push value
  }

  return rawTileData;
}

int Tilemap::PointToKey(const Point2f& point) const
{
  return 
    int(std::round(point.x / (m_TileSize * m_Size.x))) * 
    int(std::round(point.y / (m_TileSize * m_Size.y)));
}

Point2f Tilemap::KeyToPoint(int key) const
{
  return Point2f{
    (key / m_Size.x)* m_TileSize* m_Size.x,
    (key % int(m_Size.x))* m_TileSize* m_Size.y
  };
}