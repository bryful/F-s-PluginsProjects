//-----------------------------------------------------------------------------------
/*
	SparkRing for VS2010
*/
//-----------------------------------------------------------------------------------

#pragma once
#ifndef SparkRing_H
#define SparkRing_H

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
	ID_CENTER,
	ID_RADIUS,
	ID_ASPECT,
	ID_POINT_COUNT,
	ID_POIN_ROT,
	ID_POINT_RAND,
	ID_ROT,
	ID_LINE_SIZE,
	ID_LINE_MOVE,
	ID_FOLD_COUNT,
	ID_SUB_COUNT,
	ID_DRAW_COUNT,
	ID_COLOR,
	ID_BLEND,

	ID_NUM_PARAMS
};



//UIの表示文字列
#define	STR_SEED		"seed"
#define	STR_SEEDPOS		"seedPos"
#define	STR_SEEDMOVE	"seedMove"
#define	STR_OFFSET		"offset"

#define	STR_CENTER		"center"
#define	STR_RADIUS		"radius"
#define	STR_ASPECT		"aspect"

#define	STR_POINT_COUNT	"point_count"
#define	STR_POINT_ROT	"point_rot"
#define	STR_POINT_ROND	"point_rand"

#define	STR_ROT			"rot"


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

	A_LPoint	center;
	A_long		radius;
	PF_FpLong	aspect;
	A_long		point_count;
	PF_FpLong	point_rot;
	A_long		point_round;
	PF_FpLong	rot;

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

#endif // SparkRing_H
