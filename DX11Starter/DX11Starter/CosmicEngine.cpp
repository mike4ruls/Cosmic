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
	ImGui_ImplDX11_Shutdown();
	if (rend) { delete rend; rend = nullptr; }
	if (physicEngine) { delete physicEngine; physicEngine = nullptr; }
	inputManager->Release();
	currentScene->Quit();
	QuitLevel();
	
}

// --------------------------------------------------------
// Called once per program, after DirectX and the window
// are initialized but before the game loop.
// --------------------------------------------------------
void CosmicEngine::Init()
{
	ImGui_ImplDX11_Init(hWnd, device, context);
	rend = new Renderer(cam, device, context, backBufferRTV, depthStencilView, swapChain);
	physicEngine = new CosmicPhysic();
	currentScene->SetWidthHeight(width, height);
	currentScene->SetAssetManager(rend->assets, physicEngine, rend->sunLight, &rend->skyBoxSVR);
	pauseGame = false;

	srand((unsigned int)time(NULL));

	currentScene->Init();
	inputManager = InputManager::GetInstance();
	inputManager->currentMousePos = MouseScreen();
	inputManager->currentMousePos.x = 0.0f;
	inputManager->currentMousePos.y = 0.0f;
	inputManager->click = false;
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

void CosmicEngine::UpdateObjects(float dt)
{
	if (!pauseGame)
	{
		for (unsigned int i = 0; i < allObj.size(); i++)
		{
			allObj[i]->Update(dt);
			allObj[i]->SetWorld();
		}
		for (unsigned int i = 0; i < allEmitter.size(); i++)
		{
			allEmitter[i]->Update(dt);
		}
	}

	for (unsigned int i = 0; i < allUI.size(); i++)
	{
		allUI[i]->Update(dt);
	}
}

// --------------------------------------------------------
// Update your game here - user input, move objects, AI, etc.
// --------------------------------------------------------
void CosmicEngine::Update(float deltaTime, float totalTime)
{
	ImGuiIO& IO = ImGui::GetIO();
	IO.DeltaTime = deltaTime;
	ImGui_ImplDX11_NewFrame();

	currentScene->Update(deltaTime, totalTime);
	UpdateObjects(deltaTime);
	cam->UpdateCamShake(deltaTime);
	cam->Update(deltaTime);

	//printf("\nLight Dir Vector - (%f, %f, 0.0)", sin(dayTime), cos(dayTime));

	inputManager->Update();
	if (inputManager->IsKeyPressed(KeyCode::TAB))// || inputManager->IsButtonPressed(CosmicInput::DPAD_DOWN))
	{
		rend->ToggleSkyBox();
	}
	if (inputManager->IsKeyPressed(KeyCode::F))// || inputManager->IsButtonPressed(CosmicInput::DPAD_RIGHT))
	{
		rend->ToggleWireFrame();
	}
	inputManager->prevClick = inputManager->click;
	// Quit if the escape key is pressed
	if (inputManager->IsKeyDown(KeyCode::ESCAPE))
	{
		Quit();
	}
}

// --------------------------------------------------------
// Clear the screen, redraw everything, present to the user
// --------------------------------------------------------
void CosmicEngine::Draw(float deltaTime, float totalTime)
{
	{
		// 1. Show a simple window
		// Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets appears in a window automatically called "Debug"
		static float f = 0.0f;
		static char text = char();
		ImGui::Text("Hello, world!");
		ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
		//ImGui::ColorEdit3("clear color", (float*)&clear_color);
		//if (ImGui::Button("Test Window")) show_test_window ^= 1;
		//if (ImGui::Button("Another Window")) show_another_window ^= 1;
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::InputText("Test", &text, sizeof(char)*30);
	}
	rend->Render(deltaTime);

	ImGui::Render();

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
	inputManager->click = true;
	printf("click");
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
	inputManager->click = false;
	// We don't care about the tracking the cursor outside
	// the window anymore (we're not dragging if the mouse is up)
	ReleaseCapture();
}

// --------------------------------------------------------
// Helper method for mouse movement.  We only get this message
// if the mouse is currently over the window, or if we're 
// currently capturing the mouse.
// --------------------------------------------------------
void CosmicEngine::OnMouseMove(WPARAM buttonState, int x, int y, float dt)
{
	float sensitivity = 500.0f * dt;

	float newWidth = (float)width / 2.0f;
	float newHeigth = (float)height / 2.0f;
	inputManager->currentMousePos.x = (-newWidth + x) / (newWidth / 1.1f);
	inputManager->currentMousePos.y = (newHeigth - y) / (newHeigth / 0.62f);

	//printf("X: %f, Y: %f\n", currentMousePos.x, currentMousePos.y);
	// Add any custom code here...
	if (inputManager->click && !cam->lockCameraRot)
	{
		cam->RotateCamera((x - prevMousePos.x) * sensitivity, (y - prevMousePos.y) * sensitivity);

		/*cam->transform.rotation = { cam->transform.rotation.x + ((x - prevMousePos.x) * sensitivity), cam->transform.rotation.y + ((y - prevMousePos.y) * sensitivity), 0 };
		cam->transform.rotation.y = std::max<float>(std::min<float>(cam->transform.rotation.y, (3.1415f / 2.0f)), (-3.1415f / 2.0f));

		float newX = sinf((cam->transform.rotation.x * 180) / (2.0f*3.14f));
		cam->transform.foward = { newX, (sinf((cam->transform.rotation.y * 180) / (2.0f*3.14f))), (cosf((cam->transform.rotation.x * 180) / (2.0f*3.14f))) };*/
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
void CosmicEngine::PushGameObject(GameEntity* gameobject)
{
	gameobject->Id = allObj.size();
	if (!gameobject->isUI)
	{
		rend->PushToRenderer(gameobject->renderingComponent);
	}
	else
	{
		rend->PushToCanvas(gameobject->renderingComponent);
	}

	allObj.push_back(gameobject);
}
void CosmicEngine::PushUI(UI * newUI)
{
	newUI->Id = allUI.size();

	allUI.push_back(newUI);
}
void CosmicEngine::PushEmitter(Emitter * newEmitter)
{

	newEmitter->engineID = allEmitter.size();

	allEmitter.push_back(newEmitter);

	rend->PushToEmitter(newEmitter);
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
	currentScene->SetWidthHeight(width, height);
	currentScene->SetAssetManager(rend->assets, physicEngine, rend->sunLight, &rend->skyBoxSVR);
	cam = currentScene->cam;
	cam->Init(width, height);
	rend->cam = cam;
	cam->lockCameraRot = false;

	currentScene->Init();
}
void CosmicEngine::QuitLevel()
{
	Flush();
	if (currentScene != nullptr) { delete currentScene; currentScene = nullptr; }
	if (rend != nullptr)
	{
		rend->Flush();
	}
}
void CosmicEngine::Flush()
{
	for(unsigned int i = 0; i < allObj.size(); i++)
	{
		if (allObj[i] != nullptr) { delete allObj[i]; allObj[i] = nullptr; }
	}
	for (unsigned int i = 0; i < allUI.size(); i++)
	{
		if (allUI[i] != nullptr) { delete allUI[i]; allUI[i] = nullptr; }
	}
	for (unsigned int i = 0; i < allEmitter.size(); i++)
	{
		if (allEmitter[i] != nullptr) { delete allEmitter[i]; allEmitter[i] = nullptr; }
	}

	allObj.clear();
	allUI.clear();
	allEmitter.clear();
}
void CosmicEngine::DestroyGameObject(GameEntity * obj)
{
	unsigned int Id = obj->Id;
	rend->RemoveFromRenderer(obj->renderingComponent->meshName, obj->renderingComponent->rendID);
	allObj.erase(allObj.begin() + Id);

	if (obj != nullptr) { delete obj; obj = nullptr; }

	for (unsigned int i = Id; i < allObj.size(); i++)
	{
		allObj[i]->Id = i;
	}
}
void CosmicEngine::DestroyUIObject(UI * obj)
{
	unsigned int Id = obj->Id;
	rend->RemoveFromRenderer(obj->obj->renderingComponent->meshName, obj->obj->renderingComponent->rendID);
	allUI.erase(allUI.begin() + Id);

	if (obj != nullptr) { delete obj; obj = nullptr; }

	for (unsigned int i = Id; i < allUI.size(); i++)
	{
		allUI[i]->Id = i;
	}
}
void CosmicEngine::DestroyEmitter(Emitter * obj)
{
	unsigned int Id = obj->emitterID;
	rend->RemoveFromEmitter(Id);

	allEmitter.erase(allEmitter.begin() + obj->engineID);

	if (obj != nullptr) { delete obj; obj = nullptr; }

	for (unsigned int i = Id; i < allEmitter.size(); i++)
	{
		allEmitter[i]->engineID = i;
	}
}
ID3D11Device * CosmicEngine::GetDevice()
{
	return device;
}
//void CosmicEngine::SetKeyInputs()
//{
//	keys[VK_LEFT] = 0;
//	keys[VK_UP] = 0;
//	keys[VK_RIGHT] = 0;
//	keys[VK_DOWN] = 0;
//	keys[VK_LSHIFT] = 0;
//	keys[VK_RSHIFT] = 0;
//	keys[VK_SPACE] = 0;
//	keys[VK_RETURN] = 0;
//	keys[VK_TAB] = 0;
//	keys[VK_ESCAPE] = 0;
//
//	keys[49] = 0; // 1
//	keys[50] = 0; // 2
//
//	//Num pad
//	keys[96] = 0; // NumPad 0
//	keys[97] = 0; // NumPad 1
//	keys[98] = 0; // NumPad 2
//	keys[99] = 0; // NumPad 3
//	keys[100] = 0; // NumPad 4
//	keys[101] = 0; // NumPad 5
//
//	keys[87] = 0; // W
//	keys[65] = 0; // A
//	keys[83] = 0; // S
//	keys[68] = 0; // D
//
//	keys[69] = 0; // E
//	keys[88] = 0; // X
//	keys[70] = 0; // F
//
//	//keys[97] = 0; // A
//	//keys[99] = 0; // A
//}
#pragma endregion