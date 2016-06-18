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
	ePlayerSex_Female
};

class Player
{
public:
	/* Public methods */
	Player(Renderer* pRenderer, QubicleBinaryManager* pQubicleBinaryManager);
	~Player();

	void LoadSkinColours();
	void LoadHairColours();
	void LoadEyesNames();

	void ModifyHead();
	void ModifyHair();
	void ModifyFacialHair();
	void ModifyNose();
	void ModifyEars();
	void ModifyEyes();
	void ModifyGlasses();
	void ModifyBody();
	void ModifyLegs();
	void ModifyRightHand();
	void ModifyLeftHand();
	void ModifyRightShoulder();
	void ModifyLeftShoulder();
	void ModifyRightFoot();
	void ModifyLeftFoot();
	void ReplaceHead();
	void ReplaceHair();
	void ReplaceFacialHair();
	void ReplaceNose();
	void ReplaceEars();
	void ReplaceEyes();
	void ReplaceGlasses();
	void ReplaceBody();
	void ReplaceLegs();
	void ReplaceRightHand();
	void ReplaceLeftHand();
	void ReplaceRightShoulder();
	void ReplaceLeftShoulder();
	void ReplaceRightFoot();
	void ReplaceLeftFoot();
	void RandomizeParts();

	// Default scale and offsets
	void UpdateDefaults();

	// Colour modifiers
	void ModifySkinColour();
	void ModifyHairColour();
	void SwapHairColours();
	void SetColourModifiers();

	// Rendering Helpers
	void CalculateWorldTransformMatrix();

	// Rendering modes
	void SetWireFrameRender(bool wireframe);

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
	int m_facialHairNum;
	int m_noseNum;
	int m_earsNum;
	int m_eyesNum;
	int m_glassesNum;
	int m_bodyNum;
	int m_legsNum;
	int m_rightHandNum;
	int m_leftHandNum;
	int m_rightShoulderNum;
	int m_leftShoulderNum;
	int m_rightFootNum;
	int m_leftFootNum;
	int m_skinColourNum;
	int m_hairColourNum;

	// Max number of each body part
	int MAX_NUM_HEADS;
	int MAX_NUM_HAIRS_MALE;
	int MAX_NUM_HAIRS_FEMALE;
	int MAX_NUM_FACIAL_HAIRS;
	int MAX_NUM_NOSES;
	int MAX_NUM_EARS;
	int MAX_NUM_EYES;
	int MAX_NUM_GLASSES;
	int MAX_NUM_BODY_MALE;
	int MAX_NUM_BODY_FEMALE;
	int MAX_NUM_LEGS_MALE;
	int MAX_NUM_LEGS_FEMALE;
	int MAX_NUM_RIGHT_HAND;
	int MAX_NUM_LEFT_HAND;
	int MAX_NUM_RIGHT_SHOULDER;
	int MAX_NUM_LEFT_SHOULDER;
	int MAX_NUM_RIGHT_FOOT;
	int MAX_NUM_LEFT_FOOT;
	int MAX_NUM_SKIN_COLOURS;
	int MAX_NUM_HAIR_COLOURS;

	// Colours
	Colour* m_pSkinColours;
	Colour* m_pHair1Colours;
	Colour* m_pHair2Colours;

	// Eyes names
	string* m_pEyesNames;

	// Swap modifier for hair colours
	bool m_hairColourSwap;

	// Flag to indicate if we will lock both left and right to use the same matching models
	bool m_lockSymetricalSides;

	// Player sex
	ePlayerSex m_playerSex;

	// Colour modifiers
	unsigned int m_colourIdentifierRed[eColourModifiers_NUM];
	unsigned int m_colourIdentifierBlue[eColourModifiers_NUM];
	unsigned int m_colourIdentifierGreen[eColourModifiers_NUM];
	unsigned int m_colourModifierRed[eColourModifiers_NUM];
	unsigned int m_colourModifierBlue[eColourModifiers_NUM];
	unsigned int m_colourModifierGreen[eColourModifiers_NUM];

	// Qubicle binary file
	QubicleBinary* m_pHeadModel;
	QubicleBinary* m_pHairModel;
	QubicleBinary* m_pFacialHairModel;
	QubicleBinary* m_pNoseModel;
	QubicleBinary* m_pEarsModel;
	QubicleBinary* m_pGlassesModel;
	QubicleBinary* m_pBodyModel;
	QubicleBinary* m_pLegsModel;
	QubicleBinary* m_pRightHandModel;
	QubicleBinary* m_pLeftHandModel;
	QubicleBinary* m_pRightShoulderModel;
	QubicleBinary* m_pLeftShoulderModel;
	QubicleBinary* m_pRightFootModel;
	QubicleBinary* m_pLeftFootModel;

	// Voxel character
	VoxelCharacter* m_pVoxelCharacter;
};
