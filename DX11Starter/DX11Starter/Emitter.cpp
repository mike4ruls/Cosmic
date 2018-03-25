#include "Emitter.h"
#include "EngineManager.h"


Emitter::Emitter()
{
}

Emitter::Emitter(int maxP, ID3D11ShaderResourceView* text, BlendingType type, EmitterType emit)
{
	EngineManager* manager = EngineManager::GetInstance();

	transform = Transform();

	maxParticles = maxP;
	particles = new Particle[maxP];
	particlesVerts = new ParticleVertex[maxP * 4];

	particleMat.LoadSurfaceTexture(text);
	blendType = type;
	emitterType = emit;

	startColor = {1.0f, 1.0f, 1.0f, 1.0f};
	endColor = { 1.0f, 1.0f, 1.0f, 0.0f };
	accelerationDir = transform.foward;
	emitterAcceleration = 1.0f;
	startSize = 1.0f;
	endSize = 0.1f;

	startRadius = 0.0f;
	endRadius = 1.0f;

	cylinderRad = 1.0f;

	sphereRad = 1.0f;

	lifeTime = 1.0f;
	timeSinceEmit = 0.0f;
	emissionRate = 0.1f;
	oldestParticlePos = 0;
	newestParticlePos = 0;
	particleCount = 0;
	isLooping = true;
	isActive = true;
	stopEmitting = false;
	pauseEmitter = false;
	//localSpace = true;
	localSpace = false;

	volatile int i = 0;
	for ( ; i < maxParticles; i++)
	{
		particles[i] = Particle();
		particles[i].isDead = false;

		particlesVerts[(i * 4) + 0] = ParticleVertex({ -1.0f, 1.0f, 0.0f }, { 0.0f,0.0f }, { 1.0f,1.0f,1.0f,1.0f });
		particlesVerts[(i * 4) + 1] = ParticleVertex({ 1.0f, 1.0f, 0.0f }, { 1.0f,0.0f }, { 1.0f,1.0f,1.0f,1.0f });
		particlesVerts[(i * 4) + 2] = ParticleVertex({ -1.0f, -1.0f, 0.0f }, { 1.0f,1.0f }, { 1.0f,1.0f,1.0f,1.0f });
		particlesVerts[(i * 4) + 3] = ParticleVertex({ 1.0f, -1.0f, 0.0f }, { 0.0f,1.0f }, { 1.0f,1.0f,1.0f,1.0f });

		/*particlesVerts[(i * 4) + 0] = ParticleVertex({ -1.0f, 1.0f, 0.0f }, { 0.0f,0.0f }, { 1.0f,1.0f,1.0f,1.0f });
		particlesVerts[(i * 4) + 1] = ParticleVertex({ 1.0f, 1.0f, 0.0f }, { 1.0f,0.0f }, { 1.0f,1.0f,1.0f,1.0f });
		particlesVerts[(i * 4) + 2] = ParticleVertex({ -1.0f, -1.0f, 0.0f }, { 0.0f,1.0f }, { 1.0f,1.0f,1.0f,1.0f });
		particlesVerts[(i * 4) + 3] = ParticleVertex({ 1.0f, -1.0f, 0.0f }, { 1.0f,1.0f }, { 1.0f,1.0f,1.0f,1.0f });*/

	}

	InitBuffers(manager->GetDevice());
	manager->RegistarEmitter(*this);
}


