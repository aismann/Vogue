// ******************************************************************************
// Filename:    Player.h
// Project:     Vogue
// Author:      Steven Ball
//
// Purpose:
//
// Revision History:
//   Initial Revision - 25/05/16
//
// Copyright (c) 2005-2016, Steven Ball
// ******************************************************************************

#pragma once

#include "../Maths/3dmaths.h"
#include "../Renderer/Renderer.h"
#include "../models/modelloader.h"


class Player
{
public:
	/* Public methods */
	Player(Renderer* pRenderer, QubicleBinaryManager* pQubicleBinaryManager);
	~Player();

	void ModifyHead();
	void ModifyHair();
	void ModifyNose();
	void ModifyEars();
	void UpdateDefaults();
	void ModifySkinColor();
	void SetSkinColor();

	// Rendering Helpers
	void CalculateWorldTransformMatrix();

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

	// Player position and movement variables
	vec3 m_position;
	vec3 m_velocity;
	vec3 m_force;

	// Store previous position each frame after we have worked out the new position
	// Used for collision and other movement calculations
	vec3 m_previousPosition;

	// The direction of gravity for the player
	vec3 m_gravityDirection;

	// Local axis
	vec3 m_forward;
	vec3 m_right;
	vec3 m_up;

	// Players world matrix
	Matrix4x4 m_worldMatrix;

	// Body parts indices
	int m_headNum;
	int m_hairNum;
	int m_noseNum;
	int m_earsNum;
	float m_skinRed;
	float m_skinBlue;
	float m_skinGreen;

	// Qubicle binary file
	QubicleBinary* m_pPlayerModel;
	QubicleBinary* m_pHeadModel;
	QubicleBinary* m_pHairModel;
	QubicleBinary* m_pNoseModel;
	QubicleBinary* m_pEarsModel;
};
