#include <functional>
#include <string>
#include "Agent.h"
#include "EndPlatform.h"

using namespace std;

static unsigned long x = 123456789, y = 362436069, z = 521288629;

unsigned long xorshf96(void) {          //period 2^96-1
	unsigned long t;
	x ^= x << 16;
	x ^= x >> 5;
	x ^= x << 1;

	t = x;
	x = y;
	y = z;
	z = t ^ x ^ y;

	return z;
}


Knowledge::Knowledge(const int &x, const int &y, const int &action): x(x), y(y), action(action){
	thoughts = new double**[x];

	for (int i = 0; i < x; i++) {
		thoughts[i] = new double*[y];
		for (int j = 0; j < y; j++) {
			thoughts[i][j] = new double[action];
			for(int k = 0; k < action; k++){
					thoughts[i][j][k] = 0.0;
			}
        }
	}
}

void Knowledge::randomize(const double max) {
	int about = (int) max / 10;
	if (about == 0) { about = 1; }
	for (int i = 0; i < x; i++) {
		for (int j = 0; j < y; j++) {
			for (int k = 0; k < action; k++) {
				thoughts[i][j][k] += (int)(xorshf96() % about * 2) - about;
			}
		}
	}
};

Knowledge::~Knowledge() {
	for (int i = 0; i < x; i++) {
		for (int j = 0; j < y; j++) {
			delete thoughts[i][j];
		}
		delete thoughts[i];
	}
	delete thoughts;
}

ostream& Knowledge::draw(ostream& os) const{
    for(int i = 0; i < x; i++){
        for(int j = 0; j < y; j++){
             for(int k = 0; k < this->action; k++){
                cout << thoughts[i][j][k] << ",";
            }
            cout << "\t";
        }
        cout << endl;
    }

	return os;
};

ostream& operator<<(ostream & os, const Knowledge &knowledge) {
	knowledge.draw(os);
	return os;
}


Agent::Agent(const string &id, const int &x, const int &y, Platform *start, const int &epoch, const double &alpha, const double &gamma):
				id(id), knowledge(x, y, 4), start(start), platform(start), epoch(epoch), alpha(alpha), gamma(gamma){
	start->step(this);
}

void Agent::randomizeQ(const double max) {
	knowledge.randomize(max);
};

bool Agent::learn(){
	if (epoch > 0) {
		if (platform == finish.platform) {
			start->step(this);
			platform->step(nullptr);
			platform = start;
			epoch--;
		} else {
			learnStep();
		}
		return true;
	}

	return false;
};

void Agent::learnStep() {
	Vec2 pos = platform->getPosition();

	int direction = stepLeft;
	if (((int)(xorshf96() % 10) - 4)) {
		for (int i = 1; i <= 3; i = i + 1) {
			if (knowledge.thoughts[pos.x][pos.y][direction] < knowledge.thoughts[pos.x][pos.y][i] ||
				(knowledge.thoughts[pos.x][pos.y][direction] == knowledge.thoughts[pos.x][pos.y][i] && (int)(xorshf96() % 2))) {
				direction = i;
			}
		}
	} else {
		direction = (int)(xorshf96() % 4);
	}

	Direction dir;
	switch (direction) {
	case stepLeft:
		dir = Direction::left;
		break;
	case stepUp:
		dir = Direction::up;
		break;
	case stepRight:
		dir = Direction::right;
		break;
	case stepDown:
		dir = Direction::down;

		break;
	}

	double reward = step(dir);
	if (platform == finish.platform) { reward = finish.reward; }

	Vec2 newPos = platform->getPosition();
	int newMax = 0;
	for (int i = 1; i <= 3; i = i + 1) {
		if (knowledge.thoughts[newPos.x][newPos.y][newMax] < knowledge.thoughts[newPos.x][newPos.y][i]) {
			newMax = i;
		}
	}

	knowledge.thoughts[pos.x][pos.y][direction] += alpha * (reward + gamma * knowledge.thoughts[newPos.x][newPos.y][newMax] - knowledge.thoughts[pos.x][pos.y][direction]);
}

void Agent::setEnd(EndPlatform platform) {
    finish = platform;
    Vec2 pos = platform.platform->getPosition();

    for(int i = 0; i < 4; i++){
        knowledge.thoughts[pos.x][pos.y][i] = platform.platform->getReward();
    }
}

double Agent::step(Platform *next){
    if(next && next->step(this)){
        platform->step(nullptr);
        platform = next;
        return platform->getReward();
    }
    return -1000.0;
}

double Agent::step(const Direction &dir) {
    Platform* next = platform->inDirection(dir);

    if(next){
		if (next->step(this)) {
			platform->step(nullptr);
			platform = next;
		}

        return platform->getReward();
    }
    return -1000.0;
}

ostream& Agent::draw(ostream &os, const int &i, const int &j) const {
	for (int k = 0; k < 4; k++) {
		os << knowledge.thoughts[i][j][k];
		if (k != 3) {
			os << ",";
		}
	}

	return os;
}

ostream& Agent::draw(ostream &os) const {
	os << "A" << this->id;

	return os;
}