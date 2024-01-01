#pragma once

namespace Papyrus
{
    class ConsoleUtil
    {
    public:
        enum
        {
            kVersion = 4
        };

        static inline void ExecuteCommand(RE::StaticFunctionTag*, std::string a_command)
        {
            const auto scriptFactory = RE::IFormFactory::GetConcreteFormFactoryByType<RE::Script>();
            const auto script = scriptFactory ? scriptFactory->Create() : nullptr;
            if (script) {
                const auto selectedRef = GetSelectedRef();
                script->SetCommand(a_command);
                //script->CompileAndRun(selectedRef.get());
				CompileAndRun(script, selectedRef.get());
                delete script;
            }
        }

        static inline RE::TESObjectREFR* GetSelectedReference(RE::StaticFunctionTag*)
        {
            const auto selectedRef = GetSelectedRef();
            return selectedRef.get();
        }

        static inline void SetSelectedReference(RE::StaticFunctionTag*, RE::TESObjectREFR* a_reference)
        {
            using Message = RE::UI_MESSAGE_TYPE;

            if (a_reference) {
                const auto factory = RE::MessageDataFactoryManager::GetSingleton();
                const auto intfcStr = RE::InterfaceStrings::GetSingleton();
                const auto creator =
                    factory && intfcStr ?
                          factory->GetCreator<RE::ConsoleData>(intfcStr->consoleData) :
                          nullptr;

                const auto consoleData = creator ? creator->Create() : nullptr;
                const auto msgQ = RE::UIMessageQueue::GetSingleton();
                if (consoleData && msgQ) {
                    consoleData->type = static_cast<RE::ConsoleData::DataType>(1);
                    consoleData->pickRef = a_reference->CreateRefHandle();
                    msgQ->AddMessage(intfcStr->console, Message::kUpdate, consoleData);
                }
            } else {
                const auto ui = RE::UI::GetSingleton();
                const auto console = ui ? ui->GetMenu<RE::Console>() : nullptr;
                if (console) {
                    const RE::ObjectRefHandle null;
                    console->SetSelectedRef(null);
                }
            }
        }

        static inline std::string ReadMessage(RE::StaticFunctionTag*)
        {
            auto consoleManager = RE::ConsoleLog::GetSingleton();
            return consoleManager ? consoleManager->lastMessage : ""s;
        }

        static inline void PrintMessage(RE::StaticFunctionTag*, std::string a_message)
        {
            const auto log = RE::ConsoleLog::GetSingleton();
            if (log) {
                log->Print(a_message.c_str());
            }
        }

        static inline std::int32_t GetVersion(RE::StaticFunctionTag*)
        {
            return kVersion;
        }

        static inline bool Register(RE::BSScript::IVirtualMachine* a_vm)
        {
            a_vm->RegisterFunction("ExecuteCommand", CLASS_NAME, ExecuteCommand);
            a_vm->RegisterFunction("GetSelectedReference", CLASS_NAME, GetSelectedReference);
            a_vm->RegisterFunction("SetSelectedReference", CLASS_NAME, SetSelectedReference);
            a_vm->RegisterFunction("ReadMessage", CLASS_NAME, ReadMessage);
            a_vm->RegisterFunction("PrintMessage", CLASS_NAME, PrintMessage);
            a_vm->RegisterFunction("GetVersion", CLASS_NAME, GetVersion);

            logger::info("Registered funcs for class {}", CLASS_NAME);

            return true;
        }

    private:
        static constexpr char CLASS_NAME[] = "ConsoleUtil";

        static inline void CompileAndRun(RE::Script* script, RE::TESObjectREFR* targetRef, RE::COMPILER_NAME name = RE::COMPILER_NAME::kSystemWindowCompiler)
		{
			RE::ScriptCompiler compiler;
			CompileAndRunImpl(script, &compiler, name, targetRef);
        }

        static inline void CompileAndRunImpl(RE::Script* script, RE::ScriptCompiler* compiler, RE::COMPILER_NAME name, RE::TESObjectREFR* targetRef) {
			using func_t = decltype(CompileAndRunImpl);
			REL::Relocation<func_t> func{ RELOCATION_ID(21416, REL::Module::get().version().patch() < 1130 ? 21890 : 441582) };
			return func(script, compiler, name, targetRef);
        }

        static inline RE::NiPointer<RE::TESObjectREFR> GetSelectedRef() {
			auto handle = GetSelectedRefHandle();
			return handle.get();
        }

        static inline RE::ObjectRefHandle GetSelectedRefHandle() {
			REL::Relocation<RE::ObjectRefHandle*> selectedRef{ RELOCATION_ID(519394, REL::Module::get().version().patch() < 1130 ? 405935 : 504099) };
			return *selectedRef;
        }
    };

    inline bool Register(RE::BSScript::IVirtualMachine* a_vm)
    {
        ConsoleUtil::Register(a_vm);
        return true;
    }
}
