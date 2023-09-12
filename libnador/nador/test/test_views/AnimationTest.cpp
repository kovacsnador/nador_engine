#include "imgui.h"

#include "nador/test/test_views/AnimationTest.h"
#include "nador/common/GlobalEvents.h"
#include "nador/ui/IUiApp.h"

namespace nv = nador::video;

static nador::ElementSequenceVec_t<nador::UiImage> s_simpleAnim {
    { 40ms, [](auto& img) { img.SetOffset({4, 0}); } },  { 40ms, [](auto& img) { img.SetOffset({8, 0}); } },
    { 40ms, [](auto& img) { img.SetOffset({12, 0}); } }, { 40ms, [](auto& img) { img.SetOffset({16, 0}); } },
    { 40ms, [](auto& img) { img.SetOffset({20, 0}); } }, { 40ms, [](auto& img) { img.SetOffset({16, 0}); } },
    { 40ms, [](auto& img) { img.SetOffset({12, 0}); } }, { 40ms, [](auto& img) { img.SetOffset({8, 0}); } },
    { 40ms, [](auto& img) { img.SetOffset({4, 0}); } },  { 40ms, [](auto& img) { img.SetOffset({0, 0}); } }
};

static nador::ElementSequenceVec_t<nador::UiImage> s_imageAnim {
    { 16ms, [](auto& img) { img.SetImage(nv::EImageName::FEHERFEKETE0001); } },  { 16ms, [](auto& img) { img.SetImage(nv::EImageName::FEHERFEKETE0002); } },
    { 16ms, [](auto& img) { img.SetImage(nv::EImageName::FEHERFEKETE0003); } },  { 16ms, [](auto& img) { img.SetImage(nv::EImageName::FEHERFEKETE0004); } },
    { 16ms, [](auto& img) { img.SetImage(nv::EImageName::FEHERFEKETE0005); } },  { 16ms, [](auto& img) { img.SetImage(nv::EImageName::FEHERFEKETE0006); } },
    { 16ms, [](auto& img) { img.SetImage(nv::EImageName::FEHERFEKETE0007); } },  { 16ms, [](auto& img) { img.SetImage(nv::EImageName::FEHERFEKETE0008); } },
    { 16ms, [](auto& img) { img.SetImage(nv::EImageName::FEHERFEKETE0009); } },  { 16ms, [](auto& img) { img.SetImage(nv::EImageName::FEHERFEKETE0010); } },
    { 16ms, [](auto& img) { img.SetImage(nv::EImageName::FEHERFEKETE0011); } },  { 16ms, [](auto& img) { img.SetImage(nv::EImageName::FEHERFEKETE0012); } },
    { 16ms, [](auto& img) { img.SetImage(nv::EImageName::FEHERFEKETE0013); } }
};

nador::AnimationTest::AnimationTest(IUiApp* uiApp)
: _uiApp(uiApp)
, _simpleAnim{_image, s_simpleAnim, g_onAnimationTickEvent}
, _imageAnim(_image2, s_imageAnim, g_onAnimationTickEvent)
{
    _uiApp->AddElementToLayer(EUiLayer::OVERLAY, &_image);
    _uiApp->AddElementToLayer(EUiLayer::OVERLAY, &_image2);
}

void nador::AnimationTest::OnDebugRender(NADOR_MAYBE_UNUSED IRenderer* renderer)
{
    ImGui::BeginGroup();

    if (ImGui::Button("Play Simple Animation"))
    {
        _simpleAnim.Play();
    }

    if (ImGui::Button("Pause Simple Animation"))
    {
        _simpleAnim.Pause();
    }

    if (ImGui::Button("Stop Simple Animation"))
    {
        _simpleAnim.Stop();
    }

    ImGui::EndGroup();

    ImGui::BeginGroup();

    if (ImGui::Button("Play Image Animation"))
    {
        _imageAnim.Play();
    }

    if (ImGui::Button("Pause Image Animation"))
    {
        _imageAnim.Pause();
    }

    if (ImGui::Button("Stop Image Animation"))
    {
        _imageAnim.Stop();
    }
    ImGui::EndGroup();
}
