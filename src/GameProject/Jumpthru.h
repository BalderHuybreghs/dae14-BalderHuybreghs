#pragma once
#include "GameObject.h"

class Jumpthru : public GameObject
{
public:
  // Jumpthroughs can have multiple connected sides
  enum class ConnectedSides
  {
    TopBoth,
    TopLeft,
    TopRight,
    BottomLeft,
    BottomRight,
    BottomBoth
  };

  Jumpthru(const Point2f& position, int size, ConnectedSides connectedSides, const std::string& resource);
  Jumpthru(const Jumpthru& other);


  void Draw(const Point2f& position, bool debug) const override;

  void Draw(bool debug) const override;

  void Update(Player& player, Camera& camera, float elapsedSec) override;

  void SetPosition(const Point2f& position) override;

  GameObject* Clone() const override;

  std::string GetResource() const;
  ConnectedSides GetConnectedSides() const;
  int GetSize() const;
private:
  bool HandleCollision(Player& player) const;

  const std::string m_Resource;
  const Texture* m_Texture;

  ConnectedSides m_ConnectedSides;
  int m_Size;

  Rectf m_CollisionRect;
};

