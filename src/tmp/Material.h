#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include "Utilities.h"
#include "Ray.h"
#include "Object3D.h"


glm::vec3 rand_unit_range() {
	glm::vec3 p;
	do {
		p = 2.0f * glm::vec3(rand_float(), rand_float(), rand_float()) - glm::vec3(1.0f, 1.0f, 1.0f);
	} while (glm::dot(p, p) >= 1.0f);
	return p;
}

glm::vec3 shade_bg(const Ray& r) {

	glm::vec3 unit_dir = glm::normalize(r.direction());
	float t = 0.5f*(unit_dir.y + 1.0f);
	return (1.0f - t) * glm::vec3(1.0f, 1.0f, 1.0f) + t * glm::vec3(0.5f, 0.7f, 1.0f);
}

glm::vec3 reflect(const glm::vec3& a, const glm::vec3& b) {
	return a - 2 * glm::dot(a,b)*b;
}

bool refract(const glm::vec3& v, const glm::vec3& n, float ni_nt, glm::vec3& refracted) {
	glm::vec3 uv = glm::normalize(v);
	float dt = glm::dot(uv, n);
	float disc = 1.0f - ni_nt * ni_nt * (1.0f - dt*dt);
	if (disc > 0.0f) {
		refracted = ni_nt * (uv - n*dt) - n * sqrt(disc);
		return true;
	}
	else {
		return false;
	}
}

float schlick(float cosine, float ref_idx) {
	float r0 = (1 - ref_idx) / (1 + ref_idx);
	r0 = r0*r0;
	return r0 + (1 - r0) * pow((1 - cosine), 5);
}


class Material {
public:
	virtual bool scatter(const Ray& r, const ray_hit& rec, glm::vec3& attenuation, Ray& scatter) const = 0;
};


class Lambertian : public Material {
public:
	Lambertian(const glm::vec3& a){ albedo = a; }
	virtual bool scatter(const Ray& r, const ray_hit& rec, glm::vec3& attenuation, Ray& scatter) const {
		glm::vec3 target = rec.p + rec.n + rand_unit_range();
		scatter = Ray(rec.p, target - rec.p);
		attenuation = albedo;
		return true;
	}

	glm::vec3 albedo;
};

class Metal : public Material {
public:
	Metal(const glm::vec3& a){ albedo = a; }
	virtual bool scatter(const Ray& r, const ray_hit& rec, glm::vec3& attenuation, Ray& scatter) const {
		glm::vec3 reflected = reflect(glm::normalize(r.direction()), rec.n);
		scatter = Ray(rec.p, reflected);
		attenuation = albedo;
		return (dot(scatter.direction(), rec.n) > 0);
	}
	glm::vec3 albedo;

};

class Dielectric : public Material {
public:
	Dielectric(float index) {
		ref_index = index;
		
	};
	virtual bool scatter(const Ray& r, const ray_hit& rec, glm::vec3& attenuation, Ray& scatter) const {
		glm::vec3 out_n;
		glm::vec3 reflected = reflect(r.direction(), rec.n);
		float ni_nt;
		attenuation = glm::vec3(1.0f,1.0f,1.0f);
		glm::vec3 refracted(0.0f, 0.0f, 0.0f);
		float reflect_prob;
		float cosine;



		if (glm::dot(r.direction(), rec.n) > 0) {
			out_n = -rec.n;
			ni_nt = ref_index;

			cosine = glm::dot(r.direction(), rec.n) / r.direction().length();
			cosine = sqrt(1 - ref_index * ref_index*(1-cosine*cosine));
		}
		else {
			out_n = rec.n;
			ni_nt = 1.0f / ref_index;
			cosine = -glm::dot(r.direction(), rec.n) / r.direction().length();
		}
		

		
		if (refract(r.direction(), out_n, ni_nt, refracted)) {
			reflect_prob = schlick(cosine, ref_index);
		}
		else {
			
			reflect_prob = 1.0f;
			
		}

		if (rand_float() < reflect_prob) {
			scatter = Ray(rec.p, reflected);
		}
		else {
			scatter = Ray(rec.p, refracted);
		}
		
		return true;
	};



	float ref_index;
	
};


#endif