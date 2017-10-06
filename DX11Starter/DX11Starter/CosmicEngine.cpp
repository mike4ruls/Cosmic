#include "CosmicEngine.h"

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
CosmicEngine::CosmicEngine(HINSTANCE hInstance)
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
CosmicEngine::~CosmicEngine()
{
	if (rend) { delete rend; rend = nullptr; }
	QuitLevel();
}

// --------------------------------------------------------
// Called once per program, after DirectX and the window
// are initialized but before the game loop.
// --------------------------------------------------------
void CosmicEngine::Init()
{
	rend = new Renderer(cam, device, context, backBufferRTV, depthStencilView);

	//CreateBasicGeometry();
	SetKeyInputs();

	dayTime = 0.0f;
	click = false;

	srand((unsigned int)time(NULL));

	currentScene->Init();
	initFinished = true;

	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}


// --------------------------------------------------------
// Creates the geometry we're going to draw - a single triangle for now
// --------------------------------------------------------
void CosmicEngine::CreateBasicGeometry()
{
	// Create some temporary variables to represent colors
	// - Not necessary, just makes things more readable
	XMFLOAT4 red = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	XMFLOAT4 green = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	XMFLOAT4 blue = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);

	XMFLOAT3 genNorm = XMFLOAT3(1.0f, 0.0f, 0.0f);
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

	/*rend->LoadMesh(new Mesh(vertices, indices, vSize, iSize, "Triangle", device));
	rend->LoadMesh(new Mesh(vertices2, indices2, vSize2, iSize2, "Square", device));
	rend->LoadMesh(new Mesh("Assets/Models/sphere.obj", "Sphere", device));
	rend->LoadMesh(new Mesh("Assets/Models/cube.obj", "Cube", device));
	rend->LoadMesh(new Mesh("Assets/Models/cone.obj", "Cone", device));
	rend->LoadMesh(new Mesh("Assets/Models/torus.obj", "Torus", device));
	rend->LoadMesh(new Mesh("Assets/Models/helix.obj", "Helix", device));
	rend->LoadMesh(new Mesh("Assets/Models/raygun.obj", "RayGun", device));
	rend->LoadMesh(new Mesh("Assets/Models/plane.obj", "Plane", device));
	rend->LoadMesh(new Mesh("Assets/Models/quad.obj", "Quad", device));
	rend->LoadMesh(new Mesh("Assets/Models/teapot.obj", "Teapot", device));
	rend->LoadMesh(new Mesh("Assets/Models/HaloSword.obj", "HaloSword", device));
	rend->LoadMesh(new Mesh("Assets/Models/RainbowRoad.obj", "RainbowRoad", device));*/

}


// --------------------------------------------------------
// Handle resizing DirectX "stuff" to match the new window size.
// For instance, updating our projection matrix's aspect ratio.
// --------------------------------------------------------
void CosmicEngine::OnResize()
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
void CosmicEngine::Update(float deltaTime, float totalTime)
{

	if (IsKeyPressed(VK_TAB))
	{
		rend->ToggleSkyBox();
	}
	if (IsKeyPressed(70))
	{
		rend->ToggleWireFrame();
	}

	rend->sunLight->ligComponent->lightDir = { sin(dayTime),cos(dayTime),0.0f };
	currentScene->Update(deltaTime, totalTime);
	cam->Update(deltaTime);
	UpdateInput();
	//printf("\nLight Dir Vector - (%f, %f, 0.0)", sin(dayTime), cos(dayTime));

	// Quit if the escape key is pressed
	if (IsKeyDown(VK_ESCAPE))
	{
		Quit();
	}
}

// --------------------------------------------------------
// Clear the screen, redraw everything, present to the user
// --------------------------------------------------------
void CosmicEngine::Draw(float deltaTime, float totalTime)
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
void CosmicEngine::OnMouseDown(WPARAM buttonState, int x, int y)
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
void CosmicEngine::OnMouseUp(WPARAM buttonState, int x, int y)
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
void CosmicEngine::OnMouseMove(WPARAM buttonState, int x, int y)
{
	float sensitivity = 0.005f;
	// Add any custom code here...
	if (click)
	{
		cam->transform.rotation = { cam->transform.rotation.x + ((x - prevMousePos.x) * sensitivity), cam->transform.rotation.y + ((y - prevMousePos.y) * sensitivity), 0 };

		float newX = sinf((cam->transform.rotation.x * 180) / (2.0f*3.14f));
		cam->transform.foward = { newX, (sinf((cam->transform.rotation.y * 180) / (2.0f*3.14f))), (cosf((cam->transform.rotation.x * 180) / (2.0f*3.14f))) };
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
void CosmicEngine::OnMouseWheel(float wheelDelta, int x, int y)
{
	// Add any custom code here...
}
GameEntity * CosmicEngine::CreateGameObject(std::string name)
{
	 GameEntity* newObj = new GameEntity(rend->assets->GetMesh(name), rend);
	
	return newObj;
}
void CosmicEngine::LoadDefaultScene(Game * newScene)
{
	currentScene = newScene;
	cam = currentScene->cam;
	cam->Init(width, height);
}
void CosmicEngine::LoadScene(Game* newScene)
{

	QuitLevel();

	currentScene = newScene;
	cam = currentScene->cam;
	cam->Init(width, height);
	rend->cam = cam;

	currentScene->Init();
}
void CosmicEngine::QuitLevel()
{
	if (currentScene != nullptr) { delete currentScene; currentScene = nullptr; }
	if (rend != nullptr)
	{
		rend->Flush();
	}
}
void CosmicEngine::SetKeyInputs()
{
	keys[VK_LEFT] = 0;
	keys[VK_UP] = 0;
	keys[VK_RIGHT] = 0;
	keys[VK_DOWN] = 0;
	keys[VK_LSHIFT] = 0;
	keys[VK_SPACE] = 0;
	keys[VK_RETURN] = 0;
	keys[VK_TAB] = 0;
	keys[VK_ESCAPE] = 0;

	keys[49] = 0; // 1
	keys[50] = 0; // 2

	//Num pad
	keys[96] = 0; // NumPad 0
	keys[97] = 0; // NumPad 1
	keys[98] = 0; // NumPad 2
	keys[99] = 0; // NumPad 3
	keys[100] = 0; // NumPad 4
	keys[101] = 0; // NumPad 5

	keys[87] = 0; // W
	keys[65] = 0; // A
	keys[83] = 0; // S
	keys[68] = 0; // D

	keys[69] = 0; // E
	keys[88] = 0; // X
	keys[70] = 0; // F

	//keys[97] = 0; // A
	//keys[99] = 0; // A
}
void CosmicEngine::UpdateInput()
{
	for (auto& x : keys)
	{
		bool pressed = GetAsyncKeyState(x.first) ? true : false;

		if(pressed)
		{
			// if being held down
			if (x.second == 1)
			{
				x.second = 2;
			}
			// if it's the first press
			else if (x.second == 0)
			{
				x.second = 1;
			}
		}
		// if it's not being pressed
		else
		{
			x.second = 0;
		}

	}
}
bool CosmicEngine::IsKeyDown(int keyCode)
{
	return keys.find(keyCode)->second >= 1 ? true : false;
}
bool CosmicEngine::IsKeyPressed(int keyCode)
{
	return keys.find(keyCode)->second == 1 ? true : false;
}
#pragma endregion