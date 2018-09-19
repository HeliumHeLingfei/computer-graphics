#ifndef PHOTON_H
#define PHOTON_H

#include "ray.h"
#include "color.h"
#include "object.h"


extern const double PI;
extern const double ZERO;
const double MAX = 1e8;

struct Photon {
public:
	Point position;
    Ray direction;
	Color intensity;
	int divide_axis;//��kd���еķָ����
    Photon(){}
};

class Nearestphotons {
public:
	Point position;
	int max_photons , found;//��Χ������������õ��Ĺ�����
	bool full;
	double* D2;//���ӵľ���
	double maxD2;
	Photon** photons;

	Nearestphotons();
	~Nearestphotons();
};

class KDTree {
	int max_photons;
	Photon* photons;
	Ray box_min , box_max;//kd���ķ�֧�ı߽�

	void balance(Photon* temtree , int index , int start , int end);//����ƽ����
	void split(Photon* temtree , int start , int end , int med , int axis);
	void search(Nearestphotons* np , int index); //��ѯ�������
public:
    int stored_photons , emit_photons;
	KDTree(int size = 0);
	~KDTree();

	void insert(Photon);
	void init();
	Color getirradiance(Point origin, Ray direction, double max_dist, int n);
};
#endif