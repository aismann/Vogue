// ******************************************************************************
// Filename:    VogueGame.h
// Project:     Vogue
// Author:      Steven Ball
//
// Purpose:
//   The Vogue game class houses all the game functionality and logic that directly
//   interfaces with the game subsystems. Also this game class is the container
//   for all the renderer objects that are required to draw the scene, such as
//   shaders, viewports, frame buffers, etc. Finally this class also owns all
//   the GUI components that are created to handle user input.
//
// Revision History:
//   Initial Revision - 11/04/16
//
// Copyright (c) 2005-2016, Steven Ball
// ******************************************************************************

#pragma once

#include "Renderer/Renderer.h"
#include "gui/openglgui.h"
#include "Renderer/camera.h"
#include "VogueWindow.h"
#include "VogueSettings.h"
#include "VogueGUI.h"

#include "room/RoomManager.h"
#include "room/TileManager.h"
#include "Player/Player.h"
#include "Instance/InstanceManager.h"

#ifdef __linux__
typedef struct POINT {
  float x;
  float y;
} POINT;
#endif //__linux__

// Game modes
enum GameMode
{
	GameMode_Debug = 0,
	GameMode_Loading,
	GameMode_FrontEnd,
	GameMode_Game,
};

// Camera modes
enum CameraMode
{
	CameraMode_Debug = 0,
	CameraMode_Frontend,
	CameraMode_MouseRotate,
};

class VogueGame
{
public:
	/* Public methods */
	static VogueGame* GetInstance();

	// Creation
	void Create(VogueSettings* pVogueSettings);

	// Destruction
	void Destroy();

	// Blur
	void SetGlobalBlurAmount(float blurAmount);

	// Cinematic letterbox
	void OpenLetterBox();
	void CloseLetterBox();

	// Events
	void PollEvents();
	bool ShouldClose();

	// Window functionality
	int GetWindowCursorX();
	int GetWindowCursorY();
	void TurnCursorOn(bool resetCursorPosition, bool forceOn);
	void TurnCursorOff(bool forceOff);
	bool IsCursorOn();
	void ResizeWindow(int width, int height);
	void CloseWindow();
	void UpdateJoySticks();

	// Controls
	void UpdateControls(float dt);
	void UpdateKeyboardControls(float dt);
	void UpdateMouseControls(float dt);
	void UpdateGamePadControls(float dt);

	// Camera controls
	void UpdateCamera(float dt);

	// Input
	void KeyPressed(int key, int scancode, int mods);
	void KeyReleased(int key, int scancode, int mods);
	void CharacterEntered(int keyCode);
	void MouseLeftPressed();
	void MouseLeftReleased();
	void MouseRightPressed();
	void MouseRightReleased();
	void MouseMiddlePressed();
	void MouseMiddleReleased();
	void MouseScroll(double x, double y);
	void WrapCameraZoomValue();

	// Mouse controls
	void MouseCameraRotate();

	// Joystick controls
	void JoystickCameraMove(float dt);
	void JoystickCameraRotate(float dt);
	void JoystickCameraZoom(float dt);

	// Game functions
	void QuitToFrontEnd();
	void SetupDataForGame();
	void SetupDataForFrontEnd();
	void StartGameFromFrontEnd();
	void SetGameMode(GameMode mode);
	GameMode GetGameMode();
	void SetCameraMode(CameraMode mode);
	CameraMode GetCameraMode();

	// Updating
	void Update();
	void UpdateNamePicking();
	void UpdateLights(float dt);
	void UpdateGameGUI(float dt);

	// Rendering
	void PreRender();
	void BeginShaderRender();
	void EndShaderRender();
	void Render();
	void RenderSSAOTexture();
	void RenderFXAATexture();
	void RenderFirstPassFullScreen();
	void RenderSecondPassFullScreen();
	void RenderGUI();
	void RenderDebugInformation();

	// Accessors
	unsigned int GetDefaultViewport();
	Camera* GetGameCamera();
	VogueSettings* GetVogueSettings();
	VogueGUI* GetVogueGUI();

protected:
	/* Protected methods */
	VogueGame() {};
	VogueGame(const VogueGame&) {};
	VogueGame &operator=(const VogueGame&) {};
	
private:
	/* Private methods */

public:
	/* Public members */

protected:
	/* Protected members */

private:
	/* Private members */
	VogueWindow* m_pVogueWindow;
	VogueSettings* m_pVogueSettings;

