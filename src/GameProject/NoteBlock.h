#pragma once
#include "GameObject.h"

class NoteBlock : public GameObject
{
public:
  enum class Variant
  {
    Blue,
    Pink
  };

  enum class State
  {
    Stable,
    Gone
  };

  NoteBlock(const Point2f& position, Variant variant, int width, int height);
  NoteBlock(const NoteBlock& other);

  // Rule of 5
  ~NoteBlock() = default;
  NoteBlock(const NoteBlock&& other) = delete;
  NoteBlock& operator=(const NoteBlock& other) = delete;
  NoteBlock& operator=(const NoteBlock&& other) = delete;

  void Draw(const Point2f& position, bool debug) const override;
  void Draw(bool debug) const override;
  void Update(Player& player, Camera& camera, float elapsedSec) override;

  GameObject* Clone() const override;

  void SetPosition(const Point2f& position) override;

  int GetWidth() const;
  int GetHeight() const;
  Variant GetVariant() const;

  Color4f GetColor() const;
private:
  bool HandleCollision(Player& player) const;
  void DrawMap(const Texture* texture, const Point2f& position) const;
  Rectf GetSrcRect(int x, int y) const;

  const Texture* m_TexturePtr;
  const Texture* m_TexturePressedPtr;

  int m_Width;
  int m_Height;
  Variant m_Variant;
  State m_State;

  Rectf m_CollisionRect;

  float m_Time;
};