#ifndef __NADOR_COMMON_PACKED_SEQUENCE_UTILS_H__
#define __NADOR_COMMON_PACKED_SEQUENCE_UTILS_H__

#include "nador/utils/Sequence.h"
#include "nador/common/GlobalEvents.h"

namespace nador
{
    template<typename ObjTy, typename DurationTy = std::chrono::milliseconds>
    constexpr auto CreatePackedSequence(ObjTy& obj, const std::vector<ElementSequence<DurationTy, ObjTy&>>& seq, Event<DurationTy>& event = g_onAnimationTickEvent)
    {
        return PackedSequence<ObjTy, DurationTy>{obj, seq, event};
    }
}

#endif //!__NADOR_COMMON_PACKED_SEQUENCE_UTILS_H__