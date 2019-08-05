#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

//ACTOR EMPTY

//EXIT BEGIN
void Exit::doSomething() {
	if (getWorld()->overlappingExitableActor(this)) //
		getWorld()->saveCitizens(this);

	if (getWorld()->overlappingPenelope(this) && getWorld()->numCitizens() == 0)
		getWorld()->levelCompleted();
}

//PIT EMPTY

//LANDMINE BEGIN

void Landmine::explode() {
	die();
	getWorld()->playSound(SOUND_LANDMINE_EXPLODE);

	double x = getX();
	double y = getY();

	for (int r = -1; r <= 1; r++)
		for (int c = -1; c <= 1; c++)
			getWorld()->addFlame(x / SPRITE_WIDTH + r, y / SPRITE_HEIGHT + c, up);

	getWorld()->addPit(x / SPRITE_WIDTH, y / SPRITE_HEIGHT);
}

void Landmine::doSomething() {
	safetyTicks--;
	cerr << "my safety ticks " << safetyTicks << endl;
	if (getWorld()->overlappingLandmineActivator(this) && safetyTicks <= 0) {
		explode();
	}
}

// TIMEDITERACTABLE BEGIN
void TimedInteractable::doSomething() {
	if (!isAlive())
		return;
	cerr << "my health is " << health << endl;
	cerr << "i exist at " << getX() << " " << getY() << endl;
	health--;
	if (health == 0)
		die();
}

// FLAME BEGIN

// VOMIT BEGIN

// GOODIE BEGIN
void Goodie::doSomething() {
	if (getWorld()->overlappingLethalActor(this)) { // ummmmmm
		die();
	}
	if (getWorld()->overlappingPenelope(this)) {
		getWorld()->increaseScore(50);
		die();
		getWorld()->playSound(SOUND_GOT_GOODIE);
		giveGoodie();
		return;
	}

}

// VACCINE GOODIE BEGIN
void VaccineGoodie::giveGoodie() {
	getWorld()->giveVaccine();
}

// GAS CAN GOODIE BEGIN
void GasCanGoodie::giveGoodie() {
	getWorld()->giveGasCan();
}

// LANDMINE GOODIE BEGIN
void LandmineGoodie::giveGoodie() {
	getWorld()->giveLandmine();
}

//LIVING ACTOR BEGIN

void LivingActor::newLoc(Direction dir, double& xTo, double& yTo, double dist) {
	double dest_x = getX(), dest_y = getY();
	switch (dir) {
	case up:
		dest_y += dist;
		break;
	case down:
		dest_y -= dist;
		break;
	case left:
		dest_x -= dist;
		break;
	case right:
		dest_x += dist;
		break;
	}

	xTo = dest_x;
	yTo = dest_y;
}

void LivingActor::newLoc(Direction dir, double& xTo, double& yTo) {
	newLoc(dir, xTo, yTo, myMoveDist());
}

bool LivingActor::canMove(Direction dir) {
	double dest_x, dest_y;
	newLoc(dir, dest_x, dest_y);
	return getWorld()->isValidPos(dest_x, dest_y, this);
}

bool LivingActor::move(Direction dir) {
	if (canMove(dir)) {
		double dest_x, dest_y;
		newLoc(dir, dest_x, dest_y);

		moveTo(dest_x, dest_y);
		return true;
	}
	return false;
}

bool LivingActor::doLivingActorStuff() {
	if (!isAlive())
		return false;
	if (getWorld()->overlappingLethalActor(this)) {
		die();
		return false;
	}
	return true;
}

//HUMAN BEGIN

bool Human::touchingVomit() {
	return getWorld()->overlappingInfectiousActor(this);
}

//PENELOPE BEGIN
void Penelope::die() {
	Actor::die();
	getWorld()->playSound(SOUND_PLAYER_DIE);
}

void Penelope::shootFlames() {
	flames--;
	getWorld()->playSound(SOUND_PLAYER_FIRE);

	int dir = getDirection();
	double px = getX(), py = getY();
	bool flameAdded = true;

	for (int i = 1; i <= 3 && flameAdded; i++) {

		switch (dir) {
		case up:
			flameAdded = getWorld()->addFlame(px / SPRITE_WIDTH, py / SPRITE_HEIGHT + i, dir);
			break;
		case down:
			flameAdded = getWorld()->addFlame(px / SPRITE_WIDTH, py / SPRITE_HEIGHT - i, dir);
			break;
		case left:
			flameAdded = getWorld()->addFlame(px / SPRITE_WIDTH - i, py / SPRITE_HEIGHT, dir);
			break;
		case right:
			flameAdded = getWorld()->addFlame(px / SPRITE_WIDTH + i, py / SPRITE_HEIGHT, dir);
			break;
		default:
			cerr << "INVALID PENELOPE DIRECTION" << endl;
		}
	}
}

