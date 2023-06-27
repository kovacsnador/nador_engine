#ifndef __NADOR_CAMERA_H__
#define __NADOR_CAMERA_H__

#include <glm/glm.hpp>

namespace nador
{
    class Camera
    {
    public:
        Camera(const glm::mat4& projection);

        void SetProjection(const glm::mat4& projection);
        void SetPosition(const glm::vec3& position);
    private:
        glm::mat4 _projectionMtx { 1.0f };
        glm::mat4 _viewMtx { 1.0f };

        glm::vec3 _position { 0.0f, 0.0f, 0.0f };
        float     _rotation { 0 };
    };

} // namespace nador

#endif //!__NADOR_CAMERA_H__