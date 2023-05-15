#ifndef CAMERA_H
#define CAMERA_H

#include <glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
using namespace std;


typedef glm::vec3 gVec3;
typedef glm::mat4 gMat4;

enum wasd {
	W, A, S, D
};

class Camera {

	public:
		//World locations
		gVec3 cameraPos;
		gVec3 cameraTarget;
		gVec3 worldUp;

		//Camera 
		gVec3 cameraUp;
		gVec3 cameraDir;
		gVec3 cameraRight;

		Camera(gVec3 position, gVec3 targetPos, gVec3 upVec) {
			cameraPos = position;
			cameraTarget = targetPos;
			worldUp = upVec;

			updateCameraVectors();
		}

		gMat4 getViewMatrix() {
			return glm::lookAt(cameraPos, cameraPos - 10.0f*cameraDir , worldUp);
		}

		void changePosition(gVec3 position) {
			cameraPos = position;
			updateCameraVectors();
		}

		void keyCallback(wasd key) {
			if (key == W)		cameraPos -= cameraDir;
			else if (key == S)	cameraPos += cameraDir;
			else if (key == A)  cameraPos -= cameraRight;
			else if (key == D)	cameraPos += cameraRight;
			updateCameraVectors();

 		}

		void updateCameraVectors() {
			
			cameraDir = gVec3(0.0f, 0.0f, 1.0f );
			cameraRight = glm::normalize(glm::cross(worldUp, cameraDir));
			cameraUp = glm::normalize(glm::cross(cameraDir, cameraRight));

		}
};

#endif