void Penelope::addLandmine() {
	landmines--;
	getWorld()->addLandmine(getX() / SPRITE_WIDTH, getY() / SPRITE_HEIGHT);
}

void Penelope::useVaccine() {
	vaccines--;
	setInfected(false);
}

void Penelope::doSomething() {
	if (!doLivingActorStuff())
		return;
	if (touchingVomit()) {
		setInfected(true);
	}
	if (getInfection()) {
		cerr << "I'M INFECTED" << endl;
		cerr << "INFECTION AT: " << getInfectionCount() << endl;
		incInfectionCount();
		if (getInfectionCount() >= 500) {
			die();
			getWorld()->playSound(SOUND_PLAYER_DIE);
		}
	}
	int ch;
	if (getWorld()->getKey(ch))
	{
		// user hit a key during this tick!
		switch (ch)
		{
		case KEY_PRESS_SPACE:
			if (flames > 0)
				shootFlames();
			break;
		case KEY_PRESS_TAB:
			if (landmines > 0)
				addLandmine();
			break;
		case KEY_PRESS_ENTER:
			if (vaccines > 0)
				useVaccine();
			break;
		case KEY_PRESS_LEFT:
			setDirection(left);
			move(left);
			break;
		case KEY_PRESS_RIGHT:
			setDirection(right);
			move(right);
			break;
		case KEY_PRESS_UP:
			setDirection(up);
			move(up);
			break;
		case KEY_PRESS_DOWN:
			setDirection(down);
			move(down);
			break;
		default:
			cerr << "INVALID KEY PRESS" << endl;
			break;
		}
	}
}

//CITIZEN BEGIN

void Citizen::die() {
	Actor::die();
	getWorld()->playSound(SOUND_CITIZEN_DIE);
	getWorld()->increaseScore(-1000);
}

void Citizen::doSomething() {
	if (!doLivingActorStuff())
		return;
	if (touchingVomit()) {
		getWorld()->playSound(SOUND_CITIZEN_INFECTED); //NOT SURE
		setInfected(true);
	}
	if (getInfection()) {
		incInfectionCount();
		if (getInfectionCount() >= 500) {
			die();
			getWorld()->playSound(SOUND_ZOMBIE_BORN);
			getWorld()->increaseScore(-1000);
			if (randInt(0, 9) >= 7)
				getWorld()->addSmartZombie(getX() / SPRITE_WIDTH, getY() / SPRITE_HEIGHT);
			else
				getWorld()->addDumbZombie(getX() / SPRITE_WIDTH, getY() / SPRITE_HEIGHT);
			return;
		}
	}

	if (getParalysis()) {
		unparalyze();
		return;
	}

	paralyze();

	double dx_p, dy_p;
	getWorld()->findPenelope(this, dx_p, dy_p);

	double dist_p = dx_p * dx_p + dy_p * dy_p;
	double dist_z = getWorld()->distZombiesSq(this);

	cerr << "DISTP: " << dist_p << endl;
	cerr << "DISTZ: " << dist_z << endl;

	if ((dist_p < dist_z || dist_z == -1) && dist_p <= 80 * 80) { // USING DISTANCE SQUARED BECAUSE EUCLIDIAN AND DONT HAVE MATH

		double dx, dy;
		Direction dir;
		getWorld()->findPenelope(this, dx, dy);

		if (dx > 0)
			if (dy > 0)
				dir = 45;
			else if (dy == 0)
				dir = right;
			else
				dir = 315;
		else if (dx == 0)
			if (dy > 0)
				dir = up;
			else
				dir = down;
		else
			if (dy > 0)
				dir = 135;
			else if (dy == 0)
				dir = left;
			else
				dir = 225;

		switch (dir) {
		case left:
		case right:
		case up:
		case down:
			if (move(dir)) {
				setDirection(dir);
				return;
			}
			break;
		default:
			Direction dir1 = 90 * static_cast<int>(dir / 90) % 360;
			Direction dir2 = 90 * static_cast<int>(dir / 90 + 1) % 360;

			if (randInt(0, 1) == 0) {
				Direction t = dir1;
				dir1 = dir2;
				dir2 = t;
			}

			if (move(dir1)) {
				setDirection(dir1);
				return;
			}

			if (move(dir2)) {
				setDirection(dir2);
				return;
			}
		}
	}

	if (dist_z <= 80 * 80) { // USING SQUARE BECAUSE EUCLIDIAN

		int bestDir = -1;
		double bestDist = dist_z;
		for (Direction dir = 0; dir < 360; dir += 90) {
			if (canMove(dir)) {
				double dest_x, dest_y;
				newLoc(dir, dest_x, dest_y);
				double newdist = getWorld()->distZombiesSq(dest_x, dest_y);
				if (newdist > bestDist) {
					bestDir = dir;
					bestDist = newdist;
				}
			}
		}
		cerr << bestDir << endl;
		switch (bestDir) {
		case -1:
			return;
		case up:
		case down:
		case left:
		case right:
			setDirection(bestDir);
			move(bestDir);
			return;
		}
	}
}

