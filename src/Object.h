#ifndef _OBJECT_
#define _OBJECT_

#include "Utilities.h"
#include "Ray.h"

class Material;




struct hit_record 
{
	float t;     //which parameter
	glm::vec3 p; //where did it hit
	glm::vec3 n; //normal in point
	glm::vec3 d; //diffuse
	Material *mat_ptr;
};


class Object {
public:
	Object() {};
	~Object() {};
	virtual bool hit(const Ray& r, const float t_min, const float t_max, hit_record& rec ) const = 0;


};



#endif