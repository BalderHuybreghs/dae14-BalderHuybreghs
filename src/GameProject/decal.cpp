#include "pch.h"
#include "Decal.h"

void Decal::Draw(bool debug) const
{
}

void Decal::Update(Player& player, Camera& camera, float elapsedSec)
{
}

void Decal::Draw(const Point2f& position, bool debug) const
{
}

GameObject* Decal::Clone() const
{
  return new Decal(*this);
}
