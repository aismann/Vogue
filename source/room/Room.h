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
#include "Door.h"

#include <stdio.h>
#include <vector>
using namespace std;

class RoomManager;
typedef vector<Door*> DoorList;

class Room
{
public:
	/* Public methods */
	Room(Renderer* pRenderer, RoomManager* pRoomManager);
	~Room();

	// Clearing
	void ClearDoors();

	// Accessors
	void SetPosition(vec3 pos);
	vec3 GetPosition();
	void SetDimensions(float length, float width, float height);
	float GetLength();
	float GetWidth();
	float GetHeight();

	// Generation
	bool CanCreateDoor(eDirection doorDirection);
	void CreateDoor(eDirection doorDirection);

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
	RoomManager* m_pRoomManager;

	float m_length;
	float m_width;
	float m_height;

	// Chunk position
	vec3 m_position;

	DoorList m_vpDoorList;
};
