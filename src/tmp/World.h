#ifndef _WORLD_H_
#define _WORLD_H_

#include "Object3D.h"
#include "Plane.h"
#include "Sphere.h"
#include "Scene3D.h"
#include "Utilities.h"
#include "Camera.h"
#include "Material.h"
#include <fstream>
#include <string>


class World {
public:
	World(const glm::vec3& lpos = glm::vec3(-1.0f, 2.0f, -1.0f) ,float w = stW, float h = stH, int s = 15, Object3D* obj = nullptr) {
		_w = w;
		_h = h;
		_s = s;
		_hitables = obj;
		_light = lpos;
		_img = std::vector<rgb>(_w * _h);
	};

	~World() {
		if (_hitables) {
			std::cout << "deleting from world" << std::endl;
			delete _hitables;
		}
	}

	void prepare_camera(const glm::vec3& eye, const glm::vec3& lookat,const glm::vec3& up, const float fov) {
		_cam = Camera(eye, lookat, up, fov, _w / _s);
	}

	void change_eye(const glm::vec3& eye) {
		_cam = Camera(eye);
	}

	void build_standard_scene() {

		std::vector<Object3D*> scene;

		scene.emplace_back(new Sphere(glm::vec3(-1.5f, 0.0f, -1.0f),
								      0.5f,
			                          new Lambertian(glm::vec3(0.8, 0.3, 0.3)),
									  glm::vec3(0.2, 0.0, 0.0)
							          ));


		//scene.emplace_back(new Sphere(glm::vec3(0.0f, -100.5f, -1.1f), 
		//			                  100.0f, 
		//							  new Lambertian(glm::vec3(0.8, 0.8, 0.0)),
		//							  glm::vec3(1.0, 1.0, 0.0)
		//							  ));

		scene.emplace_back(new Plane());

		scene.emplace_back(new Sphere(glm::vec3(-1.0f, 0.0f, -2.0f),
									  0.5f, 
									  new Metal(glm::vec3(0.8f, 0.6f, 0.2f)),
									  glm::vec3(0.3f, 0.4f, 0.2f)
									  ));

		scene.emplace_back(new Sphere(glm::vec3(0.0f, 0.0f, -1.0f),
									  0.5f, 
									  new Dielectric(1.5f),
									  glm::vec3(0.0f, 1.0f, 0.4f)
									  ));

		_hitables = new Scene3D(scene, scene.size());

	};
	
	void render_to_ppm(const std::string& name) {

		std::ofstream out(name, std::ofstream::binary);

		out << "P3\n" << _w << " " << _h << "\n255\n";
		

		for (int j = 0; j < _h; j++)
		{
			for (int i = 0; i < _w; i++)
			{
				glm::vec3 col(0.0f, 0.0f, 0.0f);
				for (int s = 0; s < _s; s++)
				{
					float u = float(i + rand_float()) / float(_w);
					float v = float(j + rand_float()) / float(_h);

					Ray r = _cam.get_ray(u, v);


					col += hit_and_shade(r, _hitables, 0);
				}
				col /= (float)_s;
				col = glm::vec3(sqrt(col.x), sqrt(col.y), sqrt(col.z)); //gamma correction

				int ir = int(255.00f*col.x);
				int ig = int(255.00f*col.y);
				int ib = int(255.00f*col.z);

				

				out << ir << " " << ig << " " << ib << "\n";
			}
		}


	}

	void render() {

		for (int j = 0; j < _h; j++) {
			for (int i = 0; i < _w; i++) {

				float u = float(i) / float(_w);
				float v = float(j) / float(_h);

				Ray r = _cam.get_ray(u, v);
				glm::vec3 col = trace(r, _hitables, 0);

				int ir = int(255.99f * col.x);
				int ig = int(255.99f * col.y);
				int ib = int(255.99f * col.z);

				_img[_w * j + i] = rgb(ir, ig, ib);
			}
		}
	}

	std::vector<rgb>& get_buffer() {
		return _img;
	}
	
private:
	
	//renderer trace
	glm::vec3 trace(const Ray& r, Object3D* world, int depth) {
		ray_hit rec;

		if (world->hit(r, constants::k_min, constants::k_max, rec, _light)) {
			
			return rec.d;
		}
		else {
			return shade_bg(r);
		}
	}

	

	//renderer hit and shade
	glm::vec3 hit_and_shade(const Ray& r, Object3D* world, int depth) {
		ray_hit rec;

		if (world->hit(r, constants::k_min, constants::k_max, rec, _light))
		{
			Ray scattered;
			glm::vec3 attenuation;
			if (depth < constants::MAX_DEPTH && rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
				return attenuation * hit_and_shade(scattered, world, depth + 1);
			}
			else {
				return glm::vec3(0.0f, 0.0f, 0.0f);
			}
		}
		else
		{
			return shade_bg(r);
		}
	}




public:
	std::vector<rgb> _img;
	glm::vec3 _light;
	Camera _cam;
	float _w; //width
	float _h; //height
	int   _s; //samples
	Object3D* _hitables;





};



#endif