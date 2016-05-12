// ******************************************************************************
// Filename:    VogueGame.cpp
// Project:     Vogue
// Author:      Steven Ball
//
// Revision History:
//   Initial Revision - 11/04/16
//
// Copyright (c) 2005-2016, Steven Ball
// ******************************************************************************

#include "glew/include/GL/glew.h"

#include "VogueGame.h"
#include "utils/Interpolator.h"
#include <glm/detail/func_geometric.hpp>

#ifdef __linux__
#include <sys/time.h>
#endif //__linux__


// Initialize the singleton instance
VogueGame *VogueGame::c_instance = 0;

VogueGame* VogueGame::GetInstance()
{
	if (c_instance == 0)
		c_instance = new VogueGame;

	return c_instance;
}

// Creation
void VogueGame::Create(VogueSettings* pVogueSettings)
{
	m_pRenderer = NULL;
	m_pGameCamera = NULL;

	m_GUICreated = false;

	m_pVogueSettings = pVogueSettings;
	m_pVogueWindow = new VogueWindow(this, m_pVogueSettings);

	// Create the window
	m_pVogueWindow->Create();

	/* Setup the FPS and deltatime counters */
#ifdef _WIN32
	QueryPerformanceCounter(&m_fpsPreviousTicks);
	QueryPerformanceCounter(&m_fpsCurrentTicks);
	QueryPerformanceFrequency(&m_fpsTicksPerSecond);
#else
	struct timeval tm;
	gettimeofday(&tm, NULL);
	m_fpsCurrentTicks = (double)tm.tv_sec + (double)tm.tv_usec / 1000000.0;
	m_fpsPreviousTicks = (double)tm.tv_sec + (double)tm.tv_usec / 1000000.0;
#endif //_WIN32
	m_deltaTime = 0.0f;
	m_fps = 0.0f;

	/* Mouse name picking */
	m_pickedObject = -1;
	m_bNamePickingSelected = false;

	/* Custom cursors */
	m_bPressedCursorDown = false;
	m_bCustomCursorOn = false;

	/* Setup the initial starting wait timing */
	m_initialWaitTimer = 0.0f;
	m_initialWaitTime = 0.5f;
	m_initialStartWait = true;

	/* Create the renderer */
	m_windowWidth = m_pVogueWindow->GetWindowWidth();
	m_windowHeight = m_pVogueWindow->GetWindowHeight();
	m_pRenderer = new Renderer(m_windowWidth, m_windowHeight, 32, 8);

	/* Pause and quit */
	m_bGameQuit = false;
	m_bPaused = false;

	/* Create the GUI */
	m_pGUI = new OpenGLGUI(m_pRenderer);

	/* Create cameras */
	m_pGameCamera = new Camera(m_pRenderer);
	m_pGameCamera->SetPosition(vec3(8.0f, 8.25f, 15.5f));
	m_pGameCamera->SetFakePosition(m_pGameCamera->GetPosition());
	m_pGameCamera->SetFacing(vec3(0.0f, 0.0f, -1.0f));
	m_pGameCamera->SetUp(vec3(0.0f, 1.0f, 0.0f));
	m_pGameCamera->SetRight(vec3(1.0f, 0.0f, 0.0f));

	/* Create viewports */
	m_pRenderer->CreateViewport(0, 0, m_windowWidth, m_windowHeight, 60.0f, &m_defaultViewport);

	/* Create fonts */
	m_pRenderer->CreateFreeTypeFont("media/fonts/arial.ttf", 12, &m_defaultFont);

	/* Create the custom cursor textures */
	int lTextureWidth, lTextureHeight, lTextureWidth2, lTextureHeight2;
	m_pRenderer->LoadTexture("media/textures/cursors/finger_cursor_normal.tga", &lTextureWidth, &lTextureHeight, &lTextureWidth2, &lTextureHeight2, &m_customCursorNormalBuffer);
	m_pRenderer->LoadTexture("media/textures/cursors/finger_cursor_clicked.tga", &lTextureWidth, &lTextureHeight, &lTextureWidth2, &lTextureHeight2, &m_customCursorClickedBuffer);
	m_pRenderer->LoadTexture("media/textures/cursors/finger_cursor_rotate.tga", &lTextureWidth, &lTextureHeight, &lTextureWidth2, &lTextureHeight2, &m_customCursorRotateBuffer);
	m_pRenderer->LoadTexture("media/textures/cursors/finger_cursor_zoom.tga", &lTextureWidth, &lTextureHeight, &lTextureWidth2, &lTextureHeight2, &m_customCursorZoomBuffer);

	/* Create lights */
	m_defaultLightPosition = vec3(300.0f, 300.0f, 300.0f);
	m_defaultLightView = vec3(0.0f, 0.0f, 0.0f);
	vec3 lightDirection = m_defaultLightView - m_defaultLightPosition;
	m_pRenderer->CreateLight(Colour(1.0f, 1.0f, 1.0f, 1.0f), Colour(1.0f, 1.0f, 1.0f, 1.0f), Colour(0.0f, 0.0f, 0.0f, 1.0f),
							 m_defaultLightPosition, lightDirection, 0.0f, 0.0f, 2.0f, 0.001f, 0.0f, true, false, &m_defaultLight);

	/* Create materials */
	m_pRenderer->CreateMaterial(Colour(1.0f, 1.0f, 1.0f, 1.0f), Colour(1.0f, 1.0f, 1.0f, 1.0f), Colour(1.0f, 1.0f, 1.0f, 1.0f), Colour(0.0f, 0.0f, 0.0f, 1.0f), 64, &m_defaultMaterial);

	/* Create the frame buffers */
	bool frameBufferCreated = false;
	frameBufferCreated = m_pRenderer->CreateFrameBuffer(-1, true, true, true, true, m_windowWidth, m_windowHeight, 1.0f, "SSAO", &m_SSAOFrameBuffer);
	frameBufferCreated = m_pRenderer->CreateFrameBuffer(-1, true, true, true, true, m_windowWidth, m_windowHeight, 5.0f, "Shadow", &m_shadowFrameBuffer);
	frameBufferCreated = m_pRenderer->CreateFrameBuffer(-1, true, true, true, true, m_windowWidth, m_windowHeight, 1.0f, "Deferred Lighting", &m_lightingFrameBuffer);
	frameBufferCreated = m_pRenderer->CreateFrameBuffer(-1, true, true, true, true, m_windowWidth, m_windowHeight, 1.0f, "Transparency", &m_transparencyFrameBuffer);
	frameBufferCreated = m_pRenderer->CreateFrameBuffer(-1, true, true, true, true, m_windowWidth, m_windowHeight, 1.0f, "Water Reflection", &m_waterReflectionFrameBuffer);
	frameBufferCreated = m_pRenderer->CreateFrameBuffer(-1, true, true, true, true, m_windowWidth, m_windowHeight, 1.0f, "FXAA", &m_FXAAFrameBuffer);
	frameBufferCreated = m_pRenderer->CreateFrameBuffer(-1, true, true, true, true, m_windowWidth, m_windowHeight, 1.0f, "FullScreen 1st Pass", &m_firstPassFullscreenBuffer);
	frameBufferCreated = m_pRenderer->CreateFrameBuffer(-1, true, true, true, true, m_windowWidth, m_windowHeight, 1.0f, "FullScreen 2nd Pass", &m_secondPassFullscreenBuffer);
	frameBufferCreated = m_pRenderer->CreateFrameBuffer(-1, true, true, true, true, 800, 800, 1.0f, "Paperdoll", &m_paperdollBuffer);
	frameBufferCreated = m_pRenderer->CreateFrameBuffer(-1, true, true, true, true, 800, 800, 1.0f, "Paperdoll SSAO Texture", &m_paperdollSSAOTextureBuffer);
	frameBufferCreated = m_pRenderer->CreateFrameBuffer(-1, true, true, true, true, 800, 800, 1.0f, "Portrait", &m_portraitBuffer);
	frameBufferCreated = m_pRenderer->CreateFrameBuffer(-1, true, true, true, true, 800, 800, 1.0f, "Portrait SSAO Texture", &m_portraitSSAOTextureBuffer);

	/* Create the shaders */
	bool shaderLoaded = false;
	m_defaultShader = -1;
	m_phongShader = -1;
	m_SSAOShader = -1;
	m_shadowShader = -1;
	m_waterShader = -1;
	m_lightingShader = -1;
	m_cubeMapShader = -1;
	m_textureShader = -1;
	m_fxaaShader = -1;
	m_blurVerticalShader = -1;
	m_blurHorizontalShader = -1;
	m_paperdollShader = -1;
	shaderLoaded = m_pRenderer->LoadGLSLShader("media/shaders/default.vertex", "media/shaders/default.pixel", &m_defaultShader);
	shaderLoaded = m_pRenderer->LoadGLSLShader("media/shaders/phong.vertex", "media/shaders/phong.pixel", &m_phongShader);
	shaderLoaded = m_pRenderer->LoadGLSLShader("media/shaders/shadow.vertex", "media/shaders/shadow.pixel", &m_shadowShader);
	shaderLoaded = m_pRenderer->LoadGLSLShader("media/shaders/water_still.vertex", "media/shaders/water_still.pixel", &m_waterShader);
	shaderLoaded = m_pRenderer->LoadGLSLShader("media/shaders/texture.vertex", "media/shaders/texture.pixel", &m_textureShader);
	shaderLoaded = m_pRenderer->LoadGLSLShader("media/shaders/fullscreen/SSAO.vertex", "media/shaders/fullscreen/SSAO.pixel", &m_SSAOShader);
	shaderLoaded = m_pRenderer->LoadGLSLShader("media/shaders/fullscreen/fxaa.vertex", "media/shaders/fullscreen/fxaa.pixel", &m_fxaaShader);
	shaderLoaded = m_pRenderer->LoadGLSLShader("media/shaders/fullscreen/lighting.vertex", "media/shaders/fullscreen/lighting.pixel", &m_lightingShader);
	shaderLoaded = m_pRenderer->LoadGLSLShader("media/shaders/cube_map.vertex", "media/shaders/cube_map.pixel", &m_cubeMapShader);
	shaderLoaded = m_pRenderer->LoadGLSLShader("media/shaders/fullscreen/blur_vertical.vertex", "media/shaders/fullscreen/blur_vertical.pixel", &m_blurVerticalShader);
	shaderLoaded = m_pRenderer->LoadGLSLShader("media/shaders/fullscreen/blur_horizontal.vertex", "media/shaders/fullscreen/blur_horizontal.pixel", &m_blurHorizontalShader);
	shaderLoaded = m_pRenderer->LoadGLSLShader("media/shaders/paperdoll.vertex", "media/shaders/paperdoll.pixel", &m_paperdollShader);

	// Keyboard movement
	m_bKeyboardForward = false;
	m_bKeyboardBackward = false;
	m_bKeyboardStrafeLeft = false;
	m_bKeyboardStrafeRight = false;
	m_bKeyboardLeft = false;
	m_bKeyboardRight = false;
	m_bKeyboardUp = false;
	m_bKeyboardDown = false;
	m_bKeyboardSpace = false;
	m_bKeyboardMenu = false;

	// Joystick flags
	m_bJoystickJump = false;

	// Combat flags
	m_bAttackPressed_Mouse = false;
	m_bAttackReleased_Mouse = false;
	m_bAttackPressed_Joystick = false;
	m_bAttackReleased_Joystick = false;
	m_bCanDoAttack_Mouse = true;
	m_bCanDoAttack_Joystick = true;
	m_bTargetEnemyPressed_Joystick = false;
	m_bTargetEnemyReleased_Joystick = false;

	// Camera movement
	m_bCameraRotate = false;
	m_pressedX = 0;
	m_pressedY = 0;
	m_currentX = 0;
	m_currentY = 0;
	m_cameraDistance = m_pGameCamera->GetZoomAmount();
	m_maxCameraDistance = m_cameraDistance;

	// Movement
	m_keyboardMovement = false;
	m_gamepadMovement = false;

	// Blur
	m_globalBlurAmount = 0.0f;

	// Cinematic letterbox mode
	m_letterBoxRatio = 0.0f;

	// Water
	m_elapsedWaterTime = 0.0f;

	// Toggle flags
	m_deferredRendering = true;
	m_modelWireframe = false;
	m_modelAnimationIndex = 0;
	m_multiSampling = true;
	m_ssao = true;
	m_blur = false;
	m_shadows = true;
	m_dynamicLighting = true;
	m_animationUpdate = true;
	m_fullscreen = m_pVogueSettings->m_fullscreen;
	m_debugRender = false;
	m_instanceRender = true;
	m_fogRender = true;
	m_waterRender = true;

	// Camera mode
	m_cameraMode = CameraMode_Debug;
	m_previousCameraMode = CameraMode_Debug;
	m_shouldRestorePreviousCameraMode = false;

	// Game mode
	m_gameMode = GameMode_Loading;
	m_allowToChangeToGame = true;
	m_allowToChangeToFrontend = true;
	SetGameMode(m_gameMode);

	// Create, setup and skin the GUI components
	CreateGUI();
	SetupGUI();
	SkinGUI();
}

