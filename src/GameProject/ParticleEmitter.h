#pragma once
#include "Particle.h"
#include <vector>

// A struct designed to contain a bunch of information regarding particle spawning
struct EmitterSpawnInfo
{
  float minForce;     // The minimum force on a particle
  float maxForce;     // The maximum force on a particle
  float minRotation;  // The minimum rotation in radians
  float maxRotation;  // The maximum rotation in radians
  float minLifetime;  // The minimum lifetime for a particle in seconds
  float maxLifetime;  // The maximum lifetime for a particle in seconds
  int   minBatchSize; // The minimum amount of particles to spawn in a run
  int   maxBatchSize; // The maximum amount of particles to spawn in a run
  int   maxParticles; // The maximum amount of particles allowed at a time
  float minDelay;     // The minimum spawning delay in seconds
  float maxDelay;     // The maximum spawning delay in seconds
};

// The particle emitter is essentially a manager for all
// particles, except for that it also creates these particles in a defined pattern
class ParticleEmitter
{
public:
  // Any pointer given with this class will be cleaned automatically when this class is destroyed
  ParticleEmitter(Shape* emissionZone, const EmitterSpawnInfo& spawnInfo, const std::vector<Shape*> spawnShapes);

  // Disable copying of a particle emitter
  ParticleEmitter(const ParticleEmitter& other) = default;
  ParticleEmitter& operator=(const ParticleEmitter& other) = default;
  virtual ~ParticleEmitter();

  void Draw(bool debug = false) const;
  void Update(float elapsedSec);

  void SetPosition(const Point2f& position);
  void SetEmissionZone(Shape* emissionZone); // WARNING: This deletes the old emission zone
  void SetSpawnInfo(const EmitterSpawnInfo& spawnInfo);

  Point2f GetPosition() const;
  EmitterSpawnInfo GetSpawnInfo() const;
private:
  // The zone where particles are spawned. Points are picked randomly from here
  // in a more professional / usual way, engines have spawning patterns.
  Shape* m_EmissionZone;

  // The information for spawning new particles
  EmitterSpawnInfo m_SpawnInfo;

  // The particles that the particle emitter may pick to spawn
  const std::vector<Shape*> m_SpawnShapes;

  // INTERNAL MANAGEMENT
  // Particles are stored in a queue to be destroyed
  std::vector<Particle*> m_Particles;

  float m_Delay; // The delay until the next batch

  // Functions used by the emitter
  void UpdateParticles(float elapsedSec);

  Particle* CreateParticle(); // Creates a particle according to the criteria
  void SpawnBatch(); // Spawns a new batch of particles
};