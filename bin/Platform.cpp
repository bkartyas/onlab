#include "Platform.h"
#include "Agent.h"
#include "Pitch.h"

using namespace std;

Platform::Platform(Pitch *pitch, const Vec2 &pos, const double &reward, const double &changeReward, const char& type): 
	pitch(pitch), reward(reward), changeReward(changeReward), position(pos), type(type) {}
Platform::Platform(const Platform& platform): 
	pitch(platform.pitch), reward(platform.reward), changeReward(platform.changeReward), position(platform.position), type(platform.type) {};

double Platform::getReward(){ return reward; };
double Platform::getChangeReward() { return changeReward; };
void Platform::setReward(const double& reward) { this->reward = reward; };

Vec2 Platform::getPosition(){ return position; };

bool Platform::changePlatform(const char& type) {
	if (this->isChangeable()) { return false; }
	
	Platform *platform = nullptr;
	if (type == 'P') { platform = new NormalPlatform(pitch, position, reward, changeReward); }
	else if (type == 'W') { platform = new WallPlatform(pitch, position, reward, changeReward); }
	else { return false; }

	(*pitch)[this->position.x][this->position.y] = platform;
	
	platform->pitch = this->pitch;
	platform->reward = this->reward;
	platform->left = this->left;
	platform->up = this->up;
	platform->right = this->right;
	platform->down = this->down;

	if (platform->left) { platform->left->right = platform; };
	if (platform->up) { platform->up->down = platform; };
	if (platform->right) { platform->right->left = platform; };
	if (platform->down) { platform->down->up = platform; };

	return true;
}

Platform* Platform::inDirection(const Direction &dir) const {
    Platform* next = nullptr;
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

NormalPlatform::NormalPlatform(Pitch *pitch, const Vec2 &position, const double& reward, double changeReward): Platform(pitch, position, reward, changeReward, 'P') {};
NormalPlatform::NormalPlatform(const NormalPlatform& platform) : Platform(platform), agent(platform.agent) {};

bool NormalPlatform::isChangeable() {
	return ((bool) agent) || pitch->isStartOrFinish(this);
};

double NormalPlatform::getReward() {
	if (this->agent) {
		return -100;
	}

	return Platform::getReward();
}

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

Platform* NormalPlatform::clone() const {
	return new NormalPlatform(*this);
}


ostream& NormalPlatform::draw(ostream &os) const {
	if(!agent){
        os << "P";
	} else {
		agent->draw(os);
	}
	os << "," << reward;

	return os;
}


WallPlatform::WallPlatform(Pitch *pitch, const Vec2 &position, const double &reward, double changeReward): Platform(pitch, position, reward, changeReward, 'W'){};
WallPlatform::WallPlatform(const WallPlatform& platform) : Platform(platform){};

bool WallPlatform::isChangeable() {
	return pitch->isStartOrFinish(this);
};

double WallPlatform::getReward() {
	return reward - 10;
};

bool WallPlatform::step(Agent *agent){
    return false;
}

Platform* WallPlatform::clone() const {
	return new WallPlatform(*this);
}

ostream& WallPlatform::draw(ostream &os) const {
	os << "W," << reward;
	return os;
}


Platform* PlatformFactory::create(Pitch *pitch, const Vec2 &pos, char type, double reward, double changeReward){
    if(type == 'W'){
        return new WallPlatform(pitch, pos, reward, changeReward);
    } else {
        return new NormalPlatform(pitch, pos, reward, changeReward);
    }
}
