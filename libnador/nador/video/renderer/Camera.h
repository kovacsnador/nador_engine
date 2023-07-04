#ifndef __NADOR_CAMERA_H__
#define __NADOR_CAMERA_H__

#include <glm/glm.hpp>

namespace nador
{
    class Camera
    {
    public:
        using ViewCalcStrategy_t = std::function<glm::mat4(const glm::vec3&, const glm::vec3&)>;

        Camera(const glm::mat4& projection, ViewCalcStrategy_t viewCalcStrategy)
        : _projectionMtx(projection)
        , _viewCalcStrategy(std::move(viewCalcStrategy))
        {
            _Update();
        }

        void SetProjection(const glm::mat4& projection)
        {
            _projectionMtx = projection;
            _Update();
        }

        void SetViewMtxCalcStrategy(ViewCalcStrategy_t strategy)
        {
            _viewCalcStrategy = std::move(strategy);
            _Update();
        }
        void SetPosition(const glm::vec3& position)
        {
            _position = position;
            _Update();
        }
        void SetRotation(const glm::vec3& rotation)
        {
            _rotation = rotation;
            _Update();
        }

        glm::mat4 GetCameraMtx() const { return _cameraMtxCache; }

        const glm::mat4& GetProjection() const {return _projectionMtx; }
        const glm::mat4& GetView() const {return _viewMtx; }

    private:
        void _Update()
        {
            if (_viewCalcStrategy)
            {
                _viewMtx        = _viewCalcStrategy(_position, _rotation);
                _cameraMtxCache = _projectionMtx * _viewMtx;
            }
        }

        glm::mat4 _projectionMtx { 1.0f };
        glm::mat4 _viewMtx { 1.0f };
        glm::mat4 _cameraMtxCache { 1.0f };

        glm::vec3 _position { 0.0f, 0.0f, 0.0f };
        glm::vec3 _rotation { 0 };

        ViewCalcStrategy_t _viewCalcStrategy {};
    };

    inline glm::mat4 OrthograpicViewMtxCalculation(const glm::vec3& position, const glm::vec3& rotation)
    {
        auto transform = glm::translate(glm::mat4 { 1.0f }, position);
        auto rotationMtx  = glm::rotate(glm::mat4 { 1.0f }, glm::radians(rotation.z), glm::vec3(0, 0, 1));

        // return the view matrix
        return glm::inverse(transform * rotationMtx);
    }

} // namespace nador

#endif //!__NADOR_CAMERA_H__