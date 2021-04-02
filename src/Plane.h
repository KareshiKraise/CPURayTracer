#ifndef _PLANE_
#define _PLANE_

#include "Object.h"
#include "Material.h"

class Plane : public Object {
public:
	Plane() {
		m_p0 = point3D(0.0f, 0.0f, 0.0f);
		m_n = vetor(0.0f, 1.0f, 0.0f);
		
		m_ptr = new Pattern_m(colorRGB(0.7f, 0.7f, 0.0f), colorRGB(0.7f, 0.0f, 0.0f));

	};
	Plane(const point3D& p, const normal3D& n, Material* m) {
		m_p0 = p;
		m_n = n;
		m_ptr = m;
	};
	~Plane() {
		if (m_ptr) {
			delete m_ptr;
		}
	};
	virtual bool hit(const Ray& r, const float t_min, const float t_max, hit_record& rec) const {
		float t = glm::dot((m_p0 - r.origin()), m_n) / glm::dot(r.direction(), m_n);
		
		if (t < t_max && t > t_min) {
			rec.mat_ptr = m_ptr;
			rec.t = t;
			rec.n = (glm::normalize(m_n));
			rec.p = r.march(t);
			//rec.d = shade(r, lpos, rec);
			return true;
		}
		else {
			return false;
		}

	};

	point3D m_p0;
	normal3D  m_n;
	Material* m_ptr;
};




#endif