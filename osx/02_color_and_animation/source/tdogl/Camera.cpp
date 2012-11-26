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
    _forward(0,0,-1),
    _upward(0,1,0)
{
}

const glm::vec3& Camera::position() const {
    return _position;
}

void Camera::setPosition(const glm::vec3& position) {
    _position = position;
}

const glm::vec3& Camera::forward() const {
    return _forward;
}

void Camera::setForward(const glm::vec3& forwards) {
    _forward = glm::normalize(forwards);
}

void Camera::lookAt(const glm::vec3& lookAt) {
    setForward(lookAt - _position);
}

const glm::vec3& Camera::upward() const {
    return _upward;
}

void Camera::setUpward(const glm::vec3& upward) {
    _upward = glm::normalize(upward);
}

glm::vec3 Camera::rightward() const {
    return glm::normalize(glm::cross(_forward, _upward));
}

glm::mat4 Camera::matrix() const {
    return glm::lookAt(_position, _position + _forward, _upward);
}
