// ******************************************************************************
// Filename:    Door.h
// Project:     Vogue
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
#include <vector>
using namespace std;

enum eDirection
{
	eDirection_Up = 0,
	eDirection_Down,
	eDirection_Right,
	eDirection_Left,

	eDirection_NONE,
};


class Door
{
public:
	/* Public methods */
	Door(Renderer* pRenderer);
	~Door();

	// Accessors
	void SetPosition(vec3 pos);
	vec3 GetPosition();
	void SetDimensions(float length, float width, float height);
	float GetLength();
	float GetWidth();
	float GetHeight();
	void SetDirection(eDirection direction);
	eDirection GetDirection();

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

	// Dimensions
	float m_length;
	float m_width;
	float m_height;

	// Door direction
	eDirection m_direction;

	// Door position
	vec3 m_position;
};
