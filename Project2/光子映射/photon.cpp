#include "photon.h"
#include <iostream>
#include <cmath>
#include <algorithm>
#include <map>
#include <vector>

extern const double MAX;

Nearestphotons::Nearestphotons() {
	max_photons = found = 0;
	maxD2 = 0.0;
	full = false;
	D2 = NULL;
	photons = NULL;
}

Nearestphotons::~Nearestphotons() {
	delete[] D2;
	delete[] photons;
}

KDTree::KDTree(int size) {
	max_photons = size;
	stored_photons = 0;
	photons = new Photon[size + 1];
	box_min = Ray(MAX , MAX , MAX);
	box_max = -box_max;
}

KDTree::~KDTree() {
	delete[] photons;
}

void KDTree::insert(Photon photon) {
	if (stored_photons >= max_photons)
        return;
	photons[++stored_photons] = photon;
	box_min = Ray(std::min(box_min.x , photon.position.x) , std::min(box_min.y , photon.position.y) , std::min(box_min.z , photon.position.z));
	box_max = Ray(std::max(box_max.x , photon.position.x) , std::max(box_max.y , photon.position.y) , std::max(box_max.z , photon.position.z));
}

void KDTree::init() {
	Photon* temtree = new Photon[stored_photons + 1];
	for (int i = 0 ; i <= stored_photons ; i++)
		temtree[i] = photons[i];
	balance(temtree , 1 , 1 , stored_photons);
	delete[] temtree;
}

void KDTree::balance(Photon* temtree , int index , int start , int end) {
	if(start==end){
		photons[index]=temtree[start];
		return;
	}
	int med = (end + start) / 2;
	int axis = 2;
	if (box_max.x - box_min.x > box_max.y - box_min.y && box_max.x - box_min.x > box_max.z - box_min.z)
        axis = 0;
    else if (box_max.y - box_min.y > box_max.z - box_min.z)
        axis = 1;
	split(temtree , start , end , med , axis);
	photons[index] = temtree[med];
	photons[index].divide_axis = axis;
	if (med < end) {
		double tem = box_min.dimension(axis);
		box_min.dimension(axis) = photons[index].position.dimension(axis);
		balance(temtree, index * 2 + 1, med + 1, end);
		box_min.dimension(axis) = tem;
	}
	if (start < med) {
		double tem = box_max.dimension(axis);
		box_max.dimension(axis) = photons[index].position.dimension(axis);
		balance(temtree, index * 2, start, med - 1);
		box_max.dimension(axis) = tem;
	}
}

void KDTree::split(Photon* temtree , int start , int end , int med , int axis) {
	int l = start , r = end;
	while (l < r) {
		double key = temtree[r].position.dimension(axis);
		int i = l - 1 , j = r;
		while(true){
			while (temtree[++i].position.dimension(axis) < key){ }
			while(temtree[--j].position.dimension(axis) > key && j > l) { }
			if (i >= j)
                break;
			std::swap(temtree[i] , temtree[j]);
		}
		std::swap(temtree[i] , temtree[r]);
		if (i >= med)
            r = i - 1;
		if (i <= med)
            l = i + 1;
	}
}

Color KDTree::getirradiance(Point origin, Ray direction, double max_dist, int n) {
	Color result;

	Nearestphotons* np=new Nearestphotons;
	np->position = origin;
	np->max_photons = n;
	np->D2 = new double[n + 1];
	np->photons = new Photon*[n + 1];
	np->maxD2 = max_dist * max_dist;
	search(np, 1);
	if (np->found <= 8)//光子数太少，噪点
		return result;
	for (int i = 1 ; i <= np->found ; i++) {
		Ray dir = np->photons[i]->direction;
		if (direction.dot(dir) < 0)
			result += np->photons[i]->intensity;
	}
	result = result * (4 / (emit_photons * np->maxD2));
	return result;
}

void KDTree::search(Nearestphotons* np , int index) {
	if (index > stored_photons)
        return;
	Photon *photon = &photons[index];
	if (index * 2 <= stored_photons) {
		double dist = np->position.dimension(photon->divide_axis) - photon->position.dimension(photon->divide_axis);
		if (dist < 0) {
			search(np , index * 2);
			if (dist * dist < np->maxD2)
                search(np , index * 2 + 1);
		}
		else {
			search(np , index * 2 + 1);
			if (dist * dist < np->maxD2)
                search(np , index * 2);
		}
	}

	double dist2 = photon->position.D2(np->position);
	if (dist2 > np->maxD2)
        return;

	if (np->found < np->max_photons) {
		np->found++;
		np->D2[np->found] = dist2;
		np->photons[np->found] = photon;
	}
    else {
		if (np->full == false) {
			np->full = true;
			for (int i = np->found / 2 ; i >= 1 ; i--) {
				int _index = i;
				Photon* tmp_photon = np->photons[i];
				double tmp_dist2 = np->D2[i];
				while ((_index * 2) <= np->found) {
					int j = _index * 2;
					if((j + 1 <= np->found) && (np->D2[j] < np->D2[j + 1]))
                        j++;
					if (tmp_dist2 >= np->D2[j])
                        break;
					np->photons[_index] = np->photons[j];
					np->D2[_index] = np->D2[j];
					_index = j;
				}
				np->photons[_index] = tmp_photon;
				np->D2[_index] = tmp_dist2;
			}
		}
		int _index = 1;
		while ((_index * 2) <= np->found) {
			int j = _index * 2;
			if((j + 1 <= np->found) && (np->D2[j] < np->D2[j + 1]))
                j++;
			if (dist2 > np->D2[j])
                break;
			np->photons[_index] = np->photons[j];
			np->D2[_index] = np->D2[j];
			_index = j;
		}
		np->photons[_index] = photon;
		np->D2[_index] = dist2;
		np->maxD2 = np->D2[1];
	}
}
