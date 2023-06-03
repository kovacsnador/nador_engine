#ifndef __MOVEABLE_OBJ_WRAPPER_H__
#define __MOVEABLE_OBJ_WRAPPER_H__

#include <concepts>

namespace nador
{
    template<typename ObjTy, typename Cleanup>
    requires std::invocable<Cleanup, ObjTy&>
    struct MoveableObjWrapper
    {
        MoveableObjWrapper(ObjTy&& obj)
        : obj(std::move(obj))
        {
        }

        ~MoveableObjWrapper()
        {
            cleanup(obj);
        }

        // default
        MoveableObjWrapper(MoveableObjWrapper&&) = default;
        MoveableObjWrapper& operator=(MoveableObjWrapper&&) = default;

        // delete
        MoveableObjWrapper(MoveableObjWrapper&) = delete;
        MoveableObjWrapper& operator=(MoveableObjWrapper&) = delete;

        ObjTy obj;

    private:
        Cleanup cleanup; 
    };
}

#endif //!__MOVEABLE_OBJ_WRAPPER_H__
