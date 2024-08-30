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
#define  PANEL_NUMERIC_ADDR_2             2
#define  PANEL_SUBADDR_2                  3
#define  PANEL_DATA_2                     4
#define  PANEL_NUMERIC_ADDR               5
#define  PANEL_SUBADDR                    6
#define  PANEL_DATA                       7
#define  PANEL_WRITE                      8       /* callback function: WRITE_CALLBACK */
#define  PANEL_READ                       9       /* callback function: READ_CALLBACK */
#define  PANEL_DAC_CODE                   10      /* callback function: DAC_CODE_CALLBACK */
#define  PANEL_DAC_VOLTAGE                11      /* callback function: DAC_VOLTAGE_CALLBACK */
#define  PANEL_Vin2_CODE                  12
#define  PANEL_Vin1_CODE                  13
#define  PANEL_Vin2_VOLTAGE_2             14
#define  PANEL_Vin1_VOLTAGE               15
#define  PANEL_TIMER                      16      /* callback function: TIMER_CALLBACK */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK DAC_CODE_CALLBACK(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK DAC_VOLTAGE_CALLBACK(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK panelCB(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK READ_CALLBACK(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK TIMER_CALLBACK(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK WRITE_CALLBACK(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
