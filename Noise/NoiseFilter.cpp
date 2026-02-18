#include "Noise.h"


typedef struct {
    A_long seed;
    PF_EffectWorld* world;
    A_long widthTrue;
    A_long width;
    A_long height;
    A_long rowbytes;
    A_long heights[8];

	A_u_long  noiseAmount;
    A_u_long  noiseAmounts[8];
    float   noiseIntensity;
    A_long  taskCount;
    PF_Boolean isColor;

}NoiseInfo;


// テンプレート版
template <typename PixelType, typename ChannelType, int MaxChannelValue>
static PF_Err NoiseTemplate(
    void* refconPV, 
    A_long thread_idxL, 
    A_long cnt, 
    A_long itrtL)
{
    PF_Err err = PF_Err_NONE;
    NoiseInfo* infoP = static_cast<NoiseInfo*>(refconPV);
    
    if (cnt < 0 || cnt >= infoP->taskCount) return err;
    
    // 各タスクの開始Y座標を累積で計算
    A_long yBase = 0;
    for (A_long i = 0; i < cnt; i++) {
        yBase += infoP->heights[i];
    }
    
    A_long widthTrue = infoP->widthTrue;
    A_long rowbytes = infoP->rowbytes;
    A_long width = infoP->width;
    A_long height = infoP->height;
    char* baseData = (char*)infoP->world->data;
    float nmx = (float)(MaxChannelValue * infoP->noiseIntensity);
    PF_Boolean isColor = infoP->isColor;

    for(A_u_long idx = 0; idx < infoP->noiseAmounts[cnt]; idx++){
        A_long dy = (A_long)(hash_float(idx, cnt+20, infoP->seed) * ((float)infoP->heights[cnt]));
        A_long y = yBase + dy;
        if (cnt == 0 && y <= 0) y = 1;
        if (cnt == infoP->taskCount - 1 && y >= height - 1) y = height - 2;
        A_long x = (A_long)(1 + hash_float(idx, cnt*2+100, infoP->seed) * ((float)width - 2));

		if (x < 1) x = 1;
		else if (x >= width - 1) x = width - 2;

        float ar = hash_float(idx, 200+cnt*3, infoP->seed) * -2.0f * nmx + nmx;
        float ag = isColor ? (hash_float(idx, cnt*2 + 1300, infoP->seed) * -2.0f * nmx + nmx) : ar;
        float ab = isColor ? (hash_float(idx, cnt + 400, infoP->seed) * -2.0f * nmx + nmx) : ar;
        
        PixelType* p = reinterpret_cast<PixelType*>(baseData + (y * rowbytes)) + x;
        
        if ((int)(hash_float(idx, cnt + -50, infoP->seed) * 100) < (int)(100*infoP->noiseIntensity)) {
            if (x > 2 && x < width - 4 && y>2 && y < height - 3)
            {
                int f = (int)(hash_float(idx, cnt + 50, infoP->seed) * 3);
                if (f == 0) {
                    *(p - 1 - widthTrue) = *(p - widthTrue);
                    *(p + 0 - widthTrue) = *(p - widthTrue + 1);
                    *(p + 1 - widthTrue) = *(p - widthTrue + 2);
                    *(p - 1) = *(p);
                    *(p + 0) = *(p + 1);
                    *(p + 1) = *(p + 2);
                    *(p - 1 + widthTrue) = *(p + widthTrue);
                    *(p + 0 + widthTrue) = *(p + widthTrue + 1);
                    *(p + 1 + widthTrue) = *(p + widthTrue + 2);
                }
                else if (f==1) {
                    *(p + 1 - widthTrue) = *(p - widthTrue);
                    *(p + 0 - widthTrue) = *(p - widthTrue - 1);
                    *(p - 1 - widthTrue) = *(p - widthTrue - 2);
                    *(p + 1) = *(p);
                    *(p + 0) = *(p - 1);
                    *(p - 1) = *(p - 2);
                    *(p + 1 + widthTrue) = *(p + widthTrue);
                    *(p + 0 + widthTrue) = *(p + widthTrue - 1);
                    *(p - 1 + widthTrue) = *(p + widthTrue - 2);
                }
                else if (f == 2) {
                    *(p - 1 - widthTrue) = *(p -1);
                    *(p + 0 - widthTrue) = *(p);
                    *(p + 1 - widthTrue) = *(p +1);
                    *(p - 1) = *(p - 1 + widthTrue);
                    *(p + 0) = *(p + 0 + widthTrue);
                    *(p + 1) = *(p + 1 +widthTrue);
                    *(p - 1 + widthTrue) = *(p-1 + widthTrue*2);
                    *(p + 0 + widthTrue) = *(p+0 + widthTrue*2);
                    *(p + 1 + widthTrue) = *(p+1 + widthTrue*2);
                }
                else if (f == 3) {
                    *(p - 1 - widthTrue) = *(p - 1 - widthTrue * 2);
                    *(p + 0 - widthTrue) = *(p + 0 - widthTrue * 2);
                    *(p + 1 - widthTrue) = *(p + 1 - widthTrue * 2);
                    *(p - 1) = *(p - 1 + widthTrue);
                    *(p + 0) = *(p + 0 + widthTrue);
                    *(p + 1) = *(p + 1 + widthTrue);
                    *(p - 1 + widthTrue) = *(p - 1);
                    *(p + 0 + widthTrue) = *(p + 0);
                    *(p + 1 + widthTrue) = *(p + 1);
                }
            }
        }
        
        p->red   = (ChannelType)AE_CLAMP((float)p->red + ar, 0, MaxChannelValue);
        p->green = (ChannelType)AE_CLAMP((float)p->green + ag, 0, MaxChannelValue);
        p->blue  = (ChannelType)AE_CLAMP((float)p->blue + ab, 0, MaxChannelValue);
        
        ar *= 0.5f; ag *= 0.5f; ab *= 0.5f;
        
        (p - widthTrue)->red   = (ChannelType)AE_CLAMP((float)(p - widthTrue)->red + ar, 0, MaxChannelValue);
        (p - widthTrue)->green = (ChannelType)AE_CLAMP((float)(p - widthTrue)->green + ag, 0, MaxChannelValue);
        (p - widthTrue)->blue  = (ChannelType)AE_CLAMP((float)(p - widthTrue)->blue + ab, 0, MaxChannelValue);
        
        (p + widthTrue)->red   = (ChannelType)AE_CLAMP((float)(p + widthTrue)->red + ar, 0, MaxChannelValue);
        (p + widthTrue)->green = (ChannelType)AE_CLAMP((float)(p + widthTrue)->green + ag, 0, MaxChannelValue);
        (p + widthTrue)->blue  = (ChannelType)AE_CLAMP((float)(p + widthTrue)->blue + ab, 0, MaxChannelValue);
        
        (p - 1)->red   = (ChannelType)AE_CLAMP((float)(p - 1)->red + ar, 0, MaxChannelValue);
        (p - 1)->green = (ChannelType)AE_CLAMP((float)(p - 1)->green + ag, 0, MaxChannelValue);
        (p - 1)->blue  = (ChannelType)AE_CLAMP((float)(p - 1)->blue + ab, 0, MaxChannelValue);
        
        (p + 1)->red   = (ChannelType)AE_CLAMP((float)(p + 1)->red + ar, 0, MaxChannelValue);
        (p + 1)->green = (ChannelType)AE_CLAMP((float)(p + 1)->green + ag, 0, MaxChannelValue);
        (p + 1)->blue  = (ChannelType)AE_CLAMP((float)(p + 1)->blue + ab, 0, MaxChannelValue);
        
        ar *= 0.5f; ag *= 0.5f; ab *= 0.5f;
        
        (p - widthTrue - 1)->red   = (ChannelType)AE_CLAMP((float)(p - widthTrue - 1)->red + ar, 0, MaxChannelValue);
        (p - widthTrue - 1)->green = (ChannelType)AE_CLAMP((float)(p - widthTrue - 1)->green + ag, 0, MaxChannelValue);
        (p - widthTrue - 1)->blue  = (ChannelType)AE_CLAMP((float)(p - widthTrue - 1)->blue + ab, 0, MaxChannelValue);
        
        (p - widthTrue + 1)->red   = (ChannelType)AE_CLAMP((float)(p - widthTrue + 1)->red + ar, 0, MaxChannelValue);
        (p - widthTrue + 1)->green = (ChannelType)AE_CLAMP((float)(p - widthTrue + 1)->green + ag, 0, MaxChannelValue);
        (p - widthTrue + 1)->blue  = (ChannelType)AE_CLAMP((float)(p - widthTrue + 1)->blue + ab, 0, MaxChannelValue);
        
        (p + widthTrue - 1)->red   = (ChannelType)AE_CLAMP((float)(p + widthTrue - 1)->red + ar, 0, MaxChannelValue);
        (p + widthTrue - 1)->green = (ChannelType)AE_CLAMP((float)(p + widthTrue - 1)->green + ag, 0, MaxChannelValue);
        (p + widthTrue - 1)->blue  = (ChannelType)AE_CLAMP((float)(p + widthTrue - 1)->blue + ab, 0, MaxChannelValue);
        
        (p + widthTrue + 1)->red   = (ChannelType)AE_CLAMP((float)(p + widthTrue + 1)->red + ar, 0, MaxChannelValue);
        (p + widthTrue + 1)->green = (ChannelType)AE_CLAMP((float)(p + widthTrue + 1)->green + ag, 0, MaxChannelValue);
        (p + widthTrue + 1)->blue  = (ChannelType)AE_CLAMP((float)(p + widthTrue + 1)->blue + ab, 0, MaxChannelValue);
    }

    return err;
}

// 8bit版
static PF_Err Noise8(void* refconPV, A_long thread_idxL, A_long cnt, A_long itrtL)
{
    return NoiseTemplate<PF_Pixel, A_u_char, PF_MAX_CHAN8>(refconPV, thread_idxL, cnt, itrtL);
}

// 16bit版
static PF_Err Noise16(void* refconPV, A_long thread_idxL, A_long cnt, A_long itrtL)
{
    return NoiseTemplate<PF_Pixel16, A_u_short, PF_MAX_CHAN16>(refconPV, thread_idxL, cnt, itrtL);
}

// 32bit版
static PF_Err Noise32(void* refconPV, A_long thread_idxL, A_long cnt, A_long itrtL)
{
    return NoiseTemplate<PF_PixelFloat, float, 1>(refconPV, thread_idxL, cnt, itrtL);
}


PF_Err NoiseMain(
    NF_AE *ae,
    PF_EffectWorld* worldP,
    ParamInfo* infoP
)
{
    PF_Err err = PF_Err_NONE;
    if (infoP->noiseAmount<=0||infoP->noiseIntensity<=0) return err;
    
    NoiseInfo info;
    AEFX_CLR_STRUCT(info);
    info.seed = infoP->seed;
    if(infoP->autoSeed)
    {
        info.seed += ae->frame();
    }
    info.world = worldP;
    info.width = worldP->width;
    info.height = worldP->height;
    info.rowbytes = worldP->rowbytes;
    
    // 画像サイズを考慮したノイズ個数の計算
    A_u_long totalPixels = static_cast<A_u_long>(worldP->width) * static_cast<A_u_long>(worldP->height);
    info.noiseAmount = static_cast<A_u_long>(infoP->noiseAmount * totalPixels);
    info.noiseIntensity = static_cast<float>(infoP->noiseIntensity);
    info.isColor = infoP->isColor;
    
    switch (ae->pixelFormat())
    {
    case PF_PixelFormat_ARGB32:
        info.widthTrue = worldP->rowbytes / sizeof(PF_Pixel);
        break;
    case PF_PixelFormat_ARGB64:
        info.widthTrue = worldP->rowbytes / sizeof(PF_Pixel16);
        break;
    case PF_PixelFormat_ARGB128:
        info.widthTrue = worldP->rowbytes / sizeof(PF_PixelFloat);
        break;
    }
    
    // タスク数を決定
    A_long noiseTaskCnt=0;
    
    if (info.noiseAmount <= 150) {
        info.taskCount = 1;
        info.noiseAmounts[0] = info.noiseAmount;
        info.heights[0] = info.height;
    }
    else {
        unsigned int num_threads = std::thread::hardware_concurrency();
        if (num_threads == 0) num_threads = 4;
        noiseTaskCnt = MIN(MAX((A_long)num_threads, 2), 8);
        info.taskCount = noiseTaskCnt;
        
        A_u_long baseAmount = info.noiseAmount / noiseTaskCnt;
        A_u_long remainder = info.noiseAmount % noiseTaskCnt;
        A_long baseHeight = info.height / noiseTaskCnt;
        A_long heightRemainder = info.height % noiseTaskCnt;
        
        for (A_long i = 0; i < noiseTaskCnt; i++) {
            info.noiseAmounts[i] = baseAmount;
            info.heights[i] = baseHeight;
        }
        info.noiseAmounts[noiseTaskCnt - 1] += remainder;
        info.heights[noiseTaskCnt - 1] += heightRemainder;
    }
   
    switch (ae->pixelFormat())
    {
    case PF_PixelFormat_ARGB32:
        ERR(ae->suitesP->Iterate8Suite2()->iterate_generic(
            info.taskCount,
            &info,
            Noise8
        ));
        break;
    case PF_PixelFormat_ARGB64:
        ERR(ae->suitesP->Iterate8Suite2()->iterate_generic(
            info.taskCount,
            &info,
            Noise16
        ));
        break;
    case PF_PixelFormat_ARGB128:
        ERR(ae->suitesP->Iterate8Suite2()->iterate_generic(
            info.taskCount,
            &info,
            Noise32
        ));
        break;
    default:
        err = PF_Err_BAD_CALLBACK_PARAM;
        break;
    }
    return err;
}
