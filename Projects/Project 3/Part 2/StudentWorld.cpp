#include "StudentWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include "Level.h"
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <iomanip>
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

bool StudentWorld::addWall(double x, double y) {
	if (isValidPos(x, y)) {
		actors.push_back(new Wall(this, x, y));
		return true;
	}
	return false;
}
bool StudentWorld::addExit(double x, double y) {
	if (isValidPos(x, y)) {
		actors.push_back(new Exit(this, x, y));
		return true;
	}
	return false;
}
bool StudentWorld::addPit(double x, double y) {
	if (isValidPos(x, y)) {
		actors.push_back(new Pit(this, x, y));
		return true;
	}
	return false;
}
bool StudentWorld::addLandmine(double x, double y) {
	actors.push_back(new Landmine(this, x, y));
	return true;
}
bool StudentWorld::addFlame(double x, double y, Direction dir) { // CHECK NEEDS TO BE BLOCKE DBY WALLS
	for (Actor* a : actors) {
		if (a->blocksFlames() && overlapping(x * SPRITE_WIDTH, y*SPRITE_HEIGHT, a->getX(), a->getY()))
			return false;
	}
	actors.push_back(new Flame(this, x, y, dir));
	return true;
}
bool StudentWorld::addVomit(double x, double y, Direction dir) {
	actors.push_back(new Vomit(this, x, y, dir));
	return true;
}
bool StudentWorld::addVaccineGoodie(double x, double y) {
	if (isValidPos(x, y)) {
		actors.push_back(new VaccineGoodie(this, x, y));
		return true;
	}
	return false;
}
bool StudentWorld::addGasCanGoodie(double x, double y) {
	if (isValidPos(x, y)) {
		actors.push_back(new GasCanGoodie(this, x, y));
		return true;
	}
	return false;
}
bool StudentWorld::addLandmineGoodie(double x, double y) {
	if (isValidPos(x, y)) {
		actors.push_back(new LandmineGoodie(this, x, y));
		return true;
	}
	return false;
}
bool StudentWorld::addPenelope(double x, double y) {
	if (isValidPos(x, y)) {
		m_penelope = new Penelope(this, x, y);
		actors.push_back(m_penelope);
		return true;
	}
	return false;
}
bool StudentWorld::addCitizen(double x, double y) {
	if (isValidPos(x, y)) {
		actors.push_back(new Citizen(this, x, y));
		return true;
	}
	return false;
}
bool StudentWorld::addDumbZombie(double x, double y) {
	if (isValidPos(x, y)) {
		actors.push_back(new DumbZombie(this, x, y));
		return true;
	}
	return false;
}
bool StudentWorld::addSmartZombie(double x, double y) {
	if (isValidPos(x, y)) {
		actors.push_back(new SmartZombie(this, x, y));
		return true;
	}
	return false;
}

