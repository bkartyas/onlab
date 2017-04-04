#include <functional>
#include <string>
#include <time.h>
#include "Agent.h"
#include "EndPlatform.h"

using namespace std;

Action Agent::action = Action();

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
				thoughts[i][j][k] += (int)(rand() % (about * 2)) - about;
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
				id(id), knowledge(x, y, action.size), start(start), platform(start), epoch(epoch), alpha(alpha), gamma(gamma){
	start->step(this);
}

void Agent::randomizeQ(const double max) {
	knowledge.randomize(max);
};

bool Agent::learn(){
	if (epoch > 0) {
		if (platform == finish.platform) {
			if (start->step(this)) {
				platform->step(nullptr);
				platform = start;
				epoch--;
			}
		} else {
			learnStep();
		}
		return true;
	}

	if (platform == finish.platform) {
		start->step(this);
		platform->step(nullptr);
		platform = start;
		epoch--;
	}

	stepNext();

	return false;
};

void Agent::learnStep() {
	Platform* old = platform;
	Vec2 pos = platform->getPosition();

	int direction = action.stepLeft;
	int stepAction = action.stepLeft;
	
	if (((int)(rand() % 100) - 25)) {
		for (int i = 1; i < action.size; i += 1) {
			if (knowledge.thoughts[pos.x][pos.y][stepAction] < knowledge.thoughts[pos.x][pos.y][i] ||
				(knowledge.thoughts[pos.x][pos.y][stepAction] == knowledge.thoughts[pos.x][pos.y][i] && (int)(rand() % 2))) {
				if (i < 4) { direction = i; }
				stepAction = i;
			}
		}
	} else {
		stepAction = (int)(rand() % action.size);
		if (stepAction < 4) { direction = stepAction; }
		else { direction = (int)(rand() % 4); }
	}

	Direction dir;
	switch (direction) {
	case action.stepLeft:
		dir = Direction::left;
		break;
	case action.stepUp:
		dir = Direction::up;
		break;
	case action.stepRight:
		dir = Direction::right;
		break;
	case action.stepDown:
		dir = Direction::down;
		break;
	}

	double reward = 0.0;
	if (stepAction == direction) {
		reward = step(dir);
		if (platform == finish.platform) { reward = finish.reward; }
	} else {
		reward = changePlatform(dir, stepAction);
	}
	
	
	Vec2 newPos = platform->getPosition();
	int newMax = 0;
	for (int i = 1; i < action.size; i = i + 1) {
		if (knowledge.thoughts[newPos.x][newPos.y][newMax] < knowledge.thoughts[newPos.x][newPos.y][i]) {
			newMax = i;
		}
	}

	knowledge.thoughts[pos.x][pos.y][stepAction] += alpha * (reward + gamma * knowledge.thoughts[newPos.x][newPos.y][newMax] - knowledge.thoughts[pos.x][pos.y][stepAction]);
}

void Agent::stepNext() {
	Platform* old = platform;
	Vec2 pos = platform->getPosition();

	int direction = action.stepLeft;
	int stepAction = action.stepLeft;
	for (int i = 1; i < action.size; i += 1) {
		if (knowledge.thoughts[pos.x][pos.y][stepAction] < knowledge.thoughts[pos.x][pos.y][i] ||
			(knowledge.thoughts[pos.x][pos.y][stepAction] == knowledge.thoughts[pos.x][pos.y][i] && (int)(rand() % 2))) {
			if (i < 4) { direction = i; }
			stepAction = i;
		}
	}

	Direction dir;
	switch (direction) {
	case action.stepLeft:
		dir = Direction::left;
		break;
	case action.stepUp:
		dir = Direction::up;
		break;
	case action.stepRight:
		dir = Direction::right;
		break;
	case action.stepDown:
		dir = Direction::down;
		break;
	}

	if (stepAction == direction) { step(dir);} 
	else { changePlatform(dir, stepAction); }
}

void Agent::setEnd(EndPlatform platform) {
    finish = platform;
    Vec2 pos = platform.platform->getPosition();

    for(int i = 0; i < 4; i++){
        knowledge.thoughts[pos.x][pos.y][i] = finish.reward;
    }
}

bool Agent::isStartOrFinish(const Platform* platform) {
	return (platform == start || platform == finish.platform);
}

double Agent::step(Platform *next){
	if(next){
		//next->draw(cout); cout << endl;

		double reward = next->getReward();
		if (next->step(this)) {
			platform->step(nullptr);
			platform = next;
		}

		return reward;
    }
    return -1000.0;
}

double Agent::changePlatform(const Direction &dir, const int& act) {
	Platform* next = platform->inDirection(dir);

	if (!next) {
		return -1000.0;
	}

	if (act == action.changeToPlatform) {
		if (next->changePlatform('P')) { delete next; };
	}
	else if (act == action.changeToWall) {
		if (next->changePlatform('W')) { delete next; };
	}

	return -10.0;
}

double Agent::step(const Direction &dir) {
    Platform* next = platform->inDirection(dir);

    return step(next);
}

ostream& Agent::draw(ostream &os, const int &i, const int &j) const {
	for (int k = 0; k < action.size; k++) {
		os << knowledge.thoughts[i][j][k];
		if (k != action.size -1) {
			os << ",";
		}
	}

	return os;
}

ostream& Agent::draw(ostream &os) const {
	os << "A" << this->id;

	return os;
}