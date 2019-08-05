#include <iostream>
#include <string>
#include <queue>

using namespace std;

class Coord
{
public:
	Coord(int rr, int cc) : m_r(rr), m_c(cc) {}
	int r() const { return m_r; }
	int c() const { return m_c; }
private:
	int m_r;
	int m_c;
};

bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec) {
	queue<Coord> cstack;

	cstack.push(Coord(sr, sc));
	maze[sr][sc] = '~';

	while (!cstack.empty()) {
		Coord current = cstack.front();
		cstack.pop();

		int cR = current.r();
		int cC = current.c();

		if (cR == er && cC == ec)
			return true;

		if (maze[cR + 1][cC] == '.') {
			cstack.push(Coord(cR + 1, cC));
			maze[cR + 1][cC] = '~';
		}
		if (maze[cR][cC - 1] == '.') {
			cstack.push(Coord(cR, cC - 1));
			maze[cR][cC - 1] = '~';
		}
		if (maze[cR - 1][cC] == '.') {
			cstack.push(Coord(cR - 1, cC));
			maze[cR - 1][cC] = '~';
		}
		if (maze[cR][cC + 1] == '.') {
			cstack.push(Coord(cR, cC + 1));
			maze[cR][cC + 1] = '~';
		}
	}

	return false;
}

int main()
{
	string maze[10] = {
		"XXXXXXXXXX",
		"X....X...X",
		"X.XX.XX..X",
		"XXX....X.X",
		"X.XXX.XXXX",
		"X.X...X..X",
		"X...X.X..X",
		"XXXXX.X.XX",
		"X........X",
		"XXXXXXXXXX"
	};

	if (pathExists(maze, 10, 10, 3, 5, 8, 8))
		cout << "Solvable!" << endl;
	else
		cout << "Out of luck!" << endl;
}