void Zombie::die() {
	Actor::die();
	getWorld()->playSound(SOUND_ZOMBIE_DIE);
	getWorld()->increaseScore(1000);
}

void Zombie::doZombieStuff() {
	if (getParalysis()) {
		unparalyze();
		return;
	}

	double vomitX = getX();
	double vomitY = getY();

	switch (getDirection()) {
	case up:
		vomitY += SPRITE_HEIGHT;
		break;
	case down:
		vomitY -= SPRITE_HEIGHT;
		break;
	case right:
		vomitX += SPRITE_WIDTH;
		break;
	case left:
		vomitX -= SPRITE_WIDTH;
		break;
	}

	if (getWorld()->wouldInfect(vomitX, vomitY))
		if (randInt(0, 2) == 0) {
			getWorld()->addVomit(vomitX / SPRITE_WIDTH, vomitY / SPRITE_HEIGHT, getDirection());
			getWorld()->playSound(SOUND_ZOMBIE_VOMIT);
			return;
		}
}

void DumbZombie::die() {
	Zombie::die();
	if (randInt(0, 9) == 0) {
		Direction dir = 90 * randInt(0, 3);
		double nx, ny;
		switch (dir) {
		case up:
		case down:
			newLoc(dir, nx, ny, SPRITE_HEIGHT);
			break;
		case left:
		case right:
			newLoc(dir, nx, ny, SPRITE_WIDTH);
			break;
		}
		if (getWorld()->isEmpty(nx, ny))
			getWorld()->addVaccineGoodie(nx / SPRITE_WIDTH, ny / SPRITE_HEIGHT);

	}
}

void DumbZombie::doSomething() {
	if (!doLivingActorStuff())
		return;
	
	doZombieStuff();

	if (getMovementPlanDist() == 0) {
		setMovementPlanDist(randInt(3, 10));
		setDirection(90 * randInt(0, 3)); // not sure
	}

	if (move(getDirection())) {
		decMovementPlanDist();
	}
	else {
		setMovementPlanDist(0);
	}
}

void SmartZombie::doSomething() {
	if (!doLivingActorStuff())
		return;
	doZombieStuff();

	if (getMovementPlanDist() == 0) {
		setMovementPlanDist(randInt(3, 10));

		double dx, dy;
		getWorld()->findInfectableTarget(this, dx, dy);

		if (dx*dx + dy * dy > 80 * 80) // USING SQAURE
			setDirection(90 * randInt(0, 3));
		else {
			Direction dir;

			if (dx > 0)
				if (dy > 0)
					dir = 45;
				else if (dy == 0)
					dir = right;
				else
					dir = 315;
			else if (dx == 0)
				if (dy > 0)
					dir = up;
				else
					dir = down;
			else
				if (dy > 0)
					dir = 135;
				else if (dy == 0)
					dir = left;
				else
					dir = 225;

			switch (dir) {
			case left:
			case right:
			case up:
			case down:
				break;
			default:
				dir = 90 * static_cast<int>(dir / 90 + randInt(0, 1)) % 360;
			}
			setDirection(dir);
		}
	}

	if (move(getDirection())) {
		decMovementPlanDist();
	}
	else {
		setMovementPlanDist(0);
	}
}