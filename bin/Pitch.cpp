#include <iostream>
#include <string>
#include <stdlib.h>
#include "Pitch.h"
#include "Platform.h"

using namespace std;

vector<string> split(const string &s, const char &delimiter) {
	vector<string> result;
	int before = 0;
	for (int i = 0; i < s.size(); i++) {
		if (s[i] == delimiter) {
			result.push_back(s.substr(before, i));
			before = i+1;
		}
	}

	result.push_back(s.substr(before, s.size()));
	return result;
}


Pitch::Pitch(const int &x, const int &y): x(x), y(y) {
	pitch = new Platform**[x];
	for (int i = 0; i < x; i++) {
		pitch[i] = new Platform*[y];
	}
}

Pitch::~Pitch() {
	for (int i = 0; i < x; i++) {
		delete pitch[i];
	}
	delete pitch;
}

Agent* Pitch::initialize(){
    Platform* finish = nullptr;
    PlatformFactory pf;

	for (int i = 0; i < x; i++) {
		for (int j = 0; j < y; j++) {
			string input; cin >> input;
			vector<string> settings = split(input, ',');
			char *a;
			pitch[i][j] = pf.create(Vec2(i, j), settings[0][0], strtod(settings[1].c_str(), &a));
			if(settings[0][0] == 'E'){ finish = pitch[i][j]; } else
            if(settings[0][0] == 'A'){ agents = new Agent(x, y, pitch[i][j]); }
        }
	}

	if (agents && finish) {
		agents->setEnd(finish);
	}
	link();

	return agents;
};

void Pitch::link() {
	for (int i = 0; i < x; i++) {
		for (int j = 0; j < y; j++) {
			if (i - 1 >= 0) {
				pitch[i][j]->up = pitch[i - 1][j];
			}
			if (j - 1 >= 0) {
				pitch[i][j]->left = pitch[i][j - 1];
			}
			if (i + 1 < x) {
				pitch[i][j]->down = pitch[i + 1][j];
			}
			if (j + 1 < y) {
				pitch[i][j]->right = pitch[i][j + 1];
			}
		}
	}
}

Platform** Pitch::operator[](const int &x) {
	return pitch[x];
};

Agent* Pitch::getAgents(){
	return agents;
}

Vec2 Pitch::getSize() const {
	return Vec2(x, y);
}

void Pitch::draw(ostream &os) const {
	for (int i = 0; i < x; i++) {
		for (int j = 0; j < y; j++) {
			pitch[i][j]->draw(os);
			os << ',';
			agents->draw(os, i, j);
			os << ' ';
		}
		os << endl;
	}
}

ostream & operator<<(ostream & os, const Pitch &pitch) {
	pitch.draw(os);
	return os;
}