// Destruction
void VogueGame::Destroy()
{
	if (c_instance)
	{
		delete m_pGameCamera;
		DestroyGUI();  // Destroy the GUI components before we delete the GUI manager object.
		delete m_pGUI;
		delete m_pRenderer;

		m_pVogueWindow->Destroy();

		delete m_pVogueWindow;

		delete c_instance;
	}
}

// Blur
void VogueGame::SetGlobalBlurAmount(float blurAmount)
{
	m_globalBlurAmount = blurAmount;
}

// Cinematic letterbox
void VogueGame::OpenLetterBox()
{
	Interpolator::GetInstance()->AddFloatInterpolation(&m_letterBoxRatio, m_letterBoxRatio, 1.0f, 0.25f, -100.0f);
}

void VogueGame::CloseLetterBox()
{
	Interpolator::GetInstance()->AddFloatInterpolation(&m_letterBoxRatio, m_letterBoxRatio, 0.0f, 0.25f, 100.0f);
}

// Events
void VogueGame::PollEvents()
{
	m_pVogueWindow->PollEvents();
}

bool VogueGame::ShouldClose()
{
	return m_bGameQuit;
}

// Window functionality
int VogueGame::GetWindowCursorX()
{
	return m_pVogueWindow->GetCursorX();
}

int VogueGame::GetWindowCursorY()
{
	return m_pVogueWindow->GetCursorY();
}


