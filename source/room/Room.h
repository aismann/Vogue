// ******************************************************************************
// Filename:    Room.h
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
	int GetRoomDepth();
	void SetRoomDepth(int depth);
	float GetCorridorLength(eDirection direction);

	// Validation
	bool IsPointInsideRoom(vec3 point);

	// Generation
	bool CanCreateConnection(eDirection direction);
	bool IsRoomFullOfDoors();
	bool IsRoomAbleToCreateMoreConnections();
	void SetRoomAbleToCreateMoreConnections(bool able);
	void CreateDoor(eDirection direction);
	void CreateCorridor(eDirection direction, float corridorLengthAmount);

	// Update
	void Update(float dt);
	void UpdateRoomPlanes();;

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

	// Room depth, from the initial starting room
	int m_roomDepth;

	// Flag to indicate that we tried a number of times to create a connecting room and couldn't
	bool m_ableToCreateConnectingRooms;

	// Bounding region for the room (to stop room overlaps)
	Plane3D m_planes[6];

	// List of doors
	DoorList m_vpDoorList;

	// List of corridors
	CorridorList m_vpCorridorList;
};
