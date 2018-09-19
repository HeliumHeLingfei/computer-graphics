#include"rt.h"

int main() {
	Raytrace* pm = new Raytrace;
	pm->run("scene.txt");
	system("pause");
	return 0;
}
