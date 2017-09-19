#include "Game.h"

// For the DirectX Math library
using namespace DirectX;

// --------------------------------------------------------
// Constructor
//
// DXCore (base class) constructor will set up underlying fields.
// DirectX itself, and our window, are not ready yet!
//
// hInstance - the application's OS-level handle (unique ID)
// --------------------------------------------------------
Game::Game(HINSTANCE hInstance)
	: DXCore(
		hInstance,		   // The application's handle
		"DirectX Game",	   // Text for the window's title bar
		1280,			   // Width of the window's client area
		720,			   // Height of the window's client area
		true)			   // Show extra stats (fps) in title bar?
{

#if defined(DEBUG) || defined(_DEBUG)
	// Do we want a console window?  Probably only in debug mode
	CreateConsoleWindow(500, 120, 32, 120);
	printf("Console window created successfully.  Feel free to printf() here.");
#endif
	
}

// --------------------------------------------------------
// Destructor - Clean up anything our game has created:
//  - Release all DirectX objects created here
//  - Delete any objects to prevent memory leaks
// --------------------------------------------------------
Game::~Game()
{
	if (cam) { delete cam; cam = nullptr; }
	if (rend) { delete rend; rend = nullptr; }
	if (gameObjects.size() != 0) {
		for (unsigned int i = 0; i < gameObjects.size(); i++) {
			if (gameObjects[i]) { delete gameObjects[i]; gameObjects[i] = nullptr; }
		}
	}
	//if (rend) { delete rend; rend = nullptr; }
	// Delete our simple shader objects, which
	// will clean up their own internal DirectX stuff
}

// --------------------------------------------------------
// Called once per program, after DirectX and the window
// are initialized but before the game loop.
// --------------------------------------------------------
void Game::Init()
{
	// Helper methods for loading shaders, creating some basic
	// geometry to draw and some simple camera matrices.
	//  - You'll be expanding and/or replacing these later
	cam = new Camera();
	cam->Init(width, height);
	rend = new Renderer(cam,device,context,backBufferRTV,depthStencilView);
	CreateBasicGeometry();
	dayTime = 0.0f;
	click = false;

	srand((unsigned int)time(NULL));

	// Creating game objects
	gameObjects.push_back(new GameEntity(rend->GetMesh("Triangle"), rend));
	gameObjects.push_back(new GameEntity(rend->GetMesh("Helix"), rend));
	gameObjects.push_back(new GameEntity(rend->GetMesh("Cube"), rend));
	gameObjects.push_back(new GameEntity(rend->GetMesh("Sphere"), rend));
	//gameObjects.push_back(new GameEntity(rend->GetMesh("RainbowRoad"), rend));

	gameObjects[2]->transform.Translate(0, 3, 4);
	gameObjects[3]->transform.Translate(0, -3, 1);

	gameObjects[2]->renderingComponent.mat.surfaceColor = {1.0f, 0.0f, 0.0f, 1.0f};
	gameObjects[1]->renderingComponent.mat.surfaceColor = { 0.0f, 1.0f, 0.0f, 1.0f };

	// Tell the input assembler stage of the pipeline what kind of
	// geometric primitives (points, lines or triangles) we want to draw.  
	// Essentially: "What kind of shape should the GPU draw with our data?"
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}


