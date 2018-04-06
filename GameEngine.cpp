#define GL_GLEXT_PROTOTYPES
#define GLM_ENABLE_EXPERIMENTAL
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>
#include <GL/gl.h>
#include <GL/glx.h> 
#include <X11/X.h> 
#include <X11/keysym.h>

#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/vec3.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/quaternion.hpp"

#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "assimp/Importer.hpp"

#include "LoadHelper.h"
#include "BmpLoader.h"
#include "StandardObject.h"
#include "BumpMapGLRenderer.h"
#include "SkyBoxGLRenderer.h"
#include "StandardMesh.h"
#include "Planet.h"
#include "SkyBox.h"
#include "SkyBoxMesh.h"
#include "Camera.h"
#include "Ship.h"

static int dblBuf[]  = {GLX_RGBA, GLX_DEPTH_SIZE, 16, GLX_DOUBLEBUFFER, None};

Display   *dpy;
Window     win;

int main(int argc, char **argv)
{
	XVisualInfo         *vi;
    Colormap             cmap;
    XSetWindowAttributes swa;
    GLXContext           cx;
    XEvent               event;

    dpy = XOpenDisplay(NULL);
    vi = glXChooseVisual(dpy, DefaultScreen(dpy), dblBuf);
    cx = glXCreateContext(dpy, vi, None, GL_TRUE);
	cmap = XCreateColormap(dpy, RootWindow(dpy, vi->screen), vi->visual, AllocNone);
    swa.colormap = cmap;
    swa.border_pixel = 0;
    swa.event_mask = KeyPressMask | ExposureMask | ButtonPressMask | StructureNotifyMask;
    
    win = XCreateWindow(dpy, RootWindow(dpy, vi->screen), 0, 0, 1920, 1080, 0, vi->depth, InputOutput, vi->visual, CWBorderPixel | CWColormap | CWEventMask, &swa);
    XSetStandardProperties(dpy, win, "OpenGl Test", "OpenGl Test", None, argv, argc, NULL);
    glXMakeCurrent(dpy, win, cx);
    XMapWindow(dpy, win);
    
    glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	
    GLuint shader_program = loadShaders("Shaders/NormalShader.vert","Shaders/NormalShader.frag");
    GLuint skyBoxShader = loadShaders("Shaders/SkyBoxShader.vert", "Shaders/SkyBoxShader.frag");
    
    Camera* shipCamera = new Camera(0.1f, 1000.0f, 67.0f, 1920, 1080);
    shipCamera->setPosition(glm::vec3(0.0f, 0.0f, 90.0f));
    shipCamera->setRotation(glm::angleAxis(0.0f,glm::vec3(0.0f, 0.0f, -1.0f)));
    shipCamera->update();
    
    

	BumpMapGLRenderer* moonRenderer = new BumpMapGLRenderer();
	loadNormalMesh(*moonRenderer, "meshes/planet.dae", 0, "Textures/moonDefuse.bmp", "Textures/moonNormal.bmp", "Textures/moonSpecular.bmp", "Textures/moonAmbient.bmp");
    moonRenderer->addShader(shader_program);
	moonRenderer->updateProjectionMatrix(shipCamera->getProjectionMatrix());
    moonRenderer->updateViewMatrix(shipCamera->getViewMatrix());
    moonRenderer->updateCameraLocation(shipCamera->getLocationMatrix());
    StandardMesh* moonMesh = new StandardMesh();
    moonRenderer->updateModelMatrix(moonMesh->getModelMatrix());
    moonMesh->addRenderer(moonRenderer);
    
	Planet* moon = new Planet();
    moon->addComponent(moonMesh);
    moon->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
    moon->setRotation(glm::angleAxis(0.0f, glm::vec3(0.0f, 0.0f, -1.0f)));
    
    BumpMapGLRenderer** shipRenderer = new BumpMapGLRenderer*[13];
	StandardMesh* shipMesh = new StandardMesh();
    for(int i = 0; i < 9; i++)
    {
		shipRenderer[i] = new BumpMapGLRenderer();
		loadNormalMesh(*shipRenderer[i], "meshes/tiebomber.dae", i, "Textures/Ship/ShipDef.bmp", "Textures/Ship/ShipNormal.bmp", "Textures/Ship/ShipSpec.bmp", "Textures/Ship/ShipAmb.bmp");
		shipRenderer[i]->addShader(shader_program);
		shipRenderer[i]->updateProjectionMatrix(shipCamera->getProjectionMatrix());
		shipRenderer[i]->updateViewMatrix(shipCamera->getViewMatrix());
		shipRenderer[i]->updateCameraLocation(shipCamera->getLocationMatrix());
		shipRenderer[i]->updateModelMatrix(shipMesh->getModelMatrix());
		shipMesh->addRenderer(shipRenderer[i]);
	}
	
	Ship* spaceHunter = new Ship();
    spaceHunter->addComponent(shipMesh);
    spaceHunter->setPosition(glm::vec3(0.0f, 0.0f, 50.0f));
    spaceHunter->setRotation(glm::angleAxis(0.0f, glm::vec3(0.0f, 0.0f, 1.0f)));
    
    Ship* anotherShip = new Ship();
    anotherShip->addComponent(shipMesh);
    anotherShip->setPosition(glm::vec3(10.0f, 0.0f, 50.0f));
    anotherShip->setRotation(glm::angleAxis(0.0f, glm::vec3(0.0f, 0.0f, 1.0f)));
	
	SkyBox* skyBox = new SkyBox();
	SkyBoxGLRenderer* skyBoxRenderer = new SkyBoxGLRenderer();
	loadSkyBoxMesh(*skyBoxRenderer, "meshes/box.dae", "Textures/SkyBox/skyBoxUp.bmp", "Textures/SkyBox/skyBoxDown.bmp", "Textures/SkyBox/skyBoxLeft.bmp", "Textures/SkyBox/skyBoxRight.bmp", "Textures/SkyBox/skyBoxFront.bmp", "Textures/SkyBox/skyBoxBack.bmp");
	skyBoxRenderer->addShader(skyBoxShader);
	skyBoxRenderer->updateProjectionMatrix(shipCamera->getProjectionMatrix());

	skyBoxRenderer->updateCameraRotation(shipCamera->getRotationMatrix());
	SkyBoxMesh* skyBoxMesh = new SkyBoxMesh();
	skyBoxMesh->addRenderer(skyBoxRenderer);
	skyBox->addComponent(skyBoxMesh);
	
	
	while (1)
    {
		shipCamera->update();
		moon->update();
		skyBox->update();
		spaceHunter->update();
		anotherShip->update();
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		skyBox->draw();
		moon->draw();
		spaceHunter->draw();
		anotherShip->draw();
		glXSwapBuffers(dpy, win);
		
        while(XPending(dpy))
        {
            XNextEvent(dpy, &event);
            switch (event.type)
            {
                case KeyPress:
                {
                    KeySym     keysym;
                    char       buffer[1];
                    if((XLookupString((XKeyEvent *)&event, buffer, 1, &keysym, NULL) == 1) && (keysym == (KeySym)XK_Escape))
                    {
                        exit(0);
                    }
                    if((XLookupString((XKeyEvent *)&event, buffer, 1, &keysym, NULL) == 1) && (keysym == (KeySym)XK_w))
                    {
                        glm::mat3 camRotMat = glm::toMat3(shipCamera->getRotation());
                        glm::vec3 movement = glm::vec3(0.0f, 0.0f, -0.3f);
                        glm::vec3 finalMovement = movement * camRotMat;
                        shipCamera->setPosition(shipCamera->getPosition() + finalMovement);
                    }
                   
					if((XLookupString((XKeyEvent *)&event, buffer, 1, &keysym, NULL) == 1) && (keysym == (KeySym)XK_s))
                    {
                        glm::mat3 camRotMat = glm::toMat3(shipCamera->getRotation());
                        glm::vec3 movement = glm::vec3(0.0f, 0.0f, 0.3f);
                        glm::vec3 finalMovement = movement * camRotMat;
                        shipCamera->setPosition(shipCamera->getPosition() + finalMovement);
                    } 
					if((XLookupString((XKeyEvent *)&event, buffer, 1, &keysym, NULL) == 1) && (keysym == (KeySym)XK_a))
                    {
                        glm::mat3 camRotMat = glm::toMat3(shipCamera->getRotation());
                        glm::vec3 movement = glm::vec3(-0.3f, 0.0f, 0.0f);
                        glm::vec3 finalMovement = movement * camRotMat;
                        shipCamera->setPosition(shipCamera->getPosition() + finalMovement);
                    }
					if((XLookupString((XKeyEvent *)&event, buffer, 1, &keysym, NULL) == 1) && (keysym == (KeySym)XK_d))
                    {
                        glm::mat3 camRotMat = glm::toMat3(shipCamera->getRotation());
                        glm::vec3 movement = glm::vec3(0.3f, 0.0f, 0.0f);
                        glm::vec3 finalMovement = movement * camRotMat;
                        shipCamera->setPosition(shipCamera->getPosition() + finalMovement);
                    }
					if((XLookupString((XKeyEvent *)&event, buffer, 1, &keysym, NULL) == 1) && (keysym == (KeySym)XK_q))
                    {
						shipCamera->setRotation(shipCamera->getRotation() * glm::angleAxis(glm::radians(-2.0f),glm::vec3(0.0f, 1.0f, 0.0f)));
                    }
					if((XLookupString((XKeyEvent *)&event, buffer, 1, &keysym, NULL) == 1) && (keysym == (KeySym)XK_e))
                    {
						shipCamera->setRotation(shipCamera->getRotation() * glm::angleAxis(glm::radians(2.0f),glm::vec3(0.0f, 1.0f, 0.0f)));
                    }
                }
					break;
                case ButtonPress:
                    break;
                case ConfigureNotify:
                    glViewport(0, 0, event.xconfigure.width, event.xconfigure.height);
                case Expose:
                    break;
            }
        } 


    }
	
	return 0;
}
