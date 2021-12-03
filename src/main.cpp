#include "ZoneDataHandler.h"
#include "Papyrus.h"

enum : std::uint32_t
{
	PluginID = 'ZNOP',
	ZoneDataID = 'ZNPD',
	DataVersion = 1
};

void SaveCallback(SKSE::SerializationInterface* intfc) {
	if (!intfc->OpenRecord(ZoneDataID, DataVersion)) {
		logger::warn("couldn't open zonedata record to save to");
		return;
	}
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

extern "C" __declspec(dllexport) constinit auto SKSEPlugin_Version = []() 
{
	SKSE::PluginVersionData v{};
	v.PluginVersion(Version::MAJOR);
	v.PluginName(Version::PROJECT);
	v.AuthorName("hhuuggss"sv);
	v.UsesAddressLibrary(true);
	v.CompatibleVersions({ SKSE::RUNTIME_LATEST });
	return v;
}();

extern "C" DLLEXPORT bool SKSEAPI SKSEPlugin_Load(const SKSE::LoadInterface* a_skse)
{

	auto path = logger::log_directory();
	if (!path) {
		return false;
	}

	*path /= Version::PROJECT;
	*path += ".log"sv;
	auto sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(path->string(), true);

	auto log = std::make_shared<spdlog::logger>("global log"s, std::move(sink));

	log->set_level(spdlog::level::info);
	log->flush_on(spdlog::level::info);

	spdlog::set_default_logger(std::move(log));
	spdlog::set_pattern("[%l] %v"s);

	logger::info(FMT_STRING("{} v{} loaded"), Version::PROJECT, Version::NAME);

	SKSE::Init(a_skse);

	auto MessagingInterface = SKSE::GetMessagingInterface();
	if (!MessagingInterface->RegisterListener(MessageCallback)) return false;

	auto PapyrusInterface = SKSE::GetPapyrusInterface();
	if (!PapyrusInterface->Register(Papyrus::Register)) return false;

	auto SerializationInterface = SKSE::GetSerializationInterface();
	SerializationInterface->SetUniqueID(PluginID);
	SerializationInterface->SetLoadCallback(LoadCallback);
	SerializationInterface->SetSaveCallback(SaveCallback);

	logger::info(FMT_STRING("{} ready"), Version::PROJECT);

	return true;
}
