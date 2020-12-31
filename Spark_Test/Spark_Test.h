//-----------------------------------------------------------------------------------
/*
	Spark_Test for VS2010
*/
//-----------------------------------------------------------------------------------

#pragma once
#ifndef Spark_Test_H
#define Spark_Test_H

#include "Fs_Target.h"

#include "../FsLibrary/Fs.h"
#include "../FsLibrary/FsAE.h"


#include "..\Spark\CLineDraw.h"
#include "..\Spark\CPointInfo.h"


//ユーザーインターフェースのID
//ParamsSetup関数とRender関数のparamsパラメータのIDになる
enum {
	ID_INPUT = 0,	// default input layer

	ID_START_P,
	ID_START_S,

	ID_LAST_P,
	ID_LAST_S,

	ID_COLOR,
	ID_BLEND,

	ID_NUM_PARAMS
};

//UIの表示文字列

#define	STR_START_P		"start_point"
#define	STR_START_S		"start_side"
#define	STR_LAST_P		"last_point"
#define	STR_LAST_S		"last_size"



#define	STR_COLOR		"color"
#define	STR_BLEND		"blend"
#define	STR_ON			"on"



//-----------------------------------------------------------------------------------
typedef struct ParamInfo {

	PointInfo	start;
	PointInfo	last;

	PF_Pixel	color;
	PF_Boolean	blend;

} ParamInfo, * ParamInfoP, ** ParamInfoH;
//-----------------------------------------------------------------------------------
extern "C" {

DllExport 
PF_Err 
EntryPointFunc (	
	PF_Cmd			cmd,
	PF_InData		*in_data,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output,
	void			*extra);
}

#endif // Spark_Test_H
