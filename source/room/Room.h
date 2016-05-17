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
#include "Corridor.h"

#include <stdio.h>
#include <vector>
using namespace std;

class RoomManager;
typedef vector<Door*> DoorList;
typedef vector<Corridor*> CorridorList;

class Room
{
public:
	/* Public methods */
	Room(Renderer* pRenderer, RoomManager* pRoomManager);
	~Room();

	// Clearing
	void ClearDoors();
	void ClearCorridors();

	// Accessors
	void SetPosition(vec3 pos);
	vec3 GetPosition();
	void SetDimensions(float length, float width, float height);
	float GetLength();
	float GetWidth();
	float GetHeight();
	float GetCorridorLength(eDirection direction);

	// Generation
	bool CanCreateConnection(eDirection direction);
	void CreateDoor(eDirection direction);
	void CreateCorridor(eDirection direction);

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

	// Dimensions
	float m_length;
	float m_width;
	float m_height;

	// Chunk position
	vec3 m_position;

	// List of doors
	DoorList m_vpDoorList;

	// List of corridors
	CorridorList m_vpCorridorList;
};
