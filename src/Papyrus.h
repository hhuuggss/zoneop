#pragma once

namespace Papyrus
{
	// big slow array functions
	std::vector<RE::BGSLocation*> GetLocations(RE::StaticFunctionTag*, RE::BGSKeyword* filterKeyword);
	std::vector<RE::BGSEncounterZone*> GetZones(RE::StaticFunctionTag*, RE::BGSKeyword* filterKeyword);

	// locations
	void LocReset(RE::StaticFunctionTag*, RE::BGSLocation* targetLoc, bool resetZone);

	RE::BGSEncounterZone* LocGetEncounterZone(RE::StaticFunctionTag*, RE::BGSLocation* targetLoc);
	RE::BGSLocation* LocGetParent(RE::StaticFunctionTag*, RE::BGSLocation* targetLoc);

	bool LocHasEverBeenCleared(RE::StaticFunctionTag*, RE::BGSLocation* targetLoc);

	// zones
	void ZoneReset(RE::StaticFunctionTag*, RE::BGSEncounterZone* targetZone);

	void ZoneSetMinLevel(RE::StaticFunctionTag*, RE::BGSEncounterZone* targetZone, std::int8_t newLevel);
	void ZoneSetMaxLevel(RE::StaticFunctionTag*, RE::BGSEncounterZone* targetZone, std::int8_t newLevel);
	void ZoneSetLevel(RE::StaticFunctionTag*, RE::BGSEncounterZone* targetZone, std::uint16_t newLevel);
	std::uint32_t ZoneGetMinLevel(RE::StaticFunctionTag*, RE::BGSEncounterZone* targetZone);
	std::uint32_t ZoneGetMaxLevel(RE::StaticFunctionTag*, RE::BGSEncounterZone* targetZone);
	std::uint32_t ZoneGetLevel(RE::StaticFunctionTag*, RE::BGSEncounterZone* targetZone);

	RE::BGSLocation* ZoneGetLocation(RE::StaticFunctionTag*, RE::BGSEncounterZone* targetZone);

	void ZoneSetNeverResets(RE::StaticFunctionTag*, RE::BGSEncounterZone* targetZone, bool state);
	void ZoneSetMatchPC(RE::StaticFunctionTag*, RE::BGSEncounterZone* targetZone, bool state);
	void ZoneSetDisableCombatBoundary(RE::StaticFunctionTag*, RE::BGSEncounterZone* targetZone, bool state);
	bool ZoneGetNeverResets(RE::StaticFunctionTag*, RE::BGSEncounterZone* targetZone);
	bool ZoneGetMatchPC(RE::StaticFunctionTag*, RE::BGSEncounterZone* targetZone);
	bool ZoneGetDisableCombatBoundary(RE::StaticFunctionTag*, RE::BGSEncounterZone* targetZone);

	bool Register(RE::BSScript::IVirtualMachine* vm);
}