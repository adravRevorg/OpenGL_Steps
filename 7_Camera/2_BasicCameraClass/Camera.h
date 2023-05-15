#ifndef CAMERA_H
#define CAMERA_H

#include <glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
using namespace std;


typedef glm::vec3 gVec3;
typedef glm::mat4 gMat4;

class Camera {

	public:
		gVec3 pos;
		gVec3 up;
		gVec3 dir;
		gVec3 right;

		Camera(gVec3 position, gVec3 direction, gVec3 upVec) {
			pos = position;
			dir = direction;
			up = upVec;
		}

		gMat4 getViewMatrix() {
			return glm::lookAt(pos, dir, up);
		}

		void changePosition(gVec3 position) {
			pos = position;
		}
};



#endif