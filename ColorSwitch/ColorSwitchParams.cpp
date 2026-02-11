#include "ColorSwitch.h"


inline json PixelToJson(PF_Pixel8 p)
{
	json ret = {p.red, p.green, p.blue};
	return ret;
}

inline PF_Pixel JsonToPixel(json jsn)
{
	PF_Pixel ret = { 0,0,0,0 };
	ret.red = ret.green = ret.blue = 0;
	ret.alpha = PF_MAX_CHAN8;
	if (jsn.is_array() && jsn.size() >= 3) {
		ret.red = jsn[0].get<uint8_t>();
		ret.green = jsn[1].get<uint8_t>();
		ret.blue = jsn[2].get<uint8_t>();
	}
	return ret;
}

json ParamInfoToJson(ParamInfo* infoP)
{
	json ret;
	json jcolo = json::array();
	json jcoln = json::array();
	json jcole = json::array();
	ret["enable_all"] = infoP->enable_All;
	ret["mode"] = infoP->mode;
	ret["active_param_count"] = infoP->active_param_count;
	for (int i = 0; i < COLOR_TABLE_MAX; i++)
	{
		jcole.push_back(infoP->turnon_colors[i]);
		jcolo.push_back(PixelToJson(infoP->old_colors[i]));
		jcoln.push_back(PixelToJson(infoP->new_colors[i]));
	}
	ret["turnon_colors"] = jcole;
	ret["old_colors"] = jcolo;
	ret["new_colors"] = jcoln;
	return ret;
}
PF_Err JsonToParamInfo(json js, ParamInfo* infoP)
{
	PF_Err err = PF_Err_NONE;
	infoP->enable_All = FALSE;
	infoP->mode = SwitchMode::SWITCH_MODE_REPLACE;
	infoP->active_param_count = 4;
	for (int i = 0; i < COLOR_TABLE_MAX; i++)
	{
		infoP->turnon_colors[i] = FALSE;
		infoP->old_colors[i] = { 255,0,0,0 };
		infoP->new_colors[i] = { 0,255,0,0 };
	}
	//
	if (js.find("enable_all") != js.end()) {
		infoP->enable_All = js["enable_all"].get<PF_Boolean>();
	}
	if (js.find("mode") != js.end()) {
		infoP->mode = js["mode"].get<A_long>();
	}
	if (js.find("active_param_count") != js.end()) {
		infoP->active_param_count = js["active_param_count"].get<A_long>();
	}
	else {
		err = PF_Err_INTERNAL_STRUCT_DAMAGED;;
	}
	if (js.find("turnon_colors") != js.end()) {
		json jcole = js["turnon_colors"];
		if (jcole.is_array())
		{
			for (int i = 0; i < jcole.size() && i < COLOR_TABLE_MAX; i++)
			{
				infoP->turnon_colors[i] = jcole[i].get<PF_Boolean>();
			}
		}
		else {
			err = PF_Err_INTERNAL_STRUCT_DAMAGED;;
		}
	}
	else {
		err = PF_Err_INTERNAL_STRUCT_DAMAGED;;
	}
	if (js.find("old_colors") != js.end()) {
		json jcolo = js["old_colors"];
		if (jcolo.is_array())
		{
			for (int i = 0; i < jcolo.size() && i < COLOR_TABLE_MAX; i++)
			{
				infoP->old_colors[i] = JsonToPixel(jcolo[i]);
			}
		}
		else {
			err = PF_Err_INTERNAL_STRUCT_DAMAGED;;
		}
	}
	else {
		err = PF_Err_INTERNAL_STRUCT_DAMAGED;;
	}
	if (js.find("new_colors") != js.end()) {
		json jcoln = js["new_colors"];
		if (jcoln.is_array())
		{
			for (int i = 0; i < jcoln.size() && i < COLOR_TABLE_MAX; i++)
			{
				infoP->new_colors[i] = JsonToPixel(jcoln[i]);
			}
		}
		else {
			err = PF_Err_INTERNAL_STRUCT_DAMAGED;;
		}
	}
	else {
		err = PF_Err_INTERNAL_STRUCT_DAMAGED;;

	}
	return err;
}
