// ******************************************************************************
// Filename:    VogueSettings.h
// Project:     Vogue
// Author:      Steven Ball
//
// Purpose:
//   Vogue settings, initalized at the application creation and contains all of
//   the run time settings and configuration that is loaded for Vogue.
// 
// Revision History:
//   Initial Revision - 11/04/16
//
// Copyright (c) 2005-2016, Steven Ball
// ******************************************************************************

#pragma once

#include <string>
using namespace std;

class VogueGame;


class VogueSettings
{
public:
	/* Public methods */
	VogueSettings();
	~VogueSettings();

	// Load settings
	void LoadSettings();

	// Save settings
	void SaveSettings();

	// Load options
	void LoadOptions();

	// Save options
	void SaveOptions();

protected:
	/* Protected methods */

private:
	/* Private methods */

public:
	/* Public members */
	
	// Options ini file

	// Settings ini file
	// Graphics
	int m_windowWidth;
	int m_windowHeight;
	bool m_vsync;
	bool m_fullscreen;

	// Debug
	bool m_debugRendering;
	string m_gameMode;
	string m_version;

protected:
	/* Protected members */

private:
	/* Private members */
};