int StudentWorld::init()
{
	m_lvlCompleted = false;

	string level;
	if (getLevel() < 10)
		level = "0" + to_string(getLevel());
	else if (getLevel() >= 10)
		level = to_string(getLevel());
	else
		return GWSTATUS_LEVEL_ERROR;

	Level lev(assetPath());
	string levelFile = "level" + level + ".txt"; // depends on if level is 1 or 2 digit
	Level::LoadResult result = lev.loadLevel(levelFile);
	if (result == Level::load_fail_file_not_found) {
		cerr << "Cannot find " << levelFile << " data file" << endl;
		return GWSTATUS_LEVEL_ERROR;
	}
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
					addSmartZombie(x, y);
					break;
				case Level::dumb_zombie:
					cerr << "Location " << x << " " << y << " starts with a dumb zombie" << endl;
					addDumbZombie(x, y);
					break;
				case Level::player:
					cerr << "penelope Location " << x << " " << y << endl;
					addPenelope(x, y);
					break;
				case Level::exit:
					cerr << "Location " << x << " " << y << " is where an exit is" << endl;
					addExit(x, y);
					break;
				case Level::wall:
					cerr << "Location " << x << " " << y << " wall" << endl;
					addWall(x, y);
					break;
				case Level::pit:
					cerr << "Location " << x << " " << y << " has a pit in the ground" << endl;
					addPit(x, y);
					break;
				case Level::landmine_goodie:
					cerr << "Location " << x << " " << y << " has a landminegoodie on the ground" << endl;
					addLandmineGoodie(x, y);
					break;
				case Level::gas_can_goodie:
					cerr << "Location " << x << " " << y << " has a gascangoodie on the ground" << endl;
					addGasCanGoodie(x, y);
					break;
				case Level::vaccine_goodie:
					cerr << "Location " << x << " " << y << " has a vaccinegoodie" << endl;
					addVaccineGoodie(x, y);
					break;
				case Level::citizen:
					cerr << "Location " << x << " " << y << " has a citizen" << endl;
					addCitizen(x, y);
					break;
				}
			}
		}
	}

	return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
	for (auto it = actors.begin(); it != actors.end(); it++) {
		if ((*it)->isAlive()) {
			(*it)->doSomething();

			if (!m_penelope->isAlive()) {
				decLives();
				return GWSTATUS_PLAYER_DIED;
			}

			if (m_lvlCompleted) {
				playSound(SOUND_LEVEL_FINISHED);
				return GWSTATUS_FINISHED_LEVEL;
			}
		}
	}

	for (auto it = actors.begin(); it != actors.end(); it++) {
		if (!(*it)->isAlive()) {
			delete *it;
			actors.erase(it);
			it = actors.begin();
		}
	}

	ostringstream oss;

	oss.fill('0');
	oss << "Score: ";
	oss << setw(6) << getScore() << "  ";

	oss << "Level: ";
	oss << getLevel() << "  ";

	oss << "Lives: ";
	oss << getLives() << "  ";

	oss << "Vaccines: ";
	oss << m_penelope->getVaccines() << "  ";

	oss << "Flames: ";
	oss << m_penelope->getFlames() << "  ";

	oss << "Mines: ";
	oss << m_penelope->getMines() << "  ";

	oss << "Infected: ";
	oss << m_penelope->getInfection() << endl;

	setGameStatText(oss.str());

	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::StudentWorld::cleanUp()
{
	for (Actor* a : actors)
		delete a;
	actors.clear();

	m_penelope = nullptr;
}

StudentWorld::~StudentWorld() {
	cleanUp();
}

void StudentWorld::saveCitizens(Actor* me) {
	for (auto it = actors.begin(); it != actors.end(); it++)
		if ((*it) != m_penelope && (*it)->canExit() && overlapping(me, *it)) {
			increaseScore(500);
			playSound(SOUND_CITIZEN_SAVED);
			(*it)->remove();
		}
}

// USEFUL

bool StudentWorld::intersecting(double x1, double y1, double x2, double y2) { // CHECK IF OFF BY 1 ERROR
	if (x1 + SPRITE_WIDTH > x2 && x1 < x2 + SPRITE_WIDTH)
		if (y1 + SPRITE_HEIGHT > y2 && y1 < y2 + SPRITE_HEIGHT)
			return true;
	return false;
}

bool StudentWorld::intersecting(Actor* a, Actor* b) {
	return intersecting(a->getX(), a->getY(), b->getX(), b->getY());
}

bool StudentWorld::overlapping(double x1, double y1, double x2, double y2) {
	double dx = x1 - x2, dy = y1 - y2;
	return dx * dx + dy * dy <= 100; // using 10 squared because euclidean
}

bool StudentWorld::overlapping(Actor* a, Actor* b) {
	return intersecting(a->getX(), a->getY(), b->getX(), b->getY());
}

bool StudentWorld::isValidPos(double x, double y) { // USED TO SPAWN ITEMS IN // EXPECTS TO SEE INPUT COME 0-16
	return isValidPos(x*SPRITE_WIDTH, y*SPRITE_HEIGHT, nullptr);
}

bool StudentWorld::isValidPos(double x, double y, Actor* me) { // CHECK TO CONDENSE INTO FEWER EMBEDDED IFS // EXPECTS TO SEE INPUT COME 0-255
	for (auto a : actors)
		if (a != me)
			if (a->isAlive() && a->blocksMovement())
				if (intersecting(x, y, a->getX(), a->getY()))
					return false;
	return true;
}

bool StudentWorld::isEmpty(double x, double y) { // CHECK TO CONDENSE INTO FEWER EMBEDDED IFS // EXPECTS TO SEE INPUT COME 0-255
	for (auto a : actors)
		if (a->isAlive())
			if (overlapping(x, y, a->getX(), a->getY()))
				return false;
	return true;
}

