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

static const float MaxVerticalAngle = 85.0f; //must be less than 90 to avoid gimbal lock
static const float NearPlaneDistance = 0.1; //must be > 0


Camera::Camera() :
    _position(0,0,1),
    _horizontalAngle(0),
    _verticalAngle(0),
    _viewingAngle(50),
    _viewingDistance(100)
{
}

const glm::vec3& Camera::position() const {
    return _position;
}

void Camera::setPosition(const glm::vec3& position) {
    _position = position;
}

void Camera::offsetPosition(const glm::vec3& offset) {
    _position += offset;
}

float Camera::viewingAngle() const {
    return _viewingAngle;
}

void Camera::setViewingAngle(float viewingAngle) {
    assert(viewingAngle > 0.0f && viewingAngle < 180.0f);
    _viewingAngle = viewingAngle;
}

float Camera::viewingDistance() const {
    return _viewingDistance;
}

void Camera::setViewingDistance(float viewingDistance) {
    assert(viewingDistance > NearPlaneDistance);
    _viewingDistance = viewingDistance;
}

glm::mat4 Camera::orientation() const {
    glm::mat4 orientation;
    orientation = glm::rotate(orientation, _verticalAngle, glm::vec3(1,0,0));
    orientation = glm::rotate(orientation, _horizontalAngle, glm::vec3(0,1,0));
    return orientation;
}

glm::vec3 Camera::forward() const {
    glm::vec4 forward = glm::inverse(orientation()) * glm::vec4(0,0,-1,1);
    return glm::vec3(forward);
}

glm::vec3 Camera::right() const {
    glm::vec4 right = glm::inverse(orientation()) * glm::vec4(1,0,0,1);
    return glm::vec3(right);
}

glm::vec3 Camera::up() const {
    glm::vec4 up = glm::inverse(orientation()) * glm::vec4(0,-1,0,1);
    return glm::vec3(up);
}

void Camera::offsetOrientation(float upAngle, float rightAngle) {
    _horizontalAngle += rightAngle;
    while(_horizontalAngle > 360.0f) _horizontalAngle -= 360.0;
    while(_horizontalAngle < 0.0f) _horizontalAngle += 360.0;

    _verticalAngle += upAngle;
    if(_verticalAngle > MaxVerticalAngle) _verticalAngle = MaxVerticalAngle;
    if(_verticalAngle < -MaxVerticalAngle) _verticalAngle = -MaxVerticalAngle;
}

glm::mat4 Camera::matrix(float screenWidth, float screenHeight) const {
    glm::mat4 camera = glm::perspective(_viewingAngle, screenWidth/screenHeight, NearPlaneDistance, _viewingDistance);
    camera *= orientation();
    camera = glm::translate(camera, -_position);
    return camera;
}
