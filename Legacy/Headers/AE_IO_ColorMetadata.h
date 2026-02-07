/*******************************************************************/
/*                                                                 */
/*                      ADOBE CONFIDENTIAL                         */
/*                   _ _ _ _ _ _ _ _ _ _ _ _ _                     */
/*                                                                 */
/* Copyright 2025 Adobe Systems Incorporated                       */
/* All Rights Reserved.                                            */
/*                                                                 */
/* NOTICE:  All information contained herein is, and remains the   */
/* property of Adobe Systems Incorporated and its suppliers, if    */
/* any.  The intellectual and technical concepts contained         */
/* herein are proprietary to Adobe Systems Incorporated and its    */
/* suppliers and may be covered by U.S. and Foreign Patents,       */
/* patents in process, and are protected by trade secret or        */
/* copyright law.  Dissemination of this information or            */
/* reproduction of this material is strictly forbidden unless      */
/* prior written permission is obtained from Adobe Systems         */
/* Incorporated.                                                   */
/*                                                                 */
/*******************************************************************/

#ifndef _AEIO_ColorMetadata_Public_H
#define _AEIO_ColorMetadata_Public_H

#include <A.h>

enum
{
    kAEIO_DisplayPrimaries_Rec709 = 0,
    kAEIO_DisplayPrimaries_P3D65 = 1,
    kAEIO_DisplayPrimaries_Rec2020 = 2
};
typedef A_long AEIO_DisplayPrimaries;


typedef struct AEIO_MasteringDisplayMetadata
{
    A_FpLong md_chromaticity_redX;
    A_FpLong md_chromaticity_redY;
    A_FpLong md_chromaticity_greenX;
    A_FpLong md_chromaticity_greenY;
    A_FpLong md_chromaticity_blueX;
    A_FpLong md_chromaticity_blueY;
    A_FpLong md_chromaticity_whiteX;
    A_FpLong md_chromaticity_whiteY;
    A_FpLong md_min_luminance;
    A_FpLong md_max_luminance;
} AEIO_MasteringDisplayMetadata;

typedef struct AEIO_ContentLighLevelMetadata
{
    A_FpLong max_content_light_level;
    A_FpLong max_frame_avg_light_level;
} AEIO_ContentLighLevelMetadata;

#endif	//	_AEIO_ColorMetadata_Public_H
