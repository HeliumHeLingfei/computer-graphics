#include "scene.h"
#define ran() (double(rand() % 32768) / 32768)

Scene::~Scene() {
	while(object_head) {
		Object* object = object_head->getnext();
		if(object_head->getmaterial()->texture)
			delete object_head->getmaterial()->texture;
		delete object_head;
		object_head = object;
	}
	while(light_head) {
		Light* light = light_head->getnext();
		delete light_head;
		light_head = light;
	}
	delete camera;
}

void Scene::init(std::string filename) {
	std::ifstream fin(filename.c_str());
	std::string info;
	while(fin >> info) {
		Object* object = NULL;
		Light* light = NULL;

		std::cout << "new " <<info<< std::endl;
		if(info == "object") {
			std::string type;
			fin >> type;
			std::cout << "otype "<<type << std::endl;
			if(type == "sphere")
				object = new Sphere;
			else if(type == "plain")
				object = new Plain;
			else if(type == "bezier")
				object = new Bezier;
			if(object) {
				object->setnext(object_head);
				object_head = object;
			}
		}
		else if(info == "light") {
			std::string type;
			fin >> type;
			std::cout << "ltype " << type << std::endl;
			if(type == "point")
				light = new Pointlight;
			else if(type == "plain")
				light = new Plainlight;
			if(light) {
				light->setnext(light_head);
				light_head = light;
			}
		}
		else if(info != "background" && info != "camera")
			continue;
		fin.ignore(1024, '\n');
		std::string property;
		while(getline(fin, property, '\n')) {
			std::stringstream fin2(property);
			std::string para; 
			fin2 >> para;
			std::cout <<"para "<< para << std::endl;
			if(para == "end")
				break;
			else if(info == "background" && para == "color=")
				fin2 >> background_color.r >> background_color.g >> background_color.b;
			else if(info == "object" && object != NULL)
				object->input(para, fin2);
			else if(info == "light" && light != NULL)
				light->input(para, fin2);
			else if(info == "camera")
				camera->input(para, fin2);
		}
	}
	camera->init();
}

Object * Scene::nearestobject(Point Origin, Ray direction) {
	Object* result = NULL;
	Object* object = object_head;
	while(object) {
		if(object->collide(Origin, direction))
			if(result == NULL || object->crash_distance < result->crash_distance)
				result = object;
		object = object->getnext();
	}
	return result;
}

Light * Scene::nearestlight(Point Origin, Ray direction) {
	Light* result = NULL;
	Light* light = light_head;
	while(light) {
		if(light->collide(Origin, direction))
			if(result == NULL || light->crash_distance < result->crash_distance)
				result = light;
		light = light->getnext();
	}
	return result;
}
