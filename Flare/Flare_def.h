#pragma once

PF_Boolean FI_Enabled[PCOUNT] = { FALSE,FALSE, FALSE, FALSE, FALSE };

A_long FI_REV[PCOUNT] = { 1, 1, 1, 1, 1 };

PF_Boolean FI_BORDER[PCOUNT] = { FALSE,FALSE, FALSE, FALSE, FALSE };

A_long FI_INSIDE[PCOUNT] = { 1, 1, 1, 1, 1 };

A_long FI_OUTSIDE[PCOUNT] = { 1, 1, 1, 1, 1 };

A_long FI_MAX[PCOUNT] = { 0, 0, 0, 0, 0 };

A_long FI_BLUR[PCOUNT] = { 0, 0, 0, 0, 0 };

PF_Pixel FI_COLOR[PCOUNT] = {
	{0xFF,0xFF,0xFF,0xFF},
	{0xFF,0xFF,0xFF,0xFF},
	{0xFF,0xFF,0xFF,0xFF},
	{0xFF,0xFF,0xFF,0xFF},
	{0xFF,0xFF,0xFF,0xFF} };

A_long FI_BLEND[PCOUNT] = { 1, 1, 1, 1, 1 };



PF_FpLong FI_OPACITY[PCOUNT] = { 75, 35, 20, 100, 100 };