#include "StudentWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include "Level.h"
#include <string>
#include <vector>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h and Actor.cpp

StudentWorld::StudentWorld(string assetPath) : GameWorld(assetPath) {
	m_penelope = nullptr;
	actors.clear();
}

int StudentWorld::init()
{
	Level lev(assetPath());
	string levelFile = "level0" + to_string(getLevel()) + ".txt"; // depends on if level is 1 or 2 digit
	Level::LoadResult result = lev.loadLevel(levelFile);
	if (result == Level::load_fail_file_not_found)
		cerr << "Cannot find " << levelFile << " data file" << endl;
	else if (result == Level::load_fail_bad_format)
		cerr << "Your level was improperly formatted" << endl;
	else if (result == Level::load_success)
	{
		cerr << "Successfully loaded level" << endl;
		for (int x = 0; x < 16; x++) {
			for (int y = 0; y < 16; y++) {
				Level::MazeEntry ge = lev.getContentsOf(x, y);
				switch (ge)
				{
				case Level::empty:
					cerr << "Location " << x << " " << y << " is empty" << endl;
					break;
				case Level::smart_zombie:
					cerr << "Location " << x << " " << y << " starts with a smart zombie" << endl;
					break;
				case Level::dumb_zombie:
					cerr << "Location " << x << " " << y << " starts with a dumb zombie" << endl;
					break;
				case Level::player:
					cerr << "penelope Location " << x << " " << y << endl;
					m_penelope = new Penelope(this, x, y);
					break;
				case Level::exit:
					cerr << "Location " << x << " " << y << " is where an exit is" << endl;
					break;
				case Level::wall:
					cerr << "Location " << x << " " << y << " wall" << endl;
					actors.push_back(new Wall(this, x, y));
					break;
				case Level::pit:
					cerr << "Location " << x << " " << y << " has a pit in the ground" << endl;
					break;
					// etc…
				}
			}
		}
	}

	return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
	m_penelope->doSomething();
	for (Actor* a : actors) {
		a->doSomething();
	}
	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
	for (Actor* a : actors)
		delete a;
	actors.clear();

	delete m_penelope;
	m_penelope = nullptr;
}

StudentWorld::~StudentWorld() {
	cleanUp();
}

bool StudentWorld::isValidPos(double x, double y) {
	for (auto a : actors)
		if (x + SPRITE_WIDTH - 1 > a->getX() && x < a->getX() + SPRITE_WIDTH - 1)
			if (y + SPRITE_HEIGHT - 1 > a->getY() && y < a->getY() + SPRITE_HEIGHT - 1)
				return false;
	return true;
}