Emitter::Emitter(int maxP, ID3D11ShaderResourceView * text)
{
	EngineManager* manager = EngineManager::GetInstance();

	transform = Transform();

	maxParticles = maxP;
	particles = new Particle[maxP];
	particlesVerts = new ParticleVertex[maxP * 4];

	particleMat.LoadSurfaceTexture(text);
	blendType = BlendingType::CutOut;
	emitterType = EmitterType::Cone;

	startColor = { 1.0f, 1.0f, 1.0f, 1.0f };
	endColor = { 1.0f, 1.0f, 1.0f, 0.0f };
	accelerationDir = transform.foward;
	emitterAcceleration = 1.0f;
	startSize = 1.0f;
	endSize = 0.1f;

	startRadius = 0.0f;
	endRadius = 1.0f;

	cylinderRad = 1.0f;

	sphereRad = 1.0f;

	lifeTime = 1.0f;
	timeSinceEmit = 0.0f;
	emissionRate = 0.1f;
	oldestParticlePos = 0;
	newestParticlePos = 0;
	particleCount = 0;
	isLooping = true;
	isActive = true;
	stopEmitting = false;
	pauseEmitter = false;
	//localSpace = true;
	localSpace = false;

	volatile int i = 0;
	for (; i < maxParticles; i++)
	{
		particles[i] = Particle();
		particles[i].isDead = false;

		particlesVerts[(i * 4) + 0] = ParticleVertex({ -1.0f, 1.0f, 0.0f }, { 0.0f,0.0f }, { 1.0f,1.0f,1.0f,1.0f });
		particlesVerts[(i * 4) + 1] = ParticleVertex({ 1.0f, 1.0f, 0.0f }, { 1.0f,0.0f }, { 1.0f,1.0f,1.0f,1.0f });
		particlesVerts[(i * 4) + 2] = ParticleVertex({ -1.0f, -1.0f, 0.0f }, { 1.0f,1.0f }, { 1.0f,1.0f,1.0f,1.0f });
		particlesVerts[(i * 4) + 3] = ParticleVertex({ 1.0f, -1.0f, 0.0f }, { 0.0f,1.0f }, { 1.0f,1.0f,1.0f,1.0f });

		/*particlesVerts[(i * 4) + 0] = ParticleVertex({ -1.0f, 1.0f, 0.0f }, { 0.0f,0.0f }, { 1.0f,1.0f,1.0f,1.0f });
		particlesVerts[(i * 4) + 1] = ParticleVertex({ 1.0f, 1.0f, 0.0f }, { 1.0f,0.0f }, { 1.0f,1.0f,1.0f,1.0f });
		particlesVerts[(i * 4) + 2] = ParticleVertex({ -1.0f, -1.0f, 0.0f }, { 0.0f,1.0f }, { 1.0f,1.0f,1.0f,1.0f });
		particlesVerts[(i * 4) + 3] = ParticleVertex({ 1.0f, -1.0f, 0.0f }, { 1.0f,1.0f }, { 1.0f,1.0f,1.0f,1.0f });*/

	}

	InitBuffers(manager->GetDevice());
	manager->RegistarEmitter(*this);
}

Emitter::~Emitter()
{
	if (particles != nullptr) { delete[] particles; particles = nullptr; }
	if (particlesVerts != nullptr) { delete[] particlesVerts; particlesVerts = nullptr; }

	emitterBuffer->Release();
	indBuffer->Release();
}

Emitter * Emitter::CreateSnowEmitter(ID3D11ShaderResourceView * text)
{
	Emitter* newEmitter = new Emitter(2000, text, Emitter::BlendingType::CutOut, Emitter::EmitterType::Cone);

	//newEmitter->startColor = { 1.0f, 0.0f, 0.0f, 1.0f };
	newEmitter->endColor = { 1.0f, 1.0f, 1.0f, 0.8f };
	newEmitter->emitterAcceleration = 10.0f;
	newEmitter->transform.Rotate(0.0f, 90.0f, 0.0f);
	newEmitter->accelerationDir = newEmitter->transform.foward;
	newEmitter->emissionRate = 0.005f;
	newEmitter->lifeTime = 10.0f;
	newEmitter->startRadius = 50.0f;
	newEmitter->endRadius = 60.0f;
	newEmitter->cylinderRad = 200.0f;
	//newEmitter->localSpace = true;
	newEmitter->startSize = 1.5f;
	newEmitter->endSize = 1.5f;

	return newEmitter;
}

