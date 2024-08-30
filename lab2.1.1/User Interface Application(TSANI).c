//==============================================================================
//
// Title:       User Interface Application(TSANI)
// Purpose:     A short description of the application.
//
// Created on:  27.02.2024 at 12:48:00 by TC.
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
double Aout0and1;
double Ain1;
double Ain0;
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
    if (event == EVENT_CLOSE){
		QuitUserInterface (0);
		analogOut(0, 0);
		analogOut(1, 0);
		
	}
    return 0;
}

int CVICALLBACK OUT0AND1CALLBACK (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal(panelHandle, PANEL_NUMERICSLIDE, &Aout0and1);
			analogOut(0, Aout0and1);
			analogOut(1, Aout0and1);
			break;
	}
	return 0;
}

int CVICALLBACK TIMERCALLBACK (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_TIMER_TICK:
			analogIn(1, &Ain1);
			analogIn(0, &Ain0);
			SetCtrlVal(panelHandle,PANEL_NUMERICSLIDE_3, Ain1);
			SetCtrlVal(panelHandle,PANEL_NUMERICSLIDE_2, Ain0);
			break;
	}
	return 0;
}
