#include "Ship.h"
#include "StandardMesh.h"
#include "ObjectSocket.h"

Ship::Ship()
{
	for(unsigned int i = 0; i < 10; i++)
	{
		components.push_back(0);
	}
	desiredYaw = 0;
	desiredPitch = 0;
	desiredRoll = 0;
	actualYaw = 0;
	actualPitch = 0;
	actualRoll = 0;
	maxYaw = 0;
	maxPitch = 0;
	maxRoll = 0;
	rateOfYaw = 0;
	rateOfPitch = 0;
	rateOfRoll = 0;
	desiredAcceleration = 0;
	actualAcceleration = 0;
	maxAcceleration = 0;
	rateOfAcceleration = 0;
	maxSpeed = 0;
	speed = 0;

}

Ship::~Ship()
{
}

void Ship::draw()
{
	ModelMesh* temp = (ModelMesh*)components.at(0);
	glm::mat4 translateMatrix = glm::translate(glm::mat4(1.0f), position);
	glm::mat4 rotationMatrix = glm::toMat4(rotation);
	glm::mat4 matrix = translateMatrix * rotationMatrix;
	temp->updateModelMatrix((float*)glm::value_ptr(matrix));
	temp->draw();
}



void Ship::update(double timeLapse)
{
	if(components.at(1) != 0)
	{
		updateInputDevices();
	}
	updateYaw(timeLapse);
	updatePitch(timeLapse);
	updateRoll(timeLapse);
	updateAcceleration(timeLapse);
	if(sockets.size() > 0)
	{
		updateCamera();
	}

}

void Ship::setup()
{
	
}

void Ship::addMesh(Mesh* inMesh)
{
	components.at(0) = inMesh;
}

void Ship::addCamera(Camera* inCamera, glm::vec3 inPosition, glm::quat inRotation)
{
	ObjectSocket* cameraSocket = new ObjectSocket();
	cameraSocket->setPosition(inPosition);
	cameraSocket->setRotation(inRotation);
	cameraSocket->setChildObject(inCamera);
	sockets.push_back(cameraSocket);
}

Camera* Ship::getCamera()const
{
	return (Camera*)sockets.at(0)->getChildObject();
}

void Ship::addJoystick(JoystickDevice* inJoystick)
{
	components.at(1) = inJoystick;
}

void Ship::removeJoystick()
{
	components.at(1) = 0;
}


void Ship::yaw(const float inYaw)
{
	desiredYaw = inYaw*maxYaw;
}

void Ship::pitch(const float inPitch)
{
	desiredPitch = inPitch*maxPitch;
}

void Ship::roll(const float inRoll)
{
	desiredRoll = inRoll*maxRoll;
}

void Ship::accelerate(const float inAcceleration)
{
	desiredAcceleration = inAcceleration * maxAcceleration;
}


void Ship::setYawSettings(const float inMaxYaw, const float inRateOfYaw)
{
	maxYaw = inMaxYaw;
	rateOfYaw = inRateOfYaw;
}

void Ship::setPitchSettings(const float inMaxPitch, const float inRateOfPitch)
{
	maxPitch = inMaxPitch;
	rateOfPitch = inRateOfPitch;
}

void Ship::setRollSettings(const float inMaxRoll, const float inRateOfRoll)
{
	maxRoll = inMaxRoll;
	rateOfRoll = inRateOfRoll;
}

void Ship::setAccelerationSettings(const float inMaxAcceleration, const float inRateOfAcceleration, const float inMaxSpeed)
{
	maxAcceleration = inMaxAcceleration;
	rateOfAcceleration = inRateOfAcceleration;
	maxSpeed = inMaxSpeed;
}


void Ship::updateYaw(double timeLapse)
{
	actualYaw += (rateOfYaw * (desiredYaw - actualYaw)) * timeLapse;
	rotation *= glm::angleAxis(glm::radians(actualYaw),glm::vec3(0.0f, 1.0f, 0.0f));
}

void Ship::updatePitch(double timeLapse)
{
	actualPitch += (rateOfPitch * (desiredPitch - actualPitch)) * timeLapse;
	rotation *= glm::angleAxis(glm::radians(actualPitch),glm::vec3(1.0f, 0.0f, 0.0f));
}

void Ship::updateRoll(double timeLapse)
{
	actualRoll += (rateOfRoll * (desiredRoll - actualRoll)) * timeLapse;
	rotation *= glm::angleAxis(glm::radians(actualRoll),glm::vec3(0.0f, 0.0f, 1.0f));
}

void Ship::updateAcceleration(double timeLapse)
{
	actualAcceleration += (rateOfAcceleration * (desiredAcceleration - actualAcceleration)) * timeLapse;
	speed += actualAcceleration;
	if(speed > maxSpeed)
	{
		speed = maxSpeed;
	}
	if(speed < 0)
	{
		speed = 0;
	}
	
	glm::vec3 movement = glm::vec3(0.0f, 0.0f, (speed * (float)timeLapse));
	position +=  glm::toMat3(rotation) * movement;
}

void Ship::updateCamera()
{
	Camera* shipCamera = (Camera*)sockets.at(0)->getChildObject();
	shipCamera->setRotation(rotation * sockets.at(0)->getRotation());
	glm::mat3 shipRotMat = glm::toMat3(rotation);
	shipCamera->setPosition(position + (shipRotMat * sockets.at(0)->getPosition()));
	shipCamera->update(0);
}

void Ship::updateInputDevices()
{
	JoystickDevice* joystick = (JoystickDevice*)components.at(1);
	roll(joystick->getAxis(0));
	pitch(-joystick->getAxis(1));
	accelerate(-joystick->getAxis(2));
	yaw(-joystick->getAxis(3));
	
}



