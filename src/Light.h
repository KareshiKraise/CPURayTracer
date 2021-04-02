#ifndef _LIGHT_
#define _LIGHT_

#include "Utilities.h"

class Light {
public:
	Light(const point3D& pos1 = glm::vec3(2.0f, 10.0f, -1.0f)) {
		m_pos = pos1;
		
	}
		point3D m_pos;
};


#endif