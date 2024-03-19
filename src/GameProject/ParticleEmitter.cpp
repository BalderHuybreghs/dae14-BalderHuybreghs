#include "pch.h"
#include "ParticleEmitter.h"

GameObject* ParticleEmitter::clone() const
{
  return new ParticleEmitter(*this);
}
