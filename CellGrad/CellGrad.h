//-----------------------------------------------------------------------------------
/*
	CellGrad for VS2010
*/
//-----------------------------------------------------------------------------------

#pragma once
#ifndef CellGrad_H
#define CellGrad_H

#include "../FsLibrary/Fs.h"

#include "Fs_Target.h"

#include "../FsLibrary/FGrad.h"
#include "../FsLibrary/FsAE.h"
#include "../FsLibrary/FWorld.h"
#include "FWorldGrad.h"
#include "FRectGrad.h"

//ユーザーインターフェースのID
//ParamsSetup関数とRender関数のparamsパラメータのIDになる
enum {
	ID_INPUT = 0,	// default input layer

	ID_TARGET_TOPIC,
	ID_TARGET_COUNT,
	ID_TARGET_LEVEL,
	ID_TARGET_COL1,
	ID_TARGET_COL2,
	ID_TARGET_COL3,
	ID_TARGET_COL4,
	ID_TARGET_COL5,
	ID_TARGET_COL6,
	ID_TARGET_COL7,
	ID_TARGET_COL8,
	ID_TARGET_TOPIC_END,

	ID_ANGLE,
	ID_START_OVER,
	ID_LAST_OVER,

	ID_START_COL,
	ID_LAST_COL,

	ID_GUIDE_SHOW,
	ID_GUIDE_COL,

	ID_NUM_PARAMS
};

//UIの表示文字列
#define	STR_TARGET_TOPIC	"target"
#define	STR_TARGET_LEVEL	"targetLevel"
#define	STR_TARGET_COUNT	"targetCount"
#define	STR_TARGET_COL1		"color1"
#define	STR_TARGET_COL2		"color2"
#define	STR_TARGET_COL3		"color3"
#define	STR_TARGET_COL4		"color4"
#define	STR_TARGET_COL5		"color5"
#define	STR_TARGET_COL6		"color6"
#define	STR_TARGET_COL7		"color7"
#define	STR_TARGET_COL8		"color8"

#define	STR_ANGLE			"angle"

#define	STR_START_OVER		"startOver"
#define	STR_LAST_OVER		"lastOver"

#define	STR_START_COL		"startCol"
#define	STR_LAST_COL		"lastCol"

#define	STR_GUIDE_SHOW		"showGuide"
#define	STR_GUIDE_COLOR		"showGuide"
#define	STR_ON				"on"

//UIのパラメータ
typedef struct ParamInfo {
	A_long			targetCount;
	A_u_char		tagetLevel;
	PF_Pixel		targetCol[8];

	A_FloatPoint startPos;
	A_FloatPoint lastPos;

	PF_FpLong		angle;

	PF_FpLong		startOver;
	PF_FpLong		lastOver;
	PF_Pixel		startCol;
	PF_Pixel		lastCol;
	PF_Boolean		guideShow;
	PF_Pixel		guideCol;


} ParamInfo, *ParamInfoP, **ParamInfoH;

//-------------------------------------------------------


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
#endif // CellGrad_H
