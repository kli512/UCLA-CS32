#include "Set.h"

Set::Set() {
	m_size = 0;
}

bool Set::empty() const {
	return m_size == 0;
}

int Set::size() const {
	return m_size;
}

bool Set::insert(const ItemType& value) {

	if (m_size >= DEFAULT_MAX_ITEMS)
		return false;

	if (m_size == 0) {
		m_size = 1;
		m_items[0] = value;
		return true;
	}

	for (int i = 0; i < m_size; i++) {
		if (m_items[i] == value)
			return false;
	}

	for (int i = 0; i < m_size; i++) {
		if (m_items[i] > value) {

			for (int j = m_size; j > i; j--) {
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


bool Set::erase(const ItemType& value) {

	for (int i = 0; i < m_size; i++) {
		if (m_items[i] == value) {
			for (int j = i; j < m_size - 1; j++) {
				m_items[j] = m_items[j + 1];
			}
			m_size--;
			return true;
		}
	}

	return false;
}

bool Set::contains(const ItemType& value) const {

	for (int i = 0; i < m_size; i++) {
		if (m_items[i] == value)
			return true;
	}

	return false;
}

bool Set::get(int i, ItemType& value) const {
	if (i < 0 || i >= m_size)
		return false;

	value = m_items[i];
	return true;
}

void Set::swap(Set& other) {

	int largest = (this->size() > other.size()) ? this->size() : other.size();

	for (int i = 0; i < largest; i++) {
		ItemType t = other.m_items[i];
		other.m_items[i] = this->m_items[i];
		this->m_items[i] = t;
	}

	int t_size = other.m_size;
	other.m_size = this->m_size;
	this->m_size = t_size;

}