#include <iostream>
#include <functional>
#include <string>
#include <map>
#include <utility>
#include <stdlib.h>
#include "Pitch.h"
#include "Vec2.h"
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

vector<Agent*> Pitch::initialize(){
	map<string, pair<Agent*, Platform*>> agent_end;
	Platform* finish = nullptr;
    PlatformFactory pf;
	int epoch;
	double alpha, gamma;
	cin >> epoch >> alpha >> gamma;
	
	double maxRe = 0;
	for (int i = 0; i < x; i++) {
		for (int j = 0; j < y; j++) {
			string input; cin >> input;
			vector<string> settings = split(input, ',');
			string id = settings[0].substr(1, settings[0].size() - 1);
			char *a;
			double reward = strtod(settings[1].c_str(), &a);

			pitch[i][j] = pf.create(Vec2(i, j), settings[0][0], reward);
			if(settings[0][0] == 'E'){ 
				agent_end[id].second = pitch[i][j]; 
			} else if(settings[0][0] == 'A'){ 
				agent_end[id].first = new Agent(id, x, y, pitch[i][j], epoch, alpha, gamma); 
			}
			if (reward > maxRe) { maxRe = reward; }
        }
	}
	
	for (auto const &agent_end_pair : agent_end) {
		auto without_id = agent_end_pair.second;
		agents.push_back(without_id.first);
		if (without_id.first && without_id.second) {
			without_id.first->setEnd(without_id.second); 
			without_id.first->randomizeQ(maxRe);
		}
	}
	
	link();

	return agents;
}

void Pitch::learn(function<void()> callAfterStep) {
	bool end = false;
	while (!end) {
		end = true;
		for (int i = 0; i < agents.size(); i++) {
			if (agents[i]->learn()) {
				cout << *this << endl;
				end = false;
				callAfterStep();
			}
		}
	}
}

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

Vec2 Pitch::getSize() const {
	return Vec2(x, y);
}

void Pitch::draw(ostream &os) const {
	for (int i = 0; i < x; i++) {
		for (int j = 0; j < y; j++) {
			pitch[i][j]->draw(os);
			os << ',';
			agents[0]->draw(os, i, j);
			os << ' ';
		}
		os << endl;
	}
}

Platform** Pitch::operator[](const int &x) {
	return pitch[x];
}

ostream & operator<<(ostream & os, const Pitch &pitch) {
	pitch.draw(os);
	return os;
}