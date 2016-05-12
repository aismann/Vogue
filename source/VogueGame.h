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
	void DestroyGUI();

	// Blur
	void SetGlobalBlurAmount(float blurAmount);

	// Cinematic letterbox
	void OpenLetterBox();
	void CloseLetterBox();

	// Paperdoll rendering
	void SetPaperdollRotation(float rotation);
	void RotatePaperdollModel(float rot);
	unsigned int GetDynamicPaperdollTexture();

	// Portrait
	unsigned int GetDynamicPortraitTexture();

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
	void UpdateCameraModeSwitching();
	void InitializeCameraRotation();
	void UpdateCameraAutoCamera(float dt, bool updateCameraPosition);
	void UpdateCameraFirstPerson(float dt);
	void UpdateCameraNPCDialog(float dt);
	void UpdateCameraEnemyTarget(float dt);
	void UpdateCameraClipping(float dt);
	void UpdateCameraZoom(float dt);
	bool ShouldRestorePreviousCameraMode();
	void SavePreviousCameraMode();
	void RestorePreviousCameraMode();

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
	void PlayerRespawned();
	void SetGameMode(GameMode mode);
	GameMode GetGameMode();
	void SetCameraMode(CameraMode mode);
	CameraMode GetCameraMode();

	// Updating
	void Update();
	void UpdateNamePicking();
	void UpdateLights(float dt);
	void UpdateGUI(float dt);
	void UpdateGameGUI(float dt);

	// Rendering
	void PreRender();
	void BeginShaderRender();
	void EndShaderRender();
	void Render();

	// GUI
	void CreateGUI();
	void SetupGUI();
	void SkinGUI();
	void UnSkinGUI();
	void GUITurnOffCursor();
	void ShowGUI();
	void HideGUI();

	// Accessors
	unsigned int GetDefaultViewport();
	Camera* GetGameCamera();
	VogueSettings* GetVogueSettings();

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
	bool m_shouldRestorePreviousCameraMode;

	// Window width and height
	int m_windowWidth;
	int m_windowHeight;

	// Quit message
	bool m_bGameQuit;

	// Paused
	bool m_bPaused;

	// View ports
	unsigned int m_defaultViewport;
	unsigned int m_firstpersonViewport;
	unsigned int m_paperdollViewport;
	unsigned int m_portraitViewport;

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
	unsigned int m_paperdollBuffer;
	unsigned int m_paperdollSSAOTextureBuffer;
	unsigned int m_portraitBuffer;
	unsigned int m_portraitSSAOTextureBuffer;

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
	unsigned int m_paperdollShader;

	// Custom cursor textures
	unsigned int m_customCursorNormalBuffer;
	unsigned int m_customCursorClickedBuffer;
	unsigned int m_customCursorRotateBuffer;
	unsigned int m_customCursorZoomBuffer;

	// Paperdoll viewport
	int m_paperdollViewportX;
	int m_paperdollViewportY;
	int m_paperdollViewportWidth;
	int m_paperdollViewportHeight;

	// Portrait viewport
	int m_portraitViewportX;
	int m_portraitViewportY;
	int m_portraitViewportWidth;
	int m_portraitViewportHeight;

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

	// Custom cursor
	bool m_bPressedCursorDown;
	bool m_bCustomCursorOn;

	// Combat flags
	bool m_bAttackPressed_Mouse;
	bool m_bAttackReleased_Mouse;
	bool m_bCanDoAttack_Mouse;
	bool m_bAttackPressed_Joystick;
	bool m_bAttackReleased_Joystick;
	bool m_bCanDoAttack_Joystick;
	bool m_bTargetEnemyPressed_Joystick;
	bool m_bTargetEnemyReleased_Joystick;

	// Camera movement
	bool m_bCameraRotate;
	int m_pressedX;
	int m_pressedY;
	int m_currentX;
	int m_currentY;
	float m_cameraDistance;
	float m_maxCameraDistance;

	// Auto camera mode
	vec3 m_cameraPosition_AutoModeCached;
	vec3 m_cameraPosition_AutoMode;
	vec3 m_cameraBehindPlayerPosition;
	float m_autoCameraMovingModifier;

	// NPC dialog camera mode
	vec3 m_targetCameraView_NPCDialog;
	vec3 m_targetCameraPosition_NPCDialog;

	// Enemy target camera mode
	float m_targetCameraXAxisAmount;
	float m_targetCameraXAxisAmount_Target;
	float m_targetCameraYRatio;
	float m_targetCameraForwardRatio;

	// Camera clipping
	vec3 m_targetCameraPositionBeforeClipping;
	vec3 m_cameraPositionAfterClipping;

	// Player movement
	bool m_keyboardMovement;
	bool m_gamepadMovement;
	vec3 m_movementDirection;
	float m_movementSpeed;
	float m_movementDragTime;
	float m_movementIncreaseTime;
	float m_maxMovementSpeed;
	float m_movementStopThreshold;

	// Blur
	float m_globalBlurAmount;

	// Cinematic letterbox mode
	float m_letterBoxRatio;

	// Water
	float m_elapsedWaterTime;

	// Paperdoll rendering
	float m_paperdollRenderRotation;

	// GUI Components
	bool m_GUICreated;

	// Toggle flags
	bool m_deferredRendering;
	bool m_modelWireframe;
	int m_modelAnimationIndex;
	bool m_multiSampling;
	bool m_ssao;
	bool m_blur;
	bool m_shadows;
	bool m_dynamicLighting;
	bool m_animationUpdate;
	bool m_fullscreen;
	bool m_debugRender;
	bool m_instanceRender;
	bool m_fogRender;
	bool m_waterRender;

	// Singleton instance
	static VogueGame *c_instance;
};
