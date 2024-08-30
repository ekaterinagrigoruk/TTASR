//==============================================================================
//
// Title:       User Interface Application(TSANI)
// Purpose:     A short description of the application.
//
// Created on:  05.03.2024 at 13:30:44 by TC.
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
#define MAX_LENGTH 50
#define kx - 100./243
#define ky 100./243
#define bx 1
#define by -1
//==============================================================================
// Types

//==============================================================================
// Static global variables

static int panelHandle;

//==============================================================================
// Static functions

//==============================================================================
// Global variables
double Ain2;
double Ain3;

typedef struct{
	int array_handles[MAX_LENGTH];
	int last_index;
}Signal;

Signal joystick = {.array_handles = {0}, .last_index = 0};
//==============================================================================
// Global functions
void NewPoint(int h){
	joystick.array_handles[joystick.last_index] = h;
	if (joystick.last_index == MAX_LENGTH - 1) {
		joystick.last_index = 0;
	}else{
		joystick.last_index += 1;
	}
}
	
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
			analogIn(2, &Ain2);
			analogIn(3, &Ain3);
			double x = kx * Ain2 + bx;
			double y = ky * Ain3 + by;
			int H = PlotXY(panelHandle, PANEL_GRAPH, &x, &y, 1, VAL_DOUBLE,VAL_DOUBLE, VAL_CONNECTED_POINTS,VAL_SOLID_SQUARE,VAL_SOLID, 1, VAL_GREEN);
			if(joystick.array_handles[joystick.last_index] != 0){
				DeleteGraphPlot(panelHandle, PANEL_GRAPH,joystick.array_handles[joystick.last_index],VAL_IMMEDIATE_DRAW);
			}
			NewPoint(H);
			SetCtrlVal(panelHandle,PANEL_NUMERIC,Ain2);
			SetCtrlVal(panelHandle,PANEL_NUMERIC_3, x);
			SetCtrlVal(panelHandle, PANEL_NUMERIC_2, Ain3);
			SetCtrlVal(panelHandle,PANEL_NUMERIC_4, y);
			break;
	}
	return 0;
}
