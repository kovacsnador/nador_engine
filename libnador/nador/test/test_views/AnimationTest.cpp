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
    { 0ms, [](auto& img) { img.SetImage(nv::EImageName::FEHERFEKETE0001); } },  { 16ms, [](auto& img) { img.SetImage(nv::EImageName::FEHERFEKETE0002); } },
    { 16ms, [](auto& img) { img.SetImage(nv::EImageName::FEHERFEKETE0003); } },  { 16ms, [](auto& img) { img.SetImage(nv::EImageName::FEHERFEKETE0004); } },
    { 16ms, [](auto& img) { img.SetImage(nv::EImageName::FEHERFEKETE0005); } },  { 16ms, [](auto& img) { img.SetImage(nv::EImageName::FEHERFEKETE0006); } },
    { 16ms, [](auto& img) { img.SetImage(nv::EImageName::FEHERFEKETE0007); } },  { 16ms, [](auto& img) { img.SetImage(nv::EImageName::FEHERFEKETE0008); } },
    { 16ms, [](auto& img) { img.SetImage(nv::EImageName::FEHERFEKETE0009); } },  { 16ms, [](auto& img) { img.SetImage(nv::EImageName::FEHERFEKETE0010); } },
    { 16ms, [](auto& img) { img.SetImage(nv::EImageName::FEHERFEKETE0011); } },  { 16ms, [](auto& img) { img.SetImage(nv::EImageName::FEHERFEKETE0012); } },
    { 16ms, [](auto& img) { img.SetImage(nv::EImageName::FEHERFEKETE0013); } },
    { 100ms, [](auto& img) { img.SetImage(nv::EImageName::FEHERFEKETE0012); } },  { 16ms, [](auto& img) { img.SetImage(nv::EImageName::FEHERFEKETE0011); } },
    { 16ms, [](auto& img) { img.SetImage(nv::EImageName::FEHERFEKETE0010); } },  { 16ms, [](auto& img) { img.SetImage(nv::EImageName::FEHERFEKETE0009); } },
    { 16ms, [](auto& img) { img.SetImage(nv::EImageName::FEHERFEKETE0008); } },  { 16ms, [](auto& img) { img.SetImage(nv::EImageName::FEHERFEKETE0007); } },
    { 16ms, [](auto& img) { img.SetImage(nv::EImageName::FEHERFEKETE0006); } },  { 16ms, [](auto& img) { img.SetImage(nv::EImageName::FEHERFEKETE0005); } },
    { 16ms, [](auto& img) { img.SetImage(nv::EImageName::FEHERFEKETE0004); } },  { 16ms, [](auto& img) { img.SetImage(nv::EImageName::FEHERFEKETE0003); } },
    { 16ms, [](auto& img) { img.SetImage(nv::EImageName::FEHERFEKETE0012); } },  { 16ms, [](auto& img) { img.SetImage(nv::EImageName::FEHERFEKETE0001); } },
};

static auto s_test = nador::CreateElemSequence<nador::UiImage>(std::vector {nv::EImageName::FEHERFEKETE0001,
                                                                            nv::EImageName::FEHERFEKETE0002,
                                                                            nv::EImageName::FEHERFEKETE0003,
                                                                            nv::EImageName::FEHERFEKETE0004,
                                                                            nv::EImageName::FEHERFEKETE0005,
                                                                            nv::EImageName::FEHERFEKETE0006,
                                                                            nv::EImageName::FEHERFEKETE0007,
                                                                            nv::EImageName::FEHERFEKETE0008,
                                                                            nv::EImageName::FEHERFEKETE0009,
                                                                            nv::EImageName::FEHERFEKETE0010,
                                                                            nv::EImageName::FEHERFEKETE0011,
                                                                            nv::EImageName::FEHERFEKETE0012,
                                                                            nv::EImageName::FEHERFEKETE0013,
                                                                            nv::EImageName::FEHERFEKETE0012,
                                                                            nv::EImageName::FEHERFEKETE0011,
                                                                            nv::EImageName::FEHERFEKETE0010,
                                                                            nv::EImageName::FEHERFEKETE0009,
                                                                            nv::EImageName::FEHERFEKETE0008,
                                                                            nv::EImageName::FEHERFEKETE0007,
                                                                            nv::EImageName::FEHERFEKETE0006,
                                                                            nv::EImageName::FEHERFEKETE0005,
                                                                            nv::EImageName::FEHERFEKETE0004,
                                                                            nv::EImageName::FEHERFEKETE0003,
                                                                            nv::EImageName::FEHERFEKETE0002,
                                                                            nv::EImageName::FEHERFEKETE0001},
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

void nador::AnimationTest::OnDebugRender(NADOR_MAYBE_UNUSED IRenderer* renderer)
{
    PlayRender("Move animation:", _simpleAnim, 1);
    PlayRender("Image animation:", _imageAnim, 2);
}
