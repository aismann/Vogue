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

#include <stdio.h>
#include <vector>
using namespace std;

typedef vector<Room*> ChunkList;


class RoomManager
{
public:
	/* Public methods */
	RoomManager();
	~RoomManager();

	void Update(float dt);

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

	ChunkList m_vpChunkList;
};
