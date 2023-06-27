#include "Camera.h"

nador::Camera::Camera(const glm::mat4& projection)
: _projectionMtx(projection)
{
}

void nador::Camera::SetProjection(const glm::mat4& projection)
{
    _projectionMtx = projection;
}

void nador::Camera::SetPosition(const glm::vec3& position)
{
    _position = position;
}
