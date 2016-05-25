// ******************************************************************************
// Filename:    Tile.h
// Project:     Vogue
// Author:      Steven Ball
//
// Purpose:
//
// Revision History:
//   Initial Revision - 22/04/16
//
// Copyright (c) 2005-2016, Steven Ball
// ******************************************************************************

#pragma once

#include "../Maths/3dmaths.h"
#include "../Renderer/Renderer.h"


class Tile
{
public:
	/* Public methods */
	Tile(Renderer* pRenderer);
	~Tile();

	// Update
	void Update(float dt);

	// Render
    void Render();

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
};
