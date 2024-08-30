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
#define  PANEL_GRAPH_CAD                  2
#define  PANEL_START_CAD                  3       /* callback function: SHOW_CAD */
#define  PANEL_PARSER                     4       /* callback function: CAD_PARSER */
#define  PANEL_DIFFERENCE_2               5
#define  PANEL_DIFFERENCE                 6
#define  PANEL_DIFFERENCE2_2              7
#define  PANEL_DIFFERENCE2                8
#define  PANEL_CODE                       9
#define  PANEL_DIFFERENTIAL               10
#define  PANEL_INTEGRAL_2                 11
#define  PANEL_INTEGRAL                   12
#define  PANEL_START_ADC                  13      /* callback function: SHOW_ACD */
#define  PANEL_GRAPH_ACD                  14
#define  PANEL_TIMER_CAD                  15


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK CAD_PARSER(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK panelCB(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK SHOW_ACD(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK SHOW_CAD(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
