#ifndef _LIST_
#define _LIST_

#include "Object.h"
#include <vector>


class List : public Object {

public:
	
	List(std::vector<Object*> l, int siz) {
		m_list = l;
		m_size = siz;
	};

	~List() {
		for (int i = 0; i < m_size; i++) {
			if (m_list[i]) {
				//std::cout << "deleting from scene" << std::endl;
				delete m_list[i];
			}
		}
	}

	//override
	virtual bool hit(const Ray& r, float t_min, float t_max, hit_record& rec) const {

		hit_record tmp_rec;
		bool did_hit = false;
		float closest_t = t_max;

		for (int i = 0; i < m_size; i++) {
			if (m_list[i]->hit(r, t_min, closest_t, tmp_rec)) {

				did_hit = true;
				closest_t = tmp_rec.t;
				rec = tmp_rec;

			}
		}
		return did_hit;

	};
	
	void add_object(Object* obj) {
		m_list.emplace_back(obj);
	}
	
	int m_size;
	std::vector<Object*> m_list;

};


#endif