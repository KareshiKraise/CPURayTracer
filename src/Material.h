#ifndef _MATERIAL_
#define _MATERIAL_

#include "Utilities.h"
#include "Light.h"
#include "Ray.h"
#include "Object.h"

struct hit_record;
class Ray;
class Light;

enum class mat_t {
	Pattern_t = 0x01,
	Phong_t,
	Dielectric_t,
	Metal_t,
	max_num
};

class Material {
public:

	virtual bool scatter(const Ray& r, hit_record& rec, glm::vec3& attenuation, Ray& scatter, const Light* light, const int num_light) const = 0;
	virtual mat_t get_type() const =0;
	mat_t mat_type;
	
};

#define offset (int)3893343

class Pattern_m : public Material {
public:
	Pattern_m(const colorRGB& a, const colorRGB& b) {
		m_albedo1 = a;
		m_albedo2 = b;
	}

	virtual bool scatter(const Ray& r, hit_record& rec, glm::vec3& attenuation, Ray& scatter, const Light* light, const int num_light) const {
		
		
		if ( (int(abs(rec.p.x)) + int(abs(rec.p.z))) % 2 )
		{
			attenuation = m_albedo1;
		}
		else
		{
			attenuation = m_albedo2;
		}
		return true;
	}
	
	virtual mat_t get_type() const{ return mat_type; }

	const mat_t mat_type = mat_t::Pattern_t;
	colorRGB m_albedo1;
	colorRGB m_albedo2;
};

class Phong_m : public Material {

public:
	Phong_m(const colorRGB& a) {
		m_albedo = a;
		
	};

	virtual bool scatter(const Ray& r, hit_record& rec, glm::vec3& attenuation, Ray& scatter, const Light* light, const int num_light) const {

		colorRGB ambient = colorRGB(0.25f, 0.25f, 0.25f)*m_albedo;
		colorRGB diffuse;
		colorRGB sp = colorRGB(0.4f, 0.4f, 0.4f);;
		float df = 0.0f;

		if (num_light == 1)
		{/*-------------------------------- 1 LIGHT -----------------------------*/

			vetor to_light = glm::normalize(light[0].m_pos - rec.p);
			vetor incoming = glm::normalize(r.m_o - rec.p);

			float check_dot = glm::dot(to_light, rec.n);
			diffuse = m_albedo * max(check_dot, 0.0f);

			//float df = 0.0f;
			//sp = colorRGB(0.4f, 0.4f, 0.4f);

			if (check_dot > 0.0f) {
				vetor H = glm::normalize(incoming + to_light);
				float dot = glm::clamp(glm::dot(rec.n, H), 0.0f, 1.0f);
				df = pow(dot, 16.0f);
			}
		}
		/*-------------------------------- 2 LIGHTS -----------------------------*/
		else {
			vetor light1 = glm::normalize(light[0].m_pos - rec.p);

			vetor light2 = glm::normalize(light[1].m_pos - rec.p);

			vetor incoming = glm::normalize(r.m_o - rec.p);

			float check_dot1 = glm::dot(light1, rec.n);

			float check_dot2 = glm::dot(light2, rec.n);

			diffuse =  m_albedo * (max(check_dot1, 0.0f));
			diffuse += m_albedo * (max(check_dot2, 0.0f));

			vetor H;
			float dot;

			if (check_dot1 > 0.0f) {
				H= glm::normalize(incoming + light1);
				dot = glm::clamp(glm::dot(rec.n, H), 0.0f, 1.0f);
				df = pow(dot, 16.0f);
			}
				

			if (check_dot2 > 0.0f) {
				H = glm::normalize(incoming + light2);
				dot = glm::clamp(glm::dot(rec.n, H), 0.0f, 1.0f);
				df += pow(dot, 16.0f);
			}

			
		}



		sp = df * sp;
				
		attenuation = ambient + diffuse + sp;
		
		return true;
	};
	virtual mat_t get_type() const {
		return mat_type;
	};

	const mat_t mat_type = mat_t::Phong_t;
	colorRGB m_albedo;	

};

class Metal_m : public Material {
public:
	Metal_m(const colorRGB& a) {
		m_albedo = a;
	}

	virtual bool scatter(const Ray& r, hit_record& rec, glm::vec3& attenuation, Ray& scatter, const Light* light, const int num_light) const {

		normal3D reflected = glm::reflect(glm::normalize(r.direction()), rec.n);

		scatter = Ray(rec.p, reflected);

		attenuation = m_albedo;

		return (glm::dot((scatter.direction()), rec.n) > 0);
	}
	virtual mat_t get_type() const {
		return mat_type;
	};

	const mat_t mat_type = mat_t::Metal_t;
	colorRGB m_albedo;
};

//peter shirley schlick approximation
float schlick(float cosine, float ref_index) {
	float r0 = (1 - ref_index) / (1 + ref_index);
	r0 = r0*r0;
	return r0 + pow((1 - cosine), 5);
}

bool refract(const vetor& v, const normal3D& n, float N, vetor& ref_ray) {
	vetor uv = glm::normalize(v);
	float dt = glm::dot(uv, n);
	float disc = 1.0f - N  * N * (1 - dt*dt);
	if (disc > 0) {
		ref_ray = N * (uv - n*dt) - n * sqrt(disc);
		return true;
	}
	else {
		return false;
	}
}

class Dielectric_m : public Material {
public:
	Dielectric_m(const float& index) {
		ref_index = index;
	}
	virtual bool scatter(const Ray& r, hit_record& rec, glm::vec3& attenuation, Ray& scatter, const Light* light, const int num_light) const {
		normal3D outward;
		vetor    reflected = glm::reflect(r.direction(), rec.n);
		float    N_eta;
		attenuation = colorRGB(1.0, 1.0, 1.0);
		vetor   refracted;
		float   reflect_k;
		float   cosine;

		if (glm::dot(r.direction(), rec.n) > 0) {
			outward = -rec.n;
			N_eta = ref_index;
			cosine = glm::dot(r.direction(), rec.n) / r.direction().length();
			cosine = sqrt(1 - ref_index * ref_index * (1 - cosine * cosine));
		}
		else {
			outward = rec.n;
			N_eta = 1.0 / ref_index;
			cosine = -glm::dot(r.direction(), rec.n) / r.direction().length();
		}
		if (refract(r.direction(), outward, N_eta, refracted)) {
			reflect_k = schlick(cosine, ref_index);
		}
		else {
			reflect_k = 1.0f;
		}

		scatter = Ray(rec.p, refracted);
		
		return true;
	}


	virtual mat_t get_type() const { return mat_type; }

	const mat_t mat_type = mat_t::Dielectric_t;
	float ref_index;

};


#endif 