#include "rt.h"
#define ran() (double(rand() % 32768) / 32768)

Color Raytrace::diffusion_direct(Object * object, int * hash) {
	//std::cout << "diff "<< *hash << std::endl;
	Color color = object->getcolor();
	Color result = color * scene->background_color * object->getmaterial()->diffuse_rate;

	result += color * tree->getirradiance(object->crash_point , object->crash_normal_vector , scene->camera->sample_dist , scene->camera->sample_photons) * object->getmaterial()->diffuse_rate;//间接光照

	Light* head = scene->light_head;
	while(head) {
		result += color * head->getirr(object , scene->object_head , scene->camera->shade_quality , hash);
		head = head->getnext();//
	}
	
	//std::cout << "diff " << result.r << " " << result.g << " " << result.b << std::endl;
	return result;
}

Color Raytrace::reflection(Object * object, Ray direction, int dep, int * hash) {
	//std::cout << "refl " << direction.x << " " << direction.y << " " << direction.z << " " << std::endl;
	direction = direction.reflect(object->crash_normal_vector);//改变方向继续追踪
	
	return raytracing(object->crash_point, direction, dep + 1, hash) * object->getcolor() * object->getmaterial()->reflect_rate;
}

Color Raytrace::refraction(Object * object, Ray direction, int dep, int * hash) {
	//std::cout << "refr " << direction.x << " " << direction.y << " " << direction.z << " " << std::endl;
	if(object->front)
		direction = direction.refract(object->crash_normal_vector, 1 / object->getmaterial()->n);
	else
		direction = direction.refract(object->crash_normal_vector, object->getmaterial()->n);
	Color color = raytracing(object->crash_point, direction, dep + 1, hash);
	if(object->front)
		return color * object->getmaterial()->refract_rate;
	Color absor = -(object->getmaterial()->absor * object->crash_distance);
	Color result = color * absor.exp() * object->getmaterial()->refract_rate;
	//std::cout << "refr " << result.r << " " << result.g << " " << result.b << std::endl;
	return result;
}

Color Raytrace::raytracing(Point origin, Ray direction, int dep, int * hash) {
	if(dep > MAX_RAYTRACING_DEP)
		return Color();

	Color result;
	Object* nearest_object = scene->nearestobject(origin, direction);
	Light* nearest_light = scene->nearestlight(origin, direction);

	if(nearest_light != NULL && (nearest_object == NULL || nearest_light->crash_distance < nearest_object->crash_distance)) {
		if(hash != NULL)
			*hash = (*hash + nearest_light->getcode()) % HASH_MOD;
		result += nearest_light->getcolor();
	}
	if(nearest_object != NULL) {
		if(hash != NULL)
			*hash = (*hash + nearest_object->getcode()) % HASH_MOD;
		Object* object = nearest_object->copy();
		if(object->getmaterial()->diffuse_rate > ZERO || object->getmaterial()->specular_rate > ZERO)
			result += diffusion_direct(object, hash);
		if(object->getmaterial()->reflect_rate > ZERO) 
			result += reflection(object, direction, dep, hash);
		if(object->getmaterial()->refract_rate > ZERO)
			result += refraction(object, direction, dep, hash);
		delete object;
	}
	if(hash != NULL)
		*hash = (*hash * HASH_FAC) % HASH_MOD;
	result.limit();
	//std::cout << "ratr " << result.r << " " << result.g << " " << result.b << std::endl;
	return result;
}

void Raytrace::run(std::string inputname) {
	Camera* camera = scene->camera;
	Photontrace* pt=new Photontrace;
	scene->init(inputname);
	pt->scene=scene;
	pt->run();
	tree=pt->tree;
	delete pt;

	Point ray_O = camera->center;
	int H = camera->h, W = camera->w;
	int** code = new int*[H];
	for(int i = 0; i < H; i++) {
		code[i] = new int[W];
		for(int j = 0; j < W; j++)
			code[i][j] = 0;
	}
	for(int i = 0; i < H; i++)
		for(int j = 0; j < W; j++) {
			Ray ray_V = camera->getview(i, j);
			Color color = raytracing(ray_O, ray_V, 1, &code[i][j]);
			camera->setcolor(i, j, color);
		}
	camera->output("_picture");
	for(int i = 0; i < H; i++)
		for(int j = 0; j < W; j++) {
			if(((i == 0 || code[i][j] == code[i - 1][j]) 
				&& (i == H - 1 || code[i][j] == code[i + 1][j])) 
				&& ((j == 0 || code[i][j] == code[i][j - 1]) 
				&& (j == W - 1 || code[i][j] == code[i][j + 1])))
				continue;
			Color color = camera->getcolor(i, j) / 9;
			for(int r = -1; r <= 1; r++){
				for(int c = -1; c <= 1; c++) {
					if(r == 0 && c == 0)
						continue;
					Ray ray_V = camera->getview(i + (double)r / 3, j + (double)c / 3);
					color += raytracing(ray_O, ray_V, 1, NULL) / 9;
				}
			}
			camera->setcolor(i, j, color);
		}
	for(int i = 0; i < H; i++)
		delete[] code[i];
	delete[] code;
	camera->output("picture");
}