Emitter * Emitter::CreateExplosionEmitter(ID3D11ShaderResourceView * text)
{
	Emitter* newEmitter = new Emitter(10, text, Emitter::BlendingType::CutOut, Emitter::EmitterType::Sphere);

	newEmitter->emitterAcceleration = 5.0f;
	newEmitter->emissionRate = 0.0f;
	newEmitter->lifeTime = 2.0f;
	newEmitter->sphereRad = 1.0f;
	newEmitter->startSize = 1.0f;
	newEmitter->endSize = 20.0f;
	newEmitter->isLooping = false;

	return newEmitter;
}

void Emitter::InitBuffers(ID3D11Device* device)
{
	//D3D11_BUFFER_DESC vbd = {};
	//vbd.Usage = D3D11_USAGE_DYNAMIC;
	//vbd.ByteWidth = sizeof(ParticleVertex) * (maxParticles*4);       // 3 = number of vertices in the buffer
	//vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER; // Tells DirectX this is a vertex buffer
	//vbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	//vbd.Usage = D3D11_USAGE_DYNAMIC;
	//vbd.MiscFlags = 0;
	//vbd.StructureByteStride = 0;

	D3D11_BUFFER_DESC vbDesc = {};
	vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vbDesc.Usage = D3D11_USAGE_DYNAMIC;
	vbDesc.ByteWidth = sizeof(ParticleVertex) * 4 * maxParticles;


	D3D11_SUBRESOURCE_DATA initialVertexData;
	initialVertexData.pSysMem = particlesVerts;


	device->CreateBuffer(&vbDesc, 0, &emitterBuffer);

	//int iArr[6] = {2,0,1,2,1,3};
	unsigned int* indices = new unsigned int[maxParticles * 6];
	int indexCount = 0;
	for (int i = 0; i < maxParticles * 4; i += 4)
	{
		indices[indexCount++] = i;
		indices[indexCount++] = i + 1;
		indices[indexCount++] = i + 2;
		indices[indexCount++] = i;
		indices[indexCount++] = i + 2;
		indices[indexCount++] = i + 3;
	}

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_DEFAULT;
	ibd.ByteWidth = sizeof(unsigned int) * maxParticles * 6;         // 3 = number of indices in the buffer
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER; // Tells DirectX this is an index buffer
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;

	// Create the proper struct to hold the initial index data
	// - This is how we put the initial data into the buffer
	D3D11_SUBRESOURCE_DATA initialIndexData;
	initialIndexData.pSysMem = indices;

	// Actually create the buffer with the initial data
	// - Once we do this, we'll NEVER CHANGE THE BUFFER AGAIN
	device->CreateBuffer(&ibd, &initialIndexData, &indBuffer);

	delete[] indices;
}

void Emitter::Update(float dt)
{
	if(isActive)
	{
		// Update all particles - Check cyclic buffer first
		if (oldestParticlePos < newestParticlePos)
		{
			// First alive is BEFORE first dead, so the "living" particles are contiguous
			// 
			// 0 -------- FIRST ALIVE ----------- FIRST DEAD -------- MAX
			// |    dead    |            alive       |         dead    |

			// First alive is before first dead, so no wrapping
			for (int i = oldestParticlePos; i < newestParticlePos; i++)
				UpdateParticle(dt, i);
		}
		else
		{
			// First alive is AFTER first dead, so the "living" particles wrap around
			// 
			// 0 -------- FIRST DEAD ----------- FIRST ALIVE -------- MAX
			// |    alive    |            dead       |         alive   |

			// Update first half (from firstAlive to max particles)
			for (int i = oldestParticlePos; i < maxParticles; i++)
				UpdateParticle(dt, i);

			if(particles[maxParticles - 1].isDead && stopEmitting)
			{
				isActive = false;
			}

			// Update second half (from 0 to first dead)
			for (int i = 0; i < newestParticlePos; i++)
				UpdateParticle(dt, i);
		}

		// Add to the time
		timeSinceEmit += dt;

		// Enough time to emit?
		if (timeSinceEmit > emissionRate && !stopEmitting)
		{
			SpawnParticle();
			timeSinceEmit = 0.0f;
		}
	}	
}

