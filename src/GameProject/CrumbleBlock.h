#pragma once
#include "GameObject.h"
#include "SoundEffect.h"

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
  ~CrumbleBlock();

  // Application of rule of 5
  CrumbleBlock(const CrumbleBlock&& other) = delete;
  CrumbleBlock& operator=(const CrumbleBlock& other) = delete;
  CrumbleBlock& operator=(const CrumbleBlock&& other) = delete;

  void Draw(const Point2f& position, bool debug) const override;
  void Draw(bool debug) const override;
  void Update(Player& player, Camera& camera, float elapsedSec) override;

  void SetPosition(const Point2f& position) override;

  GameObject* Clone() const override;

  int GetSize() const;
  float GetCrumbleTime() const;
  std::string GetResource() const;
private:
  // Handles simple collision with the player. The function returns if the player collided with the block
  bool HandleCollision(Player& player) const;

  const std::string m_Resource;
  const Texture* m_TexturePtr;        // The texture that will be used
  const Texture* m_OutlineTexturePtr; // The outline texture of the block

  State m_State;

  int m_Size;               // The size of the cumbleblock
  float m_CrumbleTime;      // The time it takes for the block to crumble
  float m_Time;             // The general timer variable used to detect when the block should fall appart

  // Collision related stuffs :)
  Point2f m_PlayerPosBeforeCollision; // The player position before collision

  // The collision rectangle only needs to be calculated upon the creation of an object
  Rectf m_CollisionRect;

  SoundEffect* m_BreakSoundEffectPtr;
};

