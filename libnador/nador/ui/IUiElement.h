#ifndef __I_UI_ELEMENT_H__
#define __I_UI_ELEMENT_H__

#include <functional>
#include <list>

#include "nador/ui/UiCommon.h"
#include "nador/common/Mouse.h"
#include "nador/common/KeyCodes.h"
#include "nador/utils/NonCopyable.h"
#include "nador/ui/UiEdgeDrawer.h"

namespace nador
{
	class IRenderer;
	class IUiLogicState;
	class IUiApp;

	class IUiElement;
	CREATE_PTR_TYPES(IUiElement);

	class IUiElement : private NonCopyable
	{
	public:
		using ui_element_list_t = std::list<IUiElement*>;

		using OnMousePressedCallback_t = std::function<bool(EMouseButton, const glm::vec2&, IUiElement*)>;
		using OnMouseReleasedCallback_t = std::function<bool(EMouseButton, const glm::vec2&, IUiElement*)>;

		using OnKeyPressedCallback_t = std::function<bool(EKeyCode)>;
		using OnKeyHoldedCallback_t = std::function<bool(EKeyCode)>;
		using OnKeyReleasedCallback_t = std::function<bool(EKeyCode)>;

		/*!
		 * IUiElement default constructor.
		 */
		IUiElement() = default;

		/*!
		 * IUiElement constructor.
		 * 
		 * \param position		The ui element position
		 * \param size			The ui element size
		 * \param alignment		The alignment to parent
		 * \param parent		The parent ui element
		 * \param isShow		Set the show flag.
		 */
		IUiElement(const glm::ivec2& position, const glm::ivec2& size, UiAlignment alignment, IUiElement* parent = nullptr, bool isShow = true);

	protected:
		/*!
		 * IUiElement destructor (protected).
		 */
		virtual ~IUiElement();

	public:
		/*!
		 * Shows the ui element.
		 */
		void Show();
		
		/*!
		 * Hides the ui element.
		 */
		void Hide();

		/*!
		 * Gets if the ui element is showing.
		 * 
		 * \return True if showing false otherwise.
		 */
		bool IsShow() const noexcept;

		bool IsHide() const noexcept;

		/*!
		 * Gets the ui element size.
		 *
		 * \return	The size.
		 */
		const glm::ivec2& GetSize() const noexcept;

		/*!
		 * Sets the ui element size.
		 *
		 * \param size	The new size.
		 */
		void SetSize(const glm::ivec2& size);

		void SetSize(int32_t width, int32_t height);

		void SetWidth(int32_t width);

		void SetHeight(int32_t height);

		/*!
		 * Gets the ui element position.
		 *
		 * \return	The position.
		 */
		const glm::ivec2& GetPosition() const noexcept;

		/*!
		 * Sets the ui element position.
		 *
		 * \param position	The new position.
		 */
		void SetPosition(const glm::ivec2& position);

		/*!
		 * Gets the ui element alignment.
		 *
		 * \return	The alignment.
		 */
		const UiAlignment& GetAligner() const noexcept;

		/*!
		 * Sets the ui element orientation.
		 *
		 * \param alignment	The new alignment.
		 */
		void SetAlignment(const UiAlignment& aligner);

		/*!
		 * Gets the parent ui element.
		 *
		 * \return	The parent ui element.
		 */
		IUiElement* GetParent() const noexcept;

		void BringToFront();
		void PushToBack();

		void PushToBack(IUiElement* elem);

		/*!
		 * Sets the parent ui element.
		 *
		 * \param elem	The parent ui element.
		 */
		void SetParent(IUiElement* elem);

		/*!
		 * Add children ui element.
		 *
		 * \param elem	The child ui element.
		 */
		void AddChild(IUiElement* elem);

		/*!
		 * Remove children ui element.
		 *
		 * \param elem	The child ui element.
		 */
		void RemoveChild(IUiElement* elem);

		void SuspendInputEvents(bool suspend);
		void SuspendInputEventsForChildrens(bool suspend);

		void HandleInputEventBeforeChildren(bool beforeChildren);
		void SetDefaultMouseHandling(bool handled);

		void SetScale(const glm::vec3& scale);
		const glm::vec3& GetScale() const noexcept;

		void OnTickImpl(IUiLogicState* uiLogicState);
		void OnRenderImpl(IRenderer* renderer, bool drawDebugEdge);

		bool OnMousePressedImpl(EMouseButton mouseButton, const glm::vec2& position);

		bool OnMouseReleasedImpl(EMouseButton mouseButton, const glm::vec2& position);

		bool OnKeyPressedImpl(EKeyCode keyCode);

