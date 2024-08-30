//==============================================================================
//
// Title:       User Interface Application(TSANI)
// Purpose:     A short description of the application.
//
// Created on:  09.04.2024 at 11:21:56 by TC.
// Copyright:   ANY. All Rights Reserved.
//
//==============================================================================

//==============================================================================
// Include files

#include <analysis.h>
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
int ring_state;

double power;

double ai2;
double temperature;
double big_T = 1;

double k1 = 7;
double k2 = 0.8;
double k3 = 0.5;

double last_x[10];
double last_temperatures[3];
int t = 0;

double quant = 0.1;
unsigned char line_state;
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
	
	portMask(1, 1 << 7);
	
	portOut(1, 0);
	SetCtrlAttribute(panelHandle, PANEL_TIMER_2, ATTR_INTERVAL, big_T);
	SetCtrlVal(panelHandle, PANEL_K1, k1);
	SetCtrlVal(panelHandle, PANEL_K2, k2);
	SetCtrlVal(panelHandle, PANEL_K3, k3);
    /* display the panel and run the user interface */
    errChk (DisplayPanel (panelHandle));
    errChk (RunUserInterface ());
	
	portOut(1, 0);
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

int CVICALLBACK MODE_CALLBACK (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal(panelHandle, PANEL_RING, &ring_state); 
			if (ring_state == 0){  //off
				SetCtrlAttribute(panelHandle, PANEL_TIMER, ATTR_ENABLED, 0);   
				portOut(1, 0);
				SetCtrlAttribute(panelHandle, PANEL_NUMERIC, ATTR_CTRL_MODE, VAL_HOT);
				power = 0;
				SetCtrlVal(panelHandle, PANEL_NUMERIC,power);
			}
			else if(ring_state == 1){  //on
				SetCtrlAttribute(panelHandle, PANEL_TIMER, ATTR_ENABLED, 0);   
				portOut(1, 1 << 7);
				SetCtrlAttribute(panelHandle, PANEL_NUMERIC, ATTR_CTRL_MODE, VAL_HOT);
				power = 1;
				SetCtrlVal(panelHandle, PANEL_NUMERIC,power);
			}
			else if(ring_state == 2){	 //pwm
				SetCtrlAttribute(panelHandle, PANEL_TIMER, ATTR_ENABLED, 1);
				SetCtrlAttribute(panelHandle, PANEL_NUMERIC, ATTR_CTRL_MODE, VAL_HOT);
			}
			else if(ring_state == 3){   //pid
				SetCtrlAttribute(panelHandle, PANEL_TIMER, ATTR_ENABLED, 1);
				SetCtrlAttribute(panelHandle, PANEL_NUMERIC, ATTR_CTRL_MODE, VAL_INDICATOR);
			}
			break;
	}
	return 0;
}

int CVICALLBACK TIMER_CALLBACK (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_TIMER_TICK:
			portIn(1, &line_state);
			if(line_state & (1 << 7)){
				if(power >= 1)
					break;
				SetCtrlAttribute(panelHandle, PANEL_TIMER, ATTR_INTERVAL, big_T * (1 - power)); 
				portOut(1, 0);
			}
			else{
				if (power <= 0)
					break;
				SetCtrlAttribute(panelHandle, PANEL_TIMER, ATTR_INTERVAL, big_T * power); 
				portOut(1, 1 << 7);
				
			}
			 
			break;
	}
	return 0;
}

int CVICALLBACK POWER_CALLBACK (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal(panelHandle, PANEL_NUMERIC, &power);
			break;
	}
	return 0;
}

int CVICALLBACK GRAPH_TENPERATURE_CALLBACK (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_TIMER_TICK:
			analogIn(2, &ai2);
			last_temperatures[t % 3] = ai2;
			Sort(last_temperatures, 3, ANALYSIS_SORT_DESCENDING, last_temperatures);
			PlotStripChartPoint(panelHandle, PANEL_GRAPH, ai2 * 100);
			SetCtrlVal(panelHandle, PANEL_NUMERIC_2, ai2 * 100);
			last_x[t % 10] = temperature - last_temperatures[1] * 100;	   
			
			if (ring_state == 3){
				double different = 0;
				if(t % 10 == 0 && t > 10){
				  	 different = last_x[0] - last_x[9];
				}
				else if(t > 10){
					different = last_x[t % 10] - last_x[t % 10 - 1];
				}
				double integral = 0;
				for(int i = 0; i < 10; i++){
					integral += last_x[i];
				}
				power = (k1 * last_x[t % 10] + k2 * integral + k3 * different)/20;
				SetCtrlVal(panelHandle, PANEL_NUMERIC,power);
				if (power < 0) {
/*					if(power > - 0.8){
						power = power/2;
					}
					else{
*/						power = 0;
					};
/*				}
*/				if (power > 1){
/*					if(power < 0.8){
						power = power/2;
					}
					else{
*/						power = 1;
//					};
				}
				SetCtrlVal(panelHandle, PANEL_FIRST, k1 * last_x[t % 10]);
				SetCtrlVal(panelHandle, PANEL_SECOND, k2 * integral);
				SetCtrlVal(panelHandle, PANEL_THIRD, k3 * different);
			}
			t++;
			break;
	}
	return 0;
}

int CVICALLBACK INPUT_TEMP_CALLBACK (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal(panelHandle, PANEL_NUMERIC_3, &temperature);
			break;
	}
	return 0;
}

int CVICALLBACK K1_CALLBACK (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal(panelHandle, PANEL_K1, &k1);
			break;
	}
	return 0;
}

int CVICALLBACK K2_CALLBACK (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal(panelHandle, PANEL_K2, &k2);
			break;
	}
	return 0;
}

int CVICALLBACK K3_CALLBACK (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal(panelHandle, PANEL_K3, &k3);
			break;
	}
	return 0;
}
