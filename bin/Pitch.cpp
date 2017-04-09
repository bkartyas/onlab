#include <iostream>
#include <functional>
#include <string>
#include <map>
#include <utility>
#include <Windows.h>
#include <stdlib.h>
#include "Pitch.h"
#include "Vec2.h"
#include "Platform.h"
#include "EndPlatform.h"

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
	srand(GetTickCount());
	pitch = new Platform**[x];
	for (int i = 0; i < x; i++) {
		pitch[i] = new Platform*[y];
	}
}

Pitch::~Pitch() {
	for (int i = 0; i < agents.size(); i++) {
		delete agents[i];
	}
	
	for (int i = 0; i < x; i++) {
		delete pitch[i];
	}
	delete pitch;
}

vector<Agent*> Pitch::initialize(){
	map<string, pair<Agent*, EndPlatform>> agent_end;
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

			pitch[i][j] = pf.create(this, Vec2(i, j), settings[0][0], reward);
			if(settings[0][0] == 'E'){ 
				pitch[i][j]->setReward(-10.0);
				agent_end[id].second = EndPlatform(pitch[i][j], reward);
			} else if(settings[0][0] == 'A'){ 
				agent_end[id].first = new Agent(id, x, y, pitch[i][j], alpha, gamma); 
			}
			if (reward > maxRe) { maxRe = reward; }
        }
	}
	
	for (auto const &agent_end_pairs : agent_end) {
		auto agent_end_pair = agent_end_pairs.second;
		agents.push_back(agent_end_pair.first);
		agent_end_pair.first->setEpoch(agents.size() * epoch / agent_end.size());
		if (agent_end_pair.first && agent_end_pair.second.platform) {
			agent_end_pair.first->setEnd(agent_end_pair.second);
			//agent_end_pair.first->randomizeQ(maxRe);
		}
	}
	
	link();

	return agents;
}

void Pitch::learn(function<void()> callAfterStep) {
	bool end = false;
	while (!end) {
		bool epochEnd = true;
		for (auto &agent: agents) {
			if (agent->learn()) {
				epochEnd = false;
			}
			cout << *this << endl;
			callAfterStep();
		}
		if (epochEnd) {
			end = true; 
			for (auto &agent : agents) {
				bool back = agent->restart();
				end = end && !back;
			}
			cout << *this << endl;
		}
	}
}

bool Pitch::isStartOrFinish(const Platform* platform) {
	for (auto agent : agents) {
		if (agent->isStartOrFinish(platform)) { return true; }
	}
	return false;
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

ostream& Pitch::draw(ostream &os) const {
	for (int i = 0; i < x; i++) {
		for (int j = 0; j < y; j++) {
			pitch[i][j]->draw(os) << ',';
			for (int k = 0; k < agents.size(); k++) {
				agents[k]->draw(os, i, j);
				if (k < agents.size() - 1) { os << ','; }
			}
			os << ' ';
		}
		os << endl;
	}

	return os;
}

Platform** Pitch::operator[](const int &x) {
	return pitch[x];
}

ostream & operator<<(ostream & os, const Pitch &pitch) {
	pitch.draw(os);
	return os;
}