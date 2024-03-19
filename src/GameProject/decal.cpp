#include "pch.h"
#include "Decal.h"

void Decal::Draw(bool debug) const
{
}

void Decal::Update(Player& player, float elapsedSec)
{
}

void Decal::Draw(Point2f position, bool debug) const
{
}

GameObject* Decal::clone() const
{
  return new Decal(*this);
}
