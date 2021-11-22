#pragma once

namespace Events
{
	class MenuOpenCloseEventHandler :
		public RE::BSTEventSink<RE::MenuOpenCloseEvent>
	{
	public:
		MenuOpenCloseEventHandler(const volatile MenuOpenCloseEventHandler&) = delete;
		MenuOpenCloseEventHandler& operator=(const volatile MenuOpenCloseEventHandler&) = delete;

		static inline MenuOpenCloseEventHandler* GetSingleton()
		{
			static MenuOpenCloseEventHandler singleton;
			return std::addressof(singleton);
		}

		static inline void Register()
		{
			const auto ui = RE::UI::GetSingleton();
			if (ui) {
				ui->AddEventSink(GetSingleton());
				logger::debug("Registered menu open/close handler"sv);
			}
		}

	protected:
		using EventResult = RE::BSEventNotifyControl;

		inline EventResult ProcessEvent(const RE::MenuOpenCloseEvent* a_event, RE::BSTEventSource<RE::MenuOpenCloseEvent>*) override
		{
			using Message = RE::UI_MESSAGE_TYPE;

			const auto intfcStr = RE::InterfaceStrings::GetSingleton();
			if (a_event->menuName == intfcStr->mainMenu) {
				const auto msgQ = RE::UIMessageQueue::GetSingleton();
				msgQ->AddMessage(intfcStr->console, Message::kShow, nullptr);
			} else if (a_event->menuName == intfcStr->console) {
				const auto msgQ = RE::UIMessageQueue::GetSingleton();
				msgQ->AddMessage(intfcStr->console, Message::kHide, nullptr);
				const auto ui = RE::UI::GetSingleton();
				ui->RemoveEventSink(this);
			}

			return EventResult::kContinue;
		}

	private:
		MenuOpenCloseEventHandler() = default;
	};

	inline void Register()
	{
		MenuOpenCloseEventHandler::Register();
	}
}
