/*******************************************************************/
/*                                                                 */
/*                      ADOBE CONFIDENTIAL                         */
/*                   _ _ _ _ _ _ _ _ _ _ _ _ _                     */
/*                                                                 */
/* Copyright 2007-2023 Adobe Inc.                                  */
/* All Rights Reserved.                                            */
/*                                                                 */
/* NOTICE:  All information contained herein is, and remains the   */
/* property of Adobe Inc. and its suppliers, if                    */
/* any.  The intellectual and technical concepts contained         */
/* herein are proprietary to Adobe Inc. and its                    */
/* suppliers and may be covered by U.S. and Foreign Patents,       */
/* patents in process, and are protected by trade secret or        */
/* copyright law.  Dissemination of this information or            */
/* reproduction of this material is strictly forbidden unless      */
/* prior written permission is obtained from Adobe Inc.            */
/* Incorporated.                                                   */
/*                                                                 */
/*******************************************************************/
/*
    Supervisor.cpp
    
    This plug-in demonstrates parameter supervision.

    Revision History

    Version     Change                                                    Engineer    Date
    =======     ======                                                    ========    ======
    1.0         created                                                   bbb         9/1/2000
    1.1         updated for 16bpc                                         bbb         2/14/2001
    1.2         added use of START_COLLAPSED_FLAG,                        bbb         9/1/2001
                fixed a supervision logic bug or two...                                
    1.3         Fixed usage of initializedB flag                          bbb         1/1/2002
    2.0         Bumped version number higher to                           bbb         2/20/2003
                reflect a bunch of changes. Adding
                lots of comments.
    3.0         Replaced param supervision with whizzy                    bbb         5/7/2003
                new DynamicStreamSuite calls.
    4.0         I am so SMRT! I am so SMRT! S-M-R-T!                     bbb         10/4/2006
                No, wait...S-M-A-R-T!
    5.0         Premiere Pro compatibility                                zal         2/20/2008
    5.1         Better param supervision logic                            zal         5/28/2009
    5.2         Demonstrate renaming choices in popup param               zal         6/2/2009
    5.5         64-bit                                                    zal         2/2/2010
    5.6         Demonstrate HaveInputsChangedOverTimeSpan                 zal         4/20/2011
    5.7         Demonstrate changing param values                         zal         6/22/2011
                Improved supervision logic, code cleanup
    5.8         Added new entry point                                     zal         9/18/2017
    5.9         Remove deprecated 'register' keyword                      cb          12/18/2020
    5.10        Added 'Support URL' to PiPL and entry point              cjr         3/31/2023
    5.11        Dynamic Popup Choices for Control Only Param               pc          9/8/2025

*/

#include <cassert>
#include "Supervisor.h"

static PF_Err
About (    
    PF_InData       *in_data,
    PF_OutData      *out_data,
    PF_ParamDef     *params[],
    PF_LayerDef     *output )
{
    
    PF_Err  err         =   PF_Err_NONE;
    AEGP_SuiteHandler       suites(in_data->pica_basicP);
    PF_AppPersonalTextInfo  personal_info;

    AEFX_CLR_STRUCT(personal_info);

    ERR(suites.AppSuite4()->PF_GetPersonalInfo(&personal_info));
        
    if (!err) {
        suites.ANSICallbacksSuite1()->sprintf(
            out_data->return_msg,
            "%s\r%s\r%s, v%d.%d\r\r%s",
            STR(StrID_Name),
            personal_info.name,
            personal_info.serial_str,
            MAJOR_VERSION, 
            MINOR_VERSION,
            STR(StrID_Description));
    }
    return err;
}

static void
GetPresetColorValue(
    A_long          flavor,
    A_long          mode,
    PF_Pixel        *scratchP)
{
    if (mode == MODE_BASIC) {
        // Basic mode flavors
        switch (flavor) {
            case FLAVOR_CHOCOLATE:    // Chocolate - brown
                scratchP->red = 136; scratchP->green = 83; scratchP->blue = 51; break;
            case FLAVOR_STRAWBERRY:   // Strawberry - red
                scratchP->red = 232; scratchP->green = 21; scratchP->blue = 84; break;
            case FLAVOR_SHERBET:      // Sherbet - orange
                scratchP->red = 255; scratchP->green = 128; scratchP->blue = 0; break;
            default:
                // Debug: Force bright green for any unmatched flavor value
                scratchP->red = 0; scratchP->green = 255; scratchP->blue = 0; break;
        }
    } else {
        // Advanced mode flavors
        switch (flavor) {
            case FLAVOR_EXPLODING_SNAPS:  // Exploding Snaps - bright yellow/orange
                scratchP->red = 255; scratchP->green = 215; scratchP->blue = 0; break;
            case FLAVOR_TREACLE_TART:     // Treacle Tart - dark brown
                scratchP->red = 101; scratchP->green = 67; scratchP->blue = 33; break;
            case FLAVOR_DIRACAWL_SLICES:  // Diracawl Slices - dark red/brown
                scratchP->red = 139; scratchP->green = 69; scratchP->blue = 19; break;
            case FLAVOR_BUTTER_BEER:      // Butter Beer - golden yellow
                scratchP->red = 255; scratchP->green = 223; scratchP->blue = 0; break;
            default:
                scratchP->red = 0; scratchP->green = 0; scratchP->blue = 0; break;
        }
    }
    scratchP->alpha = 255;  // Fully opaque
}

