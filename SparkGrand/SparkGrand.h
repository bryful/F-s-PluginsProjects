//-----------------------------------------------------------------------------------
/*
	SparkGrand for VS2010
*/
//-----------------------------------------------------------------------------------

#pragma once
#ifndef SparkGrand_H
#define SparkGrand_H

#include "Fs_Target.h"

#include "../FsLibrary/Fs.h"
#include "../FsLibrary/FsAE.h"


#include "..\Spark\CLineDraw.h"
#include "..\Spark\CPointInfo.h"


//ユーザーインターフェースのID
//ParamsSetup関数とRender関数のparamsパラメータのIDになる
enum {
	ID_INPUT = 0,	// default input layer

	ID_SEED,
	ID_SEEDPOS,
	ID_SEEDMOVE,
	ID_OFFSET,
	ID_WIPE,

	ID_START,
	ID_LAST1,
	ID_LAST2,

	ID_DRAW_COUNT,

	ID_START_RX,
	ID_START_RY,

	ID_LAST_RX,
	ID_LAST_RY,


	ID_LINE_SIZE,
	ID_LINE_MOVE,
	ID_FOLD_COUNT,
	ID_COLOR,
	ID_BLEND,

	ID_NUM_PARAMS
};



//UIの表示文字列
#define	STR_SEED		"seed"
#define	STR_SEEDPOS		"seedPos"
#define	STR_SEEDMOVE	"seedMove"
#define	STR_OFFSET		"offset"
#define	STR_WIPE		"wipe"

#define	STR_START		"start"
#define	STR_LAST1		"last1"
#define	STR_LAST2		"last2"

#define	STR_START_RX	"startRandX"
#define	STR_START_RY	"startRandY"

#define	STR_LAST_RX		"lastRandX"
#define	STR_LAST_RY		"laxtRandY"


#define	STR_LINE_SIZE	"lineSize"
#define	STR_LINE_MOVE	"lineMove"
#define	STR_FOLDCOUNT	"foldCount"
#define	STR_SUBCOUNT	"subCount"
#define	STR_DRAWCOUNT	"drawCount"

#define	STR_COLOR		"color"
#define	STR_BLEND		"blend"
#define	STR_ON			"on"


#define POINT_COUNT_MAX	36
//-----------------------------------------------------------------------------------
typedef struct ParamInfo {
	A_long		frame;
	A_long		seed;
	A_long		seedPos;
	A_long		seedMove;
	PF_FpLong	offset;
	PF_FpLong	wipe;

	PointInfo	start;
	PointInfo	last1;
	PointInfo	last2;

	A_long		startRandX;
	A_long		startRandY;
	A_long		lastRandX;
	A_long		lastRandY;


	PF_FpLong	lineSize;
	A_long		lineMove;
	A_long		foldCount;
	A_long		drawCount;


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

#endif // SparkGrand_H
