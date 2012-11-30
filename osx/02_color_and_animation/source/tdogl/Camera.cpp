/*
 tdogl::Camera

 Copyright 2012 Thomas Dalling - http://tomdalling.com/

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

 http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
 */

#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>

using namespace tdogl;


Camera::Camera() :
    _position(0,0,1),
    _horizontalAngle(0),
    _verticalAngle(0)
{
}

const glm::vec3& Camera::position() const {
    return _position;
}

void Camera::setPosition(const glm::vec3& position) {
    _position = position;
}

glm::quat Camera::orientation() const {
    glm::quat q;
    q = glm::rotate(q, _verticalAngle, glm::vec3(1,0,0));
    q = glm::rotate(q, _horizontalAngle, glm::vec3(0,1,0));
    return q;
}

glm::vec3 Camera::forward() const {
    return glm::vec3(0,0,-1) * orientation();
}

glm::vec3 Camera::right() const {
    return glm::vec3(1,0,0) * orientation();
}

glm::vec3 Camera::up() const {
    return glm::vec3(0,-1,0) * orientation();
}

void Camera::offsetOrientation(float upOffset, float rightOffset, float sensitivity) {
    _horizontalAngle += rightOffset * sensitivity;
    while(_horizontalAngle > 360.0f) _horizontalAngle -= 360.0;
    while(_horizontalAngle < 0.0f) _horizontalAngle += 360.0;

    _verticalAngle += upOffset * sensitivity;
    if(_verticalAngle > 80.0f) _verticalAngle = 80.0f;
    if(_verticalAngle < -80.0f) _verticalAngle = -80.0f;
}

glm::mat4 Camera::matrix() const {
    return glm::mat4_cast(orientation()) * glm::translate(glm::mat4(), -_position);
}