//ACCESSOR AND HELPER FUNCTIONS CALLED FROM ACTORS
bool StudentWorld::overlappingKillableActor(Actor* me) {
	for (auto a : actors)
		if (a != me)
			if (a->isAlive() && a->killable() && overlapping(me, a))
				return true;
	return false;
}

bool StudentWorld::overlappingLethalActor(Actor* me) {
	for (auto a : actors)
		if (a != me)
			if (a->isAlive() && a->lethal() && overlapping(me, a))
				return true;
	return false;
}

bool StudentWorld::overlappingLandmineActivator(Actor* me) {
	for (auto a : actors)
		if (a != me)
			if (a->isAlive() && a->landmineActivator() && overlapping(me, a))
				return true;
	return false;
}

bool StudentWorld::overlappingExitableActor(Actor* me) {
	for (auto a : actors)
		if (a != me)
			if (a->isAlive() && a->canExit() && overlapping(me, a))
				return true;
	return false;
}

bool StudentWorld::overlappingPenelope(Actor* me) {
	if (m_penelope->isAlive() && overlapping(me, m_penelope))
		return true;
	return false;
}

bool StudentWorld::overlappingInfectiousActor(Actor* me) {
	for (auto a : actors)
		if (a != me)
			if (a->isAlive() && a->infectious() && overlapping(me, a))
				return true;
	return false;
}


bool StudentWorld::wouldInfect(double x, double y) {
	for (auto a : actors)
		if (a->isAlive() && a->canBeInfected() && overlapping(x, y, a->getX(), a->getY()))
			return true;
	return false;
}

int StudentWorld::numCitizens() {
	int c = -1;
	for (auto a : actors)
		if (a->canExit())
			c++;
	return c;
}
void StudentWorld::findPenelope(Actor* me, double& dx, double& dy) {
	dx = m_penelope->getX() - me->getX();
	dy = m_penelope->getY() - me->getY();
}

bool StudentWorld::findInfectableTarget(Actor* me, double& dx, double& dy) {

	bool infectableFound = false;
	for (auto a : actors)
		if (a->canBeInfected()) {
			infectableFound = true;
			dx = a->getX() - me->getX();
			dy = a->getY() - me->getY();
			break;
		}

	if (!infectableFound)
		return false;

	auto bestDistSq = [&]() {return distSq(dx, dy); };

	for (auto a : actors)
		if (a->canBeInfected()) {
			double tx = a->getX() - me->getX();
			double ty = a->getY() - me->getY();

			if (distSq(tx, ty) < bestDistSq()) {
				dx = tx;
				dy = ty;
			}
		}

	return true;
}

double StudentWorld::distZombiesSq(Actor* me) {
	double dx, dy;
	bool zombieFound = false;
	for (auto a : actors)
		if (a->scary()) {
			zombieFound = true;
			dx = a->getX() - me->getX();
			dy = a->getY() - me->getY();
			break;
		}

	if (!zombieFound)
		return -1;

	auto bestDistSq = [&]() {return distSq(dx, dy); };

	for (auto a : actors)
		if (a->scary()) {
			double tx = a->getX() - me->getX();
			double ty = a->getY() - me->getY();

			if (distSq(tx, ty) < bestDistSq()) {
				dx = tx;
				dy = ty;
			}
		}

	return distSq(dx, dy);
}

double StudentWorld::distZombiesSq(double mx, double my) {
	double dx, dy;
	bool zombieFound = false;
	for (auto a : actors)
		if (a->scary()) {
			zombieFound = true;
			dx = a->getX() - mx;
			dy = a->getY() - my;
			break;
		}

	if (!zombieFound)
		return -1;

	auto bestDistSq = [&]() {return distSq(dx, dy); };

	for (auto a : actors)
		if (a->scary()) {
			double tx = a->getX() - mx;
			double ty = a->getY() - my;

			if (distSq(tx, ty) < bestDistSq()) {
				dx = tx;
				dy = ty;
			}
		}

	return distSq(dx, dy);
}


void StudentWorld::giveVaccine() {
	m_penelope->giveVaccines(1);
}
void StudentWorld::giveGasCan() {
	m_penelope->giveGasCan(5);
}
void StudentWorld::giveLandmine() {
	m_penelope->giveMines(2);
}