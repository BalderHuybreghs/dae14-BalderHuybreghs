#pragma once
#include "Texture.h"
#include <unordered_map>
#include <vector>

class Sprite
{
public:
  Sprite(const Point2f& size);
  Sprite(const Point2f& size, const std::string& resource);

  void Draw(const Point2f& position, bool debug = false) const;
  void Update(float elapsedTime);

  // Resources are states in this case, since resources will be added 
  // within code, states should remain relatively unchanged
  void SetResource(int state);

  // Adds a resource to this sprite and returns its state
  int AddResource(const std::string& resource);

  Point2f GetSize() const;
  void SetSize(const Point2f& size);

  struct StateInfo
  {
    const Texture* texture;
    int frames;
  };
protected:
  Point2f m_Size;

  // Internal management
  std::vector<StateInfo> m_States;
  StateInfo m_State;
  float m_Time;
  int m_Frame;
};