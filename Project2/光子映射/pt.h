#ifndef PT_H
#define PT_H

#include"scene.h"
#include"photon.h"

class Photontrace {

	void trace(Photon , int dep);
	bool diffusion(Object* , Photon , int dep , double& prob);
	bool reflection(Object* , Photon , int dep , double& prob);
	bool refraction(Object* , Photon , int dep , double& prob);

public:
	Scene* scene;
	KDTree* tree;

	void run();
};

#endif// !PT_H
