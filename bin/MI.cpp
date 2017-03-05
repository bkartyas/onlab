#include <iostream>
#include <stdlib.h>
#include <vector>
#include <string>

using namespace std;


class Agent;

class Vec2 {
public:
	int x, y;

	Vec2() : x(0), y(0) {}
	Vec2(const int &x, const int &y) : x(x), y(y) {}

	Vec2 operator+(const Vec2 &other) {
		return Vec2(other.x+x, other.y+y);
	}
};


class Platform {
protected:
	friend class Pitch;
	double reward;
    Agent *agent;
	Platform *left;
	Platform *up;
	Platform *right;
	Platform *down;

public:

	Platform(const double &reward): reward(reward) {}

	bool step(Agent *agent){
        this->agent = agent;
        return false;
	};

	virtual void draw(ostream &os) {
		cout << "P;" << (reward) << ' ';
	}
};

class WallPlatform : public Platform {
public:

	WallPlatform(const double &reward) : Platform(reward) {}
	virtual void draw(ostream &os) {
		cout << "W;" << reward << ' ';
	}

	bool step(const Agent *agent){
	    return false;
	}
};

class Pitch {
	int x, y;
	Platform*** pitch;
	Platform* start;
	Platform* end;

public:
	Pitch(const int &x, const int &y): x(x), y(y) {
		pitch = new Platform**[x];
		for (int i = 0; i < x; i++) {
			pitch[i] = new Platform*[y];
		}
	}

	~Pitch() {
		for (int i = 0; i < x; i++) {
			delete pitch[i];
		}
		delete pitch;
	}

	void add(const Vec2 &pos, const char &type, const double & weight) {

		switch (type)
		{
		case 'P':
			pitch[pos.x][pos.y] = new Platform(weight);
			break;
		case 'A':
			//Agent *agent = new Agent(this);
			pitch[pos.x][pos.y] = new Platform(weight);
			//Agent->setPlatform(pitch[pos.x][pos.y]);
			break;
		case 'E':
			pitch[pos.x][pos.y] = new Platform(weight);
			break;
		case 'W':
			pitch[pos.x][pos.y] = new WallPlatform(weight);
			break;
		default:
			break;
		}
	}

	void link() {
		for (int i = 0; i < x; i++) {
			for (int j = 0; j < y; j++) {
				if (i - 1 >= 0) {
					pitch[i][j]->left = pitch[i - 1][j];
				}
				if (j - 1 >= 0) {
					pitch[i][j]->up = pitch[i][j - 1];
				}
				if (i + 1 < x) {
					pitch[i][j]->right = pitch[i + 1][j];
				}
				if (j + 1 < y) {
					pitch[i][j]->down = pitch[i][j + 1];
				}
			}
		}
	}

	Platform** operator[](const int &x) {
		return pitch[x];
	};

	void setReward(const Vec2 &pos, const double &reward) const {

	}

	void step(Agent &Agent, const Vec2 &dir) const {

	}

	Vec2 getSize() const {
		return Vec2(x, y);
	}

	void draw(ostream &os) const {
		for (int i = 0; i < x; i++) {
			for (int j = 0; j < y; j++) {
				pitch[i][j]->draw(os);
			}
			os << endl;
		}
	}
};

ostream & operator<<(ostream & os, const Pitch &pitch) {
	pitch.draw(os);
	return os;
}

class Knowledge{
	int x, y;
	double **thoughts;
public:
	Knowledge(const int &x, const int &y): x(x), y(y){
		thoughts = new double*[x];
		for (int i = 0; i < x; i++) {
			thoughts[i] = new double[y];
		}

		for (int i = 0; i < x; i++) {
			for (int j = 0; j < y; j++) {
				thoughts[i][j] = 0.0;
			}
		}
	}

	~Knowledge() {
		for (int i = 0; i < x; i++) {
			delete thoughts[i];
		}
		delete thoughts;
	}
};

class Agent {
	Knowledge knowledge;
	Platform *platform;
public:
	Agent(const Pitch & pitch) : knowledge(pitch.getSize().x*pitch.getSize().y, 4) {}

    void setPlatform(Platform *platform) { this->platform = platform; platform->step(this); }

	void setWeight(const Vec2 &dir, const double &w) {}

	void step(const Vec2 dir) {
		//pitch.step(this, position+dir);
	};
};


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

int main()
{
	cout << "start" << endl;

	int sizeX, sizeY;
	cin >> sizeX >> sizeY;
	Pitch pitch(sizeX, sizeY);
	Agent agent(pitch);

	for (int i = 0; i < sizeY; i++) {
		for (int j = 0; j < sizeX; j++) {
			string input; cin >> input;
			vector<string> settings = split(input, ';');
			char *a;
			pitch.add(Vec2(i, j), settings[0][0], strtod(settings[1].c_str(), &a));
			//cout << settings[0][0] << stod(settings[1]);
		}
	}
	cout << endl;
	//pitch.link();
	cout << pitch;
	cout << pitch;
}
