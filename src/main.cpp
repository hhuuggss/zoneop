#include "ZoneDataHandler.h"
#include "Papyrus.h"

enum : std::uint32_t
{
	PluginID = 'ZNOP',
	ZoneDataID = 'ZNPD',
	DataVersion = 1
};

void SaveCallback(SKSE::SerializationInterface* intfc) {
	intfc->OpenRecord(ZoneDataID, DataVersion);
	ZoneDataHandler::Save(intfc);
}

void LoadCallback(SKSE::SerializationInterface* intfc) {
	std::uint32_t type, ver, length;
	while (intfc->GetNextRecordInfo(type, ver, length)) {
		if (ver > DataVersion) {
			logger::warn("encountered unexpected record version!");
		}
		switch (type) {
		case ZoneDataID:
			ZoneDataHandler::Load(intfc);
			break;
		default:
			logger::warn("encountered unexpected record type!");
			continue;
		}
	}
}

void MessageCallback(SKSE::MessagingInterface::Message* msg) {
	switch (msg->type) {
	case SKSE::MessagingInterface::kDataLoaded:
		ZoneDataHandler::InitZones();
		break;
	case SKSE::MessagingInterface::kPreLoadGame:
	case SKSE::MessagingInterface::kNewGame:
		ZoneDataHandler::Reset();	
	}
}

extern "C" DLLEXPORT bool SKSEAPI SKSEPlugin_Query(const SKSE::QueryInterface* a_skse, SKSE::PluginInfo* a_info)
{

	auto path = logger::log_directory();
	if (!path) return false;
	*path /= "zoneop.log"sv;
	auto sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(path->string(), true);
	
	auto log = std::make_shared<spdlog::logger>("global log"s, std::move(sink));
	log->set_level(spdlog::level::info);

	spdlog::set_default_logger(std::move(log));
	spdlog::set_pattern("[%l] %v"s);

	logger::info("zoneop plugin v2");

	a_info->infoVersion = SKSE::PluginInfo::kVersion;
	a_info->name = "zoneop";
	a_info->version = 2;

	if (a_skse->IsEditor()) {
		logger::critical("Loaded in editor, marking as incompatible"sv);
		return false;
	}
	
	const auto ver = a_skse->RuntimeVersion();
	if (ver < SKSE::RUNTIME_1_5_39) {
		logger::critical(FMT_STRING("Unsupported runtime version {}"), ver.string());
		return false;
	}

	return true;
}

extern "C" DLLEXPORT bool SKSEAPI SKSEPlugin_Load(const SKSE::LoadInterface* a_skse)
{

	SKSE::Init(a_skse);

	auto MessagingInterface = SKSE::GetMessagingInterface();
	if (!MessagingInterface->RegisterListener(MessageCallback)) return false;

	auto PapyrusInterface = SKSE::GetPapyrusInterface();
	if (!PapyrusInterface->Register(Papyrus::Register)) return false;

	auto SerializationInterface = SKSE::GetSerializationInterface();
	SerializationInterface->SetUniqueID(PluginID);
	SerializationInterface->SetLoadCallback(LoadCallback);
	SerializationInterface->SetSaveCallback(SaveCallback);

	logger::info("loaded!");

	return true;
}
