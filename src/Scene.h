#ifndef _SCENE_
#define _SCENE_

#include "Utilities.h"
#include "Object.h"
#include <vector>
#include "Camera.h"
#include "Light.h"
#include "Material.h"
#include "Sphere.h"
#include "Plane.h"
#include "List.h"

glm::vec3 shade_bg(const Ray& r) {

	glm::vec3 unit_dir = glm::normalize(r.direction());
	float t = 0.5f*(unit_dir.y + 1.0f);
	return (1.0f - t) * glm::vec3(1.0f, 1.0f, 1.0f) + t * glm::vec3(0.5f, 0.7f, 1.0f);
}

class Scene {
public:
		
	Scene(const int W = stW, const int H = stH) {
		m_w = W;
		m_h = H;
		m_image = std::vector<colorRGB>(m_w * m_h);		
		m_angle = 0.02f;
		buf_size = m_w * m_h * sizeof(colorRGB);
		num_lights = 2;
		m_light = new Light[num_lights];
		m_light[num_lights - 1].m_pos = point3D(-3.0f, 10.0f, -2.5f);
		
	
	};

	~Scene() {
		for (int i = 0; i < 2; i++) {
			delete[] m_light;
		}
		if (m_list) {
			delete m_list;
		}
	};
	


	int calc_buf_size() {
		return buf_size = m_w * m_h * sizeof(colorRGB);
	}
	
	void change_eye(const point3D& eye, const point3D& lookat = point3D(0.0f, 0.0f, 0.0f)) {
		m_cam = Camera(eye, lookat);
	}

	void build_standard_scene() { 

		std::vector<Object*> list;

		list.emplace_back(new Sphere());
		
		list.emplace_back(new Sphere(0.5f, point3D(1.5f, 1.5f, 0.0f), new Metal_m( colorRGB(0.8f,0.8f,0.8f)	)));

		list.emplace_back(new Sphere(1.0f, point3D(-2.5f, 2.0f, 0.0f), new Dielectric_m(1.5f)));
				
		list.emplace_back(new Plane());		

		m_list = new List(list, list.size());
		
	};

	void rotate_eye() {

		point3D rotated	= m_cam.m_eye;
		vetor up = m_cam.m_up;//vetor(0.0f, 1.0f, 0.0f);
		//vetor c1(cos(1.0f*constants::PI_ON_180), 0.0f, sin(1.0f*constants::PI_ON_180));
		//vetor c2(0.0f, 1.0f, 0.0f);
		//vetor c3(-sin(1.0f*constants::PI_ON_180), 0.0f, cos(1.0f*constants::PI_ON_180));
		//glm::mat3 R(c1, c2, c3);
		rotated = glm::rotate(m_cam.m_eye, m_angle, up);


		
		change_eye(rotated);

	}

	void render() {
		if (rotate)
		{
			rotate_eye();
		}
		

		for (int j = 0; j < m_h; j++) {
			for (int i = 0; i < m_w; i++) {

				float u = float(i) / float(m_w);
				float v = float(j) / float(m_h);

				Ray r = m_cam.get_ray(u, v);

				colorRGB col = trace_lights(r, m_list, 0);				

				m_image[(m_w * j) + i] = glm::clamp(col, 0.0f, 255.0f);
				
			}
		}
	}

	/*------------------------SHADOWS----------------------------------*/
	/*-----------------------------------------------------------------*/
	/*Ray to_light(rec.p, glm::normalize(m_light.m_pos - rec.p));	   */
	/*																   */
	/*if (obj->hit(to_light, constants::k_min, constants::k_max, rec)){*/
	/*	return { 0.1f, 0.1f, 0.1f };								   */
	/*}																   */
	/*-----------------------------------------------------------------*/

	colorRGB trace_lights(const Ray& r, Object* obj, int depth) {
		hit_record rec;
		
		if (obj->hit(r, constants::k_min, constants::k_max, rec))
		{
			
			Ray scatter;
			colorRGB attenuation(0.0f,0.0f,0.0f);

			if (depth < constants::MAX_DEPTH && rec.mat_ptr->scatter(r, rec, attenuation, scatter, m_light, num_lights))
			{
				if (rec.mat_ptr->get_type() == mat_t::Phong_t) {
					return attenuation;
				}
				else if (rec.mat_ptr->get_type() == mat_t::Pattern_t) {
					//cast shadows only for Pattern_t for performance reasons

					if (num_lights == 1)
					{
						Ray to_light(rec.p, glm::normalize(m_light[0].m_pos - rec.p));
						if (obj->hit(to_light, constants::k_min, constants::k_max, rec))
							return attenuation * colorRGB(0.2f, 0.2f, 0.2f);
						else
							return attenuation;
					}

					else
					{
						Ray light1(rec.p, glm::normalize(m_light[0].m_pos - rec.p));

						Ray light2(rec.p, glm::normalize(m_light[1].m_pos - rec.p));

						if (obj->hit(light1, constants::k_min, constants::k_max, rec))
							attenuation = attenuation * colorRGB(0.2f, 0.2f, 0.2f);
						

						if (obj->hit(light2, constants::k_min, constants::k_max, rec))
							attenuation = attenuation * colorRGB(0.2f,0.2f,0.2f);
					}
					return attenuation;

				}
				else {
					return  attenuation * trace(scatter, obj, depth + 1);
				}
			}
			else {
				return colorRGB(0.0f, 0.0f, 0.0f);
			}
		}
			
	else{
			return shade_bg(r);
		}
}
	
	colorRGB trace(const Ray& r, Object* obj, int depth) {		

		hit_record rec;

		if (obj->hit(r, constants::k_min, constants::k_max, rec))
		{
			Ray scatter;
			colorRGB attenuation;
			
			if (depth < constants::MAX_DEPTH && rec.mat_ptr->scatter(r, rec, attenuation, scatter, m_light, num_lights))
			{
				if (rec.mat_ptr->get_type() == mat_t::Phong_t) {
					return attenuation;
				}
				else if (rec.mat_ptr->get_type() == mat_t::Pattern_t) {

					Ray to_light(rec.p, glm::normalize(m_light[0].m_pos - rec.p));
					if (obj->hit(to_light, constants::k_min, constants::k_max, rec))
						return attenuation * colorRGB(0.2f, 0.2f, 0.2f);
					else
						return attenuation;

				}
				else {
					return  attenuation * trace(scatter, obj, depth + 1);
				}

			}
			else
			{
				return colorRGB(0.0f, 0.0f, 0.0f);
				
			}
			
		}
		else
		{
			return shade_bg(r);
		}
		
	}

	
	float m_angle;
	Light* m_light;
	int   num_lights;
	int buf_size;
	Camera m_cam;
	int m_w, m_h;
	Object*  m_list;
	bool rotate = false;

	std::vector<colorRGB> m_image;
	
	

};




#endif