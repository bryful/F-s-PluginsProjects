//-----------------------------------------------------------------------------------
/*
	blue_Text for VS2010
*/
//-----------------------------------------------------------------------------------

#pragma once
#ifndef blue_Text_H
#define blue_Text_H

#include "../FsLibrary/Fs.h"
#include "Fs_Target.h"
#include "../FsLibrary/FsAE.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <algorithm>
#include "TinyBlue.h"

//ユーザーインターフェースのID
//ParamsSetup関数とRender関数のparamsパラメータのIDになる
enum {
	ID_INPUT = 0,	// default input layer

	ID_RADIUS,

	ID_NUM_PARAMS
};


//UIの表示文字列
#define	STR_RADIUS			"radius"
//UIのパラメータ
typedef struct {
	A_long				radius;
} ParamInfo;


//-----------------------------------------------------------------------------------
extern "C" {

DllExport 
PF_Err 
EffectMain(
	PF_Cmd			cmd,
	PF_InData		*in_data,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output,
	void			*extra);
}
#endif // blue_Text_H
