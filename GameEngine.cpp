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
    
	glm::mat4 ident = glm::mat4(1.0f);
	glm::vec3 camPos = glm::vec3(0.0f, 0.0f, 90.0f);
    glm::quat camRot = glm::angleAxis(0.0f,glm::vec3(0.0f, 0.0f, -1.0f));
    
    glm::vec3 modelPos = glm::vec3(0.0f, 0.0f, -10.0f);
    glm::quat modelRot = glm::angleAxis(0.0f,glm::vec3(0.0f, 0.0f, -1.0f));
    
    glm::vec3 moonPosition = glm::vec3(0.0f, 0.0f, -3.0f);
    glm::quat moonRotationQuat = glm::angleAxis(0.0f, glm::vec3(-1.0f, 0.0f, 0.0f));
    
	glm::mat4 moonTranslateMatrix = glm::translate(ident,moonPosition);
	glm::mat4 moonRotationMatrix = glm::toMat4(moonRotationQuat);
	glm::mat4 moonMatrix = moonTranslateMatrix * moonRotationMatrix;
    


	glm::mat4 T = glm::translate(ident,-camPos);
	glm::mat4 R = glm::toMat4(camRot);
	glm::mat4 view_mat = R * T;
	
	glm::mat4 modelTranslate = glm::translate(ident,modelPos);
	glm::mat4 modelRotation = glm::toMat4(modelRot);
	glm::mat4 modelMatrix = modelTranslate * modelRotation;
	
	
	float near = 0.1f;
	float far = 1000.0f;
	float fov = (67.0f * 2 * M_PI) / 360;
	float aspect = (float)1024/(float)768;
	float range = tan(fov * 0.5f) * near;
	float Sx = (2.0f * near) / (range * aspect + range * aspect);
	float Sy = near / range;
	float Sz = -(far + near) / (far - near);
	float Pz = -(2.0f * far * near) / (far - near);
	float proj_mat[] = {Sx, 0.0f, 0.0f, 0.0f,
						0.0f, Sy, 0.0f, 0.0f,
						0.0f, 0.0f, Sz, -1.0f,
						0.0f, 0.0f, Pz, 0.0f};


	BumpMapGLRenderer moon;
	loadNormalMesh(moon, "meshes/planet.dae", "Textures/moonDefuse.bmp", "Textures/moonNormal.bmp", "Textures/moonSpecular.bmp", "Textures/moonAmbient.bmp");
    moon.addShader(shader_program);
    moon.updateProjectionMatrix(proj_mat);
    moon.updateViewMatrix((const float*)glm::value_ptr(view_mat));
    moon.updateModelMatrix((const float*)glm::value_ptr(moonMatrix));
    moon.updateCameraLocation((const float*)glm::value_ptr(camPos));
    	
	BumpMapGLRenderer test;
	loadNormalMesh(test, "meshes/monkey.dae", "Textures/brick.bmp", "Textures/brickNormal.bmp", "Textures/SpecularMap.bmp", "Textures/AmbientOcclusionMap.bmp");
	test.addShader(shader_program);
	test.updateProjectionMatrix(proj_mat);
	test.updateViewMatrix((const float*)glm::value_ptr(view_mat));
	test.updateModelMatrix((const float*)glm::value_ptr(modelMatrix));
	test.updateCameraLocation((const float*)glm::value_ptr(camPos));
		
	
	SkyBoxGLRenderer skyBox;
	loadSkyBoxMesh(skyBox, "meshes/box.dae", "Textures/SkyBox/skyBoxUp.bmp", "Textures/SkyBox/skyBoxDown.bmp", "Textures/SkyBox/skyBoxLeft.bmp", "Textures/SkyBox/skyBoxRight.bmp", "Textures/SkyBox/skyBoxFront.bmp", "Textures/SkyBox/skyBoxBack.bmp");
	skyBox.addShader(skyBoxShader);
	skyBox.updateProjectionMatrix(proj_mat);
	
	while (1)
    {
		
		T = glm::translate(ident,-camPos);
		R = glm::toMat4(camRot);
		view_mat = R * T;
		
		modelTranslate = glm::translate(ident,modelPos);
		modelRotation = glm::toMat4(modelRot);
		modelMatrix = modelTranslate * modelRotation;
		
		glm::quat newMoonRotationQuat = glm::angleAxis(0.001f, glm::vec3(0.0f, -1.0f, 0.0f));
		moonRotationQuat = moonRotationQuat * newMoonRotationQuat;
		moonTranslateMatrix = glm::translate(ident,moonPosition);
		moonRotationMatrix = glm::toMat4(moonRotationQuat);
		moonMatrix = moonTranslateMatrix * moonRotationMatrix;
		
		moon.updateViewMatrix((const float*)glm::value_ptr(view_mat));
		moon.updateModelMatrix((const float*)glm::value_ptr(moonMatrix));
		moon.updateCameraLocation((const float*)glm::value_ptr(camPos));
		
		test.updateViewMatrix((const float*)glm::value_ptr(view_mat));
		test.updateModelMatrix((const float*)glm::value_ptr(modelMatrix));
		test.updateCameraLocation((const float*)glm::value_ptr(camPos));
		
		skyBox.updateCameraRotation((const float*)glm::value_ptr(R));
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		skyBox.draw();
		test.draw();
		moon.draw();
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
                        glm::mat3 camRotMat = glm::toMat3(camRot);
                        glm::vec3 movement = glm::vec3(0.0f, 0.0f, -0.3f);
                        glm::vec3 finalMovement = movement * camRotMat;
                        camPos += finalMovement;
                    }
					if((XLookupString((XKeyEvent *)&event, buffer, 1, &keysym, NULL) == 1) && (keysym == (KeySym)XK_s))
                    {
                        glm::mat3 camRotMat = glm::toMat3(camRot);
                        glm::vec3 movement = glm::vec3(0.0f, 0.0f, 0.3f);
                        glm::vec3 finalMovement = movement * camRotMat;
                        camPos += finalMovement;
                    }
					if((XLookupString((XKeyEvent *)&event, buffer, 1, &keysym, NULL) == 1) && (keysym == (KeySym)XK_a))
                    {
                        glm::mat3 camRotMat = glm::toMat3(camRot);
                        glm::vec3 movement = glm::vec3(-0.3f, 0.0f, 0.0f);
                        glm::vec3 finalMovement = movement * camRotMat;
                        camPos += finalMovement;
                    }
					if((XLookupString((XKeyEvent *)&event, buffer, 1, &keysym, NULL) == 1) && (keysym == (KeySym)XK_d))
                    {
                        glm::mat3 camRotMat = glm::toMat3(camRot);
                        glm::vec3 movement = glm::vec3(0.3f, 0.0f, 0.0f);
                        glm::vec3 finalMovement = movement * camRotMat;
                        camPos += finalMovement;
                    }
					if((XLookupString((XKeyEvent *)&event, buffer, 1, &keysym, NULL) == 1) && (keysym == (KeySym)XK_q))
                    {
                        glm::quat rotateLeft = glm::angleAxis(glm::radians(-2.0f),glm::vec3(0.0f, 1.0f, 0.0f));
                        camRot *= rotateLeft;
                    }
					if((XLookupString((XKeyEvent *)&event, buffer, 1, &keysym, NULL) == 1) && (keysym == (KeySym)XK_e))
                    {
                        glm::quat rotateRight = glm::angleAxis(glm::radians(2.0f),glm::vec3(0.0f, 1.0f, 0.0f));
                        camRot *= rotateRight;
                    }
                    if((XLookupString((XKeyEvent *)&event, buffer, 1, &keysym, NULL) == 1) && (keysym == (KeySym)XK_j))
                    {
                        glm::quat rotateLeft = glm::angleAxis(glm::radians(2.0f),glm::vec3(0.0f, 1.0f, 0.0f));
                        modelRot *= rotateLeft;
                    }
					if((XLookupString((XKeyEvent *)&event, buffer, 1, &keysym, NULL) == 1) && (keysym == (KeySym)XK_l))
                    {
                        glm::quat rotateRight = glm::angleAxis(glm::radians(-2.0f),glm::vec3(0.0f, 1.0f, 0.0f));
                        modelRot *= rotateRight;
                    }
                    if((XLookupString((XKeyEvent *)&event, buffer, 1, &keysym, NULL) == 1) && (keysym == (KeySym)XK_i))
                    {
                        glm::quat rotateUp = glm::angleAxis(glm::radians(-2.0f),glm::vec3(1.0f, 0.0f, 0.0f));
                        modelRot *= rotateUp;
                        
                    }
                    if((XLookupString((XKeyEvent *)&event, buffer, 1, &keysym, NULL) == 1) && (keysym == (KeySym)XK_k))
                    {
                        glm::quat rotateUp = glm::angleAxis(glm::radians(2.0f),glm::vec3(1.0f, 0.0f, 0.0f));
                        modelRot *= rotateUp;
                    }
                    if((XLookupString((XKeyEvent *)&event, buffer, 1, &keysym, NULL) == 1) && (keysym == (KeySym)XK_u))
                    {
                        glm::quat rotateUp = glm::angleAxis(glm::radians(2.0f),glm::vec3(0.0f, 0.0f, 1.0f));
                        modelRot *= rotateUp;
                    }
                    if((XLookupString((XKeyEvent *)&event, buffer, 1, &keysym, NULL) == 1) && (keysym == (KeySym)XK_o))
                    {
                        glm::quat rotateUp = glm::angleAxis(glm::radians(-2.0f),glm::vec3(0.0f, 0.0f, 1.0f));
                        modelRot *= rotateUp;
                    }

                    break;
                }
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
