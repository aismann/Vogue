// ******************************************************************************
// Filename:    Door.h
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


class Door
{
public:
	/* Public methods */
	Door(Renderer* pRenderer);
	~Door();

	// Accessors
	void SetPosition(vec3 pos);
	void SetDimensions(float length, float width, float height);

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

	float m_length;
	float m_width;
	float m_height;

	// Chunk position
	vec3 m_position;
};