void VogueGame::TurnCursorOn(bool resetCursorPosition, bool forceOn)
{
	m_pVogueWindow->TurnCursorOn(resetCursorPosition, forceOn);

	m_bCustomCursorOn = true;
}

void VogueGame::TurnCursorOff(bool forceOff)
{
	m_pVogueWindow->TurnCursorOff(forceOff);

	m_bCustomCursorOn = false;

	// Make sure to set the current X and Y when we turn the cursor off, so that camera controls don't glitch.
	m_currentX = m_pVogueWindow->GetCursorX();
	m_currentY = m_pVogueWindow->GetCursorY();
}

bool VogueGame::IsCursorOn()
{
	return m_pVogueWindow->IsCursorOn();
}

void VogueGame::ResizeWindow(int width, int height)
{
	m_windowWidth = width;
	m_windowHeight = height;

	m_pVogueWindow->ResizeWindow(m_windowWidth, m_windowHeight);

	if(m_pRenderer)
	{
		// Let the renderer know we have resized the window
		m_pRenderer->ResizeWindow(m_windowWidth, m_windowHeight);

		// Resize the main viewport
		m_pRenderer->ResizeViewport(m_defaultViewport, 0, 0, m_windowWidth, m_windowHeight, 60.0f);

		// Resize the frame buffers
		bool frameBufferResize = false;
		frameBufferResize = m_pRenderer->CreateFrameBuffer(m_SSAOFrameBuffer, true, true, true, true, m_windowWidth, m_windowHeight, 1.0f, "SSAO", &m_SSAOFrameBuffer);
		frameBufferResize = m_pRenderer->CreateFrameBuffer(m_shadowFrameBuffer, true, true, true, true, m_windowWidth, m_windowHeight, 5.0f, "Shadow", &m_shadowFrameBuffer);
		frameBufferResize = m_pRenderer->CreateFrameBuffer(m_lightingFrameBuffer, true, true, true, true, m_windowWidth, m_windowHeight, 1.0f, "Deferred Lighting", &m_lightingFrameBuffer);
		frameBufferResize = m_pRenderer->CreateFrameBuffer(m_transparencyFrameBuffer, true, true, true, true, m_windowWidth, m_windowHeight, 1.0f, "Transparency", &m_transparencyFrameBuffer);
		frameBufferResize = m_pRenderer->CreateFrameBuffer(m_waterReflectionFrameBuffer, true, true, true, true, m_windowWidth, m_windowHeight, 1.0f, "Water Reflection", &m_waterReflectionFrameBuffer);
		frameBufferResize = m_pRenderer->CreateFrameBuffer(m_FXAAFrameBuffer, true, true, true, true, m_windowWidth, m_windowHeight, 1.0f, "FXAA", &m_FXAAFrameBuffer);
		frameBufferResize = m_pRenderer->CreateFrameBuffer(m_firstPassFullscreenBuffer, true, true, true, true, m_windowWidth, m_windowHeight, 1.0f, "FullScreen 1st Pass", &m_firstPassFullscreenBuffer);
		frameBufferResize = m_pRenderer->CreateFrameBuffer(m_secondPassFullscreenBuffer, true, true, true, true, m_windowWidth, m_windowHeight, 1.0f, "FullScreen 2nd Pass", &m_secondPassFullscreenBuffer);
		frameBufferResize = m_pRenderer->CreateFrameBuffer(m_paperdollBuffer, true, true, true, true, 800, 800, 1.0f, "Paperdoll", &m_paperdollBuffer);
		frameBufferResize = m_pRenderer->CreateFrameBuffer(m_paperdollSSAOTextureBuffer, true, true, true, true, 800, 800, 1.0f, "Paperdoll SSAO Texture", &m_paperdollSSAOTextureBuffer);
		frameBufferResize = m_pRenderer->CreateFrameBuffer(m_portraitBuffer, true, true, true, true, 800, 800, 1.0f, "Portrait", &m_portraitBuffer);
		frameBufferResize = m_pRenderer->CreateFrameBuffer(m_portraitSSAOTextureBuffer, true, true, true, true, 800, 800, 1.0f, "Portrait SSAO Texture", &m_portraitSSAOTextureBuffer);
	}
}

