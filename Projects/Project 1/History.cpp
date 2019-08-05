#include "History.h"
#include <iostream>
using namespace std;

History::History(int nRows, int nCols) {
	m_rows = nRows;
	m_cols = nCols;
	for (int r = 0; r < m_rows; r++) {
		for (int c = 0; c < m_cols; c++) {
			m_arena[r][c] = 'A' - 1;
		}
	}
}

bool History::record(int r, int c) {
	r--;
	c--;
	if (r <= m_rows && r >= 0 && c <= m_cols && c >= 0) {
		if(m_arena[r][c] != 'Z')
			m_arena[r][c]++;
		return true;
	}
	return false;
}

void History::display() const{
	clearScreen();
	for (int r = 0; r < m_rows; r++) {
		for (int c = 0; c < m_cols; c++) {
			if (m_arena[r][c] == 'A' - 1)
				cout << ".";
			else
				cout << m_arena[r][c];
		}
		cout << '\n';
	}
	cout << endl;
}