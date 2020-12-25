ScriptName ZoneOp Hidden

;========== locations ========================================================

; reset interior cells associated with targetLoc (when they are next loaded)
; optionally reset the encounter zone associated with targetLoc (see ZoneReset)
Function LocReset(Location targetLoc, Bool resetZone = false) global native

; return the encounter zone associated with targetLoc
EncounterZone Function LocGetEncounterZone(Location targetLoc) global native

; both functions assume any given location only has a single encounter zone pointing to it

; get the immediate parent of the supplied location
Location Function LocGetParent(Location targetLoc) global native

; check if the location has ever been cleared (vs Location.IsCleared checking the immediate cleared status)
bool Function LocHasEverBeenCleared(Location targetLoc) global native

;========== encounter zones ==================================================

Location Function ZoneGetLocation(EncounterZone targetZone) global native

Function ZoneSetMinLevel(EncounterZone targetZone, int newLevel) global native
Int Function ZoneGetMinLevel(EncounterZone targetZone) global native

Function ZoneSetMaxLevel(EncounterZone targetZone, int newLevel) global native
Int Function ZoneGetMaxLevel(EncounterZone targetZone) global native

; set lock level of zone (the level that is set when the player enters a cell with this zone)
Function ZoneSetLevel(EncounterZone targetZone, int newLevel) global native

Int Function ZoneGetLevel(EncounterZone targetZone) global native

; manipulate zone flags
Function ZoneSetNeverResets(EncounterZone targetZone, bool newState) global native
Function ZoneSetMatchPC(EncounterZone targetZone, bool newState) global native
Function ZoneSetDisableCombatBoundary(EncounterZone targetZone, bool newState) global native

bool Function ZoneGetNeverResets(EncounterZone targetZone) global native
bool Function ZoneGetMatchPC(EncounterZone targetZone) global native
bool Function ZoneGetDisableCombatBoundary(EncounterZone targetZone) global native

; this sets the zone's level to 0 and resets the last visit timestamp(?)
; unsure whether this is functionally any different to just setting the level to 0
; this might also cause weird respawn behaviour?
Function ZoneReset(EncounterZone targetZone) global native

;========== array stuff ======================================================

; returns an array of locations, optionally filtered by keyword
Location[] Function GetLocations(Keyword filterKeyword = none) global native

; returns an array of encounter zones, optionally checking each zone's location for the filter keyword
EncounterZone[] Function GetZones(Keyword filterKeyword = none) global native
