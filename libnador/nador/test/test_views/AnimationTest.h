#ifndef __NADOR_ANIMATION_TEST_H__
#define __NADOR_ANIMATION_TEST_H__

#include "nador/test/ITest.h"
#include "nador/ui/UiImage.h"
#include "nador/utils/Sequence.h"

namespace nador
{
    class IUiApp;

    class AnimationTest : public ITest
    {
    public:
        AnimationTest(IUiApp* uiApp);

        void OnDebugRender(IRenderer* renderer) override;

    private:
        IUiApp* _uiApp;

        UiImage _image {{150, 50}, {200, 200}, video::EImageName::FEHERMOSOLYOG};
        UiImage _image2 {{350, 50}, {150, 150}, video::EImageName::FEHER_FEKETE0001};

        PackedSequence<UiImage> _simpleAnim{};
        PackedSequence<UiImage> _imageAnim{};
    };

} // namespace nador

#endif //!__NADOR_ANIMATION_TEST_H__