static PF_Err 
GlobalSetup (    
    PF_InData       *in_data,
    PF_OutData      *out_data,
    PF_ParamDef     *params[],
    PF_LayerDef     *output )
{
    PF_Err              err         = PF_Err_NONE;
    
    PF_Handle           globH       = NULL;
    my_global_dataP     globP       = NULL;

    AEGP_SuiteHandler       suites(in_data->pica_basicP);

    out_data->my_version = PF_VERSION(    MAJOR_VERSION, 
                                        MINOR_VERSION,    
                                        BUG_VERSION, 
                                        STAGE_VERSION, 
                                        BUILD_VERSION);

    out_data->out_flags     =   PF_OutFlag_PIX_INDEPENDENT           | 
                                PF_OutFlag_SEND_UPDATE_PARAMS_UI     |
                                PF_OutFlag_USE_OUTPUT_EXTENT         |
                                PF_OutFlag_DEEP_COLOR_AWARE          |
                                PF_OutFlag_WIDE_TIME_INPUT;

    //  This new outflag, added in 5.5, makes After Effects
    //  honor the initial state of the parameter's (in our case,
    //  the parameter group's) collapsed flag.
    
    out_data->out_flags2    =   PF_OutFlag2_PARAM_GROUP_START_COLLAPSED_FLAG    |
                                PF_OutFlag2_FLOAT_COLOR_AWARE                    |
                                PF_OutFlag2_SUPPORTS_SMART_RENDER                |
                                PF_OutFlag2_DOESNT_NEED_EMPTY_PIXELS            |
                                PF_OutFlag2_AUTOMATIC_WIDE_TIME_INPUT;
    
    /*
     This plugin is not marked as Thread-Safe because it is writing to sequence data during PF_Cmd_UPDATE_PARAMS_UI.
     There is an exisiting issue that's preventing this from working during Multi-Frame rendering.
     */
    

    //  This looks more complex than it is. Basically, this
    //  code allocates a handle, and (if successful) sets a 
    //  sentinel value for later use, and gets an ID for using
    //  AEGP calls. It then stores those value in  in_data->global_data.
    
    globH   =   suites.HandleSuite1()->host_new_handle(sizeof(my_global_data));
    
    if (globH) {
        globP = reinterpret_cast<my_global_dataP>(suites.HandleSuite1()->host_lock_handle(globH));
        if (globP) {
            globP->initializedB     = TRUE;

            if (in_data->appl_id != kAppID_Premiere) {
                // This is only needed for the AEGP suites, which Premiere Pro doesn't support
                ERR(suites.UtilitySuite3()->AEGP_RegisterWithAEGP(NULL, STR(StrID_Name), &globP->my_id));
            }
                if (!err){
                    out_data->global_data     = globH;
            }
        }
        suites.HandleSuite1()->host_unlock_handle(globH);
    } else  {
        err = PF_Err_INTERNAL_STRUCT_DAMAGED;
    }
    return err;
}

static PF_Err 
SequenceSetdown (   
                    PF_InData       *in_data,
                    PF_OutData      *out_data)
{
    PF_Err err = PF_Err_NONE;
    
    if (in_data->sequence_data){
        AEGP_SuiteHandler suites(in_data->pica_basicP);
        suites.HandleSuite1()->host_dispose_handle(in_data->sequence_data);
    }
    return err;
}

static PF_Err 
SequenceSetup ( 
    PF_InData       *in_data,
    PF_OutData      *out_data)
{
    PF_Err err = PF_Err_NONE;
    AEGP_SuiteHandler suites(in_data->pica_basicP);
    
    err = SequenceSetdown(in_data, out_data);
    
    if (!err){
        PF_Handle   seq_dataH = suites.HandleSuite1()->host_new_handle(sizeof(my_sequence_data));
        
        if (seq_dataH){
            my_sequence_dataP seqP = static_cast<my_sequence_dataP>(suites.HandleSuite1()->host_lock_handle(seq_dataH));
            if (seqP){
                if (in_data->appl_id != kAppID_Premiere)
                {
                    ERR(suites.ParamUtilsSuite3()->PF_GetCurrentState(  in_data->effect_ref,
                                                                        SUPER_SLIDER,
                                                                        NULL,
                                                                        NULL,
                                                                        &seqP->state));
                }

                //  currently this is updating during SmartRender!
                //  It should be correctly set here
                //  but i do not know how - davec
                seqP->advanced_modeB =  FALSE;
                
                // Allocate memory for popup choice strings using AE's memory management
                // AE will handle lifetime of these pop up strings, so we use it to allocate and copy it in.
                const char* basic_source = STR(StrID_FlavorChoicesBasic);
                const char* advanced_source = STR(StrID_FlavorChoicesAdvanced);
                
                size_t basic_len = strlen(basic_source) + 1;
                size_t advanced_len = strlen(advanced_source) + 1;
                
                // Use AE's memory allocation system
                seqP->basic_flavor_string = suites.HandleSuite1()->host_new_handle(basic_len);
                seqP->advanced_flavor_string = suites.HandleSuite1()->host_new_handle(advanced_len);
                
                if (seqP->basic_flavor_string && seqP->advanced_flavor_string) {
                    char* basic_ptr = (char*)suites.HandleSuite1()->host_lock_handle(seqP->basic_flavor_string);
                    char* advanced_ptr = (char*)suites.HandleSuite1()->host_lock_handle(seqP->advanced_flavor_string);
                    
                    if (basic_ptr && advanced_ptr) {
                        strcpy(basic_ptr, basic_source);
                        strcpy(advanced_ptr, advanced_source);
                        
                        suites.HandleSuite1()->host_unlock_handle(seqP->basic_flavor_string);
                        suites.HandleSuite1()->host_unlock_handle(seqP->advanced_flavor_string);
                    }
                } else {
                    // Clean up on allocation failure
                    if (seqP->basic_flavor_string) {
                        suites.HandleSuite1()->host_dispose_handle(seqP->basic_flavor_string);
                        seqP->basic_flavor_string = nullptr;
                    }
                    if (seqP->advanced_flavor_string) {
                        suites.HandleSuite1()->host_dispose_handle(seqP->advanced_flavor_string);
                        seqP->advanced_flavor_string = nullptr;
                    }
                }
                out_data->sequence_data = seq_dataH;
                suites.HandleSuite1()->host_unlock_handle(seq_dataH);
            }
        } else {    // whoa, we couldn't allocate sequence data; bail!
            err = PF_Err_OUT_OF_MEMORY;
        }
    }
    return err;
}


