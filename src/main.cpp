#include "ZoneDataHandler.h"
#include "Papyrus.h"

using namespace SKSE;
using namespace SKSE::log;

enum : std::uint32_t
{
	PluginID = 'ZNOP',
	ZoneDataID = 'ZNPD',
	DataVersion = 1
};

void SaveCallback(SerializationInterface* intfc) {
	if (!intfc->OpenRecord(ZoneDataID, DataVersion)) {
		logger::warn("couldn't open zonedata record to save to");
		return;
	}
	ZoneDataHandler::Save(intfc);
}

void LoadCallback(SerializationInterface* intfc) {
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

void RevertCallback(SerializationInterface* intfc) {
	ZoneDataHandler::Reset();
}

void MessageCallback(MessagingInterface::Message* msg) {
	if (msg->type == MessagingInterface::kDataLoaded) {
		ZoneDataHandler::InitZones();
	}
}

SKSEPluginLoad(LoadInterface* skse)
{

	auto path = logger::log_directory();
	if (!path) {
		return false;
	}

	*path /= PluginDeclaration::GetSingleton()->GetName();
	*path += ".log"sv;
	std::shared_ptr<spdlog::logger> log;
	log = std::make_shared<spdlog::logger>("Global", std::make_shared<spdlog::sinks::basic_file_sink_mt>(path->string(), true));
	
	log->set_level(spdlog::level::info);
	log->flush_on(spdlog::level::info);

	spdlog::set_default_logger(std::move(log));
	spdlog::set_pattern("[%l] %v"s);

	logger::info(FMT_STRING("{} v{} loaded"), PluginDeclaration::GetSingleton()->GetName(), PluginDeclaration::GetSingleton()->GetVersion());

	Init(skse);

	auto MessagingInterface = GetMessagingInterface();
	if (!MessagingInterface->RegisterListener(MessageCallback))
		return false;

	auto PapyrusInterface = GetPapyrusInterface();
	if (!PapyrusInterface->Register(Papyrus::Register))
		return false;

	auto SerializationInterface = GetSerializationInterface();
	SerializationInterface->SetUniqueID(PluginID);
	SerializationInterface->SetLoadCallback(LoadCallback);
	SerializationInterface->SetRevertCallback(RevertCallback);
	SerializationInterface->SetSaveCallback(SaveCallback);

	logger::info(FMT_STRING("{} ready"), PluginDeclaration::GetSingleton()->GetName());

	return true;
}