/*****************************************************************************
 * video_detector.c - configuration functions for the VTC video timing
 * controller. Note that the "video detection" functions do both detection
 * and generation.
 *
 *
 * NOTES:
 * 02/04/14 by JAZ::Design created.
 *****************************************************************************/

#include <sleep.h>
#include <string.h>
#include <xbasic_types.h>
#include <xil_printf.h>
#include <xil_types.h>
#include <xstatus.h>
#include <xvtc.h>

#include "../SYSTEM_CONFIG_HEADER/system_config_header.h"
#include "../VIDEO_RESOLUTION/video_resolution.h"

/*****************************************************************************/
/**
*
* This function sets up the Video Timing Controller Signal configuration.
*
* @param    None.
*
* @return    None.
*
* @note        None.
*
****************************************************************************/
static void SignalSetup( XVtc *pVtc, Xuint32 ResolutionId, XVtc_Signal *SignalCfgPtr )
{
    vres_timing_t VideoTiming;
    int HFrontPorch;
    int HSyncWidth;
    int HBackPorch;
    int VFrontPorch;
    int VSyncWidth;
    int VBackPorch;
    int LineWidth;
    int FrameHeight;
    vres_get_timing(ResolutionId, &VideoTiming);
    HFrontPorch = VideoTiming.HFrontPorch;
    HSyncWidth  = VideoTiming.HSyncWidth;
    HBackPorch  = VideoTiming.HBackPorch;
    VFrontPorch = VideoTiming.VFrontPorch;
    VSyncWidth  = VideoTiming.VSyncWidth;
    VBackPorch  = VideoTiming.VBackPorch;
    LineWidth   = VideoTiming.HActiveVideo;
    FrameHeight = VideoTiming.VActiveVideo;
    /* Clear the VTC Signal config structure */
    memset((void *)SignalCfgPtr, 0, sizeof(XVtc_Signal));
    /* Populate the VTC Signal config structure. Ignore the Field 1 */
//    SignalCfgPtr->HFrontPorchStart = 0;
//    SignalCfgPtr->HTotal = HFrontPorch + HSyncWidth + HBackPorch
//                + LineWidth - 1;
//    SignalCfgPtr->HBackPorchStart = HFrontPorch + HSyncWidth;
//    SignalCfgPtr->HSyncStart = HFrontPorch;
//    SignalCfgPtr->HActiveStart = HFrontPorch + HSyncWidth + HBackPorch;
//
//    SignalCfgPtr->V0FrontPorchStart = 0;
//    SignalCfgPtr->V0Total = VFrontPorch + VSyncWidth + VBackPorch
//                + FrameHeight - 1;
//    SignalCfgPtr->V0BackPorchStart = VFrontPorch + VSyncWidth;
//    SignalCfgPtr->V0SyncStart = VFrontPorch;
//    SignalCfgPtr->V0ChromaStart = VFrontPorch + VSyncWidth + VBackPorch;
//    SignalCfgPtr->V0ActiveStart = VFrontPorch + VSyncWidth + VBackPorch;
    SignalCfgPtr->HFrontPorchStart = LineWidth;
    SignalCfgPtr->HTotal = HFrontPorch + HSyncWidth + HBackPorch
                + LineWidth;
    SignalCfgPtr->HBackPorchStart = LineWidth + HFrontPorch + HSyncWidth;
    SignalCfgPtr->HSyncStart = LineWidth + HFrontPorch;
    SignalCfgPtr->HActiveStart = 0;
    SignalCfgPtr->V0FrontPorchStart = FrameHeight;
    SignalCfgPtr->V0Total = VFrontPorch + VSyncWidth + VBackPorch
                + FrameHeight;
    SignalCfgPtr->V0BackPorchStart = FrameHeight + VFrontPorch + VSyncWidth;
    SignalCfgPtr->V0SyncStart = FrameHeight + VFrontPorch;
    SignalCfgPtr->V0ChromaStart = 0;
    SignalCfgPtr->V0ActiveStart = 0;
     return;
}
/*****************************************************************************/
/**
*
* vdet_init
* - initializes the VTC detector
*
* @param    VtcDeviceID is the device ID of the Video Timing Controller core.
*           pVtc is a pointer to a VTC instance
*
* @return    0 if all tests pass, 1 otherwise.
*
* @note        None.
*
******************************************************************************/
int vdet_init(XVtc *pVtc, u16 VtcDeviceID)
{
    int Status;
    XVtc_Config *VtcCfgPtr;
    Xuint32 Width;
    Xuint32 Height;
    int ResolutionId;
    /* Look for the device configuration info for the Video Timing
     * Controller.
     */
    VtcCfgPtr = XVtc_LookupConfig( VtcDeviceID );
    if (VtcCfgPtr == NULL) {
        return 1;
    }
    /* Initialize the Video Timing Controller instance */
    Status = XVtc_CfgInitialize(pVtc, VtcCfgPtr,
        VtcCfgPtr->BaseAddress);
    if (Status != XST_SUCCESS) {
        return 1;
    }
    // Enable Synchronization of Generator with Detector
//    XVtc_EnableSync(pVtc);
    XVtc_DisableSync(pVtc);
    /* Enable both generator and detector modules */
    XVtc_Enable(XVTC_EN_GENERATOR | XVTC_EN_DETECTOR);
    return 0;
}
/*****************************************************************************/
/**
*
* vdet_detect
* - enables the detector and detects the timing of incoming video
*
* @param    pVtc is a pointer to an initialized VTC instance
*           vVerbose = 0 no verbose, 1 minimal verbose, 2 most verbose
*
* @return    ResolutionId (defined in video_resolution.h).
*
* @note        None.
*
******************************************************************************/
int vdet_detect(XVtc *pVtc, int bVerbose)
{
    Xuint32 Width;
    Xuint32 Height;
    int ResolutionId;
    XVtc_Signal Signal;        /* VTC Signal configuration */
    XVtc_Polarity Polarity;        /* Polarity configuration */
    XVtc_SourceSelect SourceSelect;    /* Source Selection configuration */
    // Wait 100 msec ...
    usleep(100000);
    // Get Detector
    XVtc_GetDetector(pVtc, &Signal);
    if ( bVerbose == 2 )
    {
        xil_printf("\tVTC Detector Configuration\n\r" );
        xil_printf("\t\tHorizontal Timing:\n\r" );
        xil_printf("\t\t\tHFrontPorchStart %d\r\n", Signal.HFrontPorchStart);
        xil_printf("\t\t\tHSyncStart %d\r\n", Signal.HSyncStart);
        xil_printf("\t\t\tHBackPorchStart %d\r\n", Signal.HBackPorchStart);
        xil_printf("\t\t\tHActiveStart = %d\r\n", Signal.HActiveStart);
        xil_printf("\t\t\tHTotal = %d\r\n", Signal.HTotal);
        xil_printf("\t\tVertical Timing:\n\r" );
        xil_printf("\t\t\tV0FrontPorchStart %d\r\n", Signal.V0FrontPorchStart);
        xil_printf("\t\t\tV0SyncStart %d\r\n", Signal.V0SyncStart);
        xil_printf("\t\t\tV0BackPorchStart %d\r\n", Signal.V0BackPorchStart);
        xil_printf("\t\t\tV0ActiveStart %d\r\n", Signal.V0ActiveStart);
        xil_printf("\t\t\tV0Total %d\r\n", Signal.V0Total);
    }
    Width = (Signal.HTotal  + 1) - Signal.HActiveStart;
    Height = (Signal.V0Total + 1) - Signal.V0ActiveStart;
    if ( bVerbose )
    {
        xil_printf( "\tVideo Dimensions = %d x %d\n\r", Width, Height );
    }
    ResolutionId = vres_detect( Width, Height );
    if ( bVerbose )
    {
        xil_printf( "\tVideo Resolution = %s\n\r", vres_get_name(ResolutionId) );
    }
    return ResolutionId;
}
/*****************************************************************************/
/**
*
* vdet_config
* - configures the generator to generate missing syncs
*
* @param    pVtc is a pointer to an initialized VTC instance
*           ResolutionId identified a video resolution
*           vVerbose = 0 no verbose, 1 minimal verbose, 2 most verbose
*
* @return    0 if all tests pass, 1 otherwise.
*
* @note        None.
*
******************************************************************************/
int vdet_config(XVtc *pVtc, int ResolutionId, int bVerbose)
{
    int Status;
    XVtc_Signal Signal;        /* VTC Signal configuration */
    XVtc_Polarity Polarity;        /* Polarity configuration */
    XVtc_HoriOffsets HoriOffsets;  /* Horizontal offsets configuration */
    XVtc_SourceSelect SourceSelect;    /* Source Selection configuration */
    if ( bVerbose )
    {
        xil_printf( "\tVideo Resolution = %s\n\r", vres_get_name(ResolutionId) );
    }
    /* Set up Polarity of all outputs */
    memset((void *)&Polarity, 0, sizeof(Polarity));
    Polarity.ActiveChromaPol = 1;
    Polarity.ActiveVideoPol = 1;
    Polarity.FieldIdPol = 0;
    Polarity.VBlankPol = 1;
    Polarity.VSyncPol = 1;
    Polarity.HBlankPol = 1;
    Polarity.HSyncPol = 1;
    XVtc_SetPolarity(pVtc, &Polarity);
    /* Set up Generator */
    memset((void *)&HoriOffsets, 0, sizeof(HoriOffsets));
    HoriOffsets.V0BlankHoriEnd = SCREEN_WIDTH_HORIZONTAL;
    HoriOffsets.V0BlankHoriStart = SCREEN_WIDTH_HORIZONTAL;
    HoriOffsets.V0SyncHoriEnd = SCREEN_WIDTH_HORIZONTAL;
    HoriOffsets.V0SyncHoriStart = SCREEN_WIDTH_HORIZONTAL;
    XVtc_SetGeneratorHoriOffset(pVtc, &HoriOffsets);
    SignalSetup(pVtc,ResolutionId, &Signal);
    if ( bVerbose == 2 )
    {
        xil_printf("\tVTC Generator Configuration\n\r" );
        xil_printf("\t\tHorizontal Timing:\n\r" );
        xil_printf("\t\t\tHFrontPorchStart %d\r\n", Signal.HFrontPorchStart);
        xil_printf("\t\t\tHSyncStart %d\r\n", Signal.HSyncStart);
        xil_printf("\t\t\tHBackPorchStart %d\r\n", Signal.HBackPorchStart);
        xil_printf("\t\t\tHActiveStart = %d\r\n", Signal.HActiveStart);
        xil_printf("\t\t\tHTotal = %d\r\n", Signal.HTotal);
        xil_printf("\t\tVertical Timing:\n\r" );
        xil_printf("\t\t\tV0FrontPorchStart %d\r\n", Signal.V0FrontPorchStart);
        xil_printf("\t\t\tV0SyncStart %d\r\n", Signal.V0SyncStart);
        xil_printf("\t\t\tV0BackPorchStart %d\r\n", Signal.V0BackPorchStart);
        xil_printf("\t\t\tV0ActiveStart %d\r\n", Signal.V0ActiveStart);
        xil_printf("\t\t\tV0Total %d\r\n", Signal.V0Total);
    }
    XVtc_SetGenerator(pVtc, &Signal);
    /* Set up source select */
    memset((void *)&SourceSelect, 0, sizeof(SourceSelect));
    SourceSelect.VChromaSrc = 0;
    SourceSelect.VActiveSrc = 1;
    SourceSelect.VBackPorchSrc = 1;
    SourceSelect.VSyncSrc = 1;
    SourceSelect.VFrontPorchSrc = 1;
    SourceSelect.VTotalSrc = 1;
    SourceSelect.HActiveSrc = 1;
    SourceSelect.HBackPorchSrc = 1;
    SourceSelect.HSyncSrc = 1;
    SourceSelect.HFrontPorchSrc = 1;
    SourceSelect.HTotalSrc = 1;
    XVtc_SetSource(pVtc, &SourceSelect);
    // Enable Synchronization of Generator with Detector
    // XVtc_EnableSync(pVtc);
    XVtc_DisableSync(pVtc);
    /* Return success */
    return 0;
}
