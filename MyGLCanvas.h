#pragma once

#ifndef MYGLCANVAS_H
#define MYGLCANVAS_H

#include <FL/gl.h>
#include <FL/glut.h>
#include <FL/glu.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <time.h>
#include <iostream>

#include "Shape.h"
#include "Cube.h"
#include "Cylinder.h"
#include "Cone.h"
#include "Sphere.h"
#include "Special_Shape.h"
#include <stack>

#include "Camera.h"
#include "scene/SceneParser.h"

//! Structure for non-primitive scene objects
class FlatSceneNode
{
public:
   /*! Transformation at this node */
   glm::mat4 transformationMat;

   /*! Primitive at this node */
   ScenePrimitive* primitive;
};

class MyGLCanvas : public Fl_Gl_Window {
public:
	glm::vec3 rotVec;
	glm::vec3 eyePosition;

	int wireframe;
	int smooth;
	int fill;
	int normal;
	int segmentsX, segmentsY;

	OBJ_TYPE objType;
	Cube* cube;
	Cylinder* cylinder;
	Cone* cone;
	Sphere* sphere;
	Sword* sword;
	Shape* shape;

	Camera* camera;
	SceneParser* parser;


	MyGLCanvas(int x, int y, int w, int h, const char *l = 0);
	~MyGLCanvas();
	void renderShape(OBJ_TYPE type);
	void setSegments();
	void loadSceneFile(const char* filenamePath);

private:
	vector<FlatSceneNode *> flat_tree;
	
	void draw();
	void drawScene();
	void drawAxis();

	int handle(int);
	void resize(int x, int y, int w, int h);
	void updateCamera(int width, int height);
	void setLight(const SceneLightData &light);
	void applyMaterial(const SceneMaterial &material);
	void generate_flat_tree(SceneNode* node, glm::mat4 transformationMat, vector<FlatSceneNode *> &flat_tree);
};

#endif // !MYGLCANVAS_H