void Emitter::SpawnParticle()
{
	// Any left to spawn?
	if (particleCount == maxParticles)
		return;

	// Reset the first dead particle
	particles[newestParticlePos].age = 0;
	particles[newestParticlePos].lifeTime = lifeTime;
	particles[newestParticlePos].isDead = false;
	particles[newestParticlePos].size = startSize;
	particles[newestParticlePos].color = startColor;
	particles[newestParticlePos].accelDir = accelerationDir;
	switch (emitterType)
	{
	case EmitterType::Sphere:
		particles[newestParticlePos].age = lifeTime/5.0f;
		particles[newestParticlePos].originalPos = transform.position;
		particles[newestParticlePos].velocity = {0.0f, 0.0f, 0.0f};
		particles[newestParticlePos].velocity.x += (((((float)rand() * 2) / RAND_MAX) - 1) * sphereRad);
		particles[newestParticlePos].velocity.y += (((((float)rand() * 2) / RAND_MAX) - 1) * sphereRad);
		particles[newestParticlePos].velocity.z += (((((float)rand() * 2) / RAND_MAX) - 1) * sphereRad); //((float)rand() / RAND_MAX) * 0.4f - 0.2f;

		break;
	case EmitterType::Cylinder:
		particles[newestParticlePos].originalPos = transform.position;
		particles[newestParticlePos].originalPos.x += (((((float)rand() * 2) / RAND_MAX) - 1)* (transform.right.x + transform.up.x)) * cylinderRad;
		particles[newestParticlePos].originalPos.y += (((((float)rand() * 2) / RAND_MAX) - 1)* (transform.right.y + transform.up.y)) * cylinderRad;
		particles[newestParticlePos].originalPos.z += (((((float)rand() * 2) / RAND_MAX) - 1)* (transform.right.z + transform.up.z)) * cylinderRad;
		particles[newestParticlePos].velocity = transform.foward;
		break;
	case EmitterType::Cone:
		particles[newestParticlePos].originalPos = transform.position;
		particles[newestParticlePos].originalPos.x += (((((float)rand() * 2) / RAND_MAX) - 1)* (transform.right.x + transform.up.x)) * startRadius;
		particles[newestParticlePos].originalPos.y += (((((float)rand() * 2) / RAND_MAX) - 1)* (transform.right.y + transform.up.y)) * startRadius;
		particles[newestParticlePos].originalPos.z += (((((float)rand() * 2) / RAND_MAX) - 1)* (transform.right.z + transform.up.z)) * startRadius; //((float)rand() / RAND_MAX) * 0.4f - 0.2f;

		particles[newestParticlePos].velocity = transform.foward;

		particles[newestParticlePos].velocity.x += (((((float)rand() * 2) / RAND_MAX) - 1)* (transform.right.x + transform.up.x)) * endRadius;
		particles[newestParticlePos].velocity.y += (((((float)rand() * 2) / RAND_MAX) - 1)* (transform.right.y + transform.up.y)) * endRadius;
		particles[newestParticlePos].velocity.z += (((((float)rand() * 2) / RAND_MAX) - 1)* (transform.right.z + transform.up.z)) * endRadius; //((float)rand() / RAND_MAX) * 0.4f - 0.2f;

		break;
	}
	particles[newestParticlePos].startingPos = particles[newestParticlePos].originalPos;
	// Increment and wrap
	newestParticlePos++;
	if (!isLooping && newestParticlePos >= maxParticles)
	{
		stopEmitting = true;
	}

	newestParticlePos %= maxParticles;

	particleCount++;
}

