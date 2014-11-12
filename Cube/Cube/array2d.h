#ifndef ARRAY3D_H__
#define ARRAY3D_H__

#include "define.h"

template<class t>
class Array2d
{
public:
	Array2d(int x, int y);
	~Array2d();
	Array2d(const Array2d<t>& array);

	void Set(int x, int y, t value);
	t Get(int x, int y)  const;
	void Reset(t value);

private:
	int m_x, m_y;
	t* m_data;
};

template<class t>
Array2d<t>::Array2d(int x, int y) : m_x(x), m_y(y)
{
	m_data = new t[m_x * m_y ];

	Reset(0);
}

template<class t>
Array2d<t>::~Array2d()
{
	delete[] m_data;
}

template<class t>
Array2d<t>::Array2d(const Array2d<t>& array)
{
	m_x = array.m_x;
	m_y = array.m_y;

	m_data = new t[m_x * m_y];
	for (int i = 0; i < m_x * m_y; ++i)
		m_data[i] = array.m_data[i];
}

template<class t>
void Array2d<t>::Set(int x, int y, t value)
{
	m_data[x  + (y * m_x)] = value;
}

template<class t>
t Array2d<t>::Get(int x, int y)  const
{
	return m_data[x + (y * m_x)];
}

template<class t>
void Array2d<t>::Reset(t value)
{
	for (int i = 0; i < m_x * m_y ; ++i)
		m_data[i] = value;
	
}

#endif // ARRAY2D_H__