static PF_Err
ParamsSetup(
    PF_InData       *in_data,
    PF_OutData      *out_data,
    PF_ParamDef     *params[],
    PF_LayerDef     *output)
{
    PF_Err              err                 = PF_Err_NONE;
    PF_ParamDef         def;

    AEFX_CLR_STRUCT(def);
    def.flags       =   PF_ParamFlag_SUPERVISE          |
                        PF_ParamFlag_CANNOT_TIME_VARY   |
                        PF_ParamFlag_CANNOT_INTERP;
                        
    def.ui_flags =   PF_PUI_STD_CONTROL_ONLY;

    // Defaults to "Basic"
    PF_ADD_POPUP(       STR(StrID_ModeName), 
                        MODE_SIZE,
                        MODE_BASIC,
                        STR(StrID_ModeChoices),
                        SUPER_MODE_DISK_ID);
        
    AEFX_CLR_STRUCT(def);
    
    def.flags       =   PF_ParamFlag_SUPERVISE |
                        PF_ParamFlag_CANNOT_INTERP |
                        PF_ParamFlag_CANNOT_TIME_VARY;

    def.ui_flags =   PF_PUI_STD_CONTROL_ONLY;
    
    PF_ADD_POPUP(       STR(StrID_FlavorName), 
                        FLAVOR_SIZE,
                        FLAVOR_CHOCOLATE,
                        STR(StrID_FlavorChoicesBasic),
                        SUPER_FLAVOR_DISK_ID);
    
    AEFX_CLR_STRUCT(def);
    
    PF_Pixel temp;
    
    GetPresetColorValue(FLAVOR_CHOCOLATE, MODE_BASIC, &temp);

    PF_ADD_COLOR(   STR(StrID_ColorName), 
                    temp.red, 
                    temp.green, 
                    temp.blue, 
                    SUPER_COLOR_DISK_ID);
    
    AEFX_CLR_STRUCT(def);

    PF_ADD_FIXED(   STR(StrID_SliderName), 
                    0, 
                    100, 
                    0, 
                    100, 
                    28, 
                    1, 
                    1,
                    PF_ParamFlag_EXCLUDE_FROM_HAVE_INPUTS_CHANGED,
                    SUPER_SLIDER_DISK_ID);
    
    AEFX_CLR_STRUCT(def);

    def.flags       |=  PF_ParamFlag_SUPERVISE |
                        PF_ParamFlag_CANNOT_TIME_VARY;

    def.ui_flags    = PF_PUI_STD_CONTROL_ONLY;

    PF_ADD_CHECKBOX(STR(StrID_CheckboxName),
                    STR(StrID_CheckboxCaption),
                    FALSE, 
                    0,
                    SUPER_CHECKBOX_DISK_ID);

    AEFX_CLR_STRUCT(def);

    out_data->num_params = SUPER_NUM_PARAMS;

    return err;
}

template <typename CHANFORMAT>
static CHANFORMAT    ChanFunc(
    const CHANFORMAT&   in_chan,
    const PF_FpShort&   param_chan,
    CHANFORMAT          max_chan_val)
{
    //  this just adds the 2 pixels together, then divides by 2 (to keep it in range)
    //  note param_chan is always float, so first mult by max_chan_val to convert to
    //  whatever the CHANFORMAT is
    CHANFORMAT   out_chan = (CHANFORMAT)((in_chan + (param_chan * max_chan_val)) / 2);
    
    if (out_chan > max_chan_val){
        out_chan = max_chan_val;
    }
    
    return out_chan;
}

#define     CHAN_FUNC(_chan) \
    outP->_chan = ChanFunc<CHANFORMAT>(inP->_chan, param_pix._chan, max_chan_val)

template <typename PIXFORMAT, typename CHANFORMAT>
static PF_Err    PixFunc(
    const PIXFORMAT*        inP,
    PIXFORMAT*              outP,
    void*                   refcon,
    CHANFORMAT              max_chan_val)
{
    prerender_stuff*        stuffP = reinterpret_cast<prerender_stuff*>(refcon);
    const PF_PixelFloat&    param_pix(stuffP->color);

    CHAN_FUNC(red);
    CHAN_FUNC(green);
    CHAN_FUNC(blue);
    outP->alpha = max_chan_val;
    
    return PF_Err_NONE;
}

static PF_Err
PixelFunc8 (    
    void        *refcon,
    A_long      xL,
    A_long      yL,
    PF_Pixel8   *inP,
    PF_Pixel8   *outP)
{
    return PixFunc<PF_Pixel8, A_u_char>(inP, outP, refcon, PF_MAX_CHAN8);
}

static PF_Err 
PixelFunc16(    
    void        *refcon,
    A_long      xL,
    A_long      yL,
    PF_Pixel16 *inP,
    PF_Pixel16 *outP)
{
    return PixFunc<PF_Pixel16, A_u_short>(inP, outP, refcon, PF_MAX_CHAN16);
}

#define PF_MAX_CHAN_FLOAT   1.0

static PF_Err
PixelFuncFloat( 
    void            *refcon,
    A_long          xL,
    A_long          yL,
    PF_PixelFloat   *inP,
    PF_PixelFloat   *outP)
{
    return PixFunc<PF_PixelFloat, PF_FpShort>(inP, outP, refcon, PF_MAX_CHAN_FLOAT);
}

static PF_Err  
MakeParamCopy(
    PF_ParamDef *srcPA[],   /* >> */
    PF_ParamDef destA[])        /* << */
{
    for (A_short iS = 0; iS < SUPER_NUM_PARAMS; ++iS) {
        AEFX_CLR_STRUCT(destA[iS]);
    }
    destA[SUPER_INPUT]     = *srcPA[SUPER_INPUT];
    destA[SUPER_MODE]      = *srcPA[SUPER_MODE];
    destA[SUPER_FLAVOR]    = *srcPA[SUPER_FLAVOR];
    destA[SUPER_COLOR]     = *srcPA[SUPER_COLOR];
    destA[SUPER_SLIDER]    = *srcPA[SUPER_SLIDER];
    destA[SUPER_CHECKBOX]  = *srcPA[SUPER_CHECKBOX];

    return PF_Err_NONE;
}



