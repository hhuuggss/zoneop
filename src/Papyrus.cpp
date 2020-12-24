#include "Papyrus.h"
#include "ZoneDataHandler.h"

std::vector<RE::BGSLocation*> Papyrus::GetLocations(RE::StaticFunctionTag*, RE::BGSKeyword* filterKeyword)
{
	std::vector<RE::BGSLocation*> result;
	auto DataHandler = RE::TESDataHandler::GetSingleton();
	for (auto& loc : DataHandler->GetFormArray<RE::BGSLocation>()) {
		if (filterKeyword) {
			if (!loc->HasKeyword(filterKeyword)) continue;
		}
		result.push_back(loc);
	}
	return result;
}

std::vector<RE::BGSEncounterZone*> Papyrus::GetZones(RE::StaticFunctionTag*, RE::BGSKeyword* filterKeyword)
{
	std::vector<RE::BGSEncounterZone*> result;
	auto DataHandler = RE::TESDataHandler::GetSingleton();
	for (auto& zone : DataHandler->GetFormArray<RE::BGSEncounterZone>()) {
		if (filterKeyword && zone->data.location) {
			if (!zone->data.location->HasKeyword(filterKeyword)) continue;
		}
		result.push_back(zone);
	}
	return result;
}

void Papyrus::LocReset(RE::StaticFunctionTag* tag, RE::BGSLocation* targetLoc, bool resetZone)
{
	if (!targetLoc) return;
	auto DataHandler = RE::TESDataHandler::GetSingleton();
	for (auto& interiorCell : DataHandler->interiorCells) {
		RE::ExtraLocation* extraLoc = interiorCell->extraList.GetByType<RE::ExtraLocation>();
		if (extraLoc && (extraLoc->location == targetLoc)) {
			RE::ExtraDetachTime* detachTime = interiorCell->extraList.GetByType<RE::ExtraDetachTime>();
			if (detachTime) {
				detachTime->time = 0xffffffff;
			}
		}
	}
	if (resetZone) {
		RE::BGSEncounterZone* zone = LocGetEncounterZone(tag, targetLoc);
		if (zone) {
			ZoneReset(tag, zone);
		}
	}
}

RE::BGSEncounterZone* Papyrus::LocGetEncounterZone(RE::StaticFunctionTag*, RE::BGSLocation* targetLoc)
{
	auto DataHandler = RE::TESDataHandler::GetSingleton();
	for (auto& zone : DataHandler->GetFormArray<RE::BGSEncounterZone>()) {
		if (zone->data.location == targetLoc) {
			return(zone);
		}
	}
	return nullptr;
}

RE::BGSLocation* Papyrus::LocGetParent(RE::StaticFunctionTag*, RE::BGSLocation* targetLoc)
{
	return targetLoc ? targetLoc->parentLoc : nullptr;
}

bool Papyrus::LocHasEverBeenCleared(RE::StaticFunctionTag*, RE::BGSLocation* targetLoc)
{
	return targetLoc ? targetLoc->everCleared : false;
}

void Papyrus::ZoneReset(RE::StaticFunctionTag*, RE::BGSEncounterZone* targetZone)
{
	if (!targetZone) return;
	targetZone->gameData.attachTime = 0;
	targetZone->gameData.detachTime = 0;
	targetZone->gameData.resetTime = 0;
	targetZone->gameData.zoneLevel = 0;
	targetZone->RemoveChange(RE::BGSEncounterZone::ChangeFlags::kGameData);
}

void Papyrus::ZoneSetMinLevel(RE::StaticFunctionTag*, RE::BGSEncounterZone* targetZone, std::int8_t newLevel)
{
	if (!targetZone) return;
	targetZone->data.minLevel = newLevel;
	ZoneDataHandler::StoreChange(targetZone);
}

std::uint32_t Papyrus::ZoneGetMinLevel(RE::StaticFunctionTag*, RE::BGSEncounterZone* targetZone)
{
	return targetZone ? targetZone->data.minLevel : 0;
}

void Papyrus::ZoneSetMaxLevel(RE::StaticFunctionTag*, RE::BGSEncounterZone* targetZone, std::int8_t newLevel)
{
	if (!targetZone) return;
	targetZone->data.maxLevel = newLevel;
	ZoneDataHandler::StoreChange(targetZone);
}

std::uint32_t Papyrus::ZoneGetMaxLevel(RE::StaticFunctionTag*, RE::BGSEncounterZone* targetZone)
{
	return targetZone ? targetZone->data.maxLevel : 0;
}

void Papyrus::ZoneSetLevel(RE::StaticFunctionTag*, RE::BGSEncounterZone* targetZone, std::uint16_t newLevel)
{
	if (!targetZone) return;
	targetZone->gameData.zoneLevel = newLevel;
	targetZone->AddChange(RE::BGSEncounterZone::ChangeFlags::kGameData);
}

std::uint32_t Papyrus::ZoneGetLevel(RE::StaticFunctionTag*, RE::BGSEncounterZone* targetZone)
{
	return targetZone ? targetZone->gameData.zoneLevel : 0;
}

RE::BGSLocation* Papyrus::ZoneGetLocation(RE::StaticFunctionTag*, RE::BGSEncounterZone* targetZone)
{
	if (!targetZone) return nullptr;
	return targetZone->data.location;
}

