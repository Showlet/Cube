#include "chunk.h"
#include <iostream>

Chunk::Chunk() :m_blocks(CHUNK_SIZE_X, CHUNK_SIZE_Y, CHUNK_SIZE_Z), m_iscreated(false), m_isDirty(true), m_chunkMesh(), m_transparentMesh(), m_position(0, 0, 0), m_save(false)
{
	m_blocks.Reset(BTYPE_AIR);
	m_defaultBlock = BTYPE_AIR;

}

Chunk::~Chunk()
{

}

void Chunk::RemoveBloc(int x, int y, int z)
{
	m_blocks.Set(x, y, z, BTYPE_AIR);
	m_isDirty = true;
	m_save = true;

	// Si on efface un cube pres d'un chunk il faut le redender
	// Aussi sinon cetaine face qui ne s'afficheront pas
	if (x == 0 && m_negativeX)
		m_negativeX->m_isDirty = true;

	else if (x == CHUNK_SIZE_X - 1 && m_positiveX)
		m_positiveX->m_isDirty = true;

	if (z == 0 && m_negativeZ)
		m_negativeZ->m_isDirty = true;

	else if (z == CHUNK_SIZE_Z - 1 && m_positiveZ)
		m_positiveZ->m_isDirty = true;
}

void Chunk::SetBlock(int x, int y, int z, BlockType type)
{

	if (x >= 0 && y >= 0 && z >= 0 && x < CHUNK_SIZE_X && y < CHUNK_SIZE_Y && z < CHUNK_SIZE_Z)
	{
		m_blocks.Set(x, y, z, type);
		m_isDirty = true;

	}
	else if (x == -1 && m_negativeX)
	{
		m_negativeX->m_blocks.Set(CHUNK_SIZE_X - 1, y, z, type);
		m_negativeX->m_isDirty = true;

	}
	else if (x == CHUNK_SIZE_X && m_positiveX)
	{
		m_positiveX->m_blocks.Set(0, y, z, type);
		m_positiveX->m_isDirty = true;

	}
	else if (z == -1 && m_negativeZ)
	{
		m_negativeZ->m_blocks.Set(x, y, CHUNK_SIZE_Z - 1, type);
		m_negativeZ->m_isDirty = true;

	}
	else if (z == CHUNK_SIZE_Z && m_positiveZ)
	{
		m_positiveZ->m_blocks.Set(x, y, 0, type);
		m_positiveZ->m_isDirty = true;

	}
}

void Chunk::PlaceBlock(int x, int y, int z, BlockType type)
{
	m_save = true;
	if (m_blocks.Get(x, y, z) == BTYPE_AIR)
		SetBlock(x, y, z, type);


}

const BlockType& Chunk::GetBlock(int x, int y, int z) const
{
	if (x >= 0 && y >= 0 && z >= 0 && x < CHUNK_SIZE_X && y < CHUNK_SIZE_Y && z < CHUNK_SIZE_Z)
		return m_blocks.Get(x, y, z);

	else if (x == -1 && m_negativeX)
		return m_negativeX->GetBlock(CHUNK_SIZE_X - 1, y, z);

	else if (x == CHUNK_SIZE_X && m_positiveX)
		return m_positiveX->GetBlock(0, y, z);

	else if (z == -1 && m_negativeZ)
		return m_negativeZ->GetBlock(x, y, CHUNK_SIZE_Z - 1);

	else if (z == CHUNK_SIZE_Z && m_positiveZ)
		return m_positiveZ->GetBlock(x, y, 0);

	else
		return m_defaultBlock;
}

void Chunk::SetPosition(int x, int y, int z)
{
	m_position.x = x;
	m_position.y = y;
	m_position.z = z;
}

const Vector3<float> &Chunk::GetPosition() const
{
	return m_position;
}

