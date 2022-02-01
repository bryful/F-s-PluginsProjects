//-----------------------------------------------------------------------------------
/*
	SparkStorm for VS2010
*/
//-----------------------------------------------------------------------------------

#pragma once
#ifndef SparkStorm_H
#define SparkStorm_H

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
	ID_SEEDDRAW,
	ID_WIPE,

	ID_START_P0,
	ID_START_P1,
	ID_LAST_P0,
	ID_LAST_P1,

	ID_DRAW_COUNT,


	ID_START_RX,
	ID_START_RY,
	ID_LAST_RX,
	ID_LAST_RY,

	ID_LINESIZE,
	ID_LINEMOVE,
	
	ID_SUB_COUNT,
	ID_FOLD_COUNT,




	ID_COLOR,
	ID_BLEND,

	ID_NUM_PARAMS
};

//UIの表示文字列
#define	STR_SEED		"seed"
#define	STR_SEEDPOS		"seedPos"
#define	STR_SEEDMOVE	"seedMove"
#define	STR_SEEDDRAW	"seedDraw"
#define	STR_OFFSET		"offset"
#define	STR_WIPE		"wipe(%)"

#define	STR_START_P0	"start0"
#define	STR_START_P1	"start1"

#define	STR_LAST_P0		"last0"
#define	STR_LAST_P1		"last1"

#define	STR_START_RX	"first_randX"
#define	STR_START_RY	"first_randY"
#define	STR_LAST_RX		"last_randX"
#define	STR_LAST_RY		"last_randY"

#define	STR_LINESIZE	"lineSize"
#define	STR_LINEMOVE	"lineMove"
#define	STR_FOLDCOUNT	"foldCount"
#define	STR_DRAWCOUNT	"drawCount"
#define	STR_SUBCOUNT	"subCount"


#define	STR_COLOR		"color"
#define	STR_BLEND		"blend"
#define	STR_ON			"on"



//-----------------------------------------------------------------------------------
typedef struct ParamInfo {
	A_long		frame;
	A_long		seed;
	A_long		seedPos;
	A_long		seedMove;
	A_long		seedDraw;
	PF_FpLong	offset;
	PF_FpLong	wipe;

	PointInfo	point[4];
	A_long		startRandX;
	A_long		startRandY;
	A_long		lastRandX;
	A_long		lastRandY;

	PF_FpLong	lineSize;
	A_long		lineMove;
	A_long		foldCount;
	A_long		drawCount;
	A_long		subCount;

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

#endif // SparkStorm_H
