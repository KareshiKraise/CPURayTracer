#ifndef _SPHERE_H_
#define _SPHERE_H_

#include "Object3D.h"
#include "Material.h"



class Sphere : public Object3D {
public:
	Sphere(){
		_center = glm::vec3(0);
		_radius = 1.0f;
		m_ptr = new  Lambertian(glm::vec3(1.0, 1.0, 0.0));
		_diffuse = glm::vec3(0.3f, 0.5f, 120.0f);
	};
	Sphere(const glm::vec3& c, const float r, Material* m, const glm::vec3& col = glm::vec3(0.3f, 0.5f, 120.0f)) {
		_diffuse = col;
		_center = c;
		_radius = r;
		m_ptr = m;
	};
	~Sphere() { 
		if (m_ptr)
		{
			delete m_ptr;
		}
	};

	//override
	virtual bool hit(const Ray& r, float t_min, float t_max, ray_hit& rec, const glm::vec3& lpos) const {
		glm::vec3 oc = r.origin() - _center;
		float a = glm::dot(r.direction(), r.direction());
		float b = glm::dot(oc, r.direction());
		float c = glm::dot(oc, oc) - _radius * _radius;
		float discriminant = b * b -  a*c;
		if (discriminant > 0) 
		{
			float tmp = (-b - sqrt(discriminant))/ a;
			
			if (tmp < t_max && tmp > constants::k_min) {
				rec.t = tmp;
				rec.p = r.march_t(rec.t);
				rec.n = (rec.p - _center) / _radius;
				rec.mat_ptr = m_ptr;
				rec.d = shade(r, lpos ,rec);
				return true;
			}
			 tmp = (-b + sqrt(discriminant)) / a;

			if (tmp < t_max && tmp > constants::k_min) {
				rec.t = tmp;
				rec.p = r.march_t(rec.t);
				rec.n = (rec.p - _center) / _radius;
				rec.mat_ptr = m_ptr;
				rec.d = shade(r, lpos , rec);
				return true;
			}
		}
		return false;
		
	};
	
	glm::vec3 shade(const Ray& r, const glm::vec3& lpos, const ray_hit& rec) const {

		glm::vec3 ambient = glm::vec3(0.1f, 0.1f, 0.1f);
		
		glm::vec3 to_light = glm::normalize(lpos - rec.p);

		glm::vec3 normal = glm::normalize(rec.p - _center);

		glm::vec3 incoming = glm::normalize(r.o - rec.p);
		
		glm::vec3 diffuse = _diffuse * max(glm::dot(to_light, normal), 0.0f);			

		glm::vec3 reflected = reflect(to_light, normal);
		reflected = glm::normalize(reflected);
		
		glm::vec3 sp = glm::vec3(0.4f, 0.4f, 0.4f);
		float df = 0.0f;

		if (glm::dot(normal, to_light) > 0.0f) {

			glm::vec3 H = glm::normalize(incoming + to_light);;
			float dot = glm::clamp(glm::dot(normal, H), 0.0f, 1.0f);
			df = pow(dot, 16.0f);

		}
		sp = df * sp;
					

		glm::vec3 sum = (ambient + diffuse + sp);
		sum = glm::clamp(sum, 0.0f, 1.0f);
		return sum;

	};

	glm::vec3 get_center() const { return _center; };
	float     get_radius() const { return _radius; };

private:
	glm::vec3 _center;
	float     _radius;
	glm::vec3 _diffuse;
	Material  *m_ptr;
};




#endif