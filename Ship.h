#ifndef __SHIP__
#define __SHIP__
#include "StandardObject.h"

class Ship : public StandardObject
{
public:
	Ship();
	~Ship();
	void draw();
	void update();
	void setup();
	
	void yaw(const float inYaw);
	void pitch(const float inPitch);
	void roll(const float inRoll);
	
	void setYawSettings(const float inMaxYaw, const float inRateOfYaw);
	void setPitchSettings(const float inMaxPitch, const float inRateOfPitch);
	void setRollSettings(const float inMaxRoll, const float inRateOfRoll);


protected:
	void updateYaw();
	void updatePitch();
	void updateRoll();


	float desiredYaw;
	float desiredPitch;
	float desiredRoll;
	float actualYaw;
	float actualPitch;
	float actualRoll;
	float maxYaw;
	float maxPitch;
	float maxRoll;
	float rateOfYaw;
	float rateOfPitch;
	float rateOfRoll;
	float desiredAcceleration;
	float actualAcceleration;
	float maxAcceleration;
	float rateOfAcceleration;
	float speed;
private:
};

#endif