		bool OnKeyHoldedImpl(EKeyCode keyCode);

		bool OnKeyReleasedImpl(EKeyCode keyCode);

		bool OnCharImpl(const std::string& text);

		const quadVertices_t& GetVertices() const noexcept;

		void SetOffset(const glm::ivec2& offset);
		const glm::ivec2& GetOffset() const noexcept;

		void UpdateVertices(const quadVertices_t& parentVertices);

		void SetOnMousePressedCallback(const OnMousePressedCallback_t& cb);
		void SetOnMouseReleasedCallback(const OnMouseReleasedCallback_t& cb);

		void SetOnKeyPressedCallback(const OnKeyPressedCallback_t& cb);
		void SetOnKeyHoldedCallback(const OnKeyHoldedCallback_t& cb);
		void SetOnKeyReleasedCallback(const OnKeyReleasedCallback_t& cb);

		bool IsOver(const glm::vec2& point) const noexcept;

		const std::string& GetName() const noexcept;
		void SetName(const std::string& name);

		void SetUiAppHandler(IUiApp* uiApp) noexcept;

	protected:
		virtual void OnTick(IUiLogicState* /*uiLogicState*/, bool /*mouseOver*/) {};
		virtual void OnRender(IRenderer* /*renderer*/, const quadVertices_t& /*vertices*/) {};
		virtual void OnRenderEnd(IRenderer* /*renderer*/) {};

		virtual void OnMousePressed(EMouseButton, const glm::vec2&) {};
		virtual void OnMouseReleased(EMouseButton, const glm::vec2&) {};

		virtual bool OnCharPressed(const std::string& /*text*/) { return false; };

		virtual bool OnKeyPressed(EKeyCode /*keyCode*/) { return false; };
		virtual bool OnKeyHolded(EKeyCode /*keyCode*/) { return false; };
		virtual bool OnKeyReleased(EKeyCode /*keyCode*/) { return false; };

	private:
		bool OnMousePressedWithPositionCheck(EMouseButton mouseButton, const glm::vec2& position);
		bool OnMouseReleasedWithPositionCheck(EMouseButton mouseButton, const glm::vec2& position);

		bool OnKeyPressedLogic(EKeyCode keyCode);
		bool OnKeyHoldedLogic(EKeyCode keyCode);
		bool OnKeyReleasedLogic(EKeyCode keyCode);

		template<typename ...Params, typename ...Args>
		bool OnKeyAndMouseEventImpl(bool(IUiElement::* onEventImpl)(Params...), bool(IUiElement::* onEvent)(Params...), Args... args)
		{
			if (_inputEventSuspended && IsShow())
			{
				return false;
			}

			if (_handleInputEventBeforeChildren)
			{
				if ((this->*onEvent)(args...))
				{
					return true;
				}
			}

			// handle input by children
			for (auto it = _childrens.rbegin(); it != _childrens.rend(); ++it)
			{
				NADOR_ASSERT(*it);

				if (((*it)->*onEventImpl)(args...))
				{
					return true;
				}
			}

			if (_handleInputEventBeforeChildren == false)
			{
				return (this->*onEvent)(args...);
			}

			return false;
		}

		template<typename CallbackTy, typename ...Args>
		bool InvokeCallback(const CallbackTy& cb, bool defHandled, Args ...args)
		{
			if (cb)
			{
				return cb(args...);
			}
			return defHandled;
		}

	protected:
		IUiElement*		  _parent;
		ui_element_list_t _childrens;

		UiAlignment _alignment;

		bool _isShow{ true };

		quadVertices_t  _vertices;

		glm::ivec2 _position{ 0, 0 };
		glm::ivec2 _size{ 0, 0 };
		glm::vec3  _scale{ 1, 1, 1 };
		glm::ivec2 _offset{ 0, 0 };

		std::string _name;

	private:
		OnMousePressedCallback_t  _onMousePressedCb{ nullptr };
		OnMouseReleasedCallback_t _onMouseReleasedCb{ nullptr };

		OnKeyPressedCallback_t  _onKeyPressedCb{ nullptr };
		OnKeyHoldedCallback_t   _onKeyHoldedCb{ nullptr };
		OnKeyReleasedCallback_t _onKeyReleasedCb{ nullptr };

		bool _inputEventSuspended{ false };
		bool _handleInputEventBeforeChildren{ false };
		bool _defaultMouseHandling{ true };

		UiEdgeDrawer _edgeDrawer;
		IUiApp*		_uiApp {nullptr};
	};
}

#endif // !__I_UI_ELEMENT_H__