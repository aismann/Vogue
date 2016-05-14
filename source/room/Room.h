// ******************************************************************************
// Filename:    Room.h
// Project:     Vox
// Author:      Steven Ball
//
// Purpose:
//
// Revision History:
//   Initial Revision - 13/04/16
//
// Copyright (c) 2005-2016, Steven Ball
// ******************************************************************************

#pragma once

#include "../Maths/3dmaths.h"
#include "../Renderer/Renderer.h"

#include <stdio.h>

class Room
{
public:
	/* Public methods */
	Room(Renderer* pRenderer);
	~Room();

	void Update(float dt);

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

	float m_length;
	float m_width;
	float m_height;

	// Chunk position
	vec3 m_position;
};
