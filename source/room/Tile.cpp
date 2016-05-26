// ******************************************************************************
// Filename:    Tile.cpp
// Project:     Vogue
// Author:      Steven Ball
//
// Revision History:
//   Initial Revision - 22/04/16
//
// Copyright (c) 2005-2016, Steven Ball
// ******************************************************************************

#include "Tile.h"
#include "../utils/Random.h"

#include <vector>
#include <algorithm>
using namespace std;


Tile::Tile(Renderer* pRenderer, QubicleBinaryManager* pQubicleBinaryManager)
{
	m_pRenderer = pRenderer;
	m_pQubicleBinaryManager = pQubicleBinaryManager;

	m_pTileFile = m_pQubicleBinaryManager->GetQubicleBinaryFile("media/gamedata/tiles/wood_tile.qb", false);
}

Tile::~Tile()
{
}

// Accessors
void Tile::SetPosition(vec3 pos)
{
	m_position = pos;
}

vec3 Tile::GetPosition()
{
	return m_position;
}

// Update
void Tile::Update(float dt)
{
}

// Render
void Tile::Render()
{
	m_pRenderer->PushMatrix();
		m_pRenderer->TranslateWorldMatrix(m_position.x, m_position.y, m_position.z);

		// Scale down
		m_pRenderer->ScaleWorldMatrix(0.03125f, 0.03125f, 0.03125f);

		Colour OulineColour(1.0f, 1.0f, 0.0f, 1.0f);
		m_pTileFile->Render(false, false, false, OulineColour);
	m_pRenderer->PopMatrix();

	//RenderDebug();
}

void Tile::RenderDebug()
{
	m_pRenderer->SetRenderMode(RM_WIREFRAME);
	m_pRenderer->SetCullMode(CM_NOCULL);
	m_pRenderer->SetLineWidth(1.0f);

	float length = 0.5f;
	float height = 0.05f;
	float width = 0.5f;
	m_pRenderer->PushMatrix();
		m_pRenderer->TranslateWorldMatrix(m_position.x, m_position.y, m_position.z);
		m_pRenderer->ImmediateColourAlpha(0.0f, 1.0f, 1.0f, 1.0f);

		m_pRenderer->EnableImmediateMode(IM_QUADS);
			m_pRenderer->ImmediateNormal(0.0f, 0.0f, -1.0f);
			m_pRenderer->ImmediateVertex(length, -height, -width);
			m_pRenderer->ImmediateVertex(-length, -height, -width);
			m_pRenderer->ImmediateVertex(-length, height, -width);
			m_pRenderer->ImmediateVertex(length, height, -width);

			m_pRenderer->ImmediateNormal(0.0f, 0.0f, 1.0f);
			m_pRenderer->ImmediateVertex(-length, -height, width);
			m_pRenderer->ImmediateVertex(length, -height, width);
			m_pRenderer->ImmediateVertex(length, height, width);
			m_pRenderer->ImmediateVertex(-length, height, width);

			m_pRenderer->ImmediateNormal(1.0f, 0.0f, 0.0f);
			m_pRenderer->ImmediateVertex(length, -height, width);
			m_pRenderer->ImmediateVertex(length, -height, -width);
			m_pRenderer->ImmediateVertex(length, height, -width);
			m_pRenderer->ImmediateVertex(length, height, width);

			m_pRenderer->ImmediateNormal(-1.0f, 0.0f, 0.0f);
			m_pRenderer->ImmediateVertex(-length, -height, -width);
			m_pRenderer->ImmediateVertex(-length, -height, width);
			m_pRenderer->ImmediateVertex(-length, height, width);
			m_pRenderer->ImmediateVertex(-length, height, -width);

			m_pRenderer->ImmediateNormal(0.0f, -1.0f, 0.0f);
			m_pRenderer->ImmediateVertex(-length, -height, -width);
			m_pRenderer->ImmediateVertex(length, -height, -width);
			m_pRenderer->ImmediateVertex(length, -height, width);
			m_pRenderer->ImmediateVertex(-length, -height, width);

			m_pRenderer->ImmediateNormal(0.0f, 1.0f, 0.0f);
			m_pRenderer->ImmediateVertex(length, height, -width);
			m_pRenderer->ImmediateVertex(-length, height, -width);
			m_pRenderer->ImmediateVertex(-length, height, width);
			m_pRenderer->ImmediateVertex(length, height, width);
		m_pRenderer->DisableImmediateMode();
	m_pRenderer->PopMatrix();

	m_pRenderer->SetCullMode(CM_BACK);
}