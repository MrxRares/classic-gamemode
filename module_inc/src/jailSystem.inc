#include <YSI_Coding\y_hooks>

static const Float:adminJailSpawns[][] = {
	{215.5260, 110.2887, 1000.0156},
	{219.3602, 110.2887, 1000.0156},
	{223.3645, 110.2887, 1000.0156},
	{227.1875, 110.2887, 1000.0156}
};
new Iterator:jailedPlayers<MAX_PLAYERS>, Timer: free_timer[MAX_PLAYERS], jailArea;
hook OnGameModeInit()
{
	CreateDynamicObject(16094, 50.926998, 2492.333252, 26.527380, 0.000000, 0.000000, 0.000000, 1339);
	CreateDynamicObject(16094, 50.926998, 2492.333252, 18.574200, 0.000000, 0.000000, 0.000000, 1339);
	CreateDynamicObject(2933, -43.538200, 2544.136230, 16.755899, 0.000000, 0.000000, 89.000000, 1339);
	CreateDynamicObject(19463, 143.627396, 2516.812744, 15.626800, 1.000000, -90.000000, 0.000000, 1339);
	CreateDynamicObject(19463, 143.627396, 2507.182617, 15.486800, 1.000000, -90.000000, 0.000000, 1339);
	CreateDynamicObject(19463, 140.145294, 2516.812744, 15.627200, 1.000000, -90.000000, 0.000000, 1339);
	CreateDynamicObject(2627, 143.730392, 2504.231445, 15.527100, 0.000000, 0.000000, -91.000000, 1339);
	CreateDynamicObject(2915, 144.146500, 2508.114502, 15.710100, 0.000000, 0.000000, 91.000000, 1339);
	CreateDynamicObject(2915, 144.137894, 2506.202637, 15.678300, 0.000000, 0.000000, 91.000000, 1339);
	CreateDynamicObject(2913, 143.857697, 2510.630371, 15.794600, 4.000000, -90.000000, 89.000000, 1339);
	CreateDynamicObject(2630, 144.027496, 2514.971436, 15.680100, 0.000000, 0.000000, -90.000000, 1339);
	CreateDynamicObject(2629, 140.387299, 2519.492676, 15.741000, 0.000000, 0.000000, 0.000000, 1339);
	CreateDynamicObject(2913, 140.857101, 2520.010254, 16.598600, 0.000000, -90.000000, 0.000000, 1339);
	CreateDynamicObject(3072, 139.668701, 2514.908691, 15.737400, 0.000000, -90.000000, 0.000000, 1339);
	CreateDynamicObject(3071, 139.711395, 2514.343750, 15.737400, 0.000000, -90.000000, 0.000000, 1339);
	CreateDynamicObject(3415, 102.500801, 2440.144531, 15.485200, 0.000000, 0.000000, -105.000000, 1339);
	CreateDynamicObject(4000, -1.631900, 2523.678955, 14.928000, 0.000000, 0.000000, 0.000000, 1339);
	CreateDynamicObject(4079, -2.768200, 2524.482422, 28.392599, 0.000000, 0.000000, -5.000000, 1339);
	CreateDynamicObject(2930, 35.617199, 2559.093750, 216003.000000, 0.000000, 0.000000, 0.000000, 1339);
	CreateDynamicObject(19912, 39.072300, 2563.658936, 17.301300, 0.000000, 0.000000, 69.000000, 1339);
	CreateDynamicObject(19912, -43.574100, 2498.666260, 17.418501, 0.000000, 0.000000, 181.000000, 1339);
	CreateDynamicObject(1454, 102.730003, 2444.085938, 16.267500, 0.000000, 0.000000, -17.000000, 1339);
	CreateDynamicObject(1453, 102.227402, 2442.925781, 16.027700, 0.000000, 0.000000, 0.000000, 1339);
	CreateDynamicObject(1453, 103.854599, 2443.865479, 15.788300, 0.000000, -90.000000, 72.000000, 1339);
	CreateDynamicObject(1453, 104.641502, 2443.765869, 15.788300, 0.000000, -90.000000, 72.000000, 1339);
	CreateDynamicObject(3252, 114.171097, 2433.474609, 15.476900, 0.000000, 0.000000, 0.000000, 1339);
	CreateDynamicObject(1483, 98.823502, 2440.912598, 17.203800, 0.000000, 0.000000, -16.000000, 1339);
	CreateDynamicObject(13367, -5.700700, 2433.806885, 27.075800, 0.000000, 0.000000, 0.000000, 1339);
	CreateDynamicObject(19912, -32.038799, 2476.421875, 17.757000, 0.000000, 0.000000, 0.000000, 1339);
	CreateDynamicObject(19912, -20.503000, 2476.419189, 17.757000, 0.000000, 0.000000, 0.000000, 1339);
	CreateDynamicObject(19912, -8.971700, 2476.222412, 17.757000, 0.000000, 0.000000, -1.000000, 1339);
	CreateDynamicObject(19912, 2.549800, 2476.026123, 17.757000, 0.000000, 0.000000, -1.000000, 1339);
	CreateDynamicObject(19912, 14.085900, 2476.031738, 17.757000, 0.000000, 0.000000, 0.000000, 1339);
	CreateDynamicObject(19912, 25.636900, 2476.030518, 17.757000, 0.000000, 0.000000, 0.000000, 1339);
	CreateDynamicObject(19912, 37.174999, 2476.020752, 17.757000, 0.000000, 0.000000, 0.000000, 1339);
	CreateDynamicObject(19912, 48.712399, 2476.015625, 17.757000, 0.000000, 0.000000, 0.000000, 1339);
	CreateDynamicObject(19912, 60.257401, 2476.015625, 17.757000, 0.000000, 0.000000, 0.000000, 1339);
	CreateDynamicObject(19912, 145.441101, 2476.015625, 17.757000, 0.000000, 0.000000, 0.000000, 1339);
	CreateDynamicObject(19912, 133.910507, 2476.015625, 17.757000, 0.000000, 0.000000, 0.000000, 1339);
	CreateDynamicObject(19912, 122.354698, 2476.015625, 17.757000, 0.000000, 0.000000, 0.000000, 1339);
	CreateDynamicObject(19912, 110.799103, 2476.015625, 17.757000, 0.000000, 0.000000, 0.000000, 1339);
	CreateDynamicObject(19912, 99.266800, 2476.015625, 17.757000, 0.000000, 0.000000, 0.000000, 1339);
	CreateDynamicObject(19912, 87.744598, 2476.015625, 17.757000, 0.000000, 0.000000, 0.000000, 1339);
	CreateDynamicObject(19912, 71.785698, 2476.015625, 17.757000, 0.000000, 0.000000, 0.000000, 1339);
	CreateDynamicObject(19536, 14.859800, 2444.795898, 15.505100, 0.000000, 0.000000, 0.000000, 1339);
	CreateDynamicObject(19536, 82.998703, 2444.795898, 15.505100, 0.000000, 0.000000, 0.000000, 1339);
	CreateDynamicObject(14400, 99.717102, 2425.210449, 16.225300, 0.000000, 0.000000, 0.000000, 1339);
	CreateDynamicObject(14402, 101.545799, 2427.633789, 16.205099, 0.000000, 0.000000, 0.000000, 1339);
	CreateDynamicObject(14469, 110.834381, 2422.146484, 16.185301, 0.000000, 0.000000, 0.000000, 1339);
	CreateDynamicObject(14469, 109.765800, 2424.305908, 16.205099, 0.000000, 0.000000, -44.000000, 1339);
	CreateDynamicObject(14469, 107.114197, 2426.824463, 16.205099, 0.000000, 0.000000, -38.000000, 1339);
	CreateDynamicObject(14469, 104.202309, 2429.847168, 16.205099, 0.000000, 0.000000, -40.000000, 1339);
	CreateDynamicObject(14469, 101.588303, 2430.703857, 16.205099, 0.000000, 0.000000, 33.000000, 1339);
	CreateDynamicObject(14469, 98.584320, 2428.187988, 16.205099, 0.000000, 0.000000, 40.000000, 1339);
	CreateDynamicObject(14469, 95.371429, 2425.362549, 16.205099, 0.000000, 0.000000, 38.000000, 1339);
	CreateDynamicObject(14469, 92.621498, 2422.058594, 16.205099, 0.000000, 0.000000, 47.000000, 1339);
	CreateDynamicObject(14469, 95.705002, 2422.728027, 16.205099, 0.000000, 0.000000, 0.000000, 1339);
	CreateDynamicObject(14469, 99.873299, 2422.534668, 16.205099, 0.000000, 0.000000, 0.000000, 1339);
	CreateDynamicObject(14469, 103.610802, 2426.706787, 16.205099, 0.000000, 0.000000, 0.000000, 1339);
	CreateDynamicObject(14469, 104.578087, 2424.427490, 16.205099, 0.000000, 0.000000, 0.000000, 1339);
	CreateDynamicObject(14469, 103.882401, 2422.241211, 16.205099, 0.000000, 0.000000, 0.000000, 1339);
	CreateDynamicObject(14469, 107.486130, 2422.703613, 16.205099, 0.000000, 0.000000, -55.000000, 1339);
	CreateDynamicObject(14469, 99.973000, 2426.814941, 16.205099, 0.000000, 0.000000, 0.000000, 1339);
	CreateDynamicObject(14469, 108.801102, 2428.394287, 16.205099, 0.000000, 0.000000, 9.000000, 1339);
	CreateDynamicObject(14469, 104.261818, 2432.265381, 15.499390, 0.000000, 0.000000, 0.000000, 1339);
	CreateDynamicObject(14469, 95.084198, 2428.926514, 15.498530, 0.000000, 0.000000, 0.000000, 1339);
	CreateDynamicObject(14469, 99.342010, 2432.009277, 15.499280, 0.000000, 0.000000, 0.000000, 1339);
	CreateDynamicObject(14468, 91.535698, 2423.964600, 16.205099, 0.000000, 0.000000, 0.000000, 1339);
	CreateDynamicObject(3948, 94.494202, 2420.141846, 23.844101, 0.000000, 0.000000, 0.000000, 1339);
	CreateDynamicObject(3948, 103.648499, 2420.577393, 23.844101, 0.000000, 0.000000, 0.000000, 1339);
	CreateDynamicObject(3948, 112.247902, 2420.874756, 23.844101, 0.000000, 0.000000, 0.000000, 1339);
	CreateDynamicObject(700, 18.692699, 2497.450684, 15.995800, 0.000000, 0.000000, 0.000000, 1339);
	CreateDynamicObject(700, 12.331800, 2491.865479, 16.003700, 0.000000, 0.000000, 0.000000, 1339);
	CreateDynamicObject(3279, -38.070000, 2422.763428, 15.389300, 0.000000, 0.000000, 0.000000, 1339);
	CreateDynamicObject(3279, -48.234600, 2487.107178, 15.311600, 0.000000, 0.000000, 0.000000, 1339);
	CreateDynamicObject(3279, 142.942505, 2418.962646, 15.340100, 0.000000, 0.000000, 0.000000, 1339);
	CreateDynamicObject(3279, 140.145905, 2544.134277, 15.437500, 0.000000, 0.000000, 0.000000, 1339);
	CreateDynamicObject(3864, 88.432999, 2565.902100, 21.480000, 0.000000, 0.000000, 89.000000, 1339);
	CreateDynamicObject(3864, 147.520905, 2527.513672, 21.620001, 0.000000, 0.000000, 0.000000, 1339);
	CreateDynamicObject(3864, 148.614304, 2474.330566, 21.520000, 0.000000, 0.000000, 0.000000, 1339);
	CreateDynamicObject(3864, 126.928299, 2419.008057, 21.540001, 0.000000, 0.000000, -89.000000, 1339);
	CreateDynamicObject(3864, 72.603500, 2418.546143, 21.480000, 0.000000, 0.000000, -89.000000, 1339);
	CreateDynamicObject(3864, 11.305700, 2418.463379, 21.540001, 0.000000, 0.000000, -89.000000, 1339);
	CreateDynamicObject(3864, -45.898800, 2459.605957, 21.400000, 0.000000, 0.000000, -181.000000, 1339);
	CreateDynamicObject(16070, 106.435699, 2552.077393, 21.166401, 0.000000, 0.000000, -30.000000, 1339);
	CreateDynamicObject(736, 76.637199, 2430.839111, 26.652500, 0.000000, 0.000000, 0.000000, 1339);
	CreateDynamicObject(763, 75.203201, 2449.180176, 15.438100, 0.000000, 0.000000, 0.000000, 1339);
	CreateDynamicObject(654, 60.002201, 2458.822510, 15.477700, 0.000000, 0.000000, 0.000000, 1339);
	CreateDynamicObject(654, 45.053902, 2427.668457, 15.491000, 0.000000, 0.000000, 0.000000, 1339);
	CreateDynamicObject(673, 63.302200, 2431.177246, 15.495300, 0.000000, 0.000000, 0.000000, 1339);
	CreateDynamicObject(709, 104.533897, 2462.213379, 15.872100, 0.000000, 0.000000, 0.000000, 1339);
	CreateDynamicObject(705, -32.396301, 2454.974365, 15.488200, 0.000000, 0.000000, 0.000000, 1339);
	CreateDynamicObject(615, 50.889500, 2443.248535, 15.428400, 0.000000, 0.000000, 0.000000, 1339);
	CreateDynamicObject(736, 37.839699, 2438.108643, 26.652500, 0.000000, 0.000000, 0.000000, 1339);
	CreateDynamicObject(654, 63.341099, 2444.459473, 15.476300, 0.000000, 0.000000, 0.000000, 1339);
	CreateDynamicObject(615, 127.448097, 2442.003662, 15.490700, 0.000000, 0.000000, 0.000000, 1339);
	CreateDynamicObject(615, 40.519501, 2467.821777, 15.428600, 0.000000, 0.000000, 0.000000, 1339);
	CreateDynamicObject(673, 42.935730, 2455.869141, 15.485920, 0.000000, 0.000000, 0.000000, 1339);
	CreateDynamicObject(1280, 101.460999, 2459.352295, 15.897700, 0.000000, 0.000000, 42.000000, 1339);
	CreateDynamicObject(1280, 101.601593, 2465.065674, 15.897700, 0.000000, 0.000000, -40.000000, 1339);
	CreateDynamicObject(1280, 107.356003, 2457.847412, 15.897700, 0.000000, 0.000000, 108.000000, 1339);
	CreateDynamicObject(1280, 109.538429, 2462.725098, 15.897700, 0.000000, 0.000000, -171.000000, 1339);
	CreateDynamicObject(1280, 106.189339, 2466.669922, 15.897700, 0.000000, 0.000000, -115.000000, 1339);
	CreateDynamicObject(1280, 60.550400, 2475.641357, 15.897700, 0.000000, 0.000000, 90.000000, 1339);
	CreateDynamicObject(1280, 48.800499, 2475.646484, 15.897700, 0.000000, 0.000000, 90.000000, 1339);
	CreateDynamicObject(1280, 37.275501, 2475.618408, 15.897700, 0.000000, 0.000000, 90.000000, 1339);

	CreateDynamic3DTextLabel(""COLOR_SV_HEX"Prison{ffffff}\nPress "COLOR_SV_HEX"F{ffffff} to enter in prison yard", -1, 246.3830, 88.0079, 1003.6406, 25.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, true);
	CreateDynamic3DTextLabel(""COLOR_SV_HEX"Prison{ffffff}\nPress "COLOR_SV_HEX"F{ffffff} to exit from jail\n"COLOR_SV_HEX"(only police officers)", -1, 10.2119, 2510.8525, 17.2258, 100.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, false, 1339);
	CreateDynamic3DTextLabel(""COLOR_SV_HEX"Prison Job{ffffff}\nTasteaza comanda "COLOR_SV_HEX"/work{ffffff} pentru a munci in folosul comunitatii", -1, 98.1159, 2438.5010, 16.5051, 100.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, false, 1339);
	
	CreateDynamic3DTextLabel(""COLOR_SV_HEX"Cladire anexa{ffffff}\nApasa tasta "COLOR_SV_HEX"F{ffffff} pentru a folosi aceasta usa", -1, 100.5359, 2545.9998, 16.5526, 100.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, false, 1339);
	CreateDynamicPickup(1314, 23, 100.5359, 2545.9998, 16.5526, 0, 0, -1, 60);

	CreateDynamic3DTextLabel(""COLOR_SV_HEX"Cantina{ffffff}\nApasa tasta "COLOR_SV_HEX"F{ffffff} pentru a folosi aceasta usa", -1, 1039.8175, 3.4787, 1001.2845, 10.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, false, 1339);
	CreateDynamic3DTextLabel(""COLOR_SV_HEX"Casino{ffffff}\nApasa tasta "COLOR_SV_HEX"F{ffffff} pentru a folosi aceasta usa", -1, 1037.3796, 11.6347, 1001.2845, 10.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, false, 1339);
	CreateDynamic3DTextLabel(""COLOR_SV_HEX"Prison Escape{ffffff}\nApasa tasta "COLOR_SV_HEX"F{ffffff} pentru a incerca sa evadezi din inchisoare", -1, 1044.0648, 10.1981, 1001.2845, 10.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, false, 1339);

	CreateDynamicObject(1505, 1037.266846, -4.753200, 1000.241089, 0.000000, 0.000000, 0.000000, 1339);
	CreateDynamicObject(1507, 1044.474243, 10.951500, 1000.275391, 0.000000, 0.000000, -90.000000, 1339);
	CreateDynamicObject(1504, 1040.227295, 4.233500, 1000.266174, 0.000000, 0.000000, -90.000000, 1339);
	CreateDynamicObject(1506, 1036.618286, 12.043600, 1000.275391, 0.000000, 0.000000, 0.000000, 1339);
	CreateDynamicObject(1702, 1036.557007, 2.095400, 1000.238708, 0.000000, 0.000000, 90.000000, 1339);
	CreateDynamicObject(1766, 1039.842896, 11.566900, 1000.270020, 0.000000, 0.000000, 0.000000, 1339);
	CreateDynamicObject(2256, 1040.188599, -1.053900, 1002.083008, 0.000000, 0.000000, -90.000000, 1339);
	CreateDynamicObject(2258, 1036.061646, 8.527200, 1002.173828, 0.000000, 0.000000, 90.000000, 1339);
	CreateDynamicObject(2267, 1042.566772, 12.016000, 1002.358521, 0.000000, 0.000000, 0.000000, 1339);
	CreateDynamicObject(19354, 1040.307251, -3.106100, 1002.046875, 0.000000, 0.000000, 0.000000, 1339);
	CreateDynamicObject(19354, 1040.307251, 0.106400, 1002.046875, 0.000000, 0.000000, 0.000000, 1339);
	CreateDynamicObject(19354, 1040.307251, 3.318800, 1002.046875, 0.000000, 0.000000, 0.000000, 1339);
	CreateDynamicObject(19354, 1040.307251, 6.524800, 1002.046875, 0.000000, 0.000000, 0.000000, 1339);
	CreateDynamicObject(19354, 1041.833618, 8.358100, 1002.046875, 0.000000, 0.000000, -90.000000, 1339);
	CreateDynamicObject(19354, 1043.884644, 8.358100, 1002.046875, 0.000000, 0.000000, 90.000000, 1339);
	CreateDynamicObject(19354, 1044.554932, 10.246200, 1002.046875, 0.000000, 0.000000, 0.000000, 1339);
	CreateDynamicObject(19354, 1042.816895, 12.149900, 1002.046875, 0.000000, 0.000000, 90.000000, 1339);
	CreateDynamicObject(19354, 1039.604858, 12.147400, 1002.046875, 0.000000, 0.000000, 90.000000, 1339);
	CreateDynamicObject(19354, 1037.163452, 12.147400, 1002.046875, 0.000000, 0.000000, 90.000000, 1339);
	CreateDynamicObject(19354, 1035.921875, 10.427000, 1002.046875, 0.000000, 0.000000, 0.000000, 1339);
	CreateDynamicObject(19354, 1035.925171, 7.208800, 1002.046875, 0.000000, 0.000000, 0.000000, 1339);
	CreateDynamicObject(19354, 1035.942749, 4.004000, 1002.046875, 0.000000, 0.000000, 0.000000, 1339);
	CreateDynamicObject(19354, 1035.947754, 0.806300, 1002.046875, 0.000000, 0.000000, 0.000000, 1339);
	CreateDynamicObject(19354, 1035.948242, -2.407700, 1002.046875, 0.000000, 0.000000, 0.000000, 1339);
	CreateDynamicObject(19354, 1035.947876, -3.316000, 1002.046875, 0.000000, 0.000000, 0.000000, 1339);
	CreateDynamicObject(19354, 1038.120239, -4.846900, 1002.046875, 0.000000, 0.000000, 90.000000, 1339);
	CreateDynamicObject(955, -253.740005, 2597.949951, 62.240002, 0.000000, 0.000000, 90.000000, 1339);
	CreateDynamicObject(956, -253.740005, 2599.750000, 62.240002, 0.000000, 0.000000, 90.000000, 1339);

	CreateDynamicActor(280, 98.8796, 2438.2754, 16.5051, 42.9038, 1, 100, 1339);
	CreateDynamicActor(285, 10.0948, 2508.6169, 17.2453, 280.7433, 1, 100, 1339);
	CreateDynamicActor(285, 15.7948, 2518.2930, 16.4844, 156.4638, 1, 100, 1339);
	CreateDynamicActor(285, 144.6271, 2512.3638, 16.6351, 92.4187, 1, 100, 1339);
	CreateDynamicActor(281, 71.3430, 2476.5598, 16.4844, 2.7501, 1, 100, 1339);
	CreateDynamicActor(267, 76.6685, 2475.4790, 16.5051, 178.6367, 1, 100, 1339);
	CreateDynamicActor(266, 1043.8770, 11.5114, 1001.2845, 134.2468, 1, 100, 1339);

	jailArea = CreateDynamicRectangle(-51, 2409, 289, 2587, 1339);

	//gratii pd
    new gratie1, gratie2, gratie3, gratie4, gratie5, gratie6;
	gratie1 = CreateObject(19302, 266.32431, 85.70610, 1001.28571,   0.00000, 0.00000, 90.30000);
	gratie2 = CreateObject(19302, 266.31870, 87.46790, 1001.28571,   0.02000, 0.00000, -89.82000);
	gratie3 = CreateObject(19302, 266.32031, 82.94890, 1001.28571,   0.02000, 0.00000, -89.82000);
	gratie4 = CreateObject(19302, 266.33569, 81.20160, 1001.28571,   0.02000, 0.00000, 90.36000);
	gratie5 = CreateObject(19302, 266.29480, 76.71580, 1001.28571,   0.02000, 0.00000, 90.36000);
	gratie6 = CreateObject(19302, 266.29480, 78.45580, 1001.28571,   0.02000, 0.00000, -90.60000);
	SetObjectMaterial(gratie1, 0, 19302, "gratie1", "gratie1", 0xFFFFFFFF);
	SetObjectMaterial(gratie2, 0, 19302, "gratie2", "gratie2", 0xFFFFFFFF);
	SetObjectMaterial(gratie3, 0, 19302, "gratie3", "gratie3", 0xFFFFFFFF);
	SetObjectMaterial(gratie4, 0, 19302, "gratie4", "gratie4", 0xFFFFFFFF);
	SetObjectMaterial(gratie5, 0, 19302, "gratie5", "gratie5", 0xFFFFFFFF);
	SetObjectMaterial(gratie6, 0, 19302, "gratie6", "gratie6", 0xFFFFFFFF);
	return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnPlayerConnect(playerid)
{
	//mapping jail
	RemoveBuildingForPlayer(playerid, 3369, 108.0469, 2411.3828, 15.4766, 0.25);
	RemoveBuildingForPlayer(playerid, 3367, 134.9141, 2438.2500, 15.4766, 0.25);
	RemoveBuildingForPlayer(playerid, 3269, 108.0469, 2411.3828, 15.4766, 0.25);
	RemoveBuildingForPlayer(playerid, 3364, 77.3203, 2456.2500, 15.2813, 0.25);
	RemoveBuildingForPlayer(playerid, 3271, 134.9141, 2438.2500, 15.4766, 0.25);
	RemoveBuildingForPlayer(playerid, 16756, 63.0000, 2417.2891, 16.0156, 0.25);
	
	//anexa
	RemoveBuildingForPlayer(playerid, 14812, 1038.1250, 7.0391, 1002.2578, 0.25);
	RemoveBuildingForPlayer(playerid, 14813, 1038.1328, 10.4141, 1000.7266, 0.25);


	playerTextdraws[playerid][releaseTD] = CreatePlayerTextDraw(playerid, 550.400391, 104.962952, "releasing in ~r~03:18~w~ min");
	PlayerTextDrawLetterSize(playerid, playerTextdraws[playerid][releaseTD], 0.297666, 1.367703);
	PlayerTextDrawTextSize(playerid, playerTextdraws[playerid][releaseTD], 0.000000, 126.000000);
	PlayerTextDrawAlignment(playerid, playerTextdraws[playerid][releaseTD], 2);
	PlayerTextDrawColour(playerid, playerTextdraws[playerid][releaseTD], 0xFFFFFFFF);
	PlayerTextDrawUseBox(playerid, playerTextdraws[playerid][releaseTD], 1);
	PlayerTextDrawBoxColour(playerid, playerTextdraws[playerid][releaseTD], 0x000000FF);
	PlayerTextDrawSetShadow(playerid, playerTextdraws[playerid][releaseTD], 0);
	PlayerTextDrawSetOutline(playerid, playerTextdraws[playerid][releaseTD], 1);
	PlayerTextDrawBackgroundColour(playerid, playerTextdraws[playerid][releaseTD], 0x000000FF);
	PlayerTextDrawFont(playerid, playerTextdraws[playerid][releaseTD], 3);
	PlayerTextDrawSetProportional(playerid, playerTextdraws[playerid][releaseTD], 1);
	PlayerTextDrawSetSelectable(playerid, playerTextdraws[playerid][releaseTD], 0);
	return Y_HOOKS_CONTINUE_RETURN_1;
}

function:onCopKillWantedPlayer(playerid, killerid)
{
	if(!playerInfo[playerid][pWantedLevel])
		return Y_HOOKS_CONTINUE_RETURN_1;

	if(!Iter_Count(streamedPlayers[playerid]))
		return Y_HOOKS_CONTINUE_RETURN_1;

	new streamCops = 0, streamCop[5];
	foreach(new i : streamedPlayers[playerid])
	{
		if(!playerStaticVar[i][playerLogged] || factionType[playerInfo[i][pGroup]] != FACTION_TYPE_POLICE)
			continue;

		if(!playerStaticVar[i][groupDuty])
			continue;

		if(streamCops >= 5)
			break;

		if(i == killerid)
		{
			va_SendClientMessage(i, COLOR_BLUE, "Ai primit $3000 bonus pentru prinderea suspectului %s.", GetName(playerid));
			addMoney(playerid, GetMilliards(playerid), GetMoney(playerid), 0, 3000, false);

		}

		GameTextForPlayer(killerid, "running suspect bonus!", 5000, 1);

		streamCop[streamCops] = i;
		streamCops ++;

		insertRaportPoint(i, 0);
	}
	if(!streamCops)
		return true;

	new nearFormat[128];
	for(new i; i < streamCops; i++)
		format(nearFormat, sizeof nearFormat, "%s%s%s", nearFormat, GetName(streamCop[i]), (i == (streamCops - 1) ? ("!") : (", ")));

	SendClientMessage(playerid, COLOR_GREY, "You died and you are now in jail.");

	sendNearbyMessage(killerid, COLOR_PURPLE, "* %s is now in jail! Thanks to: %s", GetName(playerid), nearFormat);
	sendDepartmentMessage(true, COLOR_LIGHTBLUE, "Dispatch: %s %s and will be in jail for %d seconds, over.", GetName(playerid), (killerid == INVALID_PLAYER_ID ? ("died") : va_return("has been killed by %s", GetName(killerid))), playerInfo[playerid][pWantedLevel] * 300);

	mysql_format(SQL, returnString, sizeof returnString, "update `users` set `Jail` = '1', `JailTime` = '%d' where `id` = '%d';", (playerInfo[playerid][pWantedLevel] * 300), playerInfo[playerid][pSQLID]);
	mysql_tquery(SQL, returnString, "", "");

	new time = playerInfo[playerid][pWantedLevel] * (5 * 60);
	put_player_in_jail(playerid, time, JAIL_TYPE_COP);
	return Y_HOOKS_CONTINUE_RETURN_1;
}


put_player_in_jail(playerid, jailTime, const e_jailTypeOptions:type)
{


	playerInfo[playerid][pJail] = 1;
	Iter_Add(jailedPlayers, playerid);

	playerInfo[playerid][pJailTime] = jailTime;

	if(type == JAIL_TYPE_ADMIN) {

		new const rand = random(sizeof(adminJailSpawns));

		SetPlayerPos(playerid, adminJailSpawns[rand][0], adminJailSpawns[rand][1], adminJailSpawns[rand][2]);
		SetPlayerVirtualWorld(playerid, 1000);
		SetPlayerInterior(playerid, 10);

		playerStaticVar[playerid][inHQ] = 0;
		playerInfo[playerid][pJailType] = JAIL_TYPE_ADMIN;
		mysql_format(SQL, returnString, sizeof returnString, "update `users` set `Jail` = '1', `JailType` = '2', `JailTime` = '%d' where `id` = '%d';", jailTime, playerInfo[playerid][pSQLID]);
		mysql_tquery(SQL, returnString, "", "");
	}
	else if(type == JAIL_TYPE_COP) {

		mysql_format(SQL, returnString, sizeof returnString, "update `users` set `Jail` = '1', `JailType` = '1', `JailTime` = '%d' where `id` = '%d';", jailTime, playerInfo[playerid][pSQLID]);
		mysql_tquery(SQL, returnString, "", "");
		playerInfo[playerid][pJailType] = JAIL_TYPE_COP;
	}

	if(playerStaticVar[playerid][var_jail_timer] == Timer: 0)
		playerStaticVar[playerid][var_jail_timer] = repeat jailTimer(playerid);

	clearWanted(playerid);
	SpawnPlayer(playerid);
	return true;
}
YCMD:unjail(playerid, params[], help)
{
	check_admin

	if(GetPVarInt(playerid, "delay_actions") > gettime()) 
		return va_SendClientMessage(playerid, -1, "Asteapta %d secunde pentru a folosi din nou aceasta comanda.", GetPVarInt(playerid, "delay_actions") - gettime());

	new returnPlayer, returnReason[64];
	if(sscanf(params, "us[64]", returnPlayer, returnReason))
		return sendSyntaxMessage(playerid, "/unjail [playername/playerid] [reason]");

	if(!IsPlayerConnected(returnPlayer) || returnPlayer == playerid)
		return sendError(playerid,"Player not connected.");

	if(!Iter_Contains(jailedPlayers, returnPlayer))
		return sendError(playerid, "This player is not jailed");
		
    va_SendClientMessageToAll(COLOR_LIGHTRED, "{ffffff}("COLOR_SV_HEX"Admin Info{ffffff}): %s has been unjailed by %s for reason: %s.", GetName(returnPlayer), GetName(playerid), returnReason);
    va_SendClientMessage(playerid, COLOR_YELLOW, "* You unjailed player %s for reason: %s.", GetName(returnPlayer), returnReason);
    va_SendClientMessage(returnPlayer, COLOR_YELLOW, "* %s unjailed you for reason: %s.", GetName(playerid), returnReason);

	WhenJailTimeExpire(returnPlayer);
	SetPVarInt(playerid, "delay_actions", gettime() + 20);
	return true;
}

WhenJailTimeExpire(forplayer)
{
	if(playerInfo[forplayer][pJailTime] == 0)
		SendClientMessage(forplayer, COLOR_WHITE, "Your time is up! You have been released from jail/prison.");

	playerStaticVar[forplayer][inHQ] = groupInfo[GROUP_TYPE_LSPD][factionID];
	SetPlayerVirtualWorld(forplayer, playerStaticVar[forplayer][inHQ]);

	SetPlayerPos(forplayer, 268.7461, 76.8184, 1001.0391);
	SetPlayerInterior(forplayer, 6);

    RemovePlayerAttachedObject(forplayer, INDEX_JOB);
    SetPlayerSpecialAction(forplayer, SPECIAL_ACTION_NONE);
    DisablePlayerCheckpoint(forplayer);
    playerStaticVar[forplayer][pCheckpoint] = 0;

	SetPlayerSkin(forplayer, playerInfo[forplayer][pSkin]);
	PlayerTextDrawHide(forplayer, playerTextdraws[forplayer][releaseTD]);
	stop playerStaticVar[forplayer][var_jail_timer];
	playerInfo[forplayer][pJail] = 0;
	playerInfo[forplayer][pJailTime] = -1;
	Iter_Remove(jailedPlayers, forplayer);
	mysql_format(SQL, returnString, sizeof returnString, "update `users` set `Jail` = '0', `JailType` = '0', `JailTime` = '0' where `id` = '%d';", playerInfo[forplayer][pSQLID]);
	mysql_tquery(SQL, returnString, "", "");
	return true;
}

ReleasePlayer(forplayer)
{
	
	playerStaticVar[forplayer][inHQ] = groupInfo[GROUP_TYPE_LSPD][factionID];
	SetPlayerVirtualWorld(forplayer, playerStaticVar[forplayer][inHQ]);

	SetPlayerPos(forplayer, 268.7461, 76.8184, 1001.0391);
	SetPlayerInterior(forplayer, 6);

	SetPlayerSkin(forplayer, playerInfo[forplayer][pSkin]);
	PlayerTextDrawHide(forplayer, playerTextdraws[forplayer][releaseTD]);

	playerInfo[forplayer][pJail] = 0;
	playerInfo[forplayer][pJailTime] = 0;
	mysql_format(SQL, returnString, sizeof returnString, "update `users` set `Jail` = '0', `JailTime` = '0' where `id` = '%d';", playerInfo[forplayer][pSQLID]);
	mysql_tquery(SQL, returnString, "", "");
	return true;
}

hook WhenPlayerLoggedIn(playerid)
{
	if(!cache_num_rows())
		return Y_HOOKS_CONTINUE_RETURN_1;

	cache_get_value_name_int(0, "Jail", playerInfo[playerid][pJail]);

	new jailMata;
	cache_get_value_name_int(0, "JailTime", playerInfo[playerid][pJailTime]);
    
	cache_get_value_name_int(0, "JailType", jailMata);
	playerInfo[playerid][pJailType] = e_jailTypes:jailMata;

	if(playerInfo[playerid][pJail])
		put_player_in_jail(playerid, playerInfo[playerid][pJailTime], playerInfo[playerid][pJailType]);

	return Y_HOOKS_CONTINUE_RETURN_1;
}

YCMD:jailedplayers(playerid, params[], help)
{
	check_admin

	if(!Iter_Count(jailedPlayers))
		return sendError(playerid,"Currently no jailed players online.");

    SendClientMessage(playerid, COLOR_CYAN, "Jailed Players");

    foreach(new i : jailedPlayers)
    {
        va_SendClientMessage(playerid, -1, "%s (%d) - %s left.", GetName(i), i, CalculateTime(playerInfo[i][pJailTime]));
    }
	va_SendClientMessage(playerid, COLOR_LIGHTRED, "* Now are %d jailed players online.", Iter_Count(jailedPlayers));
    return true; 
}

hook OnPlayerDisconnect(playerid, reason)
{
	if(Iter_Contains(jailedPlayers, playerid))
		Iter_Remove(jailedPlayers, playerid);

	return Y_HOOKS_CONTINUE_RETURN_1;
}

hook WhenPlayerSpawn(playerid)
{
	if(!playerInfo[playerid][pJail])
		return Y_HOOKS_CONTINUE_RETURN_1;

	if(playerInfo[playerid][pJailType] == JAIL_TYPE_ADMIN) {

		new const rand = random(sizeof(adminJailSpawns));

		SetPlayerPos(playerid, adminJailSpawns[rand][0], adminJailSpawns[rand][1], adminJailSpawns[rand][2]);
		SetPlayerVirtualWorld(playerid, 1000);
		SetPlayerInterior(playerid, 10);

		playerStaticVar[playerid][inHQ] = 0;
		playerInfo[playerid][pJailType] = JAIL_TYPE_ADMIN;
	}
	else if(playerInfo[playerid][pJailType] == JAIL_TYPE_COP) {

		SetPlayerPos(playerid, 10.2119, 2510.8525, 17.2258);
		SetPlayerFacingAngle(playerid, 0.0);

		SetPlayerInterior(playerid, 0);
		SetPlayerVirtualWorld(playerid, 1339);
		playerInfo[playerid][pJailType] = JAIL_TYPE_COP;
	}


	if(playerInfo[playerid][pGender])
		SetPlayerSkin(playerid, 62);
	else
		SetPlayerSkin(playerid, 41);

	return Y_HOOKS_CONTINUE_RETURN_0;
}

hook OnPlayerLeaveDynArea(playerid, areaid)
{
	if(!playerInfo[playerid][pJail])
		return Y_HOOKS_CONTINUE_RETURN_1;

	if(areaid != jailArea)
		return Y_HOOKS_CONTINUE_RETURN_1;

	//SendClientMessage(playerid, COLOR_DCHAT, "Ai fost respawnat deoarece ai parasit zona penintenciarului.");
	SendClientMessage(playerid, COLOR_RED, "Te-ai departat de penitenciar. Ai fost respawnat.");
	SpawnPlayer(playerid);
	return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnPlayerKeyStateChange(playerid, KEY:newkeys, KEY:oldkeys)
{
	if(newkeys == KEY_SECONDARY_ATTACK)
	{
		if(IsPlayerInRangeOfPoint(playerid, 5.0, 246.3830, 88.0079, 1003.6406))
		{
			check_police_member

			if(GetPVarInt(playerid, "enter_building_deelay") > gettime())
				return va_SendClientMessage(playerid, COLOR_GREY, "[Anti-Abuz] Nu poti intra intr-o cladire timp de %d secunde.", GetPVarInt(playerid, "enter_building_deelay") - gettime());

			playerStaticVar[playerid][pLastVirtual] = playerStaticVar[playerid][inHQ];
			SetPlayerPos(playerid, 10.2119, 2510.8525, 17.2258);

			SetPlayerInterior(playerid, 0);
			SetPlayerVirtualWorld(playerid, 1339);

			SetPVarInt(playerid, "enter_building_deelay", gettime() + 15);
			return true;
		}
		else if(IsPlayerInRangeOfPoint(playerid, 5.0, 10.2119, 2510.8525, 17.2258))
		{
			check_police_member

			SetPlayerPos(playerid, 246.3830, 88.0079, 1003.6406);

			SetPlayerVirtualWorld(playerid, playerStaticVar[playerid][pLastVirtual]);
			SetPlayerInterior(playerid, 6);
			return true;
		}
		else if(IsPlayerInRangeOfPoint(playerid, 5.0, 100.5359, 2545.9998, 16.5526))
		{
			SendClientMessage(playerid, -1, "Cladirea anexa este inchisa.");
			return true;
		}
		return true;
	}
	return Y_HOOKS_CONTINUE_RETURN_1;
}

timer free_dee_expired[20000](playerid)
{
	SendClientMessage(playerid, -1, "Oferta de free a expirat, acum poti oferi free altcuiva.");

	playerStaticVar[playerid][freePlayer] = INVALID_PLAYER_ID;
	playerStaticVar[playerid][freeAmount] = 0;

	free_timer[playerid] = Timer:0;
	return true;
}

timer jailTimer[1000](playerid) {
	if(--playerInfo[playerid][pJailTime] < 1) {
		WhenJailTimeExpire(playerid);
		return 1;
	}
	va_PlayerTextDrawSetString(playerid, playerTextdraws[playerid][releaseTD], "releasing in ~r~%s~w~ min", CalculateTime(playerInfo[playerid][pJailTime]));
	PlayerTextDrawShow(playerid, playerTextdraws[playerid][releaseTD]);
	return 1;
}

YCMD:free(playerid, params[], help)
{
	check_police_member
	check_duty_status

	if(GetPVarInt(playerid, "free_time") > gettime())
		return sendError(playerid, "Ai oferit deja un free, te rugam sa astepti 20 secunde pentru a oferii un alt free.");

	new returnPlayer, returnMoney;
	if(sscanf(params, "ui", returnPlayer, returnMoney))
		return sendSyntaxMessage(playerid, "/free [playername/playerid] [amount]");

	if(!IsPlayerConnected(returnPlayer))
		return sendError(playerid, "Invalid player.");

	if(!playerInfo[returnPlayer][pJail])
		return sendError(playerid, "Acest jucator nu se afla in inchisoare.");

	if(GetDistanceBetweenPlayers(playerid, returnPlayer) > 15)
		return sendError(playerid,"You are not near that player!");

	if(returnMoney < 1 || returnMoney > 2000000) return
		SendClientMessage(playerid, -1, "Invalid amount of money ($1 - $2.000.000).");

	if(playerInfo[playerid][pPremiumAccount] == 0) {
		if(playerInfo[playerid][pRobPoints] < 7) return sendError(playerid, "You don't have 7 free points.");
	}
	else {
		if(playerInfo[playerid][pRobPoints] < 5) return sendError(playerid, "You don't have 5 free points.");
	}

	sendSplitMessage(playerid, -1, "I-ai trimis lui %s o oferta de a fi eliberat pentru $%s.", GetName(returnPlayer), FormatNumber(returnMoney));
	sendSplitMessage(returnPlayer, COLOR_DBLUE, "> %s doreste sa iti ofere posibilitatea de a iesi din jail in schimbul sumei de $%s. Scrie /accept free %d pentru a accepta.", GetName(playerid), FormatNumber(returnMoney), playerid);
	
	if(playerStaticVar[playerid][freePlayer] != 0)
		va_SendClientMessage(playerStaticVar[playerid][freePlayer], -1, "Oferta ta de free din partea lui %s a expirat.", GetName(playerid));

	playerStaticVar[playerid][freePlayer] = returnPlayer + 1;
	playerStaticVar[playerid][freeAmount] = returnMoney;

	SetPVarInt(playerid, "free_time", gettime() + 20);
	free_timer[playerid] = defer free_dee_expired(playerid);
	return true;
}