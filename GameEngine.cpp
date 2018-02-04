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
    win = XCreateWindow(dpy, RootWindow(dpy, vi->screen), 0, 0, 1024, 768, 0, vi->depth, InputOutput, vi->visual, CWBorderPixel | CWColormap | CWEventMask, &swa);
    XSetStandardProperties(dpy, win, "OpenGl Test", "OpenGl Test", None, argv, argc, NULL);
    glXMakeCurrent(dpy, win, cx);
    XMapWindow(dpy, win);
    
    glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	
	
	GLfloat* vertPoints = NULL;
	GLfloat* normalPoints = NULL;
	GLfloat* texturePoints = NULL;
	GLfloat* tangentPoints = NULL;
	unsigned int pointCount = 0;
	{
	
		Assimp::Importer importer;


		const aiScene* scene = importer.ReadFile("meshes/monkey.dae",aiProcess_CalcTangentSpace | aiProcess_Triangulate);// | aiProcess_JoinIdenticalVertices | aiProcess_SortByPType);
		if(!scene)
		{
			std::cout << "BAD!";
		}

		const aiMesh* mesh = scene->mMeshes[0];
		pointCount = mesh->mNumVertices;
		if(mesh->HasPositions())
		{
			std::cout << "Has positions" << std::endl;
			vertPoints = new GLfloat[pointCount * 3];
			for(unsigned int i = 0;i < pointCount;i++)
			{
				const aiVector3D* vp = &(mesh->mVertices[i]);
				vertPoints[i * 3 + 0] = (GLfloat)vp->x;
				vertPoints[i * 3 + 1] = (GLfloat)vp->y;
				vertPoints[i * 3 + 2] = (GLfloat)vp->z;
			}
		}
		if(mesh->HasNormals())
		{
			std::cout << "Has normals" << std::endl;
			normalPoints = new GLfloat[pointCount * 3];
			for(unsigned int i = 0; i < pointCount; i++)
			{
				const aiVector3D* vn = &(mesh->mNormals[i]);
				normalPoints[i * 3 + 0] = (GLfloat)vn->x;
				normalPoints[i * 3 + 1] = (GLfloat)vn->y;
				normalPoints[i * 3 + 2] = (GLfloat)vn->z;
			}
		}

		if(mesh->HasTextureCoords(0))
		{
			std::cout << "Has texture coordinates" << std::endl;
			texturePoints = new GLfloat[pointCount * 2];
			for(unsigned int i = 0; i < pointCount; i++)
			{
				const aiVector3D* vt = &(mesh->mTextureCoords[0][i]);
				texturePoints[i * 2 + 0] = (GLfloat)vt->x;
				texturePoints[i * 2 + 1] = (GLfloat)vt->y;
			}
		}

		if(mesh->HasTangentsAndBitangents())
		{
			std::cout << "Has tangents" << std::endl;
			tangentPoints = new GLfloat[pointCount * 4];
			for(unsigned int i = 0; i < pointCount; i++)
			{
				const aiVector3D* vt = &(mesh->mTangents[i]);
				const aiVector3D* vb = &(mesh->mBitangents[i]);
				const aiVector3D* vn = &(mesh->mNormals[i]);
				
				glm::vec3 tangent = glm::vec3(vt->x,vt->y,vt->z);
				glm::vec3 biTangent = glm::vec3(vb->x,vb->y,vb->z);
				glm::vec3 normalVec = glm::vec3(vn->x,vn->y,vn->z);
				
				glm::vec3 tempTangent = glm::normalize(tangent - normalVec * glm::dot(normalVec,tangent));
				float det = (glm::dot(glm::cross(normalVec,tangent),biTangent));
				if(det < 0.0f)
				{
					det = -1.0f;
				}
				else
				{
					det = 1.0f;
				}
				tangentPoints[i * 4 + 0] = tempTangent.x;
				tangentPoints[i * 4 + 1] = tempTangent.y;
				tangentPoints[i * 4 + 2] = tempTangent.z;
				tangentPoints[i * 4 + 3] = det;
			}
		}
	}
	
	GLfloat* planetVertPoints = NULL;
	GLfloat* planetNormalPoints = NULL;
	GLfloat* planetTexturePoints = NULL;
	GLfloat* planetTangentPoints = NULL;
	unsigned int planetPointCount = 0;
	{
	
		Assimp::Importer importer;


		const aiScene* scene = importer.ReadFile("meshes/planet.dae",aiProcess_CalcTangentSpace | aiProcess_Triangulate);
		if(!scene)
		{
			std::cout << "BAD!";
		}

		const aiMesh* mesh = scene->mMeshes[0];
		planetPointCount = mesh->mNumVertices;
		if(mesh->HasPositions())
		{
			std::cout << "Has positions" << std::endl;
			planetVertPoints = new GLfloat[planetPointCount * 3];
			for(unsigned int i = 0;i < planetPointCount;i++)
			{
				const aiVector3D* vp = &(mesh->mVertices[i]);
				planetVertPoints[i * 3 + 0] = (GLfloat)vp->x;
				planetVertPoints[i * 3 + 1] = (GLfloat)vp->y;
				planetVertPoints[i * 3 + 2] = (GLfloat)vp->z;
			}
		}
		if(mesh->HasNormals())
		{
			std::cout << "Has normals" << std::endl;
			planetNormalPoints = new GLfloat[planetPointCount * 3];
			for(unsigned int i = 0; i < planetPointCount; i++)
			{
				const aiVector3D* vn = &(mesh->mNormals[i]);
				planetNormalPoints[i * 3 + 0] = (GLfloat)vn->x;
				planetNormalPoints[i * 3 + 1] = (GLfloat)vn->y;
				planetNormalPoints[i * 3 + 2] = (GLfloat)vn->z;
			}
		}

		if(mesh->HasTextureCoords(0))
		{
			std::cout << "Has texture coordinates" << std::endl;
			planetTexturePoints = new GLfloat[planetPointCount * 2];
			for(unsigned int i = 0; i < planetPointCount; i++)
			{
				const aiVector3D* vt = &(mesh->mTextureCoords[0][i]);
				planetTexturePoints[i * 2 + 0] = (GLfloat)vt->x;
				planetTexturePoints[i * 2 + 1] = (GLfloat)vt->y;
			}
		}

		if(mesh->HasTangentsAndBitangents())
		{
			std::cout << "Has tangents" << std::endl;
			planetTangentPoints = new GLfloat[planetPointCount * 4];
			for(unsigned int i = 0; i < planetPointCount; i++)
			{
				const aiVector3D* vt = &(mesh->mTangents[i]);
				const aiVector3D* vb = &(mesh->mBitangents[i]);
				const aiVector3D* vn = &(mesh->mNormals[i]);
				
				glm::vec3 tangent = glm::vec3(vt->x,vt->y,vt->z);
				glm::vec3 biTangent = glm::vec3(vb->x,vb->y,vb->z);
				glm::vec3 normalVec = glm::vec3(vn->x,vn->y,vn->z);
				
				glm::vec3 tempTangent = glm::normalize(tangent - normalVec * glm::dot(normalVec,tangent));
				float det = (glm::dot(glm::cross(normalVec,tangent),biTangent));
				if(det < 0.0f)
				{
					det = -1.0f;
				}
				else
				{
					det = 1.0f;
				}
				planetTangentPoints[i * 4 + 0] = tempTangent.x;
				planetTangentPoints[i * 4 + 1] = tempTangent.y;
				planetTangentPoints[i * 4 + 2] = tempTangent.z;
				planetTangentPoints[i * 4 + 3] = det;
			}
		}
	}


	Assimp::Importer skyBoxMeshImporter;
	
	const aiScene* boxScene = skyBoxMeshImporter.ReadFile("meshes/box.dae",aiProcess_Triangulate);// | aiProcess_JoinIdenticalVertices | aiProcess_SortByPType);
	if(!boxScene)
	{
		std::cout << "BAD!";
	}
	GLfloat* boxVertPoints = NULL;
	const aiMesh* boxMesh = boxScene->mMeshes[0];
	unsigned int totalPoints = boxMesh->mNumVertices;
	if(boxMesh->HasPositions())
	{
		std::cout << "Has positions" << std::endl;
		boxVertPoints = new GLfloat[totalPoints * 3];
		for(unsigned int i = 0;i < totalPoints;i++)
		{
			const aiVector3D* vp = &(boxMesh->mVertices[i]);
			boxVertPoints[i * 3 + 0] = (GLfloat)vp->x;
			boxVertPoints[i * 3 + 1] = (GLfloat)vp->y;
			boxVertPoints[i * 3 + 2] = (GLfloat)vp->z;
		}
	}

	
	bmpLoader brickDefuseFile;
	bmpLoader brickNormalFile;
	bmpLoader brickSpecularFile;
	bmpLoader brickOcclusionFile;
    brickDefuseFile.loadFile("Textures/brick.bmp");
    brickNormalFile.loadFile("Textures/brickNormal.bmp");
    brickSpecularFile.loadFile("Textures/SpecularMap.bmp");
    brickOcclusionFile.loadFile("Textures/AmbientOcclusionMap.bmp");
    
	bmpLoader upSkyBox;
	bmpLoader downSkyBox;
	bmpLoader leftSkyBox;
	bmpLoader rightSkyBox;
	bmpLoader frontSkyBox;
	bmpLoader backSkyBox;
	upSkyBox.loadFile("Textures/upSkyBox.bmp");
	downSkyBox.loadFile("Textures/downSkyBox.bmp");
	leftSkyBox.loadFile("Textures/leftSkyBox.bmp");
	rightSkyBox.loadFile("Textures/rightSkyBox.bmp");
	frontSkyBox.loadFile("Textures/frontSkyBox.bmp");
	backSkyBox.loadFile("Textures/backSkyBox.bmp");
	
	bmpLoader moonDefuseFile;
	bmpLoader moonNormalFile;
	bmpLoader moonSpecularFile;
	bmpLoader moonOcclusionFile;
	moonDefuseFile.loadFile("Textures/moonDefuse.bmp");
	moonNormalFile.loadFile("Textures/moonNormal.bmp");
	moonSpecularFile.loadFile("Textures/moonSpecular.bmp");
	moonOcclusionFile.loadFile("Textures/moonOcclusion.bmp");


    
    const char* vertex_shader =
    "#version 410\n"
    "layout(location = 0) in vec3 vertex_position;"
    "layout(location = 1) in vec3 vertex_normal;"
    "layout(location = 2) in vec2 vertex_text;"
    "layout(location = 3) in vec4 vtangent;"
    "out vec3 view_dir_tan;"
    "out vec3 light_dir_tan;"
    "out vec2 texture_coordinates;"
    "uniform mat4 view, proj, model;"
    "uniform vec3 cameraPosition_world;"
    "void main()"
    "{"
   "	gl_Position = proj * view * model * vec4(vertex_position,1.0);"
    "	texture_coordinates = vertex_text;"
    
    "	vec3 lightDirection_world = normalize(vec3(0.0, 0.0, -20.0) - cameraPosition_world);"
    "	vec3 bitangent = cross(vertex_normal, vtangent.xyz) * vtangent.w;"
    
    "	vec3 cameraPosition_local = vec3(inverse(model) * vec4(cameraPosition_world,1.0));"
    "	vec3 lightDirection_local = vec3(inverse(model) * vec4(lightDirection_world,0.0));"
    "	vec3 viewDirection_local = normalize(cameraPosition_local - vertex_position);"
    
    "	view_dir_tan = vec3(dot(vtangent.xyz, viewDirection_local),dot(bitangent, viewDirection_local), dot(vertex_normal, viewDirection_local));"
    "	light_dir_tan = vec3(dot(vtangent.xyz, lightDirection_local),dot(bitangent,lightDirection_local), dot(vertex_normal, lightDirection_local));"
    "}";
    
    const char* fragment_shader = 
    "#version 410\n"
    "uniform sampler2D basic_texture;"
    "uniform sampler2D normal_map;"
    "uniform sampler2D specular_map;"
    "uniform sampler2D occlusion_map;"
    "in vec3 view_dir_tan;"
    "in vec3 light_dir_tan;"
    "in vec2 texture_coordinates;"
    "vec3 light_position_world = vec3(0.0, 0.0, 20.0);"
    "vec3 Ls = vec3(1.0, 1.0, 1.0);"
    "vec3 Ld = vec3(1.0, 1.0, 1.0);"
    "vec3 La = vec3(0.4, 0.4, 0.4);"
    
    "vec4 texel = texture(basic_texture, texture_coordinates);"
    "vec4 specularTexel = texture(specular_map, texture_coordinates);"
    "vec4 occlusionTexel = texture(occlusion_map, texture_coordinates);"
    "vec3 Ks = vec3(specularTexel.rgb);"
    "vec3 Kd = vec3(texel.rgb);"
    "vec3 Ka = vec3(occlusionTexel.rgb);"
    "float specular_exponent = 100.0;"
        
    "out vec4 frag_color;"
    
    "void main()"
    "{"
    " 	vec3 Ia = La * Ka * Kd;"
    "	vec3 normal_tan = texture(normal_map, texture_coordinates).rgb;"
    "	normal_tan = normalize(normal_tan * 2.0 - 1.0);"
    "	vec3 direction_to_light_tan = normalize(-light_dir_tan);"
    "	float dot_prod = dot(direction_to_light_tan, normal_tan);"
    "	dot_prod = max(dot_prod, 0.0);"
    "	vec3 Id = Kd * Ld * dot_prod;"
	
	"	vec3 reflection_tan = reflect(normalize(light_dir_tan),normal_tan);"
	"	float dot_prod_specular = dot(reflection_tan, normalize(view_dir_tan));"
	"	dot_prod_specular = max(dot_prod_specular, 0.0);"
	"	float specular_factor = pow(dot_prod_specular, 100.0);"
	"	vec3 Is = Ls * Ks * specular_factor;"
    "	frag_color = vec4(Is + Id + Ia, 1.0);"
    "}";
    
    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vertex_shader, NULL);
    glCompileShader(vs);
    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fragment_shader, NULL);
    glCompileShader(fs);
    GLuint shader_program = glCreateProgram();
    glAttachShader(shader_program,fs);
    glAttachShader(shader_program,vs);
    glLinkProgram(shader_program);
    
    const char* skyBoxVerticesShader = 
    "#version 410\n"
    "in vec3 vp;"
    "uniform mat4 proj_mat, camera_rotation;"
    "out vec3 texCoords;"
    "void main()"
    "{"
    "	texCoords = vp;"
    "	gl_Position = proj_mat * camera_rotation * vec4(vp, 1.0);"
    "}";
    
    const char* skyBoxFragmentShader =
    "#version 410\n"
    "in vec3 texCoords;"
    "uniform samplerCube cube_texture;"
    "out vec4 frag_color;"
    "void main()"
    "{"
    "	frag_color = texture(cube_texture, texCoords);"
    "}";
    
    GLuint SB_v_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(SB_v_shader, 1, &skyBoxVerticesShader, NULL);
    glCompileShader(SB_v_shader);
    GLuint SB_f_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(SB_f_shader, 1, &skyBoxFragmentShader, NULL);
    glCompileShader(SB_f_shader);
    GLuint skyBoxShader = glCreateProgram();
    glAttachShader(skyBoxShader, SB_v_shader);
    glAttachShader(skyBoxShader, SB_f_shader);
    glLinkProgram(skyBoxShader);
    
    
	glm::vec3 camPos = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::quat camRot = glm::angleAxis(0.0f,glm::vec3(0.0f, 0.0f, -1.0f));
    
    glm::vec3 modelPos = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::quat modelRot = glm::angleAxis(0.0f,glm::vec3(0.0f, 0.0f, -1.0f));
    

    glm::mat4 ident = glm::mat4(1.0f);
	glm::mat4 T = glm::translate(ident,-camPos);
	glm::mat4 R = glm::toMat4(camRot);
	glm::mat4 view_mat = R * T;
	
	glm::mat4 modelTranslate = glm::translate(ident,modelPos);
	glm::mat4 modelRotation = glm::toMat4(modelRot);
	glm::mat4 modelMatrix = modelTranslate * modelRotation;
	
	
	float near = 0.1f;
	float far = 100.0f;
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

    
    	
	BumpMapGLRenderer test;

	test.addShader(shader_program);
	test.setVertices(vertPoints,pointCount);
	test.setNormals(normalPoints,pointCount);
	test.setTextureCoordinates(texturePoints,pointCount);
	test.setTangents(tangentPoints,pointCount);
	test.setDiffuseTexture(brickDefuseFile.getWidth(),brickDefuseFile.getHeigth(),brickDefuseFile.getData());
	test.setNormalTexture(brickNormalFile.getWidth(),brickNormalFile.getHeigth(),brickNormalFile.getData());
	test.setSpecularTexture(brickSpecularFile.getWidth(),brickSpecularFile.getHeigth(),brickSpecularFile.getData());
	test.setOcclusionTexture(brickOcclusionFile.getWidth(),brickOcclusionFile.getHeigth(),brickOcclusionFile.getData());
	test.updateProjectionMatrix(proj_mat);
	test.updateViewMatrix((const float*)glm::value_ptr(view_mat));
	test.updateModelMatrix((const float*)glm::value_ptr(modelMatrix));
	test.updateCameraLocation((const float*)glm::value_ptr(camPos));
		

	
	SkyBoxGLRenderer skyBox;
	skyBox.addShader(skyBoxShader);
	skyBox.setVertices(boxVertPoints,totalPoints);
	skyBox.setBoxTextureUp(upSkyBox.getWidth(),upSkyBox.getHeigth(),upSkyBox.getData());
	skyBox.setBoxTextureDown(downSkyBox.getWidth(),downSkyBox.getHeigth(),downSkyBox.getData());
	skyBox.setBoxTextureLeft(leftSkyBox.getWidth(),leftSkyBox.getHeigth(),leftSkyBox.getData());
	skyBox.setBoxTextureRight(rightSkyBox.getWidth(),rightSkyBox.getHeigth(),rightSkyBox.getData());
	skyBox.setBoxTextureFront(frontSkyBox.getWidth(),frontSkyBox.getHeigth(),frontSkyBox.getData());
	skyBox.setBoxTextureBack(backSkyBox.getWidth(),backSkyBox.getHeigth(),backSkyBox.getData());
	skyBox.updateProjectionMatrix(proj_mat);
	
	while (1)
    {
		
		T = glm::translate(ident,-camPos);
		R = glm::toMat4(camRot);
		view_mat = R * T;
		
		modelTranslate = glm::translate(ident,modelPos);
		modelRotation = glm::toMat4(modelRot);
		modelMatrix = modelTranslate * modelRotation;
		
		test.updateViewMatrix((const float*)glm::value_ptr(view_mat));
		test.updateModelMatrix((const float*)glm::value_ptr(modelMatrix));
		test.updateCameraLocation((const float*)glm::value_ptr(camPos));

		skyBox.updateCameraRotation((const float*)glm::value_ptr(R));
		
		
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		skyBox.draw();
		test.draw();
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