void Chunk::Update(BlockInfo* &binfo)
{
	// Update mesh
	if (m_isDirty)
	{
		m_isDirty = false;
		int maxVertexCount = (CHUNK_SIZE_X * CHUNK_SIZE_Y * CHUNK_SIZE_Z) * (6 * 4);

		//On creer un buffer pour les solid et un autre pour les transparents 
		ChunkMesh::VertexData * vds = new ChunkMesh::VertexData[maxVertexCount];
		ChunkMesh::VertexData * vdt = new ChunkMesh::VertexData[maxVertexCount];
		int count_s = 0;
		int count_t = 0;

		//Ajoute les cubes sans transparence en premier
		for (int x = 0; x < CHUNK_SIZE_X; ++x)
			for (int z = 0; z < CHUNK_SIZE_Z; ++z)
				for (int y = 0; y < CHUNK_SIZE_Y; ++y)
				{
					if (count_s > USHRT_MAX)
						break;
					if (count_t > USHRT_MAX)
						break;

					BlockType bt = GetBlock(x, y, z);

					if (bt != BTYPE_AIR && bt != BTYPE_WATER && bt != BTYPE_LEAVE)
						AddBlockToMesh(vds, count_s, binfo[bt], Vector3<float>(x + m_position.x, y + m_position.y, z + m_position.z));

					if (bt == BTYPE_WATER || bt == BTYPE_LEAVE || bt == BTYPE_RWATER1 || bt == BTYPE_RWATER2 || bt == BTYPE_RWATER3)
						AddBlockToMesh(vdt, count_t, binfo[bt], Vector3<float>(x + m_position.x, y + m_position.y, z + m_position.z));

				}
		if (count_s > USHRT_MAX)
		{
			count_s = USHRT_MAX;
			std::cout << "[ Chunk :: Update ] Chunk data truncaned , too much vertices to have a 16 bit index " << std::endl;
		}
		if (count_t > USHRT_MAX)
		{
			count_t = USHRT_MAX;
			std::cout << "[ Chunk :: Update ] Chunk data truncaned , too much vertices to have a 16 bit index " << std::endl;
		}

		m_chunkMesh.SetMeshData(vds, count_s);
		m_transparentMesh.SetMeshData(vdt, count_t);
		delete[] vds;
		delete[] vdt;
		if (m_isDirty)
			Update(binfo);
	}
}

