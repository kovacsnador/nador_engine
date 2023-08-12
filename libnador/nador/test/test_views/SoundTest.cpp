#include "imgui.h"

#include "nador/utils/ImGuiHelper.h"
#include "nador/test/test_views/SoundTest.h"
#include "nador/sound/ISoundController.h"
#include "nador/log/ILog.h"

namespace nador
{
	SoundTest::SoundTest(ISoundController* soundCtrl)
	: _soundCtrl(soundCtrl)
	{
		NADOR_ASSERT(_soundCtrl);

		_LoadSounds();
	}

	void SoundTest::OnDebugRender(IRenderer* /*renderer*/)
	{
		static int32_t selectedSound = 0;

		if (ImGui::Button("Reload"))
		{
			_LoadSounds();
		}

		ImGui::SameLine();

		if (ImGui::Button("Stop All Sound"))
		{
			_soundCtrl->StopAllSound();
		}

		ImGui::ListBox("Sounds", &selectedSound,
			[](void* vec, int idx, const char** out_text) -> bool
			{
				vec_t* vector = static_cast<vec_t*>(vec);
				if (idx < 0 || (size_t)idx >= vector->size())
				{
					return false;
				}
				*out_text = vector->at(idx).first.fileName.c_str();
				return true;
			},
			static_cast<void*>(&_sounds), _sounds.size(), 10);


		ISoundSourcePtr soundSource = nullptr;

		if (selectedSound >= 0 && selectedSound < (int32_t)_sounds.size())
		{
			soundSource = _sounds.at(selectedSound).second;
		}

		if (soundSource)
		{
			if (ImGui::Button("Play"))
			{
				soundSource->Play();
			}

			ImGui::SameLine();

			if (ImGui::Button("Pause"))
			{
				soundSource->Pause();
			}

			ImGui::SameLine();

			if (ImGui::Button("Stop"))
			{
				soundSource->Stop();
			}

			ImGui::SameLine();

			if (ImGui::Button("Looping"))
			{
				soundSource->SetLooping(true);
				soundSource->Play();
			}

			ImGui::SameLine();

			if (ImGui::Button("Rewind"))
			{
				soundSource->Rewind();
			}

			ImGui::SameLine();

			if (ImGui::Button("Play by ID"))
			{
				_soundCtrl->PlaySound(soundSource->GetSoundId());
			}

			glm::vec3 position = soundSource->GetPosition();

			ImGui::SliderFloat("Translation X", &position.x, -20, 20);
			ImGui::SliderFloat("Translation Y", &position.y, -20, 20);
			ImGui::SliderFloat("Translation Z", &position.z, -20, 20);

			soundSource->SetPosition(position);
		}
	}

	void SoundTest::_LoadSounds()
	{
		_sounds.clear();

		for (auto& it : _soundCtrl->GetAllSoundData())
		{
			ISoundSourcePtr soundSource = _soundCtrl->CreateSoundSource(it.soundId);
			
			_sounds.push_back(std::make_pair(it, soundSource));
		}
	}
}