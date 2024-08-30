//==============================================================================
//
// Title:       User Interface Application(TSANI)
// Purpose:     A short description of the application.
//
// Created on:  26.03.2024 at 12:38:56 by TC.
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
#include <time.h>
#include "User Interface Application(TSANI).h"
#include "toolbox.h"

//==============================================================================
// Constants
#define MAX_LENGTH 5000000
//==============================================================================
// Types

//==============================================================================
// Static global variables

static int panelHandle;

//==============================================================================
// Static functions

//==============================================================================
// Global variables
double freq, ampl;
int signal_type;

int PtNum = MAX_LENGTH, nrd;
double Range = 10.0;
double TrigLevel = 0.5;
double* wfm;

double freq_osc;
double real_freq_osc;

int GENERATE = 0;

double arr_spectrum_real[MAX_LENGTH];
double arr_spectrum_im[MAX_LENGTH];
double arr_spectrum[MAX_LENGTH];
double arr_freq[MAX_LENGTH];

int N;
float freq_min, freq_max;
float H;
double *freq_list, *time_interval_list;
//==============================================================================
// Global functions
void InitDevices(){
	int res5402 = fgenSlot(6);
	int res5114 = scopeSlot(7); 
	if (res5402 < 0 || res5114 < 0){
		MessagePopup("Init error", "The module wasn't found");
		exit(-1);
	}
	scopeVertical("0", Range, SCOPE_1_MEG_OHM);
	scopeTrigger("0",TrigLevel,SCOPE_POSITIVE);
	wfm = (double*)malloc(PtNum*sizeof(double));
}

void CloseDevices(){
	fgenClose();
	scopeClose();
}

void StartGeneration(){
	GetCtrlVal(panelHandle,PANEL_NUMERIC_FREQ, &freq);
	GetCtrlVal(panelHandle, PANEL_NUMERIC_AMPL, &ampl);
	GetCtrlVal(panelHandle, PANEL_RING_SIGNAL, &signal_type);
	fgenStartStandartWaveForm(ampl, freq, signal_type);
}

void StartGenerationSequency(){
	GetCtrlVal(panelHandle,PANEL_NUMERIC_N, &N);
	GetCtrlVal(panelHandle, PANEL_NUMERIC_H, &H);
	GetCtrlVal(panelHandle, PANEL_NUMERIC_MINFREQ, &freq_min);
	GetCtrlVal(panelHandle, PANEL_NUMERIC_MAXFREQ, &freq_max);
	GetCtrlVal(panelHandle, PANEL_RING_SIGTYPE2, &signal_type);
	GetCtrlVal(panelHandle, PANEL_NUMERIC_AMPL2, &ampl);
	freq_list = (double*)malloc(N*sizeof(double));
	time_interval_list = (double*)malloc(N*sizeof(double));
	fgenGenerateFreqList(N, freq_min, freq_max, H, freq_list, time_interval_list);
	fgenExportTrig("PFI0");
	fgenStart(signal_type, ampl, N, freq_list, time_interval_list);
}
	

void StopGeneration(){
	fgenStop();
}

void CreateSpectrum(){
	memcpy(arr_spectrum_real, wfm, sizeof(double) * MAX_LENGTH);
	memset(arr_spectrum_im, 0, sizeof(double) * MAX_LENGTH);
	FFT(arr_spectrum_real,arr_spectrum_im, MAX_LENGTH/10);
	
	for (int l = 0; l < MAX_LENGTH; l++){
		arr_spectrum[l] = sqrt(arr_spectrum_real[l]*arr_spectrum_real[l] + arr_spectrum_im[l]*arr_spectrum_im[l]);
		arr_freq[l] = real_freq_osc * l / MAX_LENGTH;
	}

	
}

void ReadPlotWaveForm(){
	scopeStart(PtNum);
	while (scopeStatus() != SCOPE_COMPLETE);
	scopeFetch("0", wfm, PtNum, &nrd);
			double start = Timer();
	 
	CreateSpectrum();
	DeleteGraphPlot(panelHandle, PANEL_GRAPH, -1, VAL_IMMEDIATE_DRAW);
	DeleteGraphPlot(panelHandle, PANEL_GRAPH_2, -1, VAL_IMMEDIATE_DRAW);
	PlotY(panelHandle, PANEL_GRAPH, wfm, PtNum, VAL_DOUBLE, VAL_THIN_LINE,VAL_SOLID_SQUARE, VAL_DASH, 1, VAL_BLUE);
 
	//PlotXY(panelHandle, PANEL_GRAPH_2, arr_freq, arr_spectrum, MAX_LENGTH/2, VAL_DOUBLE, VAL_DOUBLE, VAL_CONNECTED_POINTS, VAL_X, VAL_SOLID, 1,VAL_YELLOW);
	PlotY(panelHandle, PANEL_GRAPH_2, arr_spectrum, MAX_LENGTH/2, VAL_DOUBLE, VAL_THIN_LINE,VAL_SOLID_SQUARE, VAL_DASH, 1, VAL_BLUE); 
			double end = Timer() - start;
			SetCtrlVal(panelHandle, PANEL_NUMERIC, end);
	
}


/// HIFN The main entry-point function.
int main (int argc, char *argv[])
{
    int error = 0;
    
    /* initialize and load resources */
    nullChk (InitCVIRTE (0, argv, 0));
    errChk (panelHandle = LoadPanel (0, "User Interface Application(TSANI).uir", PANEL));
	
	InitDevices();
	
    
    /* display the panel and run the user interface */
    errChk (DisplayPanel (panelHandle));
    errChk (RunUserInterface ());
	StopGeneration();
	CloseDevices();
	
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
			if (GENERATE == 1){
				ReadPlotWaveForm();
			}
			break;
	}
	return 0;
}

int CVICALLBACK CALLBACK_GENERATE (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			StartGeneration();
			GENERATE = 1;
			break;
	}
	return 0;
}

int CVICALLBACK FREQSEQUENCE_CALLBACK (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			StartGenerationSequency();
			GENERATE = 1;
			break;
	}
	return 0;
}

int CVICALLBACK ACQUISITIONCALLBACK (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			int ACQ;
			GetCtrlVal(panelHandle, PANEL_TOGGLEBUTTON, &ACQ);
			SetCtrlAttribute(panelHandle, PANEL_TIMER,ATTR_ENABLED, ACQ);
			break;
	}
	return 0;
}

int CVICALLBACK OSC_FREQ_CALLBACK (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal(panelHandle, PANEL_NUMERIC_OSCFREQ, &freq_osc);
			scopeGetFrequency(&real_freq_osc);
			SetCtrlVal(panelHandle, PANEL_NUMERIC_REALOSCFREQ, real_freq_osc);
			scopeFrequency(NULL, freq_osc, PtNum);
			break;
	}
	return 0;
}