static PF_Err
UserChangedParam(
    PF_InData                       *in_data,
    PF_OutData                      *out_data,
    PF_ParamDef                     *params[],
    PF_LayerDef                     *outputP,
    const PF_UserChangedParamExtra  *which_hitP)
{
    PF_Err              err                 = PF_Err_NONE;

    if (which_hitP->param_index == SUPER_CHECKBOX)
    {
        // If checkbox is checked, change slider value to 50 and flip checkbox back off
        if (params[SUPER_CHECKBOX]->u.bd.value == TRUE) {
            params[SUPER_SLIDER]->u.sd.value = INT2FIX(50);
            params[SUPER_SLIDER]->uu.change_flags = PF_ChangeFlag_CHANGED_VALUE;

            params[SUPER_CHECKBOX]->u.bd.value = FALSE;
            params[SUPER_CHECKBOX]->uu.change_flags = 0;

            AEGP_SuiteHandler       suites(in_data->pica_basicP);

            ERR(suites.ParamUtilsSuite3()->PF_UpdateParamUI(in_data->effect_ref, 
                                                            SUPER_SLIDER,
                                                            params[SUPER_SLIDER]));
        }
    }
    else if (which_hitP->param_index == SUPER_FLAVOR)
    {
        // When flavor changes, update the color parameter to match the selected flavor
        // Handle both Basic and Advanced modes
        PF_Pixel preset_color;
        GetPresetColorValue(params[SUPER_FLAVOR]->u.pd.value, params[SUPER_MODE]->u.pd.value, &preset_color);
        
        // Update the color parameter with the preset color
        params[SUPER_COLOR]->u.cd.value.red = preset_color.red;
        params[SUPER_COLOR]->u.cd.value.green = preset_color.green;
        params[SUPER_COLOR]->u.cd.value.blue = preset_color.blue;
        params[SUPER_COLOR]->u.cd.value.alpha = preset_color.alpha;
        params[SUPER_COLOR]->uu.change_flags = PF_ChangeFlag_CHANGED_VALUE;

        AEGP_SuiteHandler suites(in_data->pica_basicP);
        ERR(suites.ParamUtilsSuite3()->PF_UpdateParamUI(in_data->effect_ref, 
                                                        SUPER_COLOR,
                                                        params[SUPER_COLOR]));
        
        // Force UI refresh to ensure color change is visible
        out_data->out_flags |= PF_OutFlag_REFRESH_UI | PF_OutFlag_FORCE_RERENDER;
    }
    else if (which_hitP->param_index == SUPER_MODE)
    {
        // When mode changes, update the color parameter to match the current flavor
        // Handle both Basic and Advanced modes
        PF_Pixel preset_color;
        GetPresetColorValue(params[SUPER_FLAVOR]->u.pd.value, params[SUPER_MODE]->u.pd.value, &preset_color);
        
        // Update the color parameter with the preset color for current flavor
        params[SUPER_COLOR]->u.cd.value.red = preset_color.red;
        params[SUPER_COLOR]->u.cd.value.green = preset_color.green;
        params[SUPER_COLOR]->u.cd.value.blue = preset_color.blue;
        params[SUPER_COLOR]->u.cd.value.alpha = preset_color.alpha;
        params[SUPER_COLOR]->uu.change_flags = PF_ChangeFlag_CHANGED_VALUE;

        AEGP_SuiteHandler suites(in_data->pica_basicP);
        ERR(suites.ParamUtilsSuite3()->PF_UpdateParamUI(in_data->effect_ref, 
                                                        SUPER_COLOR,
                                                        params[SUPER_COLOR]));
        
        // Force UI refresh to ensure parameter visibility and color changes are visible
        out_data->out_flags |= PF_OutFlag_REFRESH_UI | PF_OutFlag_FORCE_RERENDER;
    }

    return err;
}



