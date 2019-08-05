#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Actor.h"
#include "Level.h"
#include <string>
#include <vector>

using namespace std;

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetPath);
	virtual int init();
	virtual int move();
	virtual void cleanUp();
	~StudentWorld();
	StudentWorld();
	bool isValidPos(double x, double y, Actor* me);
	bool isValidPos(double x, double y);
	void levelCompleted() { m_lvlCompleted = true; };

	// mutators

	// add to actors
	bool addWall(double x, double y);
	bool addExit(double x, double y);
	bool addPit(double x, double y);
	bool addLandmine(double x, double y);
	bool addFlame(double x, double y, Direction dir);
	bool addVomit(double x, double y, Direction dir);
	bool addVaccineGoodie(double x, double y);
	bool addGasCanGoodie(double x, double y);
	bool addLandmineGoodie(double x, double y);
	bool addPenelope(double x, double y);
	bool addCitizen(double x, double y);
	bool addDumbZombie(double x, double y);
	bool addSmartZombie(double x, double y);

	// remove from actors
	void saveCitizens(Actor* me);

	// accessors

	bool isEmpty(double x, double y);
	bool overlappingKillableActor(Actor* me);
	bool overlappingLethalActor(Actor* me);
	bool overlappingLandmineActivator(Actor* me);
	bool overlappingExitableActor(Actor* me);
	bool overlappingPenelope(Actor* me);
	bool overlappingInfectiousActor(Actor* me);
	bool wouldInfect(double x, double y);
	int numCitizens();
	void findPenelope(Actor* me, double& dx, double& dy);
	bool findInfectableTarget(Actor* me, double& dx, double& dy);
	double distZombiesSq(Actor* me);
	double distZombiesSq(double x, double y);

	// penelope mutators
	void giveVaccine();
	void giveGasCan();
	void giveLandmine();


private:

	//helper functions
	double distSq(double x1, double y1) { return x1 * x1 + y1 * y1; }

	bool intersecting(double x1, double y1, double x2, double y2);
	bool intersecting(Actor* a, Actor* b);

	bool overlapping(double x1, double y1, double x2, double y2);
	bool overlapping(Actor* a, Actor* b);

	//member variables
	bool m_lvlCompleted;
	Penelope* m_penelope;
	vector<Actor*> actors;
};

#endif // STUDENTWORLD_H_
