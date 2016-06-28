// ******************************************************************************
// Filename:    VogueGUI.h
// Project:     Vogue
// Author:      Steven Ball
//
// Revision History:
//   Initial Revision - 28/05/16
//
// Copyright (c) 2005-2016, Steven Ball
// ******************************************************************************

#include "VogueGame.h"
#include "Player/Player.h"


class VogueGUI
{
public:
	/* Public methods */
	VogueGUI(Renderer* pRenderer, OpenGLGUI* pGUI, int width, int height);
	~VogueGUI();

	// Creation
	void CreateGUI();

	// Destruction
	void DestroyGUI();

	// Setup
	void SetupGUI();

	// Resize
	void SignalResize(int width, int height);

	// Skinning
	void SkinGUI();
	void UnSkinGUI();

	// GUI Functionality
	void GUITurnOffCursor();
	void ShowGUI();
	void HideGUI();

	// Update
	void UpdateGUI(float dt);

	// Game functionality
	ePlayerSex GetPlayerSex();

protected:
	/* Protected methods */
	static void _GenerateFromSeedButtonClicked(void *apData);
	void GenerateFromSeedButtonClicked();

	static void _RandomizeButtonClicked(void *apData);
	void RandomizeButtonClicked();

private:
	/* Private methods */

public:
	/* Public members */

protected:
	/* Protected members */

private:
	/* Private members */
	Renderer* m_pRenderer;
	OpenGLGUI* m_pGUI;

	// Window width and height
	int m_windowWidth;
	int m_windowHeight;

	// Fonts
	unsigned int m_defaultGUIFont;

	// GUI components
	// Debug
	GUIWindow* m_pMainWindow;
	CheckBox* m_pShadowsCheckBox;
	CheckBox* m_pSSAOCheckBox;
	CheckBox* m_pDynamicLightingCheckBox;
	CheckBox* m_pWireframeCheckBox;
	CheckBox* m_pMSAACheckBox;
	CheckBox* m_pDeferredCheckBox;
	CheckBox* m_pBlurCheckBox;
	CheckBox* m_pDebugRenderCheckBox;
	CheckBox* m_pInstanceRenderCheckBox;

	// Params
	OptionController* m_pGenderOptionController;
	OptionBox* m_pGenderMaleOptionBox;
	OptionBox* m_pGenderFemaleOptionBox;
	OptionBox* m_pGenderBothOptionBox;
	Button* m_pGenerateFromSeedButton;
	Button* m_pRandomizeButton;
	TextBox* m_pSeedTextbox;

	// SKin
	GUIWindow* m_pSkinWindow;

	// Friend class
	friend class VogueGame;
};