void VogueGame::CloseWindow()
{
	if (m_gameMode == GameMode_Game)
	{
		// TODO : Show quit popup
	}
	else
	{
		m_bGameQuit = true;
	}
}

void VogueGame::UpdateJoySticks()
{
	m_pVogueWindow->UpdateJoySticks();
}

// Game functions
void VogueGame::QuitToFrontEnd()
{
	TurnCursorOn(true, false);
	SetGameMode(GameMode_FrontEnd);

	m_pVogueWindow->Update(m_deltaTime);
}

void VogueGame::SetupDataForGame()
{
}

void VogueGame::SetupDataForFrontEnd()
{
}

void VogueGame::StartGameFromFrontEnd()
{
	m_previousCameraMode = CameraMode_MouseRotate;
}

void VogueGame::SetGameMode(GameMode mode)
{
	GameMode previousgameMode = m_gameMode;
	m_gameMode = mode;

	if (m_gameMode == GameMode_Debug)
	{
	}

	if (m_gameMode == GameMode_FrontEnd)
	{
		if (previousgameMode == GameMode_Game || previousgameMode == GameMode_Loading)
		{
			// Setup the gamedata since we have just loaded fresh into the frontend.
			SetupDataForFrontEnd();
		}
	}

	if (m_gameMode == GameMode_Game)
	{
		if (previousgameMode == GameMode_FrontEnd || previousgameMode == GameMode_Loading)
		{
			// Setup the gamedata since we have just loaded fresh into a game.
			SetupDataForGame();
		}
	}
}

GameMode VogueGame::GetGameMode()
{
	return m_gameMode;
}

void VogueGame::SetCameraMode(CameraMode mode)
{
	m_cameraMode = mode;
}

CameraMode VogueGame::GetCameraMode()
{
	return m_cameraMode;
}

// Accessors
unsigned int VogueGame::GetDefaultViewport()
{
	return m_defaultViewport;
}

Camera* VogueGame::GetGameCamera()
{
	return m_pGameCamera;
}

VogueSettings* VogueGame::GetVogueSettings()
{
	return m_pVogueSettings;
}