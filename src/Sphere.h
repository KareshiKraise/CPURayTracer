#ifndef _SPHERE_
#define _SPHERE_

#include "Object.h"
#include "Material.h"


class Sphere : public Object {
public:
	Sphere() {
		m_radius = 0.8f;
		m_center = point3D(0.0f, 2.0f, 0.0f);
		m_ptr = new Phong_m(colorRGB(0.8f, 0.3f, 0.7f));
	};
	Sphere(const float radius, const point3D& center, Material* m) {
		m_radius = radius;
		m_center = center;
		m_ptr = m;
	};
	~Sphere() {
		if (m_ptr) {
			delete m_ptr;
		}
	};
	virtual bool hit(const Ray& r, const float t_min, const float t_max, hit_record& rec) const {
		glm::vec3 oc = r.origin() - m_center;
		float a = glm::dot(r.direction(), r.direction());
		float b = glm::dot(oc, r.direction());
		float c = glm::dot(oc, oc) - m_radius * m_radius;
		float discriminant = b * b - a*c;
		
		if (discriminant > 0)
		{
			float tmp = (-b - sqrt(discriminant)) / a;

			if (tmp < t_max && tmp > t_min) {
				rec.t = tmp;
				rec.p = r.march(rec.t);
				rec.n = glm::normalize((rec.p - m_center));
				rec.mat_ptr = m_ptr;
				//rec.d = shade(r, lpos, rec);
				return true;
			}
			tmp = (-b + sqrt(discriminant)) / a;

			if (tmp < t_max && tmp > t_min) {
				rec.t = tmp;
				rec.p = r.march(rec.t);
				rec.n = glm::normalize((rec.p - m_center));
				rec.mat_ptr = m_ptr;
				//rec.d = shade(r, lpos, rec);
				return true;
			}
		}
		return false;

	};

	float m_radius;
	point3D m_center;
	Material* m_ptr;


};



#endif