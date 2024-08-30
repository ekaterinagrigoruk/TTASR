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
#define  PANEL_COMMANDBUTTON              2       /* callback function: CALLBACK_START_SNAKE */
#define  PANEL_COMMANDBUTTON_2            3       /* callback function: CALLBACK_STOP_SNAKE */
#define  PANEL_COMMANDBUTTON_3            4       /* callback function: CALLBACK_READ */
#define  PANEL_STRING                     5
#define  PANEL_NUMERIC                    6
#define  PANEL_TIMER                      7       /* callback function: CALLBACK_TIMER */
#define  PANEL_TIMER_2                    8       /* callback function: CALLBACK_TIMER2 */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK CALLBACK_READ(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK CALLBACK_START_SNAKE(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK CALLBACK_STOP_SNAKE(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK CALLBACK_TIMER(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK CALLBACK_TIMER2(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK panelCB(int panel, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
