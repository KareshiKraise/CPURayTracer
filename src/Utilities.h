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



/*Este arquivo possui apenas funções e structs de proposito geral, e utilidades para suporte do codigo principal*/
namespace constants {

	const float k_min = 0.0001f;
	const float k_max = FLT_MAX;
	const float PI = 3.1415926f;
	const float INV_PI = 0.3183098f;
	const float PI_ON_180 = 0.0174532f;
	const float INV_TWO_PI = 0.1591549f;
	const unsigned int  MAX_DEPTH = 3;
}

const float stW = 480.0f;
const float stH = 320.0f;

typedef glm::vec3 normal3D;
typedef glm::vec3 point3D;
typedef glm::vec3 colorRGB;
typedef glm::vec3 vetor;


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
	srand((unsigned int)seed);
}

template<typename T>
T max(T a, T b) {
	
	return a >= b ? a : b ;

	
}


typedef uint8_t u8;


const glm::vec3 black(0.0f, 0.0f, 0.0f);
const glm::vec3 red(1.0f, 0.0f, 0.0f);
const glm::vec3 green(0.0f, 1.0f, 0.0f);
const glm::vec3 blue(0.0f, 0.0f, 1.0f);



#endif
