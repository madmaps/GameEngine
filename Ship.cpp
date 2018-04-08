#include "Ship.h"
#include "StandardMesh.h"

Ship::Ship()
{
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



void Ship::update()
{
	updateYaw();
	updatePitch();
	updateRoll();
}

void Ship::setup()
{
	
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


void Ship::updateYaw()
{
	actualYaw += (rateOfYaw * (desiredYaw - actualYaw));
	rotation *= glm::angleAxis(glm::radians(actualYaw),glm::vec3(0.0f, 1.0f, 0.0f));
}

void Ship::updatePitch()
{
	actualPitch += (rateOfPitch * (desiredPitch - actualPitch));
	rotation *= glm::angleAxis(glm::radians(actualPitch),glm::vec3(1.0f, 0.0f, 0.0f));
}

void Ship::updateRoll()
{
	actualRoll += (rateOfRoll * (desiredRoll - actualRoll));
	rotation *= glm::angleAxis(glm::radians(actualRoll),glm::vec3(0.0f, 0.0f, 1.0f));
}





/*shipCamera->setRotation(shipCamera->getRotation() * glm::angleAxis(glm::radians(-2.0f),glm::vec3(0.0f, 1.0f, 0.0f)));


glm::mat3 camRotMat = glm::toMat3(shipCamera->getRotation());
glm::vec3 movement = glm::vec3(0.3f, 0.0f, 0.0f);
glm::vec3 finalMovement = movement * camRotMat;
shipCamera->setPosition(shipCamera->getPosition() + finalMovement);*/
