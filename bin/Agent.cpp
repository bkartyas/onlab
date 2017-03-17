#include <stdlib.h>
#include <time.h>
#include "Agent.h"

using namespace std;

Knowledge::Knowledge(const int &x, const int &y, const int &action): x(x), y(y), action(action){
	thoughts = new double**[x];

	srand(time(NULL));
	for (int i = 0; i < x; i++) {
		thoughts[i] = new double*[y];
		for (int j = 0; j < y; j++) {
			thoughts[i][j] = new double[action];
			for(int k = 0; k < action; k++){
				//thoughts[i][j][k] = rand() % 100;
					thoughts[i][j][k] = 0.0;
			}
        }
	}
}

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


Agent::Agent(const int &x, const int &y, Platform *start) : knowledge(x, y, 4), start(start), platform(start) {
    start->step(this);
}

void Agent::learn(const double& alpha, const double& gamma){
    if(platform == finish){
        start->step(this);
        platform->step(nullptr);
        platform = start;
        return;
    }

    Vec2 pos = platform->getPosition();
    srand(time(NULL));
    int direction = stepLeft;
    for(int i = 1; i <= 3; i = i+1){
        if(knowledge.thoughts[pos.x][pos.y][direction] < knowledge.thoughts[pos.x][pos.y][i] ||
            (knowledge.thoughts[pos.x][pos.y][direction] == knowledge.thoughts[pos.x][pos.y][i] && rand() % 2 - 1)){
            direction = i;
        }
    }

    Direction dir;
    switch(direction){
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

    Vec2 newPos = platform->getPosition();
    int newMax = 0;
    for(int i = 1; i <= 3; i = i+1){
        if(knowledge.thoughts[newPos.x][newPos.y][newMax] < knowledge.thoughts[newPos.x][newPos.y][i]||
            (knowledge.thoughts[pos.x][pos.y][newMax] == knowledge.thoughts[pos.x][pos.y][newMax] && rand() % 2 - 1)){
            newMax = i;
        }
    }

    knowledge.thoughts[pos.x][pos.y][direction] += alpha * (reward + gamma * knowledge.thoughts[newPos.x][newPos.y][newMax] - knowledge.thoughts[pos.x][pos.y][direction]);
};

void Agent::setEnd(Platform *platform) {
    finish = platform;
    Vec2 pos = platform->getPosition();

    for(int i = 0; i < 4; i++){
        knowledge.thoughts[pos.x][pos.y][i] = platform->getReward();
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

    if(next && next->step(this)){
        platform->step(nullptr);
        platform = next;
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