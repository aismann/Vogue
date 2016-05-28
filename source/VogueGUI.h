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

protected:
	/* Protected methods */

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
	GUIWindow* m_pMainWindow;
};