void Papyrus::ZoneSetNeverResets(RE::StaticFunctionTag*, RE::BGSEncounterZone* targetZone, bool state)
{
	if (!targetZone) return;
	if (state) targetZone->data.flags.set(RE::ENCOUNTER_ZONE_DATA::Flag::kNeverResets);
	else targetZone->data.flags.reset(RE::ENCOUNTER_ZONE_DATA::Flag::kNeverResets);
	targetZone->AddChange(RE::BGSEncounterZone::ChangeFlags::kZoneFlags);
}

void Papyrus::ZoneSetMatchPC(RE::StaticFunctionTag*, RE::BGSEncounterZone* targetZone, bool state)
{
	if (!targetZone) return;
	if (state) targetZone->data.flags.set(RE::ENCOUNTER_ZONE_DATA::Flag::kMatchPCBelowMinimumLevel);
	else targetZone->data.flags.reset(RE::ENCOUNTER_ZONE_DATA::Flag::kMatchPCBelowMinimumLevel);
	targetZone->AddChange(RE::BGSEncounterZone::ChangeFlags::kZoneFlags);
}

void Papyrus::ZoneSetDisableCombatBoundary(RE::StaticFunctionTag*, RE::BGSEncounterZone* targetZone, bool state)
{
	if (!targetZone) return;
	if (state) targetZone->data.flags.set(RE::ENCOUNTER_ZONE_DATA::Flag::kDisableCombatBoundary);
	else targetZone->data.flags.reset(RE::ENCOUNTER_ZONE_DATA::Flag::kDisableCombatBoundary);
	targetZone->AddChange(RE::BGSEncounterZone::ChangeFlags::kZoneFlags);
}

bool Papyrus::ZoneGetNeverResets(RE::StaticFunctionTag*, RE::BGSEncounterZone* targetZone)
{
	if (!targetZone) return false;
	return (targetZone->data.flags & RE::ENCOUNTER_ZONE_DATA::Flag::kNeverResets) != RE::ENCOUNTER_ZONE_DATA::Flag::kNone;
}

bool Papyrus::ZoneGetMatchPC(RE::StaticFunctionTag*, RE::BGSEncounterZone* targetZone)
{
	if (!targetZone) return false;
	return (targetZone->data.flags & RE::ENCOUNTER_ZONE_DATA::Flag::kMatchPCBelowMinimumLevel) != RE::ENCOUNTER_ZONE_DATA::Flag::kNone;
}

bool Papyrus::ZoneGetDisableCombatBoundary(RE::StaticFunctionTag*, RE::BGSEncounterZone* targetZone)
{
	if (!targetZone) return false;
	return (targetZone->data.flags & RE::ENCOUNTER_ZONE_DATA::Flag::kDisableCombatBoundary) != RE::ENCOUNTER_ZONE_DATA::Flag::kNone;
}

bool Papyrus::Register(RE::BSScript::IVirtualMachine* vm)
{
	vm->RegisterFunction("GetLocations", "ZoneOp", GetLocations);
	vm->RegisterFunction("GetZones", "ZoneOp", GetZones);

	vm->RegisterFunction("LocReset", "ZoneOp", LocReset);
	vm->RegisterFunction("LocGetEncounterZone", "ZoneOp", LocGetEncounterZone);
	vm->RegisterFunction("LocGetParent", "ZoneOp", LocGetParent);
	vm->RegisterFunction("LocHasEverBeenCleared", "ZoneOp", LocHasEverBeenCleared);

	vm->RegisterFunction("ZoneReset", "ZoneOp", ZoneReset);
	vm->RegisterFunction("ZoneSetMinLevel", "ZoneOp", ZoneSetMinLevel);
	vm->RegisterFunction("ZoneGetMinLevel", "ZoneOp", ZoneGetMinLevel);
	vm->RegisterFunction("ZoneSetMaxLevel", "ZoneOp", ZoneSetMaxLevel);
	vm->RegisterFunction("ZoneGetMaxLevel", "ZoneOp", ZoneGetMaxLevel);
	vm->RegisterFunction("ZoneSetLevel", "ZoneOp", ZoneSetLevel);
	vm->RegisterFunction("ZoneGetLevel", "ZoneOp", ZoneGetLevel);
	vm->RegisterFunction("ZoneGetLocation", "ZoneOp", ZoneGetLocation);
	vm->RegisterFunction("ZoneSetNeverResets", "ZoneOp", ZoneSetNeverResets);
	vm->RegisterFunction("ZoneSetMatchPC", "ZoneOp", ZoneSetMatchPC);
	vm->RegisterFunction("ZoneSetDisableCombatBoundary", "ZoneOp", ZoneSetDisableCombatBoundary);
	vm->RegisterFunction("ZoneGetNeverResets", "ZoneOp", ZoneGetNeverResets);
	vm->RegisterFunction("ZoneGetMatchPC", "ZoneOp", ZoneGetMatchPC);
	vm->RegisterFunction("ZoneGetDisableCombatBoundary", "ZoneOp", ZoneGetDisableCombatBoundary);

	logger::info("papyrus functions registered");
	return true;
}