#pragma once
#include "Particle.h"
#include "GameObject.h"
#include <vector>
#include <queue>

// The particle emitter is essentially a manager for all
// particles, except for that it also creates these particles in a defined pattern
class ParticleEmitter : public GameObject
{
public:
  ParticleEmitter(const Point2f& position, int maxParticles, const Shape& emissionZone, const Vector2f& minVelocity, const Vector2f& maxVelocity, std::vector<Particle*> spawnParticles);
  ~ParticleEmitter();

  virtual void Draw(bool debug = false) const override;
  virtual void Update(float elapsedSec) override;

  void SetPosition(const Point2f& maxParticles);
  void SetMaxParticles(int maxParticles);
  void SetEmissionZone(const Shape& emissionZone);
  void SetMinVelocity(const Vector2f& minVelocity);
  void SetMaxVelocity(const Vector2f& maxVelocity);

  Point2f GetPosition() const;
  int GetMaxParticles() const;
  const Shape& GetEmissionZone() const;
  Vector2f GetMinVelocity() const;
  Vector2f GetMaxVelocity() const;
private:
  // SETTINGS
  Point2f position;
  
  // When the maximum amount of particles is reached, it will start destroying older particles
  int m_MaxParticles;

  // The zone where particles are spawned. Points are picked randomly from here
  // in a more professional / usual way, engines have spawning patterns.
  const Shape& m_EmissionZone;

  // The velocity range that particles may have
  Vector2f m_MinVelocity;
  Vector2f m_MaxVelocity;

  // The particles that the particle emitter may pick to spawn
  std::vector<Particle*> m_SpawnParticles;

  // INTERNAL MANAGEMENT
  // Particles are stored in a queue to be destroyed
  std::queue<Particle*> m_Particles;
};