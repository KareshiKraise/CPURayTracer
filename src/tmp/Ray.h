#ifndef _RAY_H_
#define _RAY_H_

#include <GLM/glm.hpp>


class Ray {

public:
	Ray() {};
	Ray(const glm::vec3& a, const glm::vec3& b) :o(a), d(b) {};
	glm::vec3 origin() const { return o; };
	glm::vec3 direction() const { return d; };
	glm::vec3 march_t(float t) const { return o + t*d; };
		

	glm::vec3 o;
	glm::vec3 d;
};


#endif