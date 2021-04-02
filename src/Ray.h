#ifndef _RAY_H_
#define _RAY_H_

#include "Utilities.h"


class Ray {

public:
	Ray() {};
	Ray(const glm::vec3& a, const glm::vec3& b) {
		m_o = a;
		m_d = b;
	};
	glm::vec3 origin() const { return m_o; };
	glm::vec3 direction() const { return m_d; };
	glm::vec3 march(float t) const { return m_o + t*m_d; };


	glm::vec3 m_o;
	glm::vec3 m_d;
};


#endif