static PF_Err
UpdateParameterUI(
    PF_InData           *in_data,
    PF_OutData          *out_data,
    PF_ParamDef         *params[],
    PF_LayerDef         *outputP)
{
    PF_Err              err                 = PF_Err_NONE,
                        err2                = PF_Err_NONE;
    my_global_dataP     globP               = reinterpret_cast<my_global_dataP>(DH(out_data->global_data));
    my_sequence_dataP   seqP                = reinterpret_cast<my_sequence_dataP>(DH(out_data->sequence_data));
    AEGP_StreamRefH     flavor_streamH      = NULL,
                        color_streamH       = NULL,
                        slider_streamH      = NULL,
                        checkbox_streamH    = NULL;
    A_Boolean           hide_themB          = FALSE;
    AEGP_EffectRefH     meH                 = NULL;
    AEGP_SuiteHandler   suites(in_data->pica_basicP);

    //  Before we can change the enabled/disabled state of parameters,
    //  we need to make a copy (remember, parts of those passed into us
    //  are read-only).

    PF_ParamDef         copy_of_paramsA[SUPER_NUM_PARAMS];

    // create a deep copy
    ERR(MakeParamCopy(params, copy_of_paramsA));

    const PF_ParamDef&  modeParam(*params[SUPER_MODE]);
    PF_ParamDef&        flavorParam(copy_of_paramsA[SUPER_FLAVOR]);
    PF_ParamDef&        sliderParam(copy_of_paramsA[SUPER_SLIDER]);
    PF_ParamDef&        colorParam(copy_of_paramsA[SUPER_COLOR]);
    PF_ParamDef&        checkboxParam(copy_of_paramsA[SUPER_CHECKBOX]);

    // Toggle enable/disabled state of flavor param
    if (
        !err
        && (modeParam.u.pd.value == MODE_BASIC)
    ) {
        flavorParam.param_type  =   PF_Param_POPUP;

        //  ensure the popup is enabled
        flavorParam.ui_flags    &= ~PF_PUI_DISABLED;
        strcpy(flavorParam.PF_DEF_NAME, STR(StrID_FlavorName));
    } else if (
        !err
        && (modeParam.u.pd.value == MODE_ADVANCED)
        && (!(flavorParam.ui_flags & PF_PUI_DISABLED))
    ) {
        flavorParam.param_type      =   PF_Param_POPUP;

        //  to disable the popup in advanced mode, uncomment this line
        // flavorParam.ui_flags      |=  PF_PUI_DISABLED;
        if (flavorParam.ui_flags & PF_PUI_DISABLED) {
            strcpy(flavorParam.PF_DEF_NAME, STR(StrID_FlavorNameDisabled));
        } else {
            strcpy(flavorParam.PF_DEF_NAME, STR(StrID_FlavorName));
        }
    }

    ERR(suites.ParamUtilsSuite3()->PF_UpdateParamUI(
        in_data->effect_ref,
        SUPER_FLAVOR,
        &flavorParam));

    if (in_data->appl_id != kAppID_Premiere) {
        seqP->advanced_modeB = modeParam.u.pd.value == MODE_ADVANCED;

        if (!seqP->advanced_modeB) {
            hide_themB = TRUE;
        }

        // Twirl open the slider param
        sliderParam.param_type  =   PF_Param_FIX_SLIDER;
        sliderParam.flags       &=  ~PF_ParamFlag_COLLAPSE_TWIRLY;

        ERR(suites.ParamUtilsSuite3()->PF_UpdateParamUI(
            in_data->effect_ref,
            SUPER_SLIDER,
            &sliderParam));

        // Changing visibility of params in AE is handled through stream suites
        ERR(suites.PFInterfaceSuite1()->AEGP_GetNewEffectForEffect(globP->my_id, in_data->effect_ref, &meH));

        ERR(suites.StreamSuite2()->AEGP_GetNewEffectStreamByIndex(globP->my_id, meH, SUPER_FLAVOR,  &flavor_streamH));
        ERR(suites.StreamSuite2()->AEGP_GetNewEffectStreamByIndex(globP->my_id, meH, SUPER_COLOR,   &color_streamH));
        ERR(suites.StreamSuite2()->AEGP_GetNewEffectStreamByIndex(globP->my_id, meH, SUPER_SLIDER,  &slider_streamH));
        ERR(suites.StreamSuite2()->AEGP_GetNewEffectStreamByIndex(globP->my_id, meH, SUPER_CHECKBOX, &checkbox_streamH));

        // Toggle visibility of parameters
        ERR(suites.DynamicStreamSuite2()->AEGP_SetDynamicStreamFlag(color_streamH,      AEGP_DynStreamFlag_HIDDEN, FALSE, hide_themB));
        ERR(suites.DynamicStreamSuite2()->AEGP_SetDynamicStreamFlag(slider_streamH,     AEGP_DynStreamFlag_HIDDEN, FALSE, hide_themB));
        ERR(suites.DynamicStreamSuite2()->AEGP_SetDynamicStreamFlag(checkbox_streamH,   AEGP_DynStreamFlag_HIDDEN, FALSE, hide_themB));

                 // Change popup menu items - update the flavorParam copy and apply via PF_UpdateParamUI
         {
             if (flavorParam.param_type == PF_Param_POPUP) {
                 // Set the new popup choices based on mode using our AE-managed handles
                 PF_Handle flavor_handle = seqP->advanced_modeB ? seqP->advanced_flavor_string : seqP->basic_flavor_string;
                 if (flavor_handle) {
                     char* flavor_string = (char*)suites.HandleSuite1()->host_lock_handle(flavor_handle);
                     if (flavor_string) {
                         flavorParam.u.pd.u.PF_DEF_NAMESPTR = flavor_string;
                         
                         // Update the number of choices based on the current mode
                         if (seqP->advanced_modeB) {
                             flavorParam.u.pd.num_choices = FLAVOR_ADV_SIZE; // = 7
                             // Advanced mode: valid values are 1, 3, 5, 7
                             if (flavorParam.u.pd.value < 1 || flavorParam.u.pd.value > 7 || 
                                 (flavorParam.u.pd.value % 2 == 0)) {
                                 flavorParam.u.pd.value = FLAVOR_EXPLODING_SNAPS; // = 1
                             }
                         } else {
                             flavorParam.u.pd.num_choices = FLAVOR_SIZE; // = 5
                             // Basic mode: valid values are 1, 3, 5
                             if (flavorParam.u.pd.value < 1 || flavorParam.u.pd.value > 5 || 
                                 (flavorParam.u.pd.value % 2 == 0)) {
                                 flavorParam.u.pd.value = FLAVOR_CHOCOLATE; // = 1
                             }
                         }
                        
                        // Apply the parameter changes to update the UI
                        ERR(suites.ParamUtilsSuite3()->PF_UpdateParamUI(
                            in_data->effect_ref,
                            SUPER_FLAVOR,
                            &flavorParam));
                        
                        // Note: Color updates are handled in UserChangedParam, not here
                        // to avoid multiple calls and feedback loops
                        
                        suites.HandleSuite1()->host_unlock_handle(flavor_handle);
                     }
                 }
             }
         }

        if (meH){
            ERR2(suites.EffectSuite2()->AEGP_DisposeEffect(meH));
        }
        if (flavor_streamH){
            ERR2(suites.StreamSuite2()->AEGP_DisposeStream(flavor_streamH));
        }
        if (color_streamH){
            ERR2(suites.StreamSuite2()->AEGP_DisposeStream(color_streamH));
        }
        if (slider_streamH){
            ERR2(suites.StreamSuite2()->AEGP_DisposeStream(slider_streamH));
        }
        if (checkbox_streamH){
            ERR2(suites.StreamSuite2()->AEGP_DisposeStream(checkbox_streamH));
        }
        if (!err){
            out_data->out_flags |= PF_OutFlag_FORCE_RERENDER;
        }

        // Demonstrate using PF_AreStatesIdentical to check whether a parameter has changed
        PF_State        new_state;
        A_Boolean       something_changedB      = FALSE;

        ERR(suites.ParamUtilsSuite3()->PF_GetCurrentState(      in_data->effect_ref,
                                                                SUPER_SLIDER,
                                                                NULL,
                                                                NULL,
                                                                &new_state));

        ERR(suites.ParamUtilsSuite3()->PF_AreStatesIdentical(   in_data->effect_ref,
                                                                &seqP->state,
                                                                &new_state,
                                                                &something_changedB));

        if (something_changedB || !globP->initializedB) {
            //  If something changed (or it's the first time we're being called),
            //  get the new state and store it in our sequence data

            ERR(suites.ParamUtilsSuite3()->PF_GetCurrentState(  in_data->effect_ref,
                                                                SUPER_SLIDER,
                                                                NULL,
                                                                NULL,
                                                                &seqP->state));
        }

    } else { // Premiere Pro doesn't support the stream suites, but uses a UI flag instead

        //  If our global data is present...
        if (!err && globP) {
            // Test all parameters except layers for changes
            
            //  If the mode is currently Basic, hide the advanced-only params           
            if (!err && (modeParam.u.pd.value == MODE_BASIC)) {

                if (!err) {
                    colorParam.ui_flags |=  PF_PUI_INVISIBLE;
                    ERR(suites.ParamUtilsSuite3()->PF_UpdateParamUI(in_data->effect_ref,
                                                                    SUPER_COLOR,
                                                                    &colorParam));
                }

                if (!err) {
                    sliderParam.ui_flags |= PF_PUI_INVISIBLE;
                    ERR(suites.ParamUtilsSuite3()->PF_UpdateParamUI(in_data->effect_ref, 
                                                                    SUPER_SLIDER,
                                                                    &sliderParam));
                }

                if (!err) {
                    checkboxParam.ui_flags |=   PF_PUI_INVISIBLE;
                    ERR(suites.ParamUtilsSuite3()->PF_UpdateParamUI(in_data->effect_ref, 
                                                                    SUPER_CHECKBOX,
                                                                    &checkboxParam));
                }

            } else {
                //  Since we're in advanced mode, show the advanced-only params 
                
                if (!err) {
                    colorParam.ui_flags &= ~PF_PUI_INVISIBLE;

                    ERR(suites.ParamUtilsSuite3()->PF_UpdateParamUI(in_data->effect_ref,
                                                                    SUPER_COLOR,
                                                                    &colorParam));
                }

                if (!err && (sliderParam.ui_flags & PF_PUI_INVISIBLE)) {
                    // Note: As of Premiere Pro CS5.5, this is currently not honored
                    sliderParam.flags   &= ~PF_ParamFlag_COLLAPSE_TWIRLY;
                    sliderParam.ui_flags &= ~PF_PUI_INVISIBLE;

                    ERR(suites.ParamUtilsSuite3()->PF_UpdateParamUI(in_data->effect_ref, 
                                                                    SUPER_SLIDER,
                                                                    &sliderParam));
                }
                
                if (!err && (checkboxParam.ui_flags & PF_PUI_INVISIBLE)) {
                    checkboxParam.ui_flags &= ~PF_PUI_INVISIBLE;

                    ERR(suites.ParamUtilsSuite3()->PF_UpdateParamUI(in_data->effect_ref, 
                                                                    SUPER_CHECKBOX,
                                                                    &checkboxParam));
                }

                if (err) { // <--- Note: not !err, err!
                    strcpy(out_data->return_msg, STR(StrID_GeneralError));
                    
                    out_data->out_flags = PF_OutFlag_DISPLAY_ERROR_MESSAGE;
                }
                else if (globP != nullptr){
                    globP->initializedB = TRUE; // set our cheesy first time indicator
                }
            }
        } 

        if (!err && globP != nullptr) {
            globP->initializedB =   TRUE;
        }
        out_data->out_flags |=  PF_OutFlag_REFRESH_UI       |
                                PF_OutFlag_FORCE_RERENDER;
    }

    return err;
}

