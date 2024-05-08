#pragma once
#include "GameObject.h"

class CrumbleBlock final : public GameObject
{
public:
  enum class State
  {
    Stable,
    Unstable,
    Gone // It's just gone
  };

  CrumbleBlock(const Point2f& position, int size, float crumbleTime, const std::string& resource);
  CrumbleBlock(const CrumbleBlock& other);

  void Draw(const Point2f& position, bool debug) const override;
  void Draw(bool debug) const override;
  void Update(Player& player, Camera& camera, float elapsedSec) override;

  GameObject* Clone() const override;

  int GetSize() const;
  float GetCrumbleTime() const;
  std::string GetResource() const;
private:
  const std::string m_Resource;
  const Texture* m_Texture; // The texture that will be used
  const Texture* m_OutlineTexture; // The outline texture of the block

  State m_State;

  int m_Size;               // The size of the cumbleblock
  float m_CrumbleTime; // The time it takes for the block to crumble
  float m_Time;        // The general timer variable used to detect when the block should fall appart
};

