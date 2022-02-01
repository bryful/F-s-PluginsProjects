//-----------------------------------------------------------------------------------
/*
	Spark for VS2010
*/
//-----------------------------------------------------------------------------------

#pragma once
#ifndef Spark_H
#define Spark_H

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
	ID_STARTRANDX,
	ID_STARTRANDY,

	ID_LAST,
	ID_LASTRANDX,
	ID_LASTRANDY,

	ID_LASTRANDR,
	ID_LASTRANDSEED,

	ID_LINESIZE,
	ID_LINEMOVE,

	ID_SUB_COUNT,
	ID_FOLD_COUNT,

	ID_DRAW_COUNT,



	ID_COLOR,
	ID_BLEND,

	ID_NUM_PARAMS
};

//UIの表示文字列
#define	STR_SEED		"seed"
#define	STR_SEEDPOS		"seedPos"
#define	STR_SEEDMOVE	"seedMove"
#define	STR_WIPE		"wipe(%)"

#define	STR_OFFSET		"offset"

#define	STR_START		"start"
#define	STR_LAST		"last"

#define	STR_STARTRANDX	"startRandX"
#define	STR_STARTRANDY	"startRandY"

#define	STR_LASTRANDX	"lastRandX"
#define	STR_LASTRANDY	"lastRandY"

#define	STR_LASTRANDR	"LastRandRot"
#define	STR_LASTROTSEED "LastRotSeed"


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
	PointInfo	start;
	A_long		startRandX;
	A_long		startRandY;

	PointInfo	last;
	A_long		lastRandX;
	A_long		lastRandY;

	PF_FpLong	lastRandR;
	A_long		lastRandSeed;

	PF_FpLong	lineSize;
	A_long		lineMove;
	A_long		foldCount;
	A_long		drawCount;
	A_long		subCount;

	PF_FpLong	offset;
	A_long		seed;
	A_long		seedPos;
	A_long		seedMove;
	A_long		frame;

	PF_FpLong	wipe;


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

#endif // Spark_H
