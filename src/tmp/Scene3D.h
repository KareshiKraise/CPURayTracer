#ifndef _SCENE_3D_
#define _SCENE_3D_

#include "Object3D.h"
#include <vector>
#include "Utilities.h"

class Object3D;

class Scene3D : public Object3D{
public:
	Scene3D() {};
	Scene3D(std::vector<Object3D*> l, int siz ) {
		_list = l;
		_size = siz;
	};
	~Scene3D() {
		for (int i = 0; i < _size; i++) {
			if (_list[0]) {
				std::cout << "deleting from scene" << std::endl;
				delete _list[i];
			}
		}
	}
	//override
	virtual bool hit(const Ray& r, float t_min, float t_max, ray_hit& rec, const glm::vec3& lpos) const {

		ray_hit tmp_rec;
		bool did_hit = false;
		double closest_t = t_max;

		for (int i = 0; i < _size; i++) {
			if (_list[i]->hit(r, t_min, closest_t ,tmp_rec, lpos)) {

				did_hit = true;
				closest_t = tmp_rec.t;
				rec = tmp_rec;

			}
		}
		return did_hit;

	};
		

	void add_object(Object3D* obj) {
		_list.push_back(obj);
	}

	int _size;
	std::vector<Object3D*> _list;

	

};


#endif