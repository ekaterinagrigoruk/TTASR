//==============================================================================
//
// Title:       User Interface Application(TSANI)
// Purpose:     A short description of the application.
//
// Created on:  12.03.2024 at 13:22:41 by TC.
// Copyright:   ANY. All Rights Reserved.
//
//==============================================================================

//==============================================================================
// Include files

#include <ansi_c.h>
#include <cvirte.h>     
#include <userint.h>
#include <tsani.h>
#include "User Interface Application(TSANI).h"
#include "toolbox.h"
#define IND1 5
#define IND2 0
#define BUTTON_PRESED(b, i) (b & (1 << i))

//==============================================================================
// Constants

//==============================================================================
// Types

//==============================================================================
// Static global variables

static int panelHandle;

//==============================================================================
// Static functions

//==============================================================================
// Global variables
unsigned char prev_button = 0;
int S2_PRESSED = 0;
int S3_PRESSED = 0;
//==============================================================================
// Global functions

/// HIFN The main entry-point function.
int main (int argc, char *argv[])
{
    int error = 0;
    
    /* initialize and load resources */
    nullChk (InitCVIRTE (0, argv, 0));
    errChk (panelHandle = LoadPanel (0, "User Interface Application(TSANI).uir", PANEL));
    ni6251Slot(2);
	
	portMask(2, 7);
	portOut(2, 1);
	
	portMask(0, 0xFF);
	portMask(1, 0x0F);
	
    /* display the panel and run the user interface */
    errChk (DisplayPanel (panelHandle));
    errChk (RunUserInterface ());
	ni6251Close();
Error:
    /* clean up */
    DiscardPanel (panelHandle);
    return 0;
}

//==============================================================================
// UI callback function prototypes

/// HIFN Exit when the user dismisses the panel.
int CVICALLBACK panelCB (int panel, int event, void *callbackData,
        int eventData1, int eventData2)
{
    if (event == EVENT_CLOSE)
        QuitUserInterface (0);
    return 0;
}

int CVICALLBACK TIMERCALLBACK (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_TIMER_TICK:
			unsigned char button;
			unsigned char light;
			portIn(1, &button);
			portIn(0, &light);
			unsigned short diods = button;
			diods = (diods << 8) + light;
			SetCtrlVal(panelHandle, PANEL_NUMERIC, button);
		    if(!BUTTON_PRESED(button, 4) && (BUTTON_PRESED(prev_button, 4))){
				diods = diods ^ (1 << (IND1 % 8));
	        }
			if(!BUTTON_PRESED(button, 5) || S2_PRESSED){
				S2_PRESSED = 1;
				S3_PRESSED = 0;
				unsigned short diods1 = diods << 1;
				unsigned short diods2 = !!(diods & (1 << 11));
				diods = diods1 | diods2;
				
			}
			if(!BUTTON_PRESED(button, 6) || S3_PRESSED){
				S3_PRESSED = 1;
				S2_PRESSED = 0;
				unsigned short diods1 = (diods & 0xFFF) >> 1;
				unsigned short diods2 = (diods & 1) << 11;
				diods = diods1 | diods2;
				
			}
			portOut(0, diods);
			portOut(1, diods >> 8);
			if (!BUTTON_PRESED(button, 7)){
				S2_PRESSED = 0;
				S3_PRESSED = 0;
			}
			prev_button = button;
			
	}
	return 0;
}
