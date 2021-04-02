#ifndef _CAMERA_
#define _CAMERA_

#include "Utilities.h"
#include "Ray.h"


class Camera {
public:
	Camera(const point3D& eye = point3D(1.0f, 1.5f, 7.0f), const point3D& lookat = point3D(0.0f, 0.0f, 0.0f), const vetor& up = vetor(0.0f, 1.0f, 0.0f), float fov = 60.0f, float W = stW, float H = stH) {
		m_eye = eye;
		m_lookat = lookat;
		m_up = up;
		m_fov = fov;
		m_W = stW;
		m_H = stH;
		m_aspect = stW / stH;
		
		build_frame();

		float theta = m_fov  * (float)constants::PI_ON_180;
		
		float half_w = tan(theta/2);
		float half_h = half_w / m_aspect;

		//float half_h = tan(theta / 2);
		//float half_w = m_aspect * half_h;			 

		m_lower = m_eye - half_w * m_u - half_h * m_v - m_w;
		horizontal = glm::vec3(2.0f * half_w * m_u);
		vertical = glm::vec3(2.0f*half_h * m_v);


	};

	void build_frame() {
		m_w = glm::normalize(m_eye - m_lookat);
		m_u = glm::normalize(glm::cross(m_up, m_w));
		m_v = glm::normalize(glm::cross(m_w, m_u));		
	}

	void rebuild() {
		build_frame();

		float theta = m_fov  * (float)constants::PI_ON_180;
		float half_h = tan(theta / 2);
		float half_w = m_aspect * half_h;

		m_lower = m_eye - half_w * m_u - half_h * m_v - m_w;
		horizontal = glm::vec3(2.0f * half_w * m_u);
		vertical = glm::vec3(2.0f*half_h * m_v);
	}

	Ray get_ray(const float u, const float v) {
		return Ray(m_eye, glm::normalize(m_lower + u * horizontal + v * vertical - m_eye));
	}


	//glm::mat4 view_mat;
	point3D m_lower, horizontal, vertical;
	float m_W, m_H, m_aspect;
	point3D m_eye;
	point3D m_lookat;
	vetor m_up;
	vetor m_u, m_v, m_w;
	float m_fov;



};





#endif