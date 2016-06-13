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


enum eColourModifiers
{
	eColourModifiers_Skin = 0,
	eColourModifiers_Hair1,
	eColourModifiers_Hair2,
	eColourModifiers_NUM
};

enum ePlayerSex
{
	ePlayerSex_Male = 0,
	ePlayerSex_Female,
	ePlayerSex_Both
};

class Player
{
public:
	/* Public methods */
	Player(Renderer* pRenderer, QubicleBinaryManager* pQubicleBinaryManager);
	~Player();

	void LoadSkinColours();
	void LoadHairColours();

	void ModifyHead();
	void ModifyHair();
	void ModifyNose();
	void ModifyEars();
	void ReplaceHead();
	void ReplaceHair();
	void ReplaceNose();
	void ReplaceEars();
	void RandomizeParts();

	void UpdateDefaults();

	void ModifySkinColour();
	void ModifyHairColour();
	void SwapHairColours();
	void SetColourModifiers();

	// Rendering Helpers
	void CalculateWorldTransformMatrix();

	// Update
	void Update(float dt);

	// Render
    void Render();
	void RenderFace();
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
	int m_skinColourNum;
	int m_hairColourNum;

	// Max number of each body part
	int MAX_NUM_HEADS;
	int MAX_NUM_HAIRS;
	int MAX_NUM_NOSES;
	int MAX_NUM_EARS;
	int MAX_NUM_SKIN_COLOURS;
	int MAX_NUM_HAIR_COLOURS;

	// Colours
	Colour* m_pSkinColours;
	Colour* m_pHair1Colours;
	Colour* m_pHair2Colours;

	// Swap modifier for hair colours
	bool m_hairColourSwap;

	// Player sex
	ePlayerSex m_playerSex;

	// Colour modifiers
	float m_colourIdentifierRed[eColourModifiers_NUM];
	float m_colourIdentifierBlue[eColourModifiers_NUM];
	float m_colourIdentifierGreen[eColourModifiers_NUM];
	float m_colourModifierRed[eColourModifiers_NUM];
	float m_colourModifierBlue[eColourModifiers_NUM];
	float m_colourModifierGreen[eColourModifiers_NUM];

	// Qubicle binary file
	QubicleBinary* m_pHeadModel;
	QubicleBinary* m_pHairModel;
	QubicleBinary* m_pNoseModel;
	QubicleBinary* m_pEarsModel;

	// Voxel character
	VoxelCharacter* m_pVoxelCharacter;
};
