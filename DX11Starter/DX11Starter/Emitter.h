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

	enum EmitterType {
		Sphere,
		Cylinder,
		Cone
	};

	Emitter();
	Emitter(int maxP, ID3D11ShaderResourceView* text, BlendingType type, EmitterType emit);
	Emitter(int maxP, ID3D11ShaderResourceView* text);
	~Emitter();

	static Emitter* CreateSnowEmitter(ID3D11ShaderResourceView* text);
	static Emitter* CreateExplosionEmitter(ID3D11ShaderResourceView* text);

	void InitBuffers(ID3D11Device* device);
	void Update(float dt);
	void SpawnParticle();
	void UpdateParticle(float dt, int pos);
	void CopyParticle(int pos);
	void LoadParticlesForGPU(ID3D11DeviceContext* context);
	void Reset();
	void Destroy();

	Transform transform;

	Particle* particles;
	ParticleVertex* particlesVerts;

	ID3D11Buffer* emitterBuffer;
	ID3D11Buffer* indBuffer;

	Material particleMat;
	BlendingType blendType;
	EmitterType emitterType;

	DirectX::XMFLOAT4 startColor;
	DirectX::XMFLOAT4 endColor;
	DirectX::XMFLOAT3 accelerationDir;
	float emitterAcceleration;
	float startSize;
	float endSize;

	float startRadius;
	float endRadius;

	float cylinderRad;

	float sphereRad;

	float lifeTime;
	float timeSinceEmit;
	float emissionRate;
	int maxParticles;
	int oldestParticlePos;
	int newestParticlePos;
	bool isLooping;
	bool isActive;
	bool stopEmitting;
	bool pauseEmitter;
	bool localSpace;	
	int particleCount;
	int engineID;
	int emitterID;
private:

};

