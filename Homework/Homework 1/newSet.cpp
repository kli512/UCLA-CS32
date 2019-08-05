#include "newSet.h"

Set::Set(int max)
{
	if (max < 0)
		std::exit;
	m_size = 0;
	m_max = max;
	m_items = new ItemType[max];
}

Set::Set(const Set &other)
{
	m_size = other.m_size;
	m_max = other.m_max;
	m_items = new ItemType[m_max];

	// Copy items
	for (int i = 0; i < m_size; ++i)
		m_items[i] = other.m_items[i];
}

Set &Set::operator=(const Set &rhs)
{
	if (this != &rhs)
	{
		Set temp(rhs);
		this->swap(temp);
	}

	return *this;
}

Set::~Set()
{
	delete[] m_items;
}

bool Set::empty() const
{
	return m_size == 0;
}

int Set::size() const
{
	return m_size;
}

bool Set::insert(const ItemType &value)
{

	if (m_size >= m_max)
		return false;

	if (m_size == 0)
	{
		m_size = 1;
		m_items[0] = value;
		return true;
	}

	for (int i = 0; i < m_size; i++)
	{
		if (m_items[i] == value)
			return false;
	}

	for (int i = 0; i < m_size; i++)
	{
		if (m_items[i] > value)
		{

			for (int j = m_size; j > i; j--)
			{
				m_items[j] = m_items[j - 1];
			}

			m_items[i] = value;
			m_size++;
			return true;
		}
	}

	m_items[m_size] = value;
	m_size++;

	return true;
}

bool Set::erase(const ItemType &value)
{

	for (int i = 0; i < m_size; i++)
	{
		if (m_items[i] == value)
		{
			for (int j = i; j < m_size - 1; j++)
			{
				m_items[j] = m_items[j + 1];
			}
			m_size--;
			return true;
		}
	}

	return false;
}

bool Set::contains(const ItemType &value) const
{

	for (int i = 0; i < m_size; i++)
	{
		if (m_items[i] == value)
			return true;
	}

	return false;
}

bool Set::get(int i, ItemType &value) const
{
	if (i < 0 || i >= m_size)
		return false;

	value = m_items[i];
	return true;
}

void Set::swap(Set &other)
{
	int tsize = other.m_size;
	other.m_size = this->m_size;
	this->m_size = tsize;

	// Switch max capacity.
	int tmax = other.m_max;
	other.m_max = this->m_max;
	this->m_max = tmax;

	// Switch pointer.
	ItemType *tptr = other.m_items;
	other.m_items = this->m_items;
	this->m_items = tptr;
}