#ifndef _OBJECT_3D_
#define _OBJECT_3D_

#include <GLM\glm.hpp>
#include "Ray.h"
#include "Utilities.h"

class Material;

struct ray_hit {
	float t;     //which parameter
	glm::vec3 p; //where did it hit
	glm::vec3 n; //normal
	glm::vec3 d; //diffuse
	Material *mat_ptr;
};

float smoothstep(float edge0, float edge1, float x) {
	float t = glm::clamp((x - edge0) / (edge1 - edge0), 0.0f, 1.0f);
	return t * t * (3.0f - 2.0f *t);
}

class Object3D {
public:
	Object3D() {};
	~Object3D() {};
	virtual bool hit(const Ray& r,float t_min ,float t_max ,ray_hit& rec, const glm::vec3& lpos) const = 0;
		
};



#endif