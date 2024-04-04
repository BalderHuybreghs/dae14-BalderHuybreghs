#include "pch.h"
#include "ParticleEmitter.h"
#include <iostream>

ParticleEmitter::ParticleEmitter(Shape* emissionZone, const Vector2f& minVelocity, const Vector2f& maxVelocity, const std::vector<Particle*>& spawnParticles, int maxParticles)
  : m_EmissionZone(emissionZone), m_MinVelocity(minVelocity), m_MaxVelocity(maxVelocity), m_SpawnParticles(spawnParticles), m_MaxParticles(maxParticles)
{
}

ParticleEmitter::~ParticleEmitter()
{
  // Delete the emissionZone
  delete m_EmissionZone;
  m_EmissionZone = nullptr;

  // Delete all the particles that have been spawned
  for (Particle* particle : m_Particles) {
    delete particle;
    particle = nullptr;
  }
  
  // Delete the spawn particle
  for (Particle* particle : m_SpawnParticles) {
    delete particle;
    particle = nullptr;
  }
}

void ParticleEmitter::Draw(bool debug) const
{
  for (const Particle* particle : m_Particles) {
    particle->Draw(debug);
  }

  // Draw some debug information about the particle emitter
  if (debug) {
    m_EmissionZone->Draw();
  }
}

void ParticleEmitter::Update(float elapsedSec)
{
}

void ParticleEmitter::SetPosition(const Point2f& position)
{
  m_EmissionZone->SetPosition(position);
}

void ParticleEmitter::SetMaxParticles(int maxParticles)
{
  if (maxParticles < 1) {
    std::cout << "Max particles invalid: " << maxParticles << std::endl;
  }

  m_MaxParticles = maxParticles;
}

void ParticleEmitter::SetEmissionZone(Shape* emissionZone)
{
  delete m_EmissionZone;
  m_EmissionZone = emissionZone;
}

void ParticleEmitter::SetMinVelocity(const Vector2f& minVelocity)
{
  m_MinVelocity = minVelocity;
}

void ParticleEmitter::SetMaxVelocity(const Vector2f& maxVelocity)
{
  m_MaxVelocity = maxVelocity;
}

Point2f ParticleEmitter::GetPosition() const
{
  return m_EmissionZone->GetPosition();
}

int ParticleEmitter::GetMaxParticles() const
{
  return m_MaxParticles;
}

Vector2f ParticleEmitter::GetMinVelocity() const
{
  return m_MinVelocity;
}

Vector2f ParticleEmitter::GetMaxVelocity() const
{
  return m_MaxVelocity;
}
