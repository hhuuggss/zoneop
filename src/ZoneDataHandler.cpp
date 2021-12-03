#include "ZoneDataHandler.h"

namespace ZoneDataHandler {
	void Reset() {
		logger::info("reset zone changes");
		for (auto& change : changes) {
			auto* zone = static_cast<RE::BGSEncounterZone*>(RE::TESForm::LookupByID(change.first));
			if (zone) {
				RevertZone(zone);
			}
		}
		changes.clear();
	}
	void InitZones() {
		logger::info("get zone min/max defaults from game data");
		auto DataHandler = RE::TESDataHandler::GetSingleton();
		for (auto& zone : DataHandler->GetFormArray<RE::BGSEncounterZone>()) {
			StoreDefault(zone);
		}
	}
	void RevertZone(RE::BGSEncounterZone* zone) {
		auto search = defaults.find(zone->formID);
		if (search != defaults.end()) {
			ZoneData data = search->second;
			zone->data.minLevel = data.minLevel;
			zone->data.maxLevel = data.maxLevel;
		}
	}
	void StoreDefault(RE::BGSEncounterZone* zone) {
		ZoneData data;
		data.minLevel = zone->data.minLevel;
		data.maxLevel = zone->data.maxLevel;
		defaults[zone->formID] = data;
	}
	void StoreChange(RE::BGSEncounterZone* zone) {
		ZoneData data;
		data.minLevel = zone->data.minLevel;
		data.maxLevel = zone->data.maxLevel;
		changes[zone->formID] = data;
	}
	void ApplyChange(RE::BGSEncounterZone* zone, ZoneData& data) {
		zone->data.minLevel = data.minLevel;
		zone->data.maxLevel = data.maxLevel;
	}
	void Save(SKSE::SerializationInterface* intfc) {
		logger::info(FMT_STRING("zone changes to write: {}"), changes.size());
		intfc->WriteRecordData(std::uint32_t(changes.size()));
		for (auto& change : changes) {
			intfc->WriteRecordData(change.first);
			intfc->WriteRecordData(change.second.minLevel);
			intfc->WriteRecordData(change.second.maxLevel);
		}
	}
	void Load(SKSE::SerializationInterface* intfc) {
		std::uint32_t count = 0;
		std::uint32_t resultCount = 0;
		intfc->ReadRecordData(count);
		logger::info(FMT_STRING("zone changes to load: {}"), count);
		for (std::uint32_t i = 0; i < count; i++) {
			std::uint32_t formID;
			std::uint32_t newFormID;
			ZoneData data;
			intfc->ReadRecordData(formID);
			intfc->ReadRecordData(data.minLevel);
			intfc->ReadRecordData(data.maxLevel);
			if (!intfc->ResolveFormID(formID, newFormID)) {
				logger::warn(FMT_STRING("couldn't resolve form {:08x}"), formID);
				continue;
			}
			auto* zone = static_cast<RE::BGSEncounterZone*>(RE::TESForm::LookupByID(newFormID));
			if (zone) {
				resultCount++;
				changes[newFormID] = data;
				ApplyChange(zone, data);
			}
			else {
				logger::warn(FMT_STRING("form {:08x} is not an encounter zone"), newFormID);
			}
		}
		logger::info(FMT_STRING("loaded {}"), resultCount);
	}
}