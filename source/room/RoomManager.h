// ******************************************************************************
// Filename:    RoomManager.h
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

#include "Room.h"
#include "Corridor.h"
#include "../Maths/3dmaths.h"
#include "../Renderer/Renderer.h"

#include <stdio.h>
#include <vector>
using namespace std;

typedef vector<Room*> RoomList;
typedef vector<Corridor*> CorridorList;


class RoomManager
{
public:
	/* Public methods */
	RoomManager(Renderer* pRenderer);
	~RoomManager();

	// Clearing
	void ClearRooms();

	// Accessors
	int GetNumRooms();

	// Validation
	bool DoesOverlap(vec3 position1, float length1, float width1, float height1, vec3 position2, float length2, float width2, float height2);
	bool DoesRoomOverlap(vec3 position, float length, float width, float height);

	// Generation
	void GenerateNewLayout();
	Room* CreateRandomRoom(Room* pRoomConnection, eDirection connectedDirection, float corridorLengthAmount, int roomDepth);
	void CreateConnectedRoom();

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

	// List of rooms
	RoomList m_vpRoomList;
};
