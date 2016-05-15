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
#include "../Maths/3dmaths.h"
#include "../Renderer/Renderer.h"

#include <stdio.h>
#include <vector>
using namespace std;

typedef vector<Room*> RoomList;


class RoomManager
{
public:
	/* Public methods */
	RoomManager(Renderer* pRenderer);
	~RoomManager();

	// Clearing
	void ClearRooms();

	// Generation
	void GenerateNewLayout();
	void CreateRandomRoom(Room* pRoomConnection, eDirection connectedDirection, int roomDepth);
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