void Emitter::UpdateParticle(float dt, int pos)
{
	// Check for valid particle age before doing anything
	if (particles[pos].age >= particles[pos].lifeTime)
		return;

	// Update and check for death
	particles[pos].age += dt;
	if (particles[pos].age >= particles[pos].lifeTime)
	{
		// Recent death, so retire by moving alive count
		oldestParticlePos++;
		oldestParticlePos %= maxParticles;
		particleCount--;
		particles[pos].isDead = true;
		return;
	}

	// Calculate age percentage for lerp
	float agePercent = particles[pos].age / particles[pos].lifeTime;

	// Interpolate the color
	DirectX::XMStoreFloat4(
		&particles[pos].color,
		DirectX::XMVectorLerp(
			XMLoadFloat4(&startColor),
			XMLoadFloat4(&endColor),
			agePercent));

	// Lerp size
	particles[pos].size = startSize + agePercent * (endSize - startSize);

	DirectX::XMFLOAT3 xmaccel;

	if (emitterType == EmitterType::Sphere)
	{
		xmaccel = { particles[pos].velocity.x * emitterAcceleration, particles[pos].velocity.y * emitterAcceleration ,particles[pos].velocity.z * emitterAcceleration };
	}
	else
	{
		//xmaccel = { accelerationDir.x * emitterAcceleration, accelerationDir.y * emitterAcceleration ,accelerationDir.z * emitterAcceleration };
		xmaccel = { particles[pos].accelDir.x * emitterAcceleration, particles[pos].accelDir.y * emitterAcceleration , particles[pos].accelDir.z * emitterAcceleration };
	}
	DirectX::XMVECTOR startPos;
	// Adjust the position
	if (localSpace) {
		startPos = DirectX::XMLoadFloat3(&transform.position);
	}
	else {
		startPos = DirectX::XMLoadFloat3(&particles[pos].originalPos);
	}

	DirectX::XMVECTOR startVel = DirectX::XMLoadFloat3(&particles[pos].velocity);
	DirectX::XMVECTOR accel = DirectX::XMLoadFloat3(&xmaccel);
	float t = particles[pos].age;

	// Use constant acceleration function
	DirectX::XMStoreFloat3(
		&particles[pos].startingPos,
		DirectX::XMVectorAdd( DirectX::operator/(DirectX::operator*((t * t), accel), 2.0f), DirectX::XMVectorAdd(DirectX::operator*(t, startVel), startPos)));
		//accel * t * t / 2.0f + startVel * t + startPos);
}

void Emitter::CopyParticle(int pos)
{
	int i = pos * 4;

	particlesVerts[i + 0].pos = particles[pos].startingPos;
	particlesVerts[i + 1].pos = particles[pos].startingPos;
	particlesVerts[i + 2].pos = particles[pos].startingPos;
	particlesVerts[i + 3].pos = particles[pos].startingPos;

	particlesVerts[i + 0].size = particles[pos].size;
	particlesVerts[i + 1].size = particles[pos].size;
	particlesVerts[i + 2].size = particles[pos].size;
	particlesVerts[i + 3].size = particles[pos].size;

	particlesVerts[i + 0].color = particles[pos].color;
	particlesVerts[i + 1].color = particles[pos].color;
	particlesVerts[i + 2].color = particles[pos].color;
	particlesVerts[i + 3].color = particles[pos].color;
}

void Emitter::LoadParticlesForGPU(ID3D11DeviceContext* context)
{
	if (oldestParticlePos < newestParticlePos)
	{
		for (int i = oldestParticlePos; i < newestParticlePos; i++)
			CopyParticle(i);
	}
	else
	{
		// Update first half (from firstAlive to max particles)
		for (int i = oldestParticlePos; i < maxParticles; i++)
			CopyParticle(i);

		// Update second half (from 0 to first dead)
		for (int i = 0; i < newestParticlePos; i++)
			CopyParticle(i);
	}

	D3D11_MAPPED_SUBRESOURCE mapped = {};
	context->Map(emitterBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);

	memcpy(mapped.pData, particlesVerts, sizeof(ParticleVertex) * 4 * maxParticles);

	context->Unmap(emitterBuffer, 0);
}

void Emitter::Reset()
{
	oldestParticlePos = 0;
	newestParticlePos = 0;
	timeSinceEmit = 0.0f;
	particleCount = 0;
	isActive = true;
	stopEmitting = false;
}

void Emitter::Destroy()
{
	EngineManager* manager = EngineManager::GetInstance();
	manager->DeleteEmitter(*this);
}