static PF_Err   
GlobalSetdown(
    PF_InData       *in_data)
{
    AEGP_SuiteHandler   suites(in_data->pica_basicP);
    
    if (in_data->global_data) {
        suites.HandleSuite1()->host_dispose_handle(in_data->global_data);
    }

    return PF_Err_NONE;
}

static PF_Err   
Render16(   
    PF_InData       *in_data,
    PF_OutData      *out_data,
    PF_ParamDef     *params[],
    PF_LayerDef     *output,
    PF_Pixel8       *scratchP)
{
    AEGP_SuiteHandler       suites(in_data->pica_basicP);
    
    PF_Pixel16      scratch16 {0, 0, 0, 0};

    if (scratchP != nullptr)
    {
        scratch16.red       =   CONVERT8TO16(scratchP->red);
        scratch16.green     =   CONVERT8TO16(scratchP->green);
        scratch16.blue      =   CONVERT8TO16(scratchP->blue);
        scratch16.alpha     =   CONVERT8TO16(scratchP->alpha);
    } else {
        return A_Err_PARAMETER;
    }
    
    return suites.Iterate16Suite2()->iterate(   in_data,
                                                0,
                                                (output->extent_hint.bottom - output->extent_hint.top),
                                                &params[SUPER_INPUT]->u.ld,
                                                &output->extent_hint,
                                                (void*)&scratch16,
                                                PixelFunc16,
                                                output);
}

