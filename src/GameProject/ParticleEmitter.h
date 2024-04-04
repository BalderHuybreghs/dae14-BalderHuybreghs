#pragma once
#include "Particle.h"
#include <vector>

// The particle emitter is essentially a manager for all
// particles, except for that it also creates these particles in a defined pattern
class ParticleEmitter
{
public:
  // Any pointer given with this class will be cleaned automatically when this class is destroyed
  ParticleEmitter(Shape* emissionZone, const Vector2f& minVelocity, const Vector2f& maxVelocity, const std::vector<Particle*>& spawnParticles, int maxParticles);
  ~ParticleEmitter();

  void Draw(bool debug = false) const;
  void Update(float elapsedSec);

  void SetPosition(const Point2f& position);
  void SetMaxParticles(int maxParticles);
  void SetEmissionZone(Shape* emissionZone); // WARNING: This deletes the old emission zone
  void SetMinVelocity(const Vector2f& minVelocity);
  void SetMaxVelocity(const Vector2f& maxVelocity);

  Point2f GetPosition() const;
  int GetMaxParticles() const;
  Vector2f GetMinVelocity() const;
  Vector2f GetMaxVelocity() const;
private:
  // When the maximum amount of particles is reached, it will start destroying older particles
  int m_MaxParticles;

  // The zone where particles are spawned. Points are picked randomly from here
  // in a more professional / usual way, engines have spawning patterns.
  Shape* m_EmissionZone;

  // The velocity range that particles may have
  Vector2f m_MinVelocity;
  Vector2f m_MaxVelocity;

  // The particles that the particle emitter may pick to spawn
  const std::vector<Particle*>& m_SpawnParticles;

  // INTERNAL MANAGEMENT
  // Particles are stored in a queue to be destroyed
  std::vector<Particle*> m_Particles;
};