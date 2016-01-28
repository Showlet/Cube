#ifndef ARRAY3D_H__
#define ARRAY3D_H__

#include "define.h"
#include <cassert>
#include "vector3.h"


template<class t>
class Array3d
{
public:
	Array3d(int x, int y, int z);
	~Array3d();
	Array3d(const Array3d<t>& array);

	void Set(int x, int y, int z, t value);
	void SetDirection(int x, int y, int z, char direction);
	t& Get(int x, int y, int z);
	const t& Get(int x, int y, int z) const;
	char& GetDirection(int x, int y, int z);
	const char& GetDirection(int x, int y, int z) const;

	void SetExploded(int x, int y, int z, bool exploded);
	bool& GetExploded(int x, int y, int z);
	const bool& GetExploded(int x, int y, int z) const;

	void SetSource(int x, int y, int z, Vector3<float> source);
	Vector3<float>& GetSource(int x, int y, int z);
	const Vector3<float>& GetSource(int x, int y, int z) const;


	void Reset(t value);

private:
	int m_x, m_y, m_z;
	t* m_data;
	char* m_direction;
	bool* m_exploded;
	Vector3<float>* m_source;
public:
};


template<class t>
Array3d<t>::Array3d(int x, int y, int z) : m_x(x), m_y(y), m_z(z)
{
	m_data = new t[m_x * m_y * m_z];
	m_direction = new char[m_x * m_y * m_z];
	m_exploded = new bool[m_x * m_y * m_z];
	m_source = new Vector3<float>[1];
	Reset(0);
}

template<class t>
Array3d<t>::~Array3d()
{
	delete[] m_data;
}

template<class t>
Array3d<t>::Array3d(const Array3d& array)
{
	m_x = array.m_x;
	m_y = array.m_y;
	m_z = array.m_z;

	m_data = new t[m_x * m_y * m_z];
	m_direction = new char[m_x * m_y * m_z];
	m_exploded = new bool[m_x * m_y * m_z];
	for (int i = 0; i < m_x * m_y * m_z; ++i)
		m_data[i] = array.m_data[i];
}

template<class t>
void Array3d<t>::Set(int x, int y, int z, t value)
{
	if (x >= 0 && y >= 0 && z >= 0 && x < m_x && y < m_y && z < m_z)
		m_data[x + (z * m_x) + (y * m_z * m_x)] = value;
}

template<class t>
void Array3d<t>::SetDirection(int x, int y, int z, char direction)
{
	if (x >= 0 && y >= 0 && z >= 0 && x < m_x && y < m_y && z < m_z)
		m_direction[x + (z * m_x) + (y * m_z * m_x)] = direction;
}

template<class t>
t& Array3d<t>::Get(int x, int y, int z)
{
	assert(x >= 0 && y >= 0 && z >= 0 && x < m_x && y < m_y && z < m_z);

	return m_data[x + (z * m_x) + (y * m_z * m_x)];
}

template<class t>
char& Array3d<t>::GetDirection(int x, int y, int z)
{
	assert(x >= 0 && y >= 0 && z >= 0 && x < m_x && y < m_y && z < m_z);

	return m_direction[x + (z * m_x) + (y * m_z * m_x)];
}

template<class t>
const char& Array3d<t>::GetDirection(int x, int y, int z) const
{
	assert(x >= 0 && y >= 0 && z >= 0 && x < m_x && y < m_y && z < m_z);

	return m_direction[x + (z * m_x) + (y * m_z * m_x)];
}

template<class t>
const t& Array3d<t>::Get(int x, int y, int z) const
{
	assert(x >= 0 && y >= 0 && z >= 0 && x < m_x && y < m_y && z < m_z);
	return m_data[x + (z * m_x) + (y * m_z * m_x)];
}

template<class t>
void Array3d<t>::Reset(t value)
{
	for (int i = 0; i < m_x * m_y * m_z; ++i)
		m_data[i] = value;
	for (int i = 0; i < m_x * m_y * m_z; ++i)
		m_exploded[i] = false;
}

template<class t>
void Array3d<t>::SetExploded(int x, int y, int z, bool exploded)
{
	if (x >= 0 && y >= 0 && z >= 0 && x < m_x && y < m_y && z < m_z)
		m_exploded[x + (z * m_x) + (y * m_z * m_x)] = exploded;
}

template<class t>
bool& Array3d<t>::GetExploded(int x, int y, int z)
{
	assert(x >= 0 && y >= 0 && z >= 0 && x < m_x && y < m_y && z < m_z);

	return m_exploded[x + (z * m_x) + (y * m_z * m_x)];
}

template<class t>
const bool& Array3d<t>::GetExploded(int x, int y, int z) const
{
	assert(x >= 0 && y >= 0 && z >= 0 && x < m_x && y < m_y && z < m_z);

	return m_exploded[x + (z * m_x) + (y * m_z * m_x)];
}

template<class t>
void Array3d<t>::SetSource(int x, int y, int z, Vector3<float> source)
{
	if (x >= 0 && y >= 0 && z >= 0 && x < m_x && y < m_y && z < m_z)
		m_source[x + (z * m_x) + (y * m_z * m_x)] = source;
}

template<class t>
Vector3<float>& Array3d<t>::GetSource(int x, int y, int z)
{
	assert(x >= 0 && y >= 0 && z >= 0 && x < m_x && y < m_y && z < m_z);

	return m_source[x + (z * m_x) + (y * m_z * m_x)];
}

template<class t>
const Vector3<float>& Array3d<t>::GetSource(int x, int y, int z) const
{
	assert(x >= 0 && y >= 0 && z >= 0 && x < m_x && y < m_y && z < m_z);

	return m_source[x + (z * m_x) + (y * m_z * m_x)];
}


#endif // ARRAY3D_H__
