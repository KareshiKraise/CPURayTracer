#ifndef _CAMERA_
#define _CAMERA_

#include "Ray.h"
#include "Utilities.h"

class Camera {
	public:
		Camera() {
			glm::vec3 u, v, w;
			float theta = 120.0f * (float)constants::PI_ON_180;
			float half_h = tan(theta / 2);
			float half_w = stW/stH * half_h;

			glm::vec3 eye(2.0f, 2.0f, 0.0f);
			glm::vec3 lookat(0.0f, 0.0f, -1.0f);
			glm::vec3 up(0.0f, 1.0f, 0.0f);

			origin = eye;
			w = glm::normalize(eye - lookat);
			u = glm::normalize(glm::cross(up, w));
			v = glm::cross(w, u);

			_u = u;
			_v = v;
			_w = w;

			lower_left = glm::vec3(-half_w, -half_h, -1.0f);
			lower_left = origin - half_w * u - half_h *v - w;

			horizontal = glm::vec3(2.0f*half_w * u);
			vertical = glm::vec3(2.0f * half_h * v);
		}
		Camera(glm::vec3 eye, glm::vec3 lookat = glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3 up= glm::vec3(0.0f, 1.0f, 0.0f), float fov = 120.0f, float aspect = stW/stH) {
			glm::vec3 u, v, w;
			float theta = fov * (float)constants::PI_ON_180;
			float half_h = tan(theta / 2);
			float half_w = aspect * half_h;
			
			origin = eye;
			w = glm::normalize(eye - lookat);
			u = glm::normalize(glm::cross(up, w));
			v = glm::cross(w, u);

			//lower_left = glm::vec3(-half_w, -half_h, -1.0f);
			lower_left = origin - half_w * u - half_h *v - w;
			
			horizontal = glm::vec3(2.0f*half_w * u);
			vertical   = glm::vec3(2.0f * half_h * v);
		}

		Ray get_ray(float u, float v) {
			return Ray(origin, lower_left + u * horizontal + v * vertical - origin);
		};
		
		glm::vec3 get_u() const { return _u; };
		glm::vec3 get_v() const { return _v; };
		glm::vec3 get_w() const { return _w; };
		
		glm::vec3 origin;
		glm::vec3 lower_left;
		glm::vec3 horizontal;
		glm::vec3 vertical;

	private:
		glm::vec3 _u;
		glm::vec3 _w;
		glm::vec3 _v;
		
};



#endif