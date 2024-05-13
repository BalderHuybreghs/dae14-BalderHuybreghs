#include "pch.h"
#include "ParticleEmitter.h"
#include <iostream>
#include "MathUtils.h" // This will be required for this lol

using namespace MathUtils;

ParticleEmitter::ParticleEmitter(Shape* emissionZone, const EmitterSpawnInfo& spawnInfo, std::vector<Shape*> spawnShapes)
  : m_EmissionZonePtr(emissionZone), m_SpawnShapePtrs(spawnShapes), m_SpawnInfo(spawnInfo)
{
  if (m_SpawnShapePtrs.empty()) {
    std::cout << "Particle Emitter spawn shapes vector is empty, cannot proceed with program" << std::endl;
    exit(-1);
  }

  m_Delay = RandFloat(m_SpawnInfo.minDelay, m_SpawnInfo.maxDelay, 2);
  m_ParticlePtrs.reserve(m_SpawnInfo.maxParticles); // Reserve the space for the maximum amount of particles 
}

ParticleEmitter::~ParticleEmitter()
{
  // Delete the emissionZone
  delete m_EmissionZonePtr;
  m_EmissionZonePtr = nullptr;

  for (Shape* shape : m_SpawnShapePtrs) {
    delete shape;
    shape = nullptr;
  }

  // Delete all the particles that have been spawned
  for (Particle* particle : m_ParticlePtrs) {
    delete particle;
    particle = nullptr;
  }
}

void ParticleEmitter::Draw(bool debug) const
{
  for (const Particle* particle : m_ParticlePtrs) {
    particle->Draw(debug);
  }

  // Draw some debug information about the particle emitter
  if (debug) {
    m_EmissionZonePtr->Draw();
  }
}

void ParticleEmitter::Update(float elapsedSec)
{
  UpdateParticles(elapsedSec);

  if (m_Delay <= 0.f) {
    m_Delay = RandFloat(m_SpawnInfo.minDelay, m_SpawnInfo.maxDelay, 2);
    SpawnBatch();
  }

  m_Delay -= elapsedSec; // Change the delay
}

void ParticleEmitter::SetPosition(const Point2f& position)
{
  m_EmissionZonePtr->SetPosition(position);
}

void ParticleEmitter::SetEmissionZone(Shape* emissionZone)
{
  delete m_EmissionZonePtr;
  m_EmissionZonePtr = emissionZone;
}

void ParticleEmitter::SetSpawnInfo(const EmitterSpawnInfo& spawnInfo)
{
  m_SpawnInfo = spawnInfo;
}

Point2f ParticleEmitter::GetPosition() const
{
  return m_EmissionZonePtr->GetPosition();
}

EmitterSpawnInfo ParticleEmitter::GetSpawnInfo() const
{
  return m_SpawnInfo;
}

void ParticleEmitter::UpdateParticles(float elapsedSec)
{
  // Go through all the particles and update them
  for (std::vector<Particle*>::iterator it = m_ParticlePtrs.begin(); it != m_ParticlePtrs.end();) {
    Particle* particle = *it;

    // If the particle is dead, delete it and remove it from the vector
    if (particle->GetLifetime() <= 0.f) {
      delete particle;
      it = m_ParticlePtrs.erase(it);
    } else {
      // Update the particle if it's still alive
      particle->Update(elapsedSec);
      ++it;
    }
  }
}

Particle* ParticleEmitter::CreateParticle()
{
  Shape* shape{ m_SpawnShapePtrs.at( (size_t)RandInt(0, (int)m_SpawnShapePtrs.size() -1) )->Copy() };
  const float lifeTime{ RandFloat(m_SpawnInfo.minLifetime, m_SpawnInfo.maxLifetime, 2)};
  const Point2f position{ m_EmissionZonePtr->GetRandomPoint() };

  const float rotation{ RandFloat(m_SpawnInfo.minRotation, m_SpawnInfo.maxRotation, 5) }; // More decimals for rotation
  const float magnitude{ RandFloat(m_SpawnInfo.minForce, m_SpawnInfo.maxForce, 2) };

  const Vector2f velocity{
    cos(rotation) * magnitude,
    sin(rotation) * magnitude
  };

  return new Particle(position, velocity, shape, lifeTime);
}

void ParticleEmitter::SpawnBatch()
{
  // Create a batchsize of particles to spawn
  const int batchSize{ RandInt(m_SpawnInfo.minBatchSize, m_SpawnInfo.maxBatchSize) };
  const int currentSize = (int)m_ParticlePtrs.size();
  const int maxSize = m_SpawnInfo.maxParticles;

  // Cleanup particles if we are over the limit
  if (currentSize + batchSize > maxSize) {
    // Calculate the number of particles to remove
    const int particlesToRemove = currentSize + batchSize - maxSize;

    // Remove the oldest particles from the beginning of the vector
    for (int i = 0; i < particlesToRemove; ++i) {
      delete m_ParticlePtrs[i];
      m_ParticlePtrs[i] = nullptr;
    }

    m_ParticlePtrs.erase(m_ParticlePtrs.begin(), m_ParticlePtrs.begin() + particlesToRemove);
  }

  // Actually create the particles
  for (int i = 0; i < batchSize; ++i) {
    m_ParticlePtrs.push_back(CreateParticle());
  }
}