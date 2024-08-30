/**************************************************************************/
/* LabWindows/CVI User Interface Resource (UIR) Include File              */
/* Copyright (c) National Instruments 2024. All Rights Reserved.          */
/*                                                                        */
/* WARNING: Do not add to, delete from, or otherwise modify the contents  */
/*          of this include file.                                         */
/**************************************************************************/

#include <userint.h>

#ifdef __cplusplus
    extern "C" {
#endif

     /* Panels and Controls: */

#define  PANEL                            1       /* callback function: panelCB */
#define  PANEL_RING                       2       /* callback function: MODE_CALLBACK */
#define  PANEL_NUMERIC                    3       /* callback function: POWER_CALLBACK */
#define  PANEL_NUMERIC_2                  4
#define  PANEL_TIMER                      5       /* callback function: TIMER_CALLBACK */
#define  PANEL_TIMER_2                    6       /* callback function: GRAPH_TENPERATURE_CALLBACK */
#define  PANEL_NUMERIC_3                  7       /* callback function: INPUT_TEMP_CALLBACK */
#define  PANEL_K1                         8       /* callback function: K1_CALLBACK */
#define  PANEL_K2                         9       /* callback function: K2_CALLBACK */
#define  PANEL_K3                         10      /* callback function: K3_CALLBACK */
#define  PANEL_FIRST                      11
#define  PANEL_SECOND                     12
#define  PANEL_THIRD                      13
#define  PANEL_GRAPH                      14


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK GRAPH_TENPERATURE_CALLBACK(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK INPUT_TEMP_CALLBACK(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK K1_CALLBACK(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK K2_CALLBACK(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK K3_CALLBACK(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK MODE_CALLBACK(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK panelCB(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK POWER_CALLBACK(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK TIMER_CALLBACK(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
