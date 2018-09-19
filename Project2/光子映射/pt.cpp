#include"pt.h"
#include"scene.h"
#include<cstdlib>
#include<iostream>
#define ran() (double(rand() % 32768) / 32768)

const int MAX_PHOTONTRACING_DEP = 8;

bool Photontrace::diffusion(Object* object , Photon photon , int dep , double& prob) {
	Color color = object->getcolor();
	double probability = (object->getmaterial()->diffuse_rate + object->getmaterial()->specular_rate) * color.intensity();
	if (probability <= prob) {//���̶�
		prob -= probability;
		return false;
	}
	photon.intensity *= color / color.intensity();//�ܲ�����ɫӰ�죬���ֹ�ǿ����
	photon.direction = object->crash_normal_vector.diffuse();//�����ɢ
	trace(photon , dep + 1);
	return true;
}

bool Photontrace::reflection(Object* object , Photon photon , int dep , double& prob) {
	Color color = object->getcolor();
	double probability = object->getmaterial()->reflect_rate * color.intensity();
	if(probability <= prob) {
		prob -= probability;
		return false;
	}
	photon.intensity *= color / color.intensity();
	photon.direction = photon.direction.reflect(object->crash_normal_vector);
	trace(photon , dep + 1);
	return true;
}

bool Photontrace::refraction(Object* object , Photon photon , int dep , double& prob) {
	double probability = object->getmaterial()->refract_rate;
	if (!object->front) {
		Color absor = -(object->getmaterial()->absor * object->crash_distance);//���������ڲ������չ�
		absor = absor.exp();
		probability *= absor.intensity();
		photon.intensity *= absor / absor.intensity();
	}
	if(probability <= prob) {
		prob -= probability;
		return false;
	}
	double n = object->getmaterial()->n;
	if (object->front)
        n = 1 / n;
	photon.direction = photon.direction.refract(object->crash_normal_vector , n);
	trace(photon , dep + 1);
	return true;
}

void Photontrace::trace(Photon photon , int dep) {
	//if(dep > MAX_PHOTONTRACING_DEP || photon.intensity.intensity() < MIN_PHOTONTPOWER)
		//return;
	if(dep > MAX_PHOTONTRACING_DEP)
		return;
	Object* nearest_object = scene->nearestobject(photon.position , photon.direction);
	if (nearest_object != NULL) {
		Object* object = nearest_object->copy();
		photon.position = object->crash_point;
		if (object->getmaterial()->diffuse_rate > ZERO && dep > 1)
			tree->insert(photon);//��ӹ���
		double prob = ran();//����Ҫ�������䣬Ҫ�����䣬Ҫ�����䣬Ҫ������
		if (!diffusion(object , photon , dep , prob))
		    if (!reflection(object , photon , dep , prob))
		        refraction(object , photon , dep , prob);
		delete object;
	}
}

void Photontrace::run() {
	int n = scene->camera->emit_photons;
	tree = new KDTree(scene->camera->max_photons);
	tree->emit_photons = n;
	double total_intensity = 0;

	Light* head = scene->light_head;
	while(head) {
		total_intensity += head->getcolor().intensity();
		head = head->getnext();
	}
	double photon_intensity = total_intensity / n;//������������
	head = scene->light_head;
	while(head) {
		double light_intensity = head->getcolor().intensity();
		while (light_intensity >= photon_intensity) {
			Photon photon = head->emit();
			photon.intensity *= total_intensity;
			trace(photon , 1);
			light_intensity -= photon_intensity;
		}
		head = head->getnext();
	}
	tree->init();
}
