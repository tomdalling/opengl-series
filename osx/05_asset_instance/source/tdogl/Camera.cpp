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


Camera::Camera() :
    _position(0.0f, 0.0f, 1.0f),
    _horizontalAngle(0.0f),
    _verticalAngle(0.0f),
    _fieldOfView(50.0f),
    _nearPlane(0.01f),
    _farPlane(100.0f),
    _viewportAspectRatio(4.0f/3.0f)
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

float Camera::fieldOfView() const {
    return _fieldOfView;
}

void Camera::setFieldOfView(float fieldOfView) {
    assert(fieldOfView > 0.0f && fieldOfView < 180.0f);
    _fieldOfView = fieldOfView;
}

float Camera::nearPlane() const {
    return _nearPlane;
}

float Camera::farPlane() const {
    return _farPlane;
}

void Camera::setNearAndFarPlanes(float nearPlane, float farPlane) {
    assert(nearPlane > 0.0f);
    assert(farPlane > nearPlane);
    _nearPlane = nearPlane;
    _farPlane = farPlane;
}

glm::mat4 Camera::orientation() const {
    glm::mat4 orientation;
    orientation = glm::rotate(orientation, _verticalAngle, glm::vec3(1,0,0));
    orientation = glm::rotate(orientation, _horizontalAngle, glm::vec3(0,1,0));
    return orientation;
}

void Camera::offsetOrientation(float upAngle, float rightAngle) {
    _horizontalAngle += rightAngle;
    while(_horizontalAngle > 360.0f) _horizontalAngle -= 360.0;
    while(_horizontalAngle < 0.0f) _horizontalAngle += 360.0;

    _verticalAngle += upAngle;
    if(_verticalAngle > MaxVerticalAngle) _verticalAngle = MaxVerticalAngle;
    if(_verticalAngle < -MaxVerticalAngle) _verticalAngle = -MaxVerticalAngle;
}

float Camera::viewportAspectRatio() const {
    return _viewportAspectRatio;
}

void Camera::setViewportAspectRatio(float viewportAspectRatio) {
    assert(viewportAspectRatio > 0.0);
    _viewportAspectRatio = viewportAspectRatio;
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
    glm::vec4 up = glm::inverse(orientation()) * glm::vec4(0,1,0,1);
    return glm::vec3(up);
}

glm::mat4 Camera::matrix() const {
    glm::mat4 camera = glm::perspective(_fieldOfView, _viewportAspectRatio, _nearPlane, _farPlane);
    camera *= orientation();
    camera = glm::translate(camera, -_position);
    return camera;
}
