#ifndef _UTILS_H_
#define _UTILS_H_

#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <GLM/gtx/rotate_vector.hpp>
#include <fstream>
#include <vector>
#include <cfloat>
#include <ctime>
#include "Ray.h"



/*Este arquivo possui apenas funções e structs de proposito geral, e utilidades para suporte do codigo principal*/
namespace constants {

	const float k_min = 0.0001f;
	const float k_max = FLT_MAX;
	const float PI = 3.1415926535897932384;
	const float INV_PI = 0.3183098861837906715;
	const float PI_ON_180 = 0.0174532925199432957;
	const float INV_TWO_PI = 0.1591549430918953358;
	const unsigned int  MAX_DEPTH = 25;
}

const float stW = 480;
const float stH = 480;



inline int rand_int() {
	return (rand());
}

inline float rand_float() {
	return ((float)rand() / (float)RAND_MAX);
}

inline float rand_float_range(const float HI, const float LO) {
	return LO + (float)(rand()) / ((float)(RAND_MAX) / (HI - LO));
}

inline void set_rand_seed(const time_t seed) {
	srand(seed);
}

template<typename T>
T max(T a, T b) {
	
	return a >= b ? a : b ;

	
}


typedef uint8_t u8;

struct rgb {
	uint8_t r;
	uint8_t g;
	uint8_t b;

	//defaults to black
	rgb() :rgb(rgb(255,0,0)) {}

	rgb(const rgb& col) :rgb(col.r, col.g, col.b) {}

	rgb(uint8_t x, uint8_t y, uint8_t z) : r(x), g(y), b(z) {}

	rgb(uint8_t a) : r(a), g(a), b(a) {}
	
	rgb& operator+(const rgb& rhs) {
		this->r + rhs.r; this->g + rhs.g; this-> b + rhs.b; return *this;
	}

	rgb& operator+(const int rhs) {
		this->r + rhs; this->g + rhs; this->b + rhs; return *this;
	}

	rgb& operator-(const rgb& rhs) {
		this->r - rhs.r; this->g - rhs.g; this->b - rhs.b; return *this;
	}

	rgb& operator-(const int rhs) {
		this->r - rhs; this->g - rhs; this->b - rhs; return *this;
	}

	rgb& operator*(const int rhs) {
		this->r * rhs; this->g * rhs; this->b * rhs; return *this;
	}

	rgb& operator/(const int rhs) {
		this->r / rhs; this->g / rhs; this->b / rhs; return *this;
	}

	rgb& operator*(const rgb& rhs) {
		this->r * rhs.r; this->g * rhs.g; this->b * rhs.b; return *this;
	}

	rgb& operator/(const rgb& rhs) {
		this->r / rhs.r; this->g / rhs.g; this->b / rhs.b; return *this;
	}
	
	rgb& operator=(const rgb& rhs) {
		this->r = rhs.r; this->g = rhs.g; this->b = rhs.b; return *this;
	}

	rgb& operator=(const int rhs) {
		this->r = rhs; this->g = rhs; this->b = rhs; return *this;
	}
	

};

const glm::vec3 black(0.0f, 0.0f, 0.0f);
const glm::vec3 red(1.0f, 0.0f, 0.0f);
const glm::vec3 green(0.0f, 1.0f, 0.0f);
const glm::vec3 blue(0.0f, 0.0f, 1.0f);

//debugging and test purposes
void test_ppm(void);

//TODO -> continuar construindo a funcao
void write_ppm(const int width, const int height, const std::string& name, const std::vector<rgb>& img); 

#endif
