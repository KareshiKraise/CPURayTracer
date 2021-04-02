#ifndef _PLANE_H_
#define _PLANE_H_

#include "Object3D.h"
#include "Material.h"



class Plane : public Object3D {
public:
	Plane() { 
		_n = glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f));
		_p0 = glm::vec3(0.0f,-1.0f, 0.0f);
		_diffuse = glm::vec3(0.2f, 0.2f, 0.2f);
		
		
	};
	Plane(const glm::vec3& normal, const glm::vec3& point, Material* m) {
		_n = normal;
		_p0 = point;
		m_ptr = m;
	};
	~Plane() { 
		if (m_ptr) {
			delete m_ptr;
		}

	};

	virtual bool hit(const Ray& r, float t_min, float t_max, ray_hit& rec, const glm::vec3& lpos) const {

		float t = glm::dot((_p0 - r.origin()), _n) / glm::dot(r.direction(), _n);
		if (t < t_max && t > t_min) {
			rec.mat_ptr = m_ptr;
			rec.t = t;
			rec.n = _n;
			rec.p = r.march_t(t);
			rec.d = shade(r, lpos, rec);
			return true;
		}
		else {
			return false;
		}
	}

	glm::vec3 shade(const Ray& r, const glm::vec3& lpos, const ray_hit& rec) const {
		glm::vec3 ambient = glm::vec3(0.1f, 0.1f, 0.1f);

		glm::vec3 to_light = glm::normalize(lpos - rec.p);
				
		glm::vec3 incoming = glm::normalize(r.o - rec.p);
		
		glm::vec3 diffuse = _diffuse * max(glm::dot(_n, to_light), 0.0f);

		glm::vec3 reflected = reflect(to_light, _n);
		reflected = glm::normalize(reflected);
		
		glm::vec3 sp = glm::vec3(0.4f, 0.4f, 0.4f);
		float df = 0.0f;
		
		if (glm::dot(_n, to_light) > 0.0f) {
						
			glm::vec3 H = glm::normalize(incoming + to_light);
			float dot = glm::clamp(glm::dot(_n,H), 0.0f, 1.0f);
			df = pow(dot, 16.0f);

		}
		
		sp = df * sp;
				
		glm::vec3 sum = ambient + diffuse + sp;
		sum = glm::clamp(sum, 0.0f, 1.0f);
		return sum;
		
	};

	glm::vec3 _n;
	glm::vec3 _p0;
	glm::vec3 _diffuse;
	Material *m_ptr;
		


};




#endif