	// Renderer
	Renderer* m_pRenderer;

	// GUI
	OpenGLGUI* m_pGUI;

	// Game camera
	Camera* m_pGameCamera;

	// Mouse picking
	int m_pickedObject;
	bool m_bNamePickingSelected;

	// Game mode
	GameMode m_gameMode;
	bool m_allowToChangeToGame;
	bool m_allowToChangeToFrontend;

	// Camera mode
	CameraMode m_cameraMode;
	CameraMode m_previousCameraMode;

	// Window width and height
	int m_windowWidth;
	int m_windowHeight;

	// Quit message
	bool m_bGameQuit;

	// Paused
	bool m_bPaused;

	// View ports
	unsigned int m_defaultViewport;

	// Fonts
	unsigned int m_defaultFont;

	// Lights
	unsigned int m_defaultLight;
	vec3 m_defaultLightPosition;
	vec3 m_defaultLightView;

	// Materials
	unsigned int m_defaultMaterial;

	// Frame buffers
	unsigned int m_SSAOFrameBuffer;
	unsigned int m_shadowFrameBuffer;
	unsigned int m_lightingFrameBuffer;
	unsigned int m_transparencyFrameBuffer;
	unsigned int m_waterReflectionFrameBuffer;
	unsigned int m_FXAAFrameBuffer;
	unsigned int m_firstPassFullscreenBuffer;
	unsigned int m_secondPassFullscreenBuffer;

	// Shaders
	unsigned int m_defaultShader;
	unsigned int m_phongShader;
	unsigned int m_SSAOShader;
	unsigned int m_shadowShader;
	unsigned int m_waterShader;
	unsigned int m_lightingShader;
	unsigned int m_cubeMapShader;
	unsigned int m_textureShader;
	unsigned int m_fxaaShader;
	unsigned int m_blurVerticalShader;
	unsigned int m_blurHorizontalShader;

	// Custom cursor textures
	unsigned int m_customCursorNormalBuffer;
	unsigned int m_customCursorClickedBuffer;
	unsigned int m_customCursorRotateBuffer;
	unsigned int m_customCursorZoomBuffer;

	// FPS and deltatime
#ifdef _WIN32
	LARGE_INTEGER m_fpsPreviousTicks;
	LARGE_INTEGER m_fpsCurrentTicks;
	LARGE_INTEGER m_fpsTicksPerSecond;
#else
	double m_fpsPreviousTicks;
	double m_fpsCurrentTicks;
#endif //_WIN32
	float m_deltaTime;
	float m_fps;

	// Initial starting wait timer
	float m_initialWaitTimer;
	float m_initialWaitTime;
	bool m_initialStartWait;

	// Keyboard flags
	bool m_bKeyboardForward;
	bool m_bKeyboardBackward;
	bool m_bKeyboardStrafeLeft;
	bool m_bKeyboardStrafeRight;
	bool m_bKeyboardLeft;
	bool m_bKeyboardRight;
	bool m_bKeyboardUp;
	bool m_bKeyboardDown;
	bool m_bKeyboardSpace;
	bool m_bKeyboardMenu;

	// Joystick flags
	bool m_bJoystickJump;

	// Camera movement
	bool m_bCameraRotate;
	int m_pressedX;
	int m_pressedY;
	int m_currentX;
	int m_currentY;
	float m_cameraDistance;
	float m_maxCameraDistance;

	// Movement
	bool m_keyboardMovement;
	bool m_gamepadMovement;

	// Blur
	float m_globalBlurAmount;

	// Cinematic letterbox mode
	float m_letterBoxRatio;

	// Water
	float m_elapsedWaterTime;

	// GUI Components
	bool m_GUICreated;

	// Toggle flags
	bool m_deferredRendering;
	bool m_multiSampling;
	bool m_ssao;
	bool m_dynamicLighting;
	bool m_blur;
	bool m_shadows;
	bool m_fullscreen;
	bool m_debugRender;
	bool m_wireframeRender;
	bool m_instanceRender;

	// Game objects
	// Qubicle binary manager
	QubicleBinaryManager* m_pQubicleBinaryManager;

	// Instance manager
	InstanceManager* m_pInstanceManager;

	// Room manager
	RoomManager *m_pRoomManager;

	// Tile manager
	TileManager *m_pTileManager;

	// Player
	Player* m_pPlayer;

	// GUI
	VogueGUI* m_pVogueGUI;

	// Singleton instance
	static VogueGame *c_instance;
};
