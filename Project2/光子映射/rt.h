#ifndef RT_H
#define RT_H

#include"scene.h"
#include "pt.h"

const int MAX_RAYTRACING_DEP = 8;
const int HASH_FAC = 3;
const int HASH_MOD = 10006481;

class Raytrace {
	Scene* scene;
	KDTree* tree;
	Color diffusion_direct(Object* object, int* hash);
	Color reflection(Object* object, Ray direction, int dep, int* hash);
	Color refraction(Object* object, Ray direction, int dep, int* hash);
	Color raytracing(Point origin, Ray direction, int dep, int* hash);

public:
	Raytrace() {
		scene=new Scene();
		tree=NULL;
	}
	~Raytrace() {
		if(scene)
			delete scene;
		if(tree)
			delete tree;
	}
	void run(std::string);
};
#endif // !RT_H