static PF_Err   
Render8(    
    PF_InData       *in_data,
    PF_OutData      *out_data,
    PF_ParamDef     *params[],
    PF_LayerDef     *output,
    PF_Pixel8       *scratchP)
{
    AEGP_SuiteHandler   suites(in_data->pica_basicP);
    prerender_stuff     stuff;

    stuff.color.red     = (PF_FpShort)scratchP->red     / PF_MAX_CHAN8;
    stuff.color.green   = (PF_FpShort)scratchP->green   / PF_MAX_CHAN8;
    stuff.color.blue    = (PF_FpShort)scratchP->blue    / PF_MAX_CHAN8;
    stuff.color.alpha   = (PF_FpShort)scratchP->alpha   / PF_MAX_CHAN8;
    
    return suites.Iterate8Suite2()->iterate(in_data,
                                            0,
                                            (output->extent_hint.bottom - output->extent_hint.top),
                                            &params[SUPER_INPUT]->u.ld,
                                            &output->extent_hint,
                                            (void*)&stuff,
                                            PixelFunc8,
                                            output);
}

static PF_Err   
Render( PF_InData       *in_data,
        PF_OutData      *out_data,
        PF_ParamDef     *params[],
        PF_LayerDef     *output)
{
    PF_Err  err = PF_Err_NONE;

    PF_Pixel scratch8;

    //  Use the color parameter value (which gets updated when flavor changes)
    scratch8 = params[SUPER_COLOR]->u.cd.value;
    PF_Boolean deepB =  PF_WORLD_IS_DEEP(output);
    
    if (deepB){
        ERR(Render16(in_data,
                    out_data,
                    params,
                    output,
                    &scratch8));

    } else {
        ERR(Render8(in_data,
                    out_data,
                    params,
                    output,
                    &scratch8));
    }
    return err;
}

static PF_Err
PreRender(
    PF_InData               *in_data,
    PF_OutData              *out_data,
    PF_PreRenderExtra       *extraP)
{
    PF_Err  err             = PF_Err_NONE;

    PF_RenderRequest req    = extraP->input->output_request;

    PF_CheckoutResult       in_result;
    AEGP_SuiteHandler       suites(in_data->pica_basicP);

    PF_Handle   infoH       =   suites.HandleSuite1()->host_new_handle(sizeof(prerender_stuff));

    prerender_stuff     *stuffP = NULL;

    if (infoH){
        stuffP = reinterpret_cast<prerender_stuff*>(suites.HandleSuite1()->host_lock_handle(infoH));
        if (stuffP){
            extraP->output->pre_render_data = infoH;
            
            /*  
                Let's investigate our input parameters, and save ourselves
                a few clues for rendering later. Because pre-render gets 
                called A LOT, it's best to put checkouts you'll always need
                in SmartRender(). Because smart pre-computing of what you'll
                actually NEED can save time, it's best to check conditionals
                here in pre-render.
            */
            
            AEFX_CLR_STRUCT(in_result);

            if (!err){
                req.preserve_rgb_of_zero_alpha = FALSE; 
                
                ERR(extraP->cb->checkout_layer( in_data->effect_ref,
                                                SUPER_INPUT,
                                                SUPER_INPUT,
                                                &req,
                                                in_data->current_time,
                                                in_data->time_step,
                                                in_data->time_scale,
                                                &in_result));
                if (!err){
                    UnionLRect(&in_result.result_rect,      &extraP->output->result_rect);
                    UnionLRect(&in_result.max_result_rect,  &extraP->output->max_result_rect);  
                }
            }
            suites.HandleSuite1()->host_unlock_handle(infoH);
        }
    }
    return err;
}

