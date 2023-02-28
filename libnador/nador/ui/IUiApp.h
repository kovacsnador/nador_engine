#ifndef __NADOR_I_UI_APP_H__
#define __NADOR_I_UI_APP_H__

#include "nador/utils/Types.h"
#include "nador/ui/UiCommon.h"
#include "nador/ui/IUiElement.h"

namespace nador
{
	class IUiApp
	{
	public:
		using ui_element_list_t = std::list<IUiElement*>;
		using ui_layer_list_t = std::map<EUiLayer, ui_element_list_t>;

		virtual ~IUiApp() = default;

		virtual const glm::ivec2& GetScreenSize() const = 0;
		virtual float_t GetScreenScale() const = 0;
		virtual float_t GetAspectRatio() const = 0;
		virtual const std::string& GetUiRootFolder() const = 0;
		
		virtual void BringToFront(IUiElement* elem) = 0;
		virtual void PushToback(IUiElement* elem) = 0;

		virtual quadVertices_t GetScreenVertices() const = 0;

		virtual void AddElementToLayer(EUiLayer layer, IUiElement* elem) = 0;
		virtual void RemoveElementFromLayer(EUiLayer layer, const IUiElement* elem) = 0;
		virtual void RemoveElement(const IUiElement* elem) = 0;
		virtual void ClearLayer(EUiLayer layer) = 0;

		virtual void DebugDrawEdge(bool draw) = 0;
		virtual bool IsDebugDrawEdge() const noexcept = 0;

		virtual bool IsInFocus(const IUiElement* elem) const noexcept = 0;
		virtual void SetInFocus(const IUiElement* elem) const noexcept = 0;
	};
	CREATE_PTR_TYPES(IUiApp);
}

#endif // !__NADOR_I_UI_APP_H__
