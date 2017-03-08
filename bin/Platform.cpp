#include "Platform.h"
#include "Agent.h"

using namespace std;

Platform::Platform(const Vec2 &pos, const double &reward): reward(reward), position(pos) {}

double Platform::getReward(){ return reward; };

Vec2 Platform::getPosition(){ return position; };

Platform* Platform::inDirection(const Direction &dir){
    Platform* next;
    switch(dir){
        case Direction::left:
            next = left;
            break;
        case Direction::up:
            next = up;
            break;
        case Direction::right:
            next = right;
            break;
        case Direction::down:
            next = down;
            break;
    }

    return next;
}

NormalPlatform::NormalPlatform(const Vec2 &position, const double& reward): Platform(position, reward) {};

bool NormalPlatform::step(Agent *agent){
    if(!agent){
        this->agent = nullptr;
        return true;
    }
    if(!this->agent){
        this->agent = agent;
        return true;
    }
    return false;
}

void NormalPlatform::draw(ostream &os) {
	if(!agent){
        cout << "P;" << (reward) << ' ';
	} else {
        cout << "A;" << (reward) << ' ';
	}
}


WallPlatform::WallPlatform(const Vec2 &position, const double& reward): Platform(position, reward) {};

bool WallPlatform::step(Agent *agent){
    return false;
}

void WallPlatform::draw(ostream &os) {
	cout << "W;" << reward << ' ';
}


Platform* PlatformFactory::create(const Vec2 &pos, char type, double reward){
    if(type == 'W'){
        return new WallPlatform(pos, reward);
    } else {
        return new NormalPlatform(pos, reward);
    }
}
