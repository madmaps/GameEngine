#ifndef __SHIP__
#define __SHIP__
#include "StandardObject.h"
#include "Camera.h"
#include "Mesh.h"
#include "JoystickDevice.h"
#include "ThreeDimSound.h"

class Ship : public StandardObject
{
public:
	Ship();
	~Ship();
	void draw();
	void update(double timeLapse);
	void setup();
	
	void addMesh(Mesh* inMesh);
	void addCamera(Camera* inCamera,glm::vec3 inPosition, glm::quat inRotation);
	Camera* getCamera()const;
	void addJoystick(JoystickDevice* inJoystick);
	void removeJoystick();
	void addSoundDevice(ThreeDimSound* inSoundDevice);
	
	void yaw(const float inYaw);
	void pitch(const float inPitch);
	void roll(const float inRoll);
	
	void accelerate(const float inAcceleration);
	
	void fireWeapon();
	
	void setYawSettings(const float inMaxYaw, const float inRateOfYaw);
	void setPitchSettings(const float inMaxPitch, const float inRateOfPitch);
	void setRollSettings(const float inMaxRoll, const float inRateOfRoll);
	
	void setAccelerationSettings(const float inMaxAcceleration, const float inRateOfAcceleration, const float inMaxSpeed);


protected:
	void updateYaw(double timeLapse);
	void updatePitch(double timeLapse);
	void updateRoll(double timeLapse);
	void updateAcceleration(double timeLapse);
	void updateCamera();
	void updateShip(double inTimeLapse);
	void updateInputDevices();
	void updateSound();


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
	float maxSpeed;
	float speed;
private:
};

#endif
