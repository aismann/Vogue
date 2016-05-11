// ******************************************************************************
// Filename:    VogueSettings.cpp
// Project:     Vogue
// Author:      Steven Ball
//
// Revision History:
//   Initial Revision - 11/04/16
//
// Copyright (c) 2005-2016, Steven Ball
// ******************************************************************************

#include "VogueSettings.h"

#include <iostream>
#include "ini/INIReader.h"

#include <fstream>
#include <ostream>
#include <iostream>
#include <string>
using namespace std;


VogueSettings::VogueSettings()
{
}

VogueSettings::~VogueSettings()
{
}

// Load settings
void VogueSettings::LoadSettings()
{
	string settingsIniFile = "media/config/settings.ini";
	INIReader reader(settingsIniFile);

	if (reader.ParseError() < 0)
	{
		cout << "Can't load '" << settingsIniFile << "'\n";
		return;
	}
}

// Save settings
void VogueSettings::SaveSettings()
{
}

// Load options
void VogueSettings::LoadOptions()
{
	string optionsIniFile = "media/config/options.ini";
	INIReader reader(optionsIniFile);

	if (reader.ParseError() < 0)
	{
		cout << "Can't load '" << optionsIniFile << "'\n";
		return;
	}
}

// Save options
void VogueSettings::SaveOptions()
{
	ofstream file;

	// Open the file
	string optionsIniFile = "media/config/options.ini";
	file.open(optionsIniFile.c_str(), ios::out);
}