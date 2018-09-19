#ifndef SCENE_H
#define SCENE_H

#include"color.h"
#include"ray.h"
#include"object.h"
#include "bezier.h"
#include"light.h"
#include"camera.h"

class Scene {
public:
	Color background_color;
	Camera* camera;
	Object* object_head;
	Light* light_head;
	Scene() :background_color(Color()), camera(new Camera), object_head(NULL), light_head(NULL) {
		srand(2016011256);
	}
	~Scene();

	void init(std::string filename);
	Object* nearestobject(Point Origin, Ray direction);
	Light* nearestlight(Point Origin, Ray direction);
};

#endif // !SCENE_H