void Chunk::AddBlockToMesh(ChunkMesh::VertexData * &vd, int& count, const BlockInfo &binfo, const Vector3<float> &Blockpos)
{
	BlockType type = binfo.GetType();
	if (type == BTYPE_WATER)
		CheckWater(Vector3<float>((int)Blockpos.x % CHUNK_SIZE_X, (int)Blockpos.y % CHUNK_SIZE_Y, (int)Blockpos.z % CHUNK_SIZE_Z));

	// face
	if (CheckFace(type, Blockpos - m_position, Vector3<float>(0, 0, -1)))
	{
		float light = CheckLightning(Blockpos - m_position, Vector3<float>(0, 0, -1));

		vd[count++] = ChunkMesh::VertexData(Blockpos.x + 0.f, Blockpos.y + 0.f, Blockpos.z + 0.f, light, light, light, binfo.u + binfo.w * .50f, binfo.v + binfo.h * .50f, type);
		vd[count++] = ChunkMesh::VertexData(Blockpos.x + 0.f, Blockpos.y + 1.f, Blockpos.z + 0.f, light, light, light, binfo.u + binfo.w * .50f, binfo.v + binfo.h * .75f, type);
		vd[count++] = ChunkMesh::VertexData(Blockpos.x + 1.f, Blockpos.y + 1.f, Blockpos.z + 0.f, light, light, light, binfo.u + binfo.w * .00f, binfo.v + binfo.h * .75f, type);
		vd[count++] = ChunkMesh::VertexData(Blockpos.x + 1.f, Blockpos.y + 0.f, Blockpos.z + 0.f, light, light, light, binfo.u + binfo.w * .00f, binfo.v + binfo.h * .50f, type);

	}
	// Droite
	if (CheckFace(type, Blockpos - m_position, Vector3<float>(1, 0, 0)))
	{
		float light = CheckLightning(Blockpos - m_position, Vector3<float>(1, 0, 0));

		vd[count++] = ChunkMesh::VertexData(Blockpos.x + 1.f, Blockpos.y + 0.f, Blockpos.z + 0.f, light, light, light, binfo.u + binfo.w * 1.0f, binfo.v + binfo.h * .25f, type);
		vd[count++] = ChunkMesh::VertexData(Blockpos.x + 1.f, Blockpos.y + 1.f, Blockpos.z + 0.f, light, light, light, binfo.u + binfo.w * 1.0f, binfo.v + binfo.h * .50f, type);
		vd[count++] = ChunkMesh::VertexData(Blockpos.x + 1.f, Blockpos.y + 1.f, Blockpos.z + 1.f, light, light, light, binfo.u + binfo.w * .50f, binfo.v + binfo.h * .50f, type);
		vd[count++] = ChunkMesh::VertexData(Blockpos.x + 1.f, Blockpos.y + 0.f, Blockpos.z + 1.f, light, light, light, binfo.u + binfo.w * .50f, binfo.v + binfo.h * .25f, type);
	}
	//Gauche
	if (CheckFace(type, Blockpos - m_position, Vector3<float>(-1, 0, 0)))
	{
		float light = CheckLightning(Blockpos - m_position, Vector3<float>(-1, 0, 0));

		vd[count++] = ChunkMesh::VertexData(Blockpos.x + 0.f, Blockpos.y + 0.f, Blockpos.z + 0.f, light, light, light, binfo.u + binfo.w * .00f, binfo.v + binfo.h * .25f, type);
		vd[count++] = ChunkMesh::VertexData(Blockpos.x + 0.f, Blockpos.y + 0.f, Blockpos.z + 1.f, light, light, light, binfo.u + binfo.w * .50f, binfo.v + binfo.h * .25f, type);
		vd[count++] = ChunkMesh::VertexData(Blockpos.x + 0.f, Blockpos.y + 1.f, Blockpos.z + 1.f, light, light, light, binfo.u + binfo.w * .50f, binfo.v + binfo.h * .50f, type);
		vd[count++] = ChunkMesh::VertexData(Blockpos.x + 0.f, Blockpos.y + 1.f, Blockpos.z + 0.f, light, light, light, binfo.u + binfo.w * .00f, binfo.v + binfo.h * .50f, type);
	}
	//Derirere
	if (CheckFace(type, Blockpos - m_position, Vector3<float>(0, 0, 1)))
	{
		float light = CheckLightning(Blockpos - m_position, Vector3<float>(0, 0, 1));

		vd[count++] = ChunkMesh::VertexData(Blockpos.x + 0.f, Blockpos.y + 0.f, Blockpos.z + 1.f, light, light, light, binfo.u + binfo.w * .50f, binfo.v + binfo.h * .50f, type);
		vd[count++] = ChunkMesh::VertexData(Blockpos.x + 1.f, Blockpos.y + 0.f, Blockpos.z + 1.f, light, light, light, binfo.u + binfo.w * 1.0f, binfo.v + binfo.h * .50f, type);
		vd[count++] = ChunkMesh::VertexData(Blockpos.x + 1.f, Blockpos.y + 1.f, Blockpos.z + 1.f, light, light, light, binfo.u + binfo.w * 1.0f, binfo.v + binfo.h * .75f, type);
		vd[count++] = ChunkMesh::VertexData(Blockpos.x + 0.f, Blockpos.y + 1.f, Blockpos.z + 1.f, light, light, light, binfo.u + binfo.w * .50f, binfo.v + binfo.h * .75f, type);
	}
	//Haut
	if (CheckFace(type, Blockpos - m_position, Vector3<float>(0, 1, 0)))
	{
		float light = CheckLightning(Blockpos - m_position, Vector3<float>(0, 1, 0));

		vd[count++] = ChunkMesh::VertexData(Blockpos.x + 0.f, Blockpos.y + 1.f, Blockpos.z + 0.f, light, light, light, binfo.u + binfo.w * .50f, binfo.v + binfo.h * .75f, type);
		vd[count++] = ChunkMesh::VertexData(Blockpos.x + 0.f, Blockpos.y + 1.f, Blockpos.z + 1.f, light, light, light, binfo.u + binfo.w * .50f, binfo.v + binfo.h * 1.0f, type);
		vd[count++] = ChunkMesh::VertexData(Blockpos.x + 1.f, Blockpos.y + 1.f, Blockpos.z + 1.f, light, light, light, binfo.u + binfo.w * .00f, binfo.v + binfo.h * 1.0f, type);
		vd[count++] = ChunkMesh::VertexData(Blockpos.x + 1.f, Blockpos.y + 1.f, Blockpos.z + 0.f, light, light, light, binfo.u + binfo.w * .00f, binfo.v + binfo.h * .75f, type);

		if (type == BTYPE_WATER) ///Lorsque sous l'eau on peut voir le haut de locean
		{
			vd[count++] = ChunkMesh::VertexData(Blockpos.x + 0.f, Blockpos.y + 1.f, Blockpos.z + 0.f, light, light, light, binfo.u + binfo.w * .50f, binfo.v + binfo.h * .75f, type);
			vd[count++] = ChunkMesh::VertexData(Blockpos.x + 1.f, Blockpos.y + 1.f, Blockpos.z + 0.f, light, light, light, binfo.u + binfo.w * .00f, binfo.v + binfo.h * .75f, type);
			vd[count++] = ChunkMesh::VertexData(Blockpos.x + 1.f, Blockpos.y + 1.f, Blockpos.z + 1.f, light, light, light, binfo.u + binfo.w * .00f, binfo.v + binfo.h * 1.0f, type);
			vd[count++] = ChunkMesh::VertexData(Blockpos.x + 0.f, Blockpos.y + 1.f, Blockpos.z + 1.f, light, light, light, binfo.u + binfo.w * .50f, binfo.v + binfo.h * 1.0f, type);

		}
	}
	//Bas
	if (CheckFace(type, Blockpos - m_position, Vector3<float>(0, -1, 0)))
	{
		float light = CheckLightning(Blockpos - m_position, Vector3<float>(0, -1, 0));

		vd[count++] = ChunkMesh::VertexData(Blockpos.x + 0.f, Blockpos.y + 0.f, Blockpos.z + 0.f, light, light, light, binfo.u + binfo.w * .50f, binfo.v + binfo.h * .75f, type);
		vd[count++] = ChunkMesh::VertexData(Blockpos.x + 1.f, Blockpos.y + 0.f, Blockpos.z + 0.f, light, light, light, binfo.u + binfo.w * 1.0f, binfo.v + binfo.h * .75f, type);
		vd[count++] = ChunkMesh::VertexData(Blockpos.x + 1.f, Blockpos.y + 0.f, Blockpos.z + 1.f, light, light, light, binfo.u + binfo.w * 1.0f, binfo.v + binfo.h * 1.0f, type);
		vd[count++] = ChunkMesh::VertexData(Blockpos.x + 0.f, Blockpos.y + 0.f, Blockpos.z + 1.f, light, light, light, binfo.u + binfo.w * .50f, binfo.v + binfo.h * 1.0f, type);

	}

}

