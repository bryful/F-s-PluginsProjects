#pragma once



PF_Boolean FI_Enabled[PCOUNT] = { FALSE,FALSE, FALSE, FALSE, FALSE };

A_long FI_REV[PCOUNT] = { 1, 1, 1, 1, 1 };

PF_Boolean FI_BORDER[PCOUNT] = { FALSE,FALSE, FALSE, TRUE, FALSE };

A_long FI_INSIDE[PCOUNT] = { 1, 1, 1, 2, 1 };

A_long FI_OUTSIDE[PCOUNT] = { 1, 1, 1, 1, 1 };

A_long FI_MAX[PCOUNT] = { 10, 3, 1, 0, 0 };

A_long FI_BLUR[PCOUNT] = { 70, 20, 5, 8, 0 };

PF_Pixel FI_COLOR[PCOUNT] = {
	{0xFF,0xFE,0xFF,0xA2},//FEFFA2
	{0xFF,0xFF,0xF5,0x51},//FFF551
	{0xFF,0xFF,0xF5,0x51},//FFF551
	{0xFF,0xCC,0xFF,0x1A},//CCFF1A
	{0xFF,0xFF,0xFF,0xFF} };

A_long FI_BLEND[PCOUNT] = { 1, 1, 1, 3, 1 };

PF_FpLong FI_OPACITY[PCOUNT] = { 70, 80, 100, 50, 100 };