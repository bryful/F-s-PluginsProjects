#pragma once
#pragma once
#ifndef NF_DRAWWORLD_H
#define NF_DRAWWORLD_H


#include "AEConfig.h" 
#include "entry.h"

#ifdef AE_OS_WIN
#include "string.h"
#endif
#include "AE_Effect.h"
#include "AE_EffectCB.h"
#include "AE_EffectCBSuites.h"
#include "AE_Macros.h"
#include "AEGP_SuiteHandler.h"
#include "String_Utils.h"
#include "Param_Utils.h"
#include "Smart_Utils.h"
#include "AE_GeneralPlug.h"

#include "AEFX_SuiteHelper.h"
#define refconType void*

#include "..\NF_Utils.h"
#include <vector>

#include "NF_Draw.h"
#include "NF_Paint.h"


class DrawWorld {
private:
    std::vector<A_long> Y_ADR;
	A_long pixelSize = 4;
protected:
public:
    PF_PixelFormat pixelFormat = PF_PixelFormat_INVALID;
    PF_EffectWorld* world =nullptr;
    DrawWorld(PF_EffectWorld* w, PF_PixelFormat pf)
    {
		pixelFormat = pf;
        world = w;
        // Y_ADR 配列の準備
        A_long height = world->height;
        A_long rowbytes = world->rowbytes;
        Y_ADR.resize(height);
        for (A_long y = 0; y < height; ++y) {
            Y_ADR[y] = y * rowbytes;
		}
        switch (pf)
        {
            case PF_PixelFormat_ARGB128:
                pixelSize = sizeof(PF_PixelFloat);
				break;
            case PF_PixelFormat_ARGB64:
				pixelSize = sizeof(PF_Pixel16);
                break;
			case PF_PixelFormat_ARGB32:
                pixelSize = sizeof(PF_Pixel);
                break;
        default:
            break;
        }
    }
	PF_Err Line(A_long x0, A_long y0, A_long x1, A_long y1, PF_Pixel color)
    {
        return DrawLine(world, pixelFormat, x0, y0, x1, y1, color);
	}
    PF_Err Box(A_long x0, A_long y0, A_long x1, A_long y1, PF_Pixel color)
    {
        return DrawBox(world, pixelFormat, x0, y0, x1, y1, color);
	}
    PF_Err BoxFill(A_long x0, A_long y0, A_long x1, A_long y1, PF_Pixel color)
    {
        return DrawBoxFill(world, pixelFormat, x0, y0, x1, y1, color);
	}
    PF_Err Circle(PF_Point pos, A_long radius, PF_Pixel color)
    {
        return DrawCircle(world, pixelFormat, pos, radius, color);
    }
    PF_Err CircleFill(PF_Point pos, A_long radius, PF_Pixel color)
    {
        return DrawCircleFill(world, pixelFormat, pos, radius, color);
	}
    PF_Err FloodFill(A_long x, A_long y, PF_Pixel color)
    {
		return Paint(world, pixelFormat, x, y, color);
    }
};
#endif // NF_DRAWWORLD_H