void Chunk::RenderSolidBuffer(GLenum &m_program) const
{
	m_chunkMesh.Render(m_program);
}

void Chunk::RenderTransparentBuffer(GLenum &m_program) const
{
	m_transparentMesh.Render(m_program);
}

void Chunk::DeleteCache()
{
	m_chunkMesh.DeleteBuffer();
	m_transparentMesh.DeleteBuffer();
	m_isDirty = true;
}

bool Chunk::NeedUpdate() const
{
	//Si le chunk est dirty et que tous ses voisin on �t� creer
	if (m_isDirty
		&& (m_negativeX == NULL || m_negativeX->m_iscreated)
		&& (m_negativeZ == NULL || m_negativeZ->m_iscreated)
		&& (m_positiveX == NULL || m_positiveX->m_iscreated)
		&& (m_positiveZ == NULL || m_positiveZ->m_iscreated))
		return m_isDirty;

	else
		return false;
}

bool& Chunk::GetSave() {
	return m_save;
}

bool Chunk::CheckFace(BlockType type, const Vector3<float> &Blockpos, const Vector3<float> &face) const
{
	BlockType faceType = GetBlock(Blockpos.x + face.x, Blockpos.y + face.y, Blockpos.z + face.z);


	if (faceType == BTYPE_AIR || faceType == BTYPE_LEAVE || (faceType != BTYPE_WATER && type == BTYPE_WATER) || faceType == BTYPE_LAVA || (faceType == BTYPE_WATER && type != BTYPE_WATER))
		return true;

	return false;
}

float Chunk::CheckLightning(const Vector3<float> &Blockpos, const Vector3<float> &face) const
{

	if (face.y == -1)
		return 0.5f;

	for (int i = 1; true; i++)
	{
		BlockType block = GetBlock(Blockpos.x + face.x, Blockpos.y + i, Blockpos.z + face.z);
		if (block == BTYPE_AIR)
		{
			if (Blockpos.y + i > CHUNK_SIZE_Y)
				return 0.9f;

		}
		else if (block == BTYPE_LEAVE)
			return 0.74f;
		else if (block == BTYPE_WATER)
			return 0.64f;
		else
			return  0.57f;
	}
}

void Chunk::CheckWater(const Vector3<float> &Blockpos)
{
	for (int i = 0; i < 4; i++)
	{
		BlockType bloc = GetBlock(Blockpos.x, Blockpos.y - 1, Blockpos.z);
		BlockType bt = GetBlock(Blockpos.x, Blockpos.y - 1, Blockpos.z);
		if (GetBlock(Blockpos.x - i, Blockpos.y - 1, Blockpos.z) == BTYPE_AIR)
		{
			SetBlock(Blockpos.x - i, Blockpos.y - 1, Blockpos.z, BTYPE_WATER);
		}
		else if (GetBlock(Blockpos.x - i, Blockpos.y, Blockpos.z) == BTYPE_AIR && bt != BTYPE_WATER && bt != BTYPE_RWATER1 && bt != BTYPE_RWATER2 && bt != BTYPE_RWATER3)
		{
			SetBlock(Blockpos.x - i, Blockpos.y, Blockpos.z, BTYPE_RWATER1);
			//m_isDirty = true;
		}


		if (GetBlock(Blockpos.x + i, Blockpos.y - 1, Blockpos.z) == BTYPE_AIR)// || bt == BTYPE_RWATER1 || bt == BTYPE_RWATER2 || bt == BTYPE_RWATER3)
		{
			SetBlock(Blockpos.x + i, Blockpos.y - 1, Blockpos.z, BTYPE_WATER);
		}
		else if (GetBlock(Blockpos.x + i, Blockpos.y, Blockpos.z) == BTYPE_AIR && bt != BTYPE_WATER && bt != BTYPE_RWATER1 && bt != BTYPE_RWATER2 && bt != BTYPE_RWATER3)
		{
			SetBlock(Blockpos.x + i, Blockpos.y, Blockpos.z, BTYPE_RWATER1);
			//m_isDirty = true;
		}


		if (GetBlock(Blockpos.x, Blockpos.y - 1, Blockpos.z - i) == BTYPE_AIR)
		{
			SetBlock(Blockpos.x, Blockpos.y - 1, Blockpos.z - i, BTYPE_WATER);
		}
		else if (GetBlock(Blockpos.x, Blockpos.y, Blockpos.z - i) == BTYPE_AIR && bt != BTYPE_WATER && bt != BTYPE_RWATER1 && bt != BTYPE_RWATER2 && bt != BTYPE_RWATER3)
		{
			SetBlock(Blockpos.x, Blockpos.y, Blockpos.z - i, BTYPE_RWATER1);
			//m_isDirty = true;
		}


		if (GetBlock(Blockpos.x, Blockpos.y - 1, Blockpos.z + 1) == BTYPE_AIR)
		{
			SetBlock(Blockpos.x, Blockpos.y - 1, Blockpos.z + i, BTYPE_WATER);
		}
		else if (GetBlock(Blockpos.x, Blockpos.y, Blockpos.z + i) == BTYPE_AIR && bt != BTYPE_WATER && bt != BTYPE_RWATER1 && bt != BTYPE_RWATER2 && bt != BTYPE_RWATER3)
		{
			SetBlock(Blockpos.x, Blockpos.y, Blockpos.z + i, BTYPE_RWATER1);
			//m_isDirty = true;
		}



		//if (GetBlock(Blockpos.x + i, Blockpos.y, Blockpos.z) == BTYPE_AIR && bt != BTYPE_WATER && bt != BTYPE_RWATER1 && bt != BTYPE_RWATER2 && bt != BTYPE_RWATER3)
		//{
		//	SetBlock(Blockpos.x + i, Blockpos.y, Blockpos.z, BTYPE_RWATER1);
		//	//m_isDirty = true;
		//}
		//if (GetBlock(Blockpos.x, Blockpos.y, Blockpos.z - i) == BTYPE_AIR && bt != BTYPE_WATER && bt != BTYPE_RWATER1 && bt != BTYPE_RWATER2 && bt != BTYPE_RWATER3)
		//{
		//	SetBlock(Blockpos.x, Blockpos.y, Blockpos.z - i, BTYPE_RWATER1);
		//	//m_isDirty = true;
		//}
		//if (GetBlock(Blockpos.x, Blockpos.y, Blockpos.z + i) == BTYPE_AIR && bt != BTYPE_WATER && bt != BTYPE_RWATER1 && bt != BTYPE_RWATER2 && bt != BTYPE_RWATER3)
		//{
		//	SetBlock(Blockpos.x, Blockpos.y, Blockpos.z + i, BTYPE_RWATER1);
		//	//m_isDirty = true;
		//}



	}
}
