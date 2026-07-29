#define MAP_ANDREAS_MODE_NONE			0
#define MAP_ANDREAS_MODE_MINIMAL		1
#define MAP_ANDREAS_MODE_FULL			2
#define MAP_ANDREAS_MODE_NOBUFFER		3

#define MAP_ANDREAS_ERROR_SUCCESS		0
#define MAP_ANDREAS_ERROR_FAILURE		1
#define MAP_ANDREAS_ERROR_MEMORY		2
#define MAP_ANDREAS_ERROR_DATA_FILES	3

native MapAndreas_Init(mode, const name[]="", len=sizeof(name));
native MapAndreas_FindZ_For2DCoord(Float:X, Float:Y, &Float:Z);
native MapAndreas_FindAverageZ(Float:X, Float:Y, &Float:Z);
native MapAndreas_SetZ_For2DCoord(Float:X, Float:Y, Float:Z);
native MapAndreas_SaveCurrentHMap(const name[]);
native MapAndreas_Unload();