#include <functional>
#include <string>
#include <time.h>
#include "Agent.h"
#include "EndPlatform.h"

using namespace std;

Action Knowledge::action = Action();

Knowledge::Knowledge(const int &x, const int &y): x(x), y(y){
	thoughts = new double**[x];

	for (int i = 0; i < x; i++) {
		thoughts[i] = new double*[y];
		for (int j = 0; j < y; j++) {
			thoughts[i][j] = new double[action.size];
			for (int k = 0; k < action.size; k++) {
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
			for (int k = 0; k < action.size; k++) {
				thoughts[i][j][k] += (int)(rand() % (about * 2)) - about;
			}
		}
	}
}

int& Knowledge::maxIndex(const int& x, const int& y, const int& offset) const {
	int max = offset;

	for (int i = offset+1; i < offset + action.groupSize; i += 1) {
		if (thoughts[x][y][max] < thoughts[x][y][i] ||
			(thoughts[x][y][max] == thoughts[x][y][i] && (int)(rand() % 2))) {
			max = i;
		}
	}

	return max;
}

int& Knowledge::bestAction(const int& x, const int& y) const {
	int maxI = 0;
	int actions[] = {
		maxIndex(x, y, 0 * action.groupSize),
		maxIndex(x, y, 1 * action.groupSize),
		maxIndex(x, y, 2 * action.groupSize),
	};

	for (int i = 1; i < 3; i++) {
		if (thoughts[x][y][actions[maxI]] < thoughts[x][y][actions[i]]) {
			maxI = i;
		}
	}
	return actions[maxI];
};

int Knowledge::random() const {
	return rand() % action.size;
}

Direction Knowledge::direction(const int& a) const {
	switch (a % 4) {
	case action.stepLeft:
		return Direction::left;
		break;
	case action.stepUp:
		return Direction::up;
		break;
	case action.stepRight:
		return Direction::right;
		break;
	case action.stepDown:
		return Direction::down;
		break;
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

ostream& Knowledge::draw(ostream& os, const int& x, const int& y) const {
	int offset = (bestAction(x, y) / action.groupSize) * action.groupSize;
	for (int i = offset; i < offset + action.groupSize; i++) {
		os << thoughts[x][y][i] << ",";
	}

	os << thoughts[x][y][maxIndex(x, y, 1 * action.groupSize)] << ","
		<< thoughts[x][y][maxIndex(x, y, 2 * action.groupSize)];

	return os;
};

ostream& Knowledge::draw(ostream& os) const{
    for(int i = 0; i < x; i++){
        for(int j = 0; j < y; j++){
             for(int k = 0; k < action.size; k++){
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


Agent::Agent(const string &id, const int &x, const int &y, Platform *start, const double &alpha, const double &gamma):
				id(id), knowledge(x, y), start(start), platform(start), alpha(alpha), gamma(gamma), epoch(0){
	start->step(this);
}

void Agent::randomizeQ(const double max) {
	knowledge.randomize(max);
};

bool Agent::learn(){
	if (epoch > 0) {
		if (platform == finish.platform) {
			return false;
		} 

		learnStep();
		return true;
	} else if (platform != finish.platform) {
		stepNext();
	}

	return false;
};

void Agent::learnStep() {
	Platform* old = platform;
	Vec2 pos = platform->getPosition();

	int action = 0; 
	
	if (((int)(rand() % 100) - 35)) {
		action = knowledge.bestAction(pos.x, pos.y);
	} else {
		action = knowledge.random();
	}

	Direction dir = knowledge.direction(action);

	double reward = 0.0;
	if (action / knowledge.action.groupSize < 1) {
		reward = step(dir);
		if (platform == finish.platform) { reward = finish.reward; }
	} else {
		reward = changePlatform(dir, action / knowledge.action.groupSize);
	}
	
	Vec2 newPos = platform->getPosition();
	int newMax = knowledge.bestAction(newPos.x, newPos.y);

	knowledge.thoughts[pos.x][pos.y][action] += alpha * (reward + gamma * knowledge.thoughts[newPos.x][newPos.y][newMax] - knowledge.thoughts[pos.x][pos.y][action]);
}

void Agent::stepNext() {
	Platform* old = platform;
	Vec2 pos = platform->getPosition();

	int action = knowledge.bestAction(pos.x, pos.y);

	Direction dir = knowledge.direction(action);

	if (action / knowledge.action.groupSize < 1) {
		step(dir);
	} 
	else { changePlatform(dir, action / knowledge.action.groupSize); }
}

void Agent::setEnd(EndPlatform platform) {
    finish = platform;
    Vec2 pos = platform.platform->getPosition();

    for(int i = 0; i < 4; i++){
        knowledge.thoughts[pos.x][pos.y][i] = finish.reward;
    }
}

void Agent::newStartOrEnd(Platform* from, Platform* to) {
	if (from == start) {
		start = to;
	}
	if (from == finish.platform) {
		finish.platform = to;
	}
	if (from == platform) {
		platform = to;
	}
}

void Agent::setEpoch(const int& epoch) {
	this->epoch = epoch;
};

bool Agent::isStartOrFinish(const Platform* platform) {
	return (platform == start || platform == finish.platform);
}

double Agent::step(Platform *next){
	if(next){
		double reward = next->getReward();
		if (next->step(this)) {
			built = 0;
			platform->step(nullptr);
			platform = next;
		}

		return reward;
    }
    return -1000.0;
}

double Agent::step(const Direction &dir) {
	Platform* next = platform->inDirection(dir);

	return step(next);
}

double Agent::changePlatform(const Direction &dir, const int& act) {
	Platform* next = platform->inDirection(dir);

	if (!next) {
		return -1000.0;
	}

	double reward = -10;

	if (act == 1) {
		built = 1;
		if (numberOfWalls > 0 && next->type != 'W' && next->changePlatform('W')) { numberOfWalls--; reward = next->getChangeReward(); delete next; };
	} else if (act == 2) {
		if (next->type != 'P' && next->changePlatform('P')) { reward = next->getChangeReward();  delete next;};
	}

	return reward;
}

bool Agent::restart() {
	if (platform != start && start->step(this)) {
		numberOfWalls = 3;
		platform->step(nullptr);
		platform = start;
		epoch--;
	}

	return epoch > 0 ? true : false;
}

ostream& Agent::draw(ostream &os, const int &i, const int &j) const {
	knowledge.draw(os, i, j);

	return os;
}

ostream& Agent::draw(ostream &os) const {
	os << "A" << this->id;

	return os;
}