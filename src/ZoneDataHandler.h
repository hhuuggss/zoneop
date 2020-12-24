#pragma once
#include <unordered_map>

namespace ZoneDataHandler
{
	struct ZoneData {
		std::uint8_t minLevel = 0;
		std::uint8_t maxLevel = 0;
	};
	static std::unordered_map<std::uint32_t, ZoneData> defaults;
	static std::unordered_map<std::uint32_t, ZoneData> changes;
	void InitZones();
	void RevertZone(RE::BGSEncounterZone* zone);
	void StoreDefault(RE::BGSEncounterZone* zone);
	void StoreChange(RE::BGSEncounterZone* zone);
	void ApplyChange(RE::BGSEncounterZone* zone, ZoneData& data);
	void Reset();
	void Save(SKSE::SerializationInterface* intfc);
	void Load(SKSE::SerializationInterface* intfc);
}