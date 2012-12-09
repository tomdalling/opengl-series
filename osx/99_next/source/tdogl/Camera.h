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

#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace tdogl {

    class Camera {
    public:
        Camera();

        const glm::vec3& position() const;
        void setPosition(const glm::vec3& position);
        void offsetPosition(const glm::vec3& offset);

        glm::quat orientation() const;
        void offsetOrientation(float upOffset, float rightOffset, float sensitivity);
        
        glm::vec3 forward() const;
        glm::vec3 right() const;
        glm::vec3 up() const;

        glm::mat4 matrix() const;

    private:
        glm::vec3 _position;
        float _horizontalAngle;
        float _verticalAngle;
    };

}

