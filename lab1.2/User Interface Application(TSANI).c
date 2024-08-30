//==============================================================================
//
// Title:       User Interface Application(TSANI)
// Purpose:     A short description of the application.
//
// Created on:  20.02.2024 at 12:36:48 by TC.
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
#define MAX_LENGTH 1000

//==============================================================================
// Types

//==============================================================================
// Static global variables

static int panelHandle;

//==============================================================================
// Static functions

//==============================================================================
// Global variables
double arr_signal[MAX_LENGTH];
double arr_spectrum_real[MAX_LENGTH];
double arr_spectrum_im[MAX_LENGTH];
double arr_spectrum[MAX_LENGTH];
double AMPL;
double FRQ;
double NOISE;

//==============================================================================
// Global functions
void CreateSinSignal(){
	for(int i=0; i < MAX_LENGTH; i++){
		arr_signal[i] = AMPL * sin(i * 0.001 * 2 * Pi() * FRQ) + Random(-NOISE, NOISE);
	}				    
}

void DeleteSignalAndSpectrum(){
	DeleteGraphPlot(panelHandle, PANEL_GRAPH, -1, VAL_DELAYED_DRAW);
	DeleteGraphPlot(panelHandle, PANEL_GRAPH_2, -1, VAL_DELAYED_DRAW);
}

void CreateSpectrum(){
	memcpy(arr_spectrum_real, arr_signal, sizeof(double) * MAX_LENGTH);
	memset(arr_spectrum_im, 0, sizeof(double) * MAX_LENGTH);
	FFT(arr_spectrum_real,arr_spectrum_im, MAX_LENGTH);
	for (int l = 0; l < MAX_LENGTH; l++){
		arr_spectrum[l] = sqrt(arr_spectrum_real[l]*arr_spectrum_real[l] + arr_spectrum_im[l]*arr_spectrum_im[l]);
	}
}

void DrawSignal(){
	PlotY(panelHandle, PANEL_GRAPH, arr_signal, MAX_LENGTH, VAL_DOUBLE, VAL_CONNECTED_POINTS, VAL_X, VAL_SOLID, 1, VAL_RED);
	PlotY(panelHandle, PANEL_GRAPH_2, arr_spectrum, MAX_LENGTH, VAL_DOUBLE, VAL_CONNECTED_POINTS, VAL_X, VAL_SOLID, 1,VAL_YELLOW);
}
	

/// HIFN The main entry-point function.
int main (int argc, char *argv[])
{
		
    int error = 0;
    
    /* initialize and load resources */
    nullChk (InitCVIRTE (0, argv, 0));
    errChk (panelHandle = LoadPanel (0, "User Interface Application(TSANI).uir", PANEL));
	
    /* display the panel and run the user interface */
    errChk (DisplayPanel (panelHandle));
    errChk (RunUserInterface ());

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


int CVICALLBACK TIMER_CALLBACK (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_TIMER_TICK:
			GetCtrlVal(panelHandle, PANEL_NUMERIC, &AMPL);
			GetCtrlVal(panelHandle, PANEL_NUMERIC_2, &FRQ);
			GetCtrlVal(panelHandle, PANEL_NUMERICKNOB, &NOISE);
			DeleteSignalAndSpectrum();
			CreateSinSignal();
			CreateSpectrum();
			DrawSignal();
			break;
	}
	return 0;
}

int CVICALLBACK SAVE_CALLBACK (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			FILE* file = fopen("d:\\TSANI\\WORK\\21352\\Grigoruk\\lab1.2\\signal.txt","w+");
			for(int k = 0; k < MAX_LENGTH; k++){
				fprintf(file, "t = %d, V = %lf\n", k, arr_signal[k]);
			}
			fclose(file);
			break;
	}
	return 0;
}
