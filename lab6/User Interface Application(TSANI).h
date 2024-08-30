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
#define  PANEL_NUMERIC_FREQ               2
#define  PANEL_NUMERIC_AMPL               3
#define  PANEL_RING_SIGNAL                4
#define  PANEL_GRAPH                      5
#define  PANEL_COMMANDBUTTON              6       /* callback function: CALLBACK_GENERATE */
#define  PANEL_NUMERIC_OSCFREQ            7       /* callback function: OSC_FREQ_CALLBACK */
#define  PANEL_NUMERIC_REALOSCFREQ        8
#define  PANEL_GRAPH_2                    9
#define  PANEL_NUMERIC_MAXFREQ            10
#define  PANEL_NUMERIC_MINFREQ            11
#define  PANEL_NUMERIC_N                  12
#define  PANEL_NUMERIC_H                  13
#define  PANEL_COMMANDBUTTON_2            14      /* callback function: FREQSEQUENCE_CALLBACK */
#define  PANEL_RING_SIGTYPE2              15
#define  PANEL_NUMERIC_AMPL2              16
#define  PANEL_TOGGLEBUTTON               17      /* callback function: ACQUISITIONCALLBACK */
#define  PANEL_NUMERIC                    18
#define  PANEL_TIMER                      19      /* callback function: TIMERCALLBACK */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK ACQUISITIONCALLBACK(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK CALLBACK_GENERATE(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK FREQSEQUENCE_CALLBACK(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OSC_FREQ_CALLBACK(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK panelCB(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK TIMERCALLBACK(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
