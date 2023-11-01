#include "imgui.h"

#include "nador/test/test_views/AnimationTest.h"
#include "nador/common/GlobalEvents.h"
#include "nador/ui/IUiApp.h"
#include "nador/common/PackedSequenceUtils.h"

namespace nv = nador::video;

static nador::ElementSequenceVec_t<nador::UiImage> s_simpleAnim {
    { 40ms, [](auto& img) { img.SetOffset({4, 0}); } },  { 40ms, [](auto& img) { img.SetOffset({8, 0}); } },
    { 40ms, [](auto& img) { img.SetOffset({12, 0}); } }, { 40ms, [](auto& img) { img.SetOffset({16, 0}); } },
    { 40ms, [](auto& img) { img.SetOffset({20, 0}); } }, { 40ms, [](auto& img) { img.SetOffset({16, 0}); } },
    { 40ms, [](auto& img) { img.SetOffset({12, 0}); } }, { 40ms, [](auto& img) { img.SetOffset({8, 0}); } },
    { 40ms, [](auto& img) { img.SetOffset({4, 0}); } },  { 40ms, [](auto& img) { img.SetOffset({0, 0}); } }
};

static nador::ElementSequenceVec_t<nador::UiImage> s_imageAnim {
    { 0ms, [](auto& img) { img.SetImage(nv::EImageName::FEHER_FEKETE0001); } },  { 16ms, [](auto& img) { img.SetImage(nv::EImageName::FEHER_FEKETE0002); } },
    { 16ms, [](auto& img) { img.SetImage(nv::EImageName::FEHER_FEKETE0003); } },  { 16ms, [](auto& img) { img.SetImage(nv::EImageName::FEHER_FEKETE0004); } },
    { 16ms, [](auto& img) { img.SetImage(nv::EImageName::FEHER_FEKETE0005); } },  { 16ms, [](auto& img) { img.SetImage(nv::EImageName::FEHER_FEKETE0006); } },
    { 16ms, [](auto& img) { img.SetImage(nv::EImageName::FEHER_FEKETE0007); } },  { 16ms, [](auto& img) { img.SetImage(nv::EImageName::FEHER_FEKETE0008); } },
    { 16ms, [](auto& img) { img.SetImage(nv::EImageName::FEHER_FEKETE0009); } },  { 16ms, [](auto& img) { img.SetImage(nv::EImageName::FEHER_FEKETE0010); } },
    { 16ms, [](auto& img) { img.SetImage(nv::EImageName::FEHER_FEKETE0011); } },  { 16ms, [](auto& img) { img.SetImage(nv::EImageName::FEHER_FEKETE0012); } },
    { 16ms, [](auto& img) { img.SetImage(nv::EImageName::FEHER_FEKETE0013); } },
    { 100ms, [](auto& img) { img.SetImage(nv::EImageName::FEHER_FEKETE0012); } },  { 16ms, [](auto& img) { img.SetImage(nv::EImageName::FEHER_FEKETE0011); } },
    { 16ms, [](auto& img) { img.SetImage(nv::EImageName::FEHER_FEKETE0010); } },  { 16ms, [](auto& img) { img.SetImage(nv::EImageName::FEHER_FEKETE0009); } },
    { 16ms, [](auto& img) { img.SetImage(nv::EImageName::FEHER_FEKETE0008); } },  { 16ms, [](auto& img) { img.SetImage(nv::EImageName::FEHER_FEKETE0007); } },
    { 16ms, [](auto& img) { img.SetImage(nv::EImageName::FEHER_FEKETE0006); } },  { 16ms, [](auto& img) { img.SetImage(nv::EImageName::FEHER_FEKETE0005); } },
    { 16ms, [](auto& img) { img.SetImage(nv::EImageName::FEHER_FEKETE0004); } },  { 16ms, [](auto& img) { img.SetImage(nv::EImageName::FEHER_FEKETE0003); } },
    { 16ms, [](auto& img) { img.SetImage(nv::EImageName::FEHER_FEKETE0012); } },  { 16ms, [](auto& img) { img.SetImage(nv::EImageName::FEHER_FEKETE0001); } },
};

static auto s_test = nador::CreateElemSequence<nador::UiImage>(std::vector {nv::EImageName::FEHER_FEKETE0001,
                                                                            nv::EImageName::FEHER_FEKETE0002,
                                                                            nv::EImageName::FEHER_FEKETE0003,
                                                                            nv::EImageName::FEHER_FEKETE0004,
                                                                            nv::EImageName::FEHER_FEKETE0005,
                                                                            nv::EImageName::FEHER_FEKETE0006,
                                                                            nv::EImageName::FEHER_FEKETE0007,
                                                                            nv::EImageName::FEHER_FEKETE0008,
                                                                            nv::EImageName::FEHER_FEKETE0009,
                                                                            nv::EImageName::FEHER_FEKETE0010,
                                                                            nv::EImageName::FEHER_FEKETE0011,
                                                                            nv::EImageName::FEHER_FEKETE0012,
                                                                            nv::EImageName::FEHER_FEKETE0013,
                                                                            nv::EImageName::FEHER_FEKETE0012,
                                                                            nv::EImageName::FEHER_FEKETE0011,
                                                                            nv::EImageName::FEHER_FEKETE0010,
                                                                            nv::EImageName::FEHER_FEKETE0009,
                                                                            nv::EImageName::FEHER_FEKETE0008,
                                                                            nv::EImageName::FEHER_FEKETE0007,
                                                                            nv::EImageName::FEHER_FEKETE0006,
                                                                            nv::EImageName::FEHER_FEKETE0005,
                                                                            nv::EImageName::FEHER_FEKETE0004,
                                                                            nv::EImageName::FEHER_FEKETE0003,
                                                                            nv::EImageName::FEHER_FEKETE0002,
                                                                            nv::EImageName::FEHER_FEKETE0001},
                                                                &nador::UiImage::SetImage);

nador::AnimationTest::AnimationTest(IUiApp* uiApp)
: _uiApp(uiApp)
{
    _uiApp->AddElementToLayer(EUiLayer::OVERLAY, &_image);
    _uiApp->AddElementToLayer(EUiLayer::OVERLAY, &_image2);

    _simpleAnim = CreatePackedSequence(_image, s_simpleAnim);
    _imageAnim = CreatePackedSequence(_image2, s_test);
}

void PlayRender(std::string_view name, nador::PackedSequence<nador::UiImage>& anim, int imguiId)
{
    ImGui::BeginGroup();
    ImGui::Text(name.data());
    ImGui::SameLine();

    ImGui::PushID(imguiId);

    if (ImGui::Button("Play"))
    {
        anim.Play();
    }

    ImGui::SameLine();

    if (ImGui::Button("Pause"))
    {
        anim.Pause();
    }

    ImGui::SameLine();

    if (ImGui::Button("Stop"))
    {
        anim.Stop();
    }

    ImGui::PopID();

    ImGui::EndGroup();
}

void nador::AnimationTest::OnDebugRender([[maybe_unused]] IRenderer* renderer)
{
    PlayRender("Move animation:", _simpleAnim, 1);
    PlayRender("Image animation:", _imageAnim, 2);
}
