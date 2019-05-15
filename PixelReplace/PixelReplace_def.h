#pragma once
#ifndef COLOR_DEF_H
#define COLOR_DEF_H

#define COLOR_DEF_SIZE	16

A_u_char pr_colorDef[COLOR_DEF_SIZE][6]={
	{	//00
		PF_MAX_CHAN8,//R1
		0,//G1
		0,//B1
		PF_MAX_CHAN8,//R1
		0,//G1
		0//B1
	},
	{	//01
		0,//R1
		PF_MAX_CHAN8,//G1
		0,//B1
		0,//R1
		PF_MAX_CHAN8,//G1
		0//B1
	},
	{	//02
		0,//R1
		0,//G1
		PF_MAX_CHAN8,//B1
		0,//R1
		0,//G1
		PF_MAX_CHAN8//B1
	},
	{	//03
		PF_MAX_CHAN8,//R1
		PF_MAX_CHAN8,//G1
		0,//B1
		PF_MAX_CHAN8,//R1
		PF_MAX_CHAN8,//G1
		0//B1
	},
	{	//04
		PF_MAX_CHAN8,//R1
		0,//G1
		PF_MAX_CHAN8,//B1
		PF_MAX_CHAN8,//R1
		0,//G1
		PF_MAX_CHAN8//B1
	},
	{	//05
		0,//R1
		PF_MAX_CHAN8,//G1
		PF_MAX_CHAN8,//B1
		0,//R1
		PF_MAX_CHAN8,//G1
		PF_MAX_CHAN8//B1
	},
	{	//06
		PF_MAX_CHAN8,//R1
		PF_MAX_CHAN8,//G1
		PF_MAX_CHAN8,//B1
		PF_MAX_CHAN8,//R1
		PF_MAX_CHAN8,//G1
		PF_MAX_CHAN8//B1
	},
	{	//07
		0,//R1
		0,//G1
		0,//B1
		0,//R1
		0,//G1
		0//B1
	},
	{	//08
		PF_HALF_CHAN8,//R1
		0,//G1
		0,//B1
		PF_HALF_CHAN8,//R1
		0,//G1
		0//B1
	},
	{	//09
		0,//R1
		PF_HALF_CHAN8,//G1
		0,//B1
		0,//R1
		PF_HALF_CHAN8,//G1
		0//B1
	},
	{	//10
		0,//R1
		0,//G1
		PF_HALF_CHAN8,//B1
		0,//R1
		0,//G1
		PF_HALF_CHAN8//B1
	},
	{	//11
		PF_HALF_CHAN8,//R1
		PF_HALF_CHAN8,//G1
		0,//B1
		PF_HALF_CHAN8,//R1
		PF_HALF_CHAN8,//G1
		0//B1
	},
	{	//12
		PF_HALF_CHAN8,//R1
		0,//G1
		PF_HALF_CHAN8,//B1
		PF_HALF_CHAN8,//R1
		0,//G1
		PF_HALF_CHAN8//B1
	},
	{	//13
		0,//R1
		PF_HALF_CHAN8,//G1
		PF_HALF_CHAN8,//B1
		0,//R1
		PF_HALF_CHAN8,//G1
		PF_HALF_CHAN8//B1
	},
	{	//14
		PF_HALF_CHAN8,//R1
		PF_HALF_CHAN8,//G1
		PF_HALF_CHAN8,//B1
		PF_HALF_CHAN8,//R1
		PF_HALF_CHAN8,//G1
		PF_HALF_CHAN8//B1
	},
	{	//15
		PF_HALF_CHAN8/2,//R1
		PF_HALF_CHAN8/2,//G1
		PF_HALF_CHAN8/2,//B1
		PF_HALF_CHAN8/2,//R1
		PF_HALF_CHAN8/2,//G1
		PF_HALF_CHAN8/2//B1
	},
};
#endif 