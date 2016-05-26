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
#include "../models/modelloader.h"


class Tile
{
public:
	/* Public methods */
	Tile(Renderer* pRenderer, QubicleBinaryManager* pQubicleBinaryManager);
	~Tile();

	// Accessors
	void SetPosition(vec3 pos);
	vec3 GetPosition();

	// Update
	void Update(float dt);

	// Render
    void Render();
	void RenderDebug();

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
	QubicleBinaryManager* m_pQubicleBinaryManager;

	// Tile position
	vec3 m_position;

	// Qubicle binary file
	QubicleBinary* m_pTileFile;
};
