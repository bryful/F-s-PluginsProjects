#include "NF_Json.h"


/*
static json PixelTableToJson(PixelTable* tableP)
{
	json ret;
	json jcol = json::array();
	json jcole = json::array();

	ret["targetColorMode"] = tableP->targetColorMode;
	for (int i = 0; i < COLOR_TABLE_COUNT; i++)
	{
		jcol.push_back(PixelToJson(tableP->target[i]));
		jcole.push_back(tableP->targetEnabled[i]);
	}
	ret["colors"] = jcol;
	ret["enables"] = jcole;
	ret["gradColor"] = PixelToJson(tableP->gradColor);
	ret["invert"] = tableP->invert;
	ret["guideEnabled"] = tableP->guideEnabled;
	ret["guideColor"] = PixelToJson(tableP->guideColor);
	return ret;
}
static PixelTable JsonToPixelTable(json jsn)
{
	PixelTable table;
	//bool ok = true;
	AEFX_CLR_STRUCT(table);
	table.ok = true;

	if (jsn.find("targetColorMode") != jsn.end()) {
		json ct = jsn["targetColorMode"];
		table.targetColorMode = ct.get<long>();
	}
	if (jsn.find("colors") != jsn.end()) {
		json ct = jsn["colors"];
		if (ct.is_array())
		{
			for (int i = 0; i < ct.size(); i++)
			{
				json it = ct[i];
				table.target[i] = JsonToPixel(it);
			}

		}
		else {
			table.ok = false;
		}
	}
	else {
		table.ok = false;
	}
	if (jsn.find("enables") != jsn.end()) {
		json ct2 = jsn["enables"];
		if (ct2.is_array())
		{
			for (int i = 0; i < ct2.size(); i++)
			{
				table.targetEnabled[i] = ct2[i].get<PF_Boolean>();
			}
		}
	}
	if (jsn.find("gradColor") != jsn.end()) {
		json ct3 = jsn["gradColor"];
		table.gradColor = JsonToPixel(ct3);
	}
	if (jsn.find("invert") != jsn.end()) {
		json ct = jsn["invert"];
		table.invert = ct.get<PF_Boolean>();
	}
	if (jsn.find("guideEnabled") != jsn.end()) {
		json ct = jsn["guideEnabled"];
		table.guideEnabled = ct.get<PF_Boolean>();
	}
	if (jsn.find("guideColor") != jsn.end()) {
		json ct = jsn["guideColor"];
		table.guideColor = JsonToPixel(ct);
	}
	return table;
}
*/