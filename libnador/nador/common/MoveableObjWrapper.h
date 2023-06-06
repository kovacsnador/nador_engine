#ifndef __MOVEABLE_OBJ_WRAPPER_H__
#define __MOVEABLE_OBJ_WRAPPER_H__

#include <concepts>

namespace nador
{
    template <typename ObjTy, typename CleanupStrategy>
        requires std::invocable<CleanupStrategy, ObjTy&>
    struct MoveableObjWrapper
    {
        MoveableObjWrapper(ObjTy&& obj)
        : obj(std::move(obj))
        {
        }

        ~MoveableObjWrapper() { cleanupStrategy(obj); }

        // default
        MoveableObjWrapper(MoveableObjWrapper&&)            = default;
        MoveableObjWrapper& operator=(MoveableObjWrapper&&) = default;

        // delete
        MoveableObjWrapper(MoveableObjWrapper&)            = delete;
        MoveableObjWrapper& operator=(MoveableObjWrapper&) = delete;

        ObjTy obj;

    private:
        CleanupStrategy cleanupStrategy;
    };
} // namespace nador

#endif //!__MOVEABLE_OBJ_WRAPPER_H__