static PF_Err
SmartRender(
    PF_InData               *in_data,
    PF_OutData              *out_data,
    PF_SmartRenderExtra     *extraP)
{
    
    PF_Err          err         = PF_Err_NONE,
                    err2        = PF_Err_NONE;
    
    PF_EffectWorld  *inputP     = NULL,
                    *outputP    = NULL;
    
    PF_PixelFormat  format      =   PF_PixelFormat_INVALID;
    PF_WorldSuite2  *wsP        =   NULL;
    my_sequence_dataP   seqP    = reinterpret_cast<my_sequence_dataP>(DH(in_data->sequence_data));

    AEGP_SuiteHandler suites(in_data->pica_basicP);

    // Why the old way? Because AEGP_SuiteHandler has a collision with
    // AEGPWorldSuite2 and PF_WorldSuite2. That, and I'm too lazy
    // don't want to fiddle with a utility class shared by N effects,
    // where N approaches "a laughable amount of testing change for
    // questionable benefit".
    
    ERR(AEFX_AcquireSuite(  in_data, 
                            out_data, 
                            kPFWorldSuite, 
                            kPFWorldSuiteVersion2, 
                            STR(StrID_Err_LoadSuite),
                            (void**)&wsP));
    if (!err && wsP && seqP){

        prerender_stuff *stuffP = reinterpret_cast<prerender_stuff*>(suites.HandleSuite1()->host_lock_handle(reinterpret_cast<PF_Handle>(extraP->input->pre_render_data)));
        if (stuffP){

            // checkout input & output buffers.

            ERR((extraP->cb->checkout_layer_pixels( in_data->effect_ref, SUPER_INPUT, &inputP)));
            ERR(extraP->cb->checkout_output(in_data->effect_ref, &outputP));
            
            // determine requested output depth
            ERR(wsP->PF_GetPixelFormat(outputP, &format)); 

            {
                PF_Err          err2(PF_Err_NONE);
                PF_ParamDef     mode_param;

                AEFX_CLR_STRUCT(mode_param);

                ERR(PF_CHECKOUT_PARAM(  in_data,
                                        SUPER_MODE,
                                        in_data->current_time,
                                        in_data->time_step,
                                        in_data->time_scale,
                                        &mode_param));

                //  this should be done during SequenceSetup
                //  but i do not know how -davec
                seqP->advanced_modeB =  mode_param.u.pd.value == MODE_ADVANCED;

                ERR2(PF_CHECKIN_PARAM(in_data, &mode_param));
            }

            // Always use the color parameter directly - it's already been updated by UserChangedParam
            // This eliminates redundant GetPresetColorValue calls and potential mode mismatches
            PF_ParamDef     color_param;
            AEFX_CLR_STRUCT(color_param);

            ERR(PF_CHECKOUT_PARAM(  in_data, 
                                    SUPER_COLOR, 
                                    in_data->current_time, 
                                    in_data->time_step, 
                                    in_data->time_scale, 
                                    &color_param));
                
            ERR(suites.ColorParamSuite1()->PF_GetFloatingPointColorFromColorDef(in_data->effect_ref, &color_param, &stuffP->color));
            ERR2(PF_CHECKIN_PARAM(in_data, &color_param));

            switch (format) {
                
                case PF_PixelFormat_ARGB128:
                    ERR(suites.IterateFloatSuite2()->iterate(   in_data,
                                                                0,
                                                                (outputP->extent_hint.bottom - outputP->extent_hint.top),
                                                                inputP,
                                                                &outputP->extent_hint,
                                                                (void*)stuffP,
                                                                PixelFuncFloat,
                                                                outputP));
                    break;
                    
                case PF_PixelFormat_ARGB64:

                    ERR(suites.Iterate16Suite2()->iterate(  in_data,
                                                            0,
                                                            (outputP->extent_hint.bottom - outputP->extent_hint.top),
                                                            inputP,
                                                            &outputP->extent_hint,
                                                            (void*)stuffP,
                                                            PixelFunc16,
                                                            outputP));
                    break;
                    
                case PF_PixelFormat_ARGB32:

                    ERR(suites.Iterate8Suite2()->iterate(   in_data,
                                                            0,
                                                            (outputP->extent_hint.bottom - outputP->extent_hint.top),
                                                            inputP,
                                                            &outputP->extent_hint,
                                                            (void*)stuffP,
                                                            PixelFunc8,
                                                            outputP));

                    break;

                default:
                    err = PF_Err_INTERNAL_STRUCT_DAMAGED;
                    break;
            }
            ERR2(extraP->cb->checkin_layer_pixels(in_data->effect_ref, SUPER_INPUT));
        }
        suites.HandleSuite1()->host_unlock_handle(reinterpret_cast<PF_Handle>(extraP->input->pre_render_data));
    }
    ERR2(AEFX_ReleaseSuite( in_data,
                            out_data,
                            kPFWorldSuite, 
                            kPFWorldSuiteVersion2, 
                            STR(StrID_Err_FreeSuite)));
    return err;
}


extern "C" DllExport
PF_Err PluginDataEntryFunction2(
    PF_PluginDataPtr inPtr,
    PF_PluginDataCB2 inPluginDataCallBackPtr,
    SPBasicSuite* inSPBasicSuitePtr,
    const char* inHostName,
    const char* inHostVersion)
{
    PF_Err result = PF_Err_INVALID_CALLBACK;

    result = PF_REGISTER_EFFECT_EXT2(
        inPtr,
        inPluginDataCallBackPtr,
        "Supervisor", // Name
        "ADBE Supervisor", // Match Name
        "Sample Plug-ins", // Category
        AE_RESERVED_INFO, // Reserved Info
        "EffectMain",   // Entry point
        "https://www.adobe.com");   // support URL

    return result;
}


PF_Err
EffectMain(
    PF_Cmd          cmd,
    PF_InData       *in_data,
    PF_OutData      *out_data,
    PF_ParamDef     *params[],
    PF_LayerDef     *output,
    void            *extra )
{
    PF_Err      err = PF_Err_NONE;

    try {
        switch (cmd) {
            case PF_Cmd_ABOUT:
                err = About(in_data,
                            out_data,
                            params,
                            output);
                break;

            case PF_Cmd_GLOBAL_SETUP:
                err = GlobalSetup(  in_data,
                                    out_data,
                                    params,
                                    output);
                break;
            
            case PF_Cmd_GLOBAL_SETDOWN:
                err = GlobalSetdown(in_data);
                                    
                break;

            case PF_Cmd_PARAMS_SETUP:
                err = ParamsSetup(  in_data,
                                    out_data,
                                    params,
                                    output);
                break;

            case PF_Cmd_SEQUENCE_SETUP:
                err = SequenceSetup(in_data,out_data);
                break;

            case PF_Cmd_SEQUENCE_SETDOWN:
                err = SequenceSetdown(in_data,out_data);
                break;

            case PF_Cmd_SEQUENCE_RESETUP:
                err = SequenceSetup(in_data,out_data);
                break;

            case PF_Cmd_RENDER:
                err = Render(   in_data,
                                out_data,
                                params,
                                output);
                break;

            case PF_Cmd_SMART_RENDER:
                err = SmartRender(  in_data,
                                    out_data,
                                    reinterpret_cast<PF_SmartRenderExtra*>(extra));
                break;

            case PF_Cmd_SMART_PRE_RENDER:
                err = PreRender(    in_data,
                                    out_data,
                                    reinterpret_cast<PF_PreRenderExtra*>(extra));
                break;
                
            case PF_Cmd_USER_CHANGED_PARAM:
                err = UserChangedParam( in_data,
                                        out_data,
                                        params,
                                        output,
                                        reinterpret_cast<const PF_UserChangedParamExtra *>(extra));
                break;
                
            // Handling this selector will ensure that the UI will be properly initialized,
            // even before the user starts changing parameters to trigger PF_Cmd_USER_CHANGED_PARAM
            case PF_Cmd_UPDATE_PARAMS_UI:
                err = UpdateParameterUI(    in_data,
                                            out_data,
                                            params,
                                            output);

            default:
                break;
        }
    }
    catch(PF_Err &thrown_err){
        err = PF_Err_INTERNAL_STRUCT_DAMAGED;
    }
    return err;
}