// --------------------------------------------------------
// Creates the geometry we're going to draw - a single triangle for now
// --------------------------------------------------------
void Game::CreateBasicGeometry()
{
	// Create some temporary variables to represent colors
	// - Not necessary, just makes things more readable
	XMFLOAT4 red = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	XMFLOAT4 green = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	XMFLOAT4 blue = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);

	XMFLOAT3 genNorm = XMFLOAT3(1.0f,0.0f,0.0f);
	XMFLOAT2 genUv = XMFLOAT2(1.0f, 1.0f);

	// Set up the vertices of the triangle we would like to draw
	// - We're going to copy this array, exactly as it exists in memory
	//    over to a DirectX-controlled data structure (the vertex buffer)
	Vertex vertices[] =
	{
		{ XMFLOAT3(+0.0f, +1.0f, +0.0f), genNorm, genUv, red },
		{ XMFLOAT3(+1.5f, -1.0f, +0.0f), genNorm, genUv, blue },
		{ XMFLOAT3(-1.5f, -1.0f, +0.0f), genNorm, genUv, green },
	};

	Vertex vertices2[] =
	{
		{ XMFLOAT3(-1.0f, -1.0f, +0.0f), genNorm, genUv, red },
		{ XMFLOAT3(-1.0f, 1.0f, +0.0f), genNorm, genUv, blue },
		{ XMFLOAT3(1.0f, 1.0f, +0.0f), genNorm, genUv, green },
		{ XMFLOAT3(1.0f, -1.0f, +0.0f), genNorm, genUv, green },
	};

	// Set up the indices, which tell us which vertices to use and in which order
	// - This is somewhat redundant for just 3 vertices (it's a simple example)
	// - Indices are technically not required if the vertices are in the buffer 
	//    in the correct order and each one will be used exactly once
	// - But just to see how it's done...
	int indices[] = { 0, 1, 2 };
	int indices2[] = { 0, 1, 2, 0, 2, 3 };

	unsigned int vSize = sizeof(vertices) / sizeof(vertices[0]);
	unsigned int iSize = sizeof(indices) / sizeof(indices[0]);

	unsigned int vSize2 = sizeof(vertices2) / sizeof(vertices2[0]);
	unsigned int iSize2 = sizeof(indices2) / sizeof(indices2[0]);

	rend->LoadMesh(new Mesh(vertices, indices, vSize, iSize, "Triangle", device));
	rend->LoadMesh(new Mesh(vertices2, indices2, vSize2, iSize2, "Square", device));
	rend->LoadMesh(new Mesh("Models/sphere.obj", "Sphere", device));
	rend->LoadMesh(new Mesh("Models/cube.obj", "Cube", device));
	rend->LoadMesh(new Mesh("Models/cone.obj", "Cone", device));
	rend->LoadMesh(new Mesh("Models/torus.obj", "Torus", device));
	rend->LoadMesh(new Mesh("Models/helix.obj", "Helix",device));
	rend->LoadMesh(new Mesh("Models/raygun.obj", "RayGun", device));
	rend->LoadMesh(new Mesh("Models/plane.obj", "Plane", device));
	rend->LoadMesh(new Mesh("Models/quad.obj", "Quad", device));
	rend->LoadMesh(new Mesh("Models/teapot.obj", "Teapot", device));
	rend->LoadMesh(new Mesh("Models/HaloSword.obj", "HaloSword", device));
	//rend->LoadMesh(new Mesh("Models/RainbowRoad.obj", "RainbowRoad", device));

}


// --------------------------------------------------------
// Handle resizing DirectX "stuff" to match the new window size.
// For instance, updating our projection matrix's aspect ratio.
// --------------------------------------------------------
void Game::OnResize()
{
	// Handle base-level DX resize stuff
	DXCore::OnResize();

	// Update our projection matrix since the window size changed
	XMMATRIX P = XMMatrixPerspectiveFovLH(
		0.25f * 3.1415926535f,	// Field of View Angle
		(float)width / height,	// Aspect ratio
		0.1f,				  	// Near clip plane distance
		100.0f);			  	// Far clip plane distance
	XMStoreFloat4x4(&cam->projectionMatrix, XMMatrixTranspose(P)); // Transpose for HLSL!
}

// --------------------------------------------------------
// Update your game here - user input, move objects, AI, etc.
// --------------------------------------------------------
void Game::Update(float deltaTime, float totalTime)
{
	/////////////////////////
	//GAME OBJECT MOVEMENT
	/////////////////////////
	if (GetAsyncKeyState(VK_LEFT))
	{
		//triangleObj->transform.Translate(-1.0f * deltaTime,0.0f,0.0f);
		gameObjects[0]->rigidBody.ApplyForce(-1.0f, 0.0f, 0.0f);
	}
	if (GetAsyncKeyState(VK_RIGHT))
	{
		//triangleObj->transform.Translate(1.0f * deltaTime, 0.0f, 0.0f);
		gameObjects[0]->rigidBody.ApplyForce(1.0f, 0.0f, 0.0f);
	}
	if (GetAsyncKeyState(VK_UP))
	{
		//triangleObj->transform.Translate(0.0f, 1.0f * deltaTime, 0.0f);
		gameObjects[0]->rigidBody.ApplyForce(0.0f, 1.0f, 0.0f);
	}
	if (GetAsyncKeyState(VK_DOWN))
	{
		//triangleObj->transform.Translate(0.0f, -1.0f * deltaTime, 0.0f);
		gameObjects[0]->rigidBody.ApplyForce(0.0f, -1.0f, 0.0f);
	}
	for (unsigned int i = 0; i < gameObjects.size(); i++) {
		gameObjects[i]->Update(deltaTime);
	}

	/////////////////////////
	//CAMERA MOVEMENT
	/////////////////////////
	if (GetAsyncKeyState(VK_LSHIFT)) //A
	{
		cam->camSpeed = cam->runSpeed;
	}
	else
	{
		cam->camSpeed = cam->normSpeed;
	}

	if (GetAsyncKeyState(65)) //A
	{
		cam->rigidBody.ApplyForce(cam->transform.right.x * -cam->camSpeed, cam->transform.right.y * -cam->camSpeed, cam->transform.right.z * -cam->camSpeed);
	}
	if (GetAsyncKeyState(68)) //D
	{
		cam->rigidBody.ApplyForce(cam->transform.right.x * cam->camSpeed, cam->transform.right.y * cam->camSpeed, cam->transform.right.z * cam->camSpeed);
	}
	if (GetAsyncKeyState(87)) //W
	{
		cam->rigidBody.ApplyForce(cam->transform.foward.x * cam->camSpeed, cam->transform.foward.y * cam->camSpeed, cam->transform.foward.z * cam->camSpeed);
	}
	if (GetAsyncKeyState(83)) //S
	{
		cam->rigidBody.ApplyForce(cam->transform.foward.x * -cam->camSpeed, -cam->transform.foward.y * -cam->camSpeed, cam->transform.foward.z * -cam->camSpeed);
	}
	if (GetAsyncKeyState(VK_SPACE))
	{
		cam->rigidBody.ApplyForce(0.0f , cam->camSpeed, 0.0f);
	}
	if (GetAsyncKeyState(88))
	{
		cam->rigidBody.ApplyForce(0.0f, -cam->camSpeed, 0.0f);
	}

	if (GetAsyncKeyState(VK_RETURN) || GetAsyncKeyState(69))
	{
		if (!enterPressed)
		{
			//"Triangle"
			//"Square"
			//"Sphere"
			//"Cube"
			//"Cone"
			//"Torus"
			//"Helix"
			//"RayGun"
			//"Plane"
			//"Quad"
			//"Teapot"
			//"HaloSword"

			float distInfront = 2.0f;
			DirectX::XMFLOAT3 spawnPoint = { cam->transform.position.x + (cam->transform.foward.x * distInfront), cam->transform.position.y + (cam->transform.foward.y * distInfront), cam->transform.position.z + (cam->transform.foward.z * distInfront) };
			SpawnGameObject("Sphere", spawnPoint, true);
			enterPressed = true;
		}
	}
	else
	{
		enterPressed = false;
	}
	if (GetAsyncKeyState(70))
	{
		if(!fPressed)
		{
			rend->ToggleWireFrame();
			fPressed = true;
		}
	}
	else
	{
		fPressed = false;
	}

	if (GetAsyncKeyState(97))
	{
		dayTime += 1.0f * deltaTime;
	}
	if (GetAsyncKeyState(99))
	{
		dayTime -= 1.0f * deltaTime;
	}


	/////////////////////////
	//UPDATES
	/////////////////////////
	for (unsigned int i = 0; i < gameObjects.size(); i++) 
	{
		gameObjects[i]->Update(deltaTime);
	}
	float scale = std::sin(((totalTime / 10)*180.0f) / (2.0f*3.14f));

	gameObjects[1]->transform.Rotate(0.0f, 0.0f, 1.0f * deltaTime);
	gameObjects[2]->transform.Rotate(1.0f * deltaTime, 1.0f * deltaTime, 1.0f * deltaTime);
	gameObjects[3]->transform.scale = { scale,scale,scale };

	rend->sunLight->lightDir = { sin(dayTime),cos(dayTime),0.0f };

	cam->Update(deltaTime);
	//printf("\nRight Vector - (%f, %f, %f)", cam->transform.right.x, cam->transform.right.y, cam->transform.right.z);
	// Quit if the escape key is pressed
	if (GetAsyncKeyState(VK_ESCAPE))
		Quit();
}

// --------------------------------------------------------
// Clear the screen, redraw everything, present to the user
// --------------------------------------------------------
void Game::Draw(float deltaTime, float totalTime)
{

	rend->Render(deltaTime);

	// Present the back buffer to the user
	//  - Puts the final frame we're drawing into the window so the user can see it
	//  - Do this exactly ONCE PER FRAME (always at the very end of the frame)
	swapChain->Present(0, 0);
}


#pragma region Mouse Input

// --------------------------------------------------------
// Helper method for mouse clicking.  We get this information
// from the OS-level messages anyway, so these helpers have
// been created to provide basic mouse input if you want it.
// --------------------------------------------------------
void Game::OnMouseDown(WPARAM buttonState, int x, int y)
{
	// Add any custom code here...
	click = true;
	// Save the previous mouse position, so we have it for the future
	prevMousePos.x = x;
	prevMousePos.y = y;

	// Caputure the mouse so we keep getting mouse move
	// events even if the mouse leaves the window.  we'll be
	// releasing the capture once a mouse button is released
	SetCapture(hWnd);
}

// --------------------------------------------------------
// Helper method for mouse release
// --------------------------------------------------------
void Game::OnMouseUp(WPARAM buttonState, int x, int y)
{
	// Add any custom code here...
	click = false;
	// We don't care about the tracking the cursor outside
	// the window anymore (we're not dragging if the mouse is up)
	ReleaseCapture();
}

// --------------------------------------------------------
// Helper method for mouse movement.  We only get this message
// if the mouse is currently over the window, or if we're 
// currently capturing the mouse.
// --------------------------------------------------------
void Game::OnMouseMove(WPARAM buttonState, int x, int y)
{
	float sensitivity = 0.005f;
	// Add any custom code here...
	if (click) 
	{
		cam->transform.rotation = { cam->transform.rotation.x + ((x - prevMousePos.x) * sensitivity), cam->transform.rotation.y + ((y - prevMousePos.y) * sensitivity), 0 };
		cam->transform.foward = {(sinf((cam->transform.rotation.x *180) / (2.0f*3.14f))), (sinf((cam->transform.rotation.y * 180) / (2.0f*3.14f))), (cosf((cam->transform.rotation.x * 180) / (2.0f*3.14f))) };
	}
	// Save the previous mouse position, so we have it for the future
	prevMousePos.x = x;
	prevMousePos.y = y;
}

// --------------------------------------------------------
// Helper method for mouse wheel scrolling.  
// WheelDelta may be positive or negative, depending 
// on the direction of the scroll
// --------------------------------------------------------
void Game::OnMouseWheel(float wheelDelta, int x, int y)
{
	// Add any custom code here...
}
void Game::SpawnGameObject(std::string meshName, DirectX::XMFLOAT3 pos, bool canShoot)
{
	GameEntity* obj = new GameEntity(rend->meshStorage[meshName], rend);
	
	obj->renderingComponent.mat.surfaceColor = { (float)(std::rand() % 100) * 0.01f, (float)(std::rand() % 100)* 0.01f, (float)(std::rand() % 100) * 0.01f, 1.0f };
	obj->transform.position = pos;
	obj->transform.rotation = cam->transform.rotation;

	if (canShoot)
	{
		float bulletSpeed = 40000;
		//obj->rigidBody.applyFriction = false;
		obj->rigidBody.ApplyForce(cam->transform.foward.x * bulletSpeed, cam->transform.foward.y * bulletSpeed, cam->transform.foward.z * bulletSpeed);
	}

	gameObjects.push_back(obj);
	printf("\nNum of '%ss': %d", &meshName[0], rend->meshStorage[meshName]->instances);
	//printf("\nColor - %f, %f, %f", obj->renderingComponent.mat.surfaceColor.x, obj->renderingComponent.mat.surfaceColor.y, obj->renderingComponent.mat.surfaceColor.z);
}
#pragma endregion