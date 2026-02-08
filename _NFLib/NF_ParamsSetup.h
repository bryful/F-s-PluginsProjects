#pragma once
#ifndef NF_PARAMSSETUP_H
#define NF_PARAMSSETUP_H

#include "AEConfig.h" 
#include "entry.h"

#ifdef AE_OS_WIN
#include "string.h"
#endif
#include "AE_Effect.h"
#include "AE_EffectCB.h"
#include "AE_EffectCBSuites.h"
#include "AE_Macros.h"
#include "AEGP_SuiteHandler.h"
#include "String_Utils.h"
#include "Param_Utils.h"
#include "Smart_Utils.h"
#include "AE_GeneralPlug.h"

#if defined(PF_AE100_PLUG_IN_VERSION)
#include "AEFX_SuiteHelper.h"
#define refconType void*
#else
#include "PF_Suite_Helper.h"
#define refconType A_long
#endif
/*
フラグ名,和訳・概要,詳細説明
CANNOT_TIME_VARY,
時間変化不可,
パラメーターの右側にキーフレーム打点用のアイコンが表示されなくなり、時間経過による値の変化（アニメーション）ができなくなります。
CANNOT_INTERP,
補間不可,
値の変化は可能ですが、キーフレーム間が補間されません。常に「停止（定数）」のような挙動になり、値が不連続に切り替わります。
COLLAPSE_TWIRLY,
ツリーを閉じる,エフェクトコントロールパネル（ECW）上の階層（ツリー）をデフォルトで閉じた状態にします。動的に変更も可能です。
START_COLLAPSED,
初期状態で閉じる,COLLAPSE_TWIRLYと同じ値です。エフェクトを適用した直後に、パラメーターグループを閉じた状態にするために使用します。
SUPERVISE
,監視対象,値が変更された際に、AEから PF_Cmd_USER_CHANGED_PARAM メッセージが送られるようになります。UIの連動制御に必須です。
USE_VALUE_FOR_OLD_PROJECTS,
旧プロジェクト用値使用,パラメーターが追加された新しいバージョンのプラグインで古いプロジェクトを開く際、dephaultではなくvalueフィールドの値で初期化します。
LAYER_PARAM_IS_TRACKMATTE,
トラックマットとして扱う,レイヤーパラメーター専用。Premiere Proでそのレイヤーをトラックマットとして提示します（AEでは無視されます）。
EXCLUDE_FROM_HAVE_INPUTS_CHANGED,
入力変更チェックから除外,PF_HaveInputsChangedOverTimeSpanを呼び出す際、このパラメーターの変更を「入力の変化」としてカウントさせないようにします。
SKIP_REVEAL_WHEN_UNHIDDEN,
表示時の自動展開スキップ,非表示だったパラメーターが表示（Unhide）された際、親階層を自動で展開したりスクロールしたりしてユーザーに見せる挙動を無効にします。

*/
class NF_ParamsSetup {
private:
	PF_InData* in_data;
	PF_OutData* out_data;
	//PF_ParamDef** params = NULL; // ポインタ配列ではなく二重ポインタに修正
	//PF_LayerDef* output = NULL;
	PF_ParamDef		def;
	//A_long		indexCount = 0;
	A_long param_count; // 追加したパラメータ数をカウント
	PF_ParamUIFlags ui_flags = PF_PUI_NONE;
	PF_ParamFlags param_flags = PF_ParamFlag_NONE;
protected:
public:
	// ***************************************************************
	void ClearDef() {
		AEFX_CLR_STRUCT(def); // AEの標準マクロを使うのが安全
	}
	/*
	void InitDef() {
		ClearDef();
		def.flags = param_flags;
		def.ui_flags = ui_flags;
	}
	void ClearFlags() {
		def.flags = PF_ParamFlag_NONE;
		def.ui_flags = PF_PUI_NONE;
	}
	void SetParamFlags(PF_ParamFlags flags) {
		param_flags = flags;
	}
	void SetUIFlags(PF_ParamUIFlags flags) {
		ui_flags = flags;
	}
	*/
	void SetNumParams(A_long num) {
		out_data->num_params = num;
	}	
	// ***************************************************************
	NF_ParamsSetup(
		PF_InData* in, PF_OutData* out)
		: in_data(in), out_data(out), param_count(0)
	{
		ClearDef();
	}
	// ***************************************************************

	void Finalize() {
		if (out_data) {
			// パラメータ0番（Inputレイヤー）を含めた数を報告
			out_data->num_params = param_count + 1;
		}
	}
	// ***************************************************************
	PF_Err AddColor(
		const char* name,
		PF_Pixel default_value,
		A_long id,
		PF_ParamFlags flags = PF_ParamFlag_NONE,
		PF_ParamUIFlags ui_flags = PF_PUI_NONE
	)
	{
		PF_Err			err = PF_Err_NONE;
		ClearDef();
		def.param_type = PF_Param_COLOR;
		PF_STRNNCPY(def.PF_DEF_NAME,name,sizeof(def.PF_DEF_NAME));
		def.u.cd.value = (PF_UnionablePixel)default_value;
		def.u.cd.value.alpha = 255;
		def.u.cd.dephault = def.u.cd.value;
		def.uu.id = (id);
		def.flags = flags;
		def.ui_flags = ui_flags;
		err = in_data->inter.add_param(in_data->effect_ref, -1, &def);
		if (!err) param_count++;
		return err;
	}
	// ***************************************************************
	PF_Err AddSlider(
		const char* name,
		A_long valid_min,
		A_long valid_max,
		A_long slider_min,
		A_long slider_max,
		A_long default_value,
		A_long id,
		PF_ParamFlags flags = PF_ParamFlag_NONE,
		PF_ParamUIFlags ui_flags = PF_PUI_NONE
	)
	{
		PF_Err			err = PF_Err_NONE;
		ClearDef();
		def.param_type = PF_Param_SLIDER;
		PF_STRNNCPY(def.PF_DEF_NAME, name, sizeof(def.PF_DEF_NAME));
		def.u.sd.value_str[0] = '\0';
		def.u.sd.value_desc[0] = '\0';
		def.u.sd.valid_min = valid_min;
		def.u.sd.valid_max = valid_max;
		def.u.sd.slider_min = slider_min;
		def.u.sd.slider_max = slider_max;
		def.u.sd.value = (A_long)default_value;
		def.u.sd.dephault = (A_long)default_value;
		def.uu.id = id;
		def.flags = flags;
		def.ui_flags = ui_flags;
		err = in_data->inter.add_param(in_data->effect_ref, -1, &def);
		if (!err) param_count++;
		return err;
	}
	// ***************************************************************
	PF_Err AddFixedSlider(
		const char* name,
		PF_Fixed valid_min,
		PF_Fixed valid_max,
		PF_Fixed slider_min,
		PF_Fixed slider_max,
		PF_Fixed default_value,
		A_long prec,
		A_long disp,
		A_long id,
		PF_ParamFlags flags = PF_ParamFlag_NONE,
		PF_ParamUIFlags ui_flags = PF_PUI_NONE
	)
	{
		PF_Err			err = PF_Err_NONE;
		ClearDef();
		def.param_type = PF_Param_FIX_SLIDER;
		PF_STRNNCPY(def.PF_DEF_NAME, name, sizeof(def.PF_DEF_NAME));
		def.u.fd.value_str[0] = '\0';
		def.u.fd.value_desc[0] = '\0';
		def.u.fd.valid_min = (PF_Fixed)((valid_min) * 65536.0);
		def.u.fd.valid_max = (PF_Fixed)((valid_max) * 65536.0);
		def.u.fd.slider_min = (PF_Fixed)((slider_min) * 65536.0);
		def.u.fd.slider_max = (PF_Fixed)((slider_max) * 65536.0);
		def.u.fd.value = (PF_Fixed)((default_value) * 65536.0);
		def.u.fd.dephault = (PF_Fixed)((default_value) * 65536.0);
		def.u.fd.precision = (A_short)prec;
		def.u.fd.display_flags = (A_short)disp;
		def.uu.id = id;
		def.flags = flags;
		def.ui_flags = ui_flags;
		err = in_data->inter.add_param(in_data->effect_ref, -1, &def);
		if (!err) param_count++;
		return err;
	}
	// ***************************************************************
	PF_Err AddFloatSlider(
		const char* name,
		PF_FpShort valid_min,
		PF_FpShort valid_max,
		PF_FpShort slider_min,
		PF_FpShort slider_max,
		PF_FpShort default_value,
		A_long prec,
		A_long disp,
		PF_Boolean want_phase,
		A_long id,
		PF_ParamFlags flags = PF_ParamFlag_NONE,
		PF_ParamUIFlags ui_flags = PF_PUI_NONE)
	{
		PF_Err			err = PF_Err_NONE;
		ClearDef();
		def.param_type = PF_Param_FLOAT_SLIDER;
		PF_STRNNCPY(def.PF_DEF_NAME, name, sizeof(def.PF_DEF_NAME));
		def.u.fs_d.valid_min = valid_min;
		def.u.fs_d.valid_max = valid_max;
		def.u.fs_d.slider_min = slider_min;
		def.u.fs_d.slider_max = slider_max;
		def.u.fs_d.value = default_value;
		def.u.fs_d.dephault = (PF_FpShort)default_value;
		def.u.fs_d.precision = prec;
		def.u.fs_d.display_flags = disp;
		def.u.fs_d.curve_tolerance = 0;
		def.u.fs_d.fs_flags = (want_phase) ? PF_FSliderFlag_WANT_PHASE : 0;
		def.uu.id = id;
		def.flags = flags;
		def.ui_flags = ui_flags;
		err = in_data->inter.add_param(in_data->effect_ref, -1, &def);
		if (!err) param_count++;
		return err;
	}
	PF_Err AddCheckBox(
		const char* name,
		const char* button_name,
		PF_Boolean default_value,
		A_long id,
		PF_ParamFlags flags = PF_ParamFlag_NONE,
		PF_ParamUIFlags ui_flags = PF_PUI_NONE
	)
	{
		PF_Err			err = PF_Err_NONE;
		ClearDef();
		def.param_type = PF_Param_CHECKBOX;
		PF_STRNNCPY(def.PF_DEF_NAME, name, sizeof(def.PF_DEF_NAME));
		def.u.bd.u.nameptr = (char*)button_name;
		def.u.bd.value = (A_long)default_value;
		def.u.bd.dephault = (PF_Boolean)(def.u.bd.value);
		def.uu.id = id;
		def.flags = flags;
		def.ui_flags = ui_flags;
		err = in_data->inter.add_param(in_data->effect_ref, -1, &def);
		if (!err) param_count++;
		return err;
	}
	PF_Err AddTopic(
		const char* name,
		A_long id,
		PF_ParamFlags flags = PF_ParamFlag_NONE,
		PF_ParamUIFlags ui_flags = PF_PUI_NONE

	)
	{
		PF_Err			err = PF_Err_NONE;
		ClearDef();
		def.param_type = PF_Param_GROUP_START;
		PF_STRNNCPY(def.PF_DEF_NAME, name, sizeof(def.PF_DEF_NAME));
		def.uu.id = id;
		def.flags = flags;
		def.ui_flags = ui_flags;
		err = in_data->inter.add_param(in_data->effect_ref, -1, &def);
		if (!err) param_count++;
		return err;
	}
	PF_Err EndTopic(
		A_long id
	)
	{
		PF_Err			err = PF_Err_NONE;
		ClearDef();
		def.param_type = PF_Param_GROUP_END;
		def.uu.id = id;
		err = in_data->inter.add_param(in_data->effect_ref, -1, &def);
		if (!err) param_count++;
		return err;
	}
	PF_Err AddAngle(
		const char* name,
		PF_FpLong default_value,
		A_long id,
		PF_ParamFlags flags = PF_ParamFlag_NONE,
		PF_ParamUIFlags ui_flags = PF_PUI_NONE

	)
	{
		PF_Err			err = PF_Err_NONE;
		ClearDef();
		def.param_type = PF_Param_ANGLE;
		PF_STRNNCPY(def.PF_DEF_NAME, name, sizeof(def.PF_DEF_NAME));
		def.u.ad.value = def.u.ad.dephault = (PF_Fixed)(default_value * 65536.0);
		def.uu.id = id;
		def.flags = flags;
		def.ui_flags = ui_flags;
		err = in_data->inter.add_param(in_data->effect_ref, -1, &def);
		if (!err) param_count++;
		return err;
	}
	PF_Err AddPopup(
		const char* name,
		A_long num_choices,
		A_long default_value,
		const char* items_string,
		A_long id,
		PF_ParamFlags flags = PF_ParamFlag_NONE,
		PF_ParamUIFlags ui_flags = PF_PUI_NONE
	)
	{
		PF_Err			err = PF_Err_NONE;
		ClearDef();
		def.param_type = PF_Param_POPUP;
		PF_STRNNCPY(def.PF_DEF_NAME, name, sizeof(def.PF_DEF_NAME));
		def.u.pd.num_choices = num_choices;
		def.u.pd.dephault = default_value;
		def.u.pd.value = def.u.pd.dephault;
		def.u.pd.u.namesptr = items_string;
		def.uu.id = id;
		def.flags = flags;
		def.ui_flags = ui_flags;
		err = in_data->inter.add_param(in_data->effect_ref, -1, &def);
		if (!err) param_count++;
		return err;
	}
	PF_Err AddPoint(
		const char* name,
		A_long x_default,
		A_long y_default,
		PF_Boolean restrict_bounds,
		A_long id,
		PF_ParamFlags flags = PF_ParamFlag_NONE,
		PF_ParamUIFlags ui_flags = PF_PUI_NONE
	)
	{
		PF_Err			err = PF_Err_NONE;
		ClearDef();
		def.param_type = PF_Param_POINT;
		PF_STRNNCPY(def.PF_DEF_NAME, name, sizeof(def.PF_DEF_NAME));
		def.u.td.restrict_bounds = restrict_bounds;
		def.u.td.x_value = def.u.td.x_dephault = (x_default << 16);
		def.u.td.y_value = def.u.td.y_dephault = (y_default << 16);
		def.uu.id = id;
		def.flags = flags;
		def.ui_flags = ui_flags;
		err = in_data->inter.add_param(in_data->effect_ref, -1, &def);
		if (!err) param_count++;
		return err;
	}
	PF_Err AddButton(
		const char* name,
		const char* btn_name,
		A_long id,
		PF_ParamFlags flags = PF_ParamFlag_NONE,
		PF_ParamUIFlags ui_flags = PF_PUI_NONE
	)
	{
		PF_Err			err = PF_Err_NONE;
		ClearDef();
		def.param_type = PF_Param_BUTTON;
		PF_STRNNCPY(def.PF_DEF_NAME, name, sizeof(def.PF_DEF_NAME));
		def.u.button_d.u.namesptr = btn_name;
		def.uu.id = id;
		def.flags = flags;
		def.ui_flags = ui_flags;
		err = in_data->inter.add_param(in_data->effect_ref, -1, &def);
		if (!err) param_count++;
		return err;
	}
	PF_Err AddLayer(
		const char* name,
		A_long default_value,
		A_long id,
		PF_ParamFlags flags = PF_ParamFlag_NONE,
		PF_ParamUIFlags ui_flags = PF_PUI_NONE
	)
	{
		PF_Err			err = PF_Err_NONE;
		ClearDef();
		def.param_type = PF_Param_LAYER;
		PF_STRNNCPY(def.PF_DEF_NAME, name, sizeof(def.PF_DEF_NAME));
		def.u.ld.dephault = (A_long)default_value;
		def.uu.id = id;
		def.flags = flags;
		def.ui_flags = ui_flags;
		err = in_data->inter.add_param(in_data->effect_ref, -1, &def);
		if (!err) param_count++;
		return err;
	}
	/*
	* #define PF_ADD_NULL(NAME, ID) \
	do {\
		PF_Err	priv_err = PF_Err_NONE; \
		def.param_type = PF_Param_NO_DATA; \
		PF_STRNNCPY(def.name, (NAME), sizeof(def.name) ); \
		def.uu.id = (ID); \
		if ((priv_err = PF_ADD_PARAM(in_data, -1, &def)) != PF_Err_NONE) return priv_err; \
	} while (0)
*/
	PF_Err AddNull(
		const char* name,
		A_long id
	)
	{
		PF_Err			err = PF_Err_NONE;
		ClearDef();
		def.param_type = PF_Param_NO_DATA;
		PF_STRNNCPY(def.PF_DEF_NAME, name, sizeof(def.PF_DEF_NAME));
		def.uu.id = id;
		err = in_data->inter.add_param(in_data->effect_ref, -1, &def);
		if (!err) param_count++;
		return err;
	}
};

#endif // CPARAMSSETUP_H