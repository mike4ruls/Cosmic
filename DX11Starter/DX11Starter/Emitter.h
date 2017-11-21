#pragma once
#include "Particle.h"
#include "ParticleVertex.h"
#include "Material.h"
#include "Transform.h"
class Emitter
{
public:
	enum BlendingType {
		AlphaBlend,
		CutOut,
		Additive
	};

	Emitter();
	Emitter(int maxP, ID3D11ShaderResourceView* text, BlendingType type, ID3D11Device* device);
	~Emitter();

	Transform transform;

	void InitBuffers(ID3D11Device* device);
	void Update(float dt);
	void SpawnParticle();
	void UpdateParticle(float dt, int pos);
	void CopyParticle(int pos);
	void LoadParticlesForGPU(ID3D11DeviceContext* context);

	Particle* particles;
	ParticleVertex* particlesVerts;

	ID3D11Buffer* emitterBuffer;
	ID3D11Buffer* indBuffer;

	Material particleMat;
	BlendingType blendType;

	DirectX::XMFLOAT4 startColor;
	DirectX::XMFLOAT4 endColor;
	DirectX::XMFLOAT3 accelerationDir;
	float emitterAcceleration;
	float startSize;
	float endSize;

	float startRadius;
	float endRadius;

	float lifeTime;
	float timeSinceEmit;
	float emissionRate;
	int maxParticles;
	int oldestParticlePos;
	int newestParticlePos;
	bool isLooping;
	bool isActive;
	bool pauseEmitter;
	bool localSpace;	
	int particleCount;
	int engineID;
	int emitterID;
private:

};

