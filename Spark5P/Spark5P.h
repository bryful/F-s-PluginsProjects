//-----------------------------------------------------------------------------------
/*
	Spark5P for VS2010
*/
//-----------------------------------------------------------------------------------

#pragma once
#ifndef Spark5P_H
#define Spark5P_H

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

	ID_FIRST_P,
	ID_FIRST_RX,
	ID_FIRST_RY,

	ID_2ND_CB,
	ID_2ND_P,
	ID_3RD_CB,
	ID_3RD_P,
	ID_4TH_CB,
	ID_4TH_P,

	ID_MID_RX,
	ID_MID_RY,

	ID_LAST_P,
	ID_LAST_RX,
	ID_LAST_RY,

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
#define	STR_OFFSET		"offset"

#define	STR_FIRST		"first_point"
#define	STR_FIRST_RX	"first_randX"
#define	STR_FIRST_RY	"first_randY"

#define	STR_2ND_CB		"2nd"
#define	STR_2ND_P		"2nd_point"
#define	STR_3RD_CB		"3rd"
#define	STR_3RD_P		"3rd_point"
#define	STR_4TH_CB		"4th"
#define	STR_4TH_P		"4th_point"

#define	STR_MID_RX		"mid_randX"
#define	STR_MID_RY		"mid_randY"

#define	STR_LAST		"last_point"
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
	PF_FpLong	offset;

	PointInfo	point[5];
	A_long		randX[5];
	A_long		randY[5];
	PF_Boolean	pointON[5];

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

#endif // Spark5P_H
