// ******************************************************************************
// Filename:    main.cpp
// Project:     Vogue
// Author:      Steven Ball
//
// Purpose:
//   The main entry point for the application. Creates the initial instances of
//   the container classes, iterates the game loop, polls events/input and 
//   detects for game closure to cleanup the with the destruction.
// 
// Revision History:
//   Initial Revision - 11/04/16
//
// Copyright (c) 2005-2016, Steven Ball
// ******************************************************************************

#include "VogueGame.h"

int main(void)
{
	/* Load the settings */
	VogueSettings* m_pVogueSettings = new VogueSettings();
	m_pVogueSettings->LoadSettings();
	m_pVogueSettings->LoadOptions();

	/* Initialize and create the VogueGame object */
	VogueGame* pVogueGame = VogueGame::GetInstance();
	pVogueGame->Create(m_pVogueSettings);

	/* Loop until the user closes the window or application */
	while (!pVogueGame->ShouldClose())
	{
		/* Poll input events*/
		pVogueGame->PollEvents();

		/* Update joysticks */
		pVogueGame->UpdateJoySticks();

		/* Update */
		pVogueGame->Update();

		/* PreRender */
		pVogueGame->PreRender();

		/* Render */
		pVogueGame->Render();
	}

	/* Cleanup */
	pVogueGame->Destroy();

	/* Exit */
	exit(EXIT_SUCCESS);
}
