#include "Events.h"
#include "Papyrus.h"

#include <stddef.h>

using namespace SKSE;

namespace {
    void MessageHandler(SKSE::MessagingInterface::Message* a_msg)
    {
        switch (a_msg->type) {
        case SKSE::MessagingInterface::kDataLoaded:
            Events::Register();
            break;
        }
    }

    void InitializeLog()
    {
#ifndef NDEBUG
        auto sink = std::make_shared<spdlog::sinks::msvc_sink_mt>();
#else
        auto path = logger::log_directory();
        if (!path) {
            stl::report_and_fail("Failed to find standard logging directory"sv);
        }

        *path /= fmt::format("{}.log", PluginDeclaration::GetSingleton()->GetName());
        auto sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(path->string(), true);
#endif

#ifndef NDEBUG
        const auto level = spdlog::level::trace;
#else
        const auto level = spdlog::level::info;
#endif

        auto log = std::make_shared<spdlog::logger>("global log"s, std::move(sink));
        log->set_level(level);
        log->flush_on(level);

        spdlog::set_default_logger(std::move(log));
        spdlog::set_pattern("%g(%#): [%^%l%$] %v"s);
    }
}

SKSEPluginLoad(const LoadInterface* skse) {
    InitializeLog();
	auto* plugin = PluginDeclaration::GetSingleton();
	auto version = plugin->GetVersion();
	logger::info("{} {} is loading...", plugin->GetName(), version);

    SKSE::Init(skse);

    const auto messaging = SKSE::GetMessagingInterface();
    if (!messaging->RegisterListener("SKSE", MessageHandler)) {
        return false;
    }

    const auto papyrus = SKSE::GetPapyrusInterface();
    if (!papyrus->Register(Papyrus::Register)) {
        logger::critical("Failed to register papyrus callback"sv);
        return false;
    }

	log::info("{} has finished loading.", plugin->GetName());
    return true;
}
