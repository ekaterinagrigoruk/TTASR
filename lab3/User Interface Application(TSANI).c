//==============================================================================
//
// Title:       User Interface Application(TSANI)
// Purpose:     A short description of the application.
//
// Created on:  14.05.2024 at 12:18:00 by TC.
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
unsigned char address;
short subaddress;
int data;
int dac_code;
double voltage;
int Vin1;
int Vin2;
int start = 1;

double voltage_arr [256];
double adc_voltage_arr[30000];
double adc_code_arr[30000];
//==============================================================================
// Global functions


void Avalon_deinit(){
	ni6251Close();
}

void Avalon_set_address(unsigned char address, short subaddress){
	short complete_addr = subaddress << 3 | (address & 0x07);
	portOut(0, complete_addr & 0xFF);
	portOut(1, complete_addr >> 8);
}

void Avalon_write(unsigned char address, short subaddress, int data){
	Avalon_set_address (address, subaddress);
	
	portOut(2, 0x07);
	portOut(2, 0x06);
	
	portOut(0, data & 0xFF);
	portOut(1, data >> 8);
	
	portOut(2, 0x02);
	portOut(2, 0x06);
}

void Avalon_init(){
	ni6251Slot(2);
	portMask(2, 7);
	portMask(0, 0xFF);
	portMask(1, 0xFF);
	portOut(2, 0x06);
	Avalon_write(2, 0x00, 3);
}

void Avalon_read(unsigned char address, short subaddress, int* data){
	Avalon_set_address (address, subaddress);
	
	portOut(2, 0x07);
	portOut(2, 0x06);
	
	portMask(0, 0x00);
	portMask(1, 0x00);
	
	portOut(2, 0x04);
	
	unsigned char part1, part2;
	portIn(0, &part1); 
	portIn(1, &part2);
	*data = part1 | (part2 << 8);
	
	portOut(2, 0x06);
	
	portMask(0, 0xFF);
	portMask(1, 0xFF);
}

void Avalon_dac(int code){
	Avalon_write(2, 2, code);
	Avalon_write(2, 3, code);
}

double Avalon_dac_code_to_voltage(int code){
	return (double)(3.3 * code / 255);
}

double Avalon_adc_code_to_voltage(int code){
	return (double)(2.56 * code / 1023);
}

int Avalon_dac_voltage_to_code(double voltage){
	return (int)(255 * voltage / 3.3); 
}

int Avalon_adc_voltage_to_code(double voltage){
	return (int)(1023 * voltage / 2.56);
}

/// HIFN The main entry-point function.
int main (int argc, char *argv[])
{
    int error = 0;
    
    /* initialize and load resources */
    nullChk (InitCVIRTE (0, argv, 0));
    errChk (panelHandle = LoadPanel (0, "User Interface Application(TSANI).uir", PANEL));
	
    Avalon_init();
	
    /* display the panel and run the user interface */
    errChk (DisplayPanel (panelHandle));
    errChk (RunUserInterface ());
	
	Avalon_deinit();  

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

int CVICALLBACK SHOW_CAD (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			int counter = 0;
			double Vout1;
			FILE* file = fopen("input.txt", "w");
			for(int counter = 0; counter <= 255; counter ++){
				Avalon_dac(counter);
				analogIn(0, &Vout1);
				fprintf(file, "%d %lf \n", counter, Vout1);
			}
			fclose(file);
				
			break;
	}
	return 0;
}





int CVICALLBACK CAD_PARSER (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			FILE* file = fopen("input.txt", "r");
			double Vout1;
			int counter;
			
			for(int i = 0; i <= 255; i++){
				fscanf(file, "%d %lf \n", &counter, &Vout1);
				voltage_arr[counter] = Vout1;
			}
			PlotY(panelHandle, PANEL_GRAPH_CAD, voltage_arr, 256, VAL_DOUBLE, VAL_CONNECTED_POINTS, VAL_EMPTY_SQUARE, VAL_SOLID, 1, VAL_GREEN);
			
			double difference = voltage_arr[0];
			for(int k = 0; k <= 255; k++){
				voltage_arr[k] -= difference;
			}
			SetCtrlVal(panelHandle, PANEL_DIFFERENCE, difference);
			PlotY(panelHandle, PANEL_GRAPH_CAD, voltage_arr, 256, VAL_DOUBLE, VAL_CONNECTED_POINTS, VAL_EMPTY_SQUARE, VAL_SOLID, 1, VAL_RED);
			
			double our_k =  voltage_arr[255] / 255;
			double k =  3.3 / 255;
			
			for(int j = 0; j <= 255; j++){
				voltage_arr[j] = voltage_arr[j] * (k / our_k);
			}
			PlotY(panelHandle, PANEL_GRAPH_CAD, voltage_arr, 256, VAL_DOUBLE, VAL_CONNECTED_POINTS, VAL_EMPTY_SQUARE, VAL_SOLID, 1, VAL_BLUE);
			SetCtrlVal(panelHandle, PANEL_DIFFERENCE2, k / our_k);
			
			double int_mistake = 0;
			for(int l = 0; l <= 255; l++){
				if(fabs(voltage_arr[l] - k * l) > int_mistake){
					int_mistake = fabs(voltage_arr[l] - k * l);
				}
			}
			SetCtrlVal(panelHandle, PANEL_INTEGRAL, int_mistake);
			
			double diff_mistake = 0;
			for(int m = 0; m <= 255; m++){
				if(fabs(voltage_arr[m] - (3.3 / 256) * m) > diff_mistake){
					diff_mistake = fabs(voltage_arr[m] - (3.3 / 256) * m);
					SetCtrlVal(panelHandle, PANEL_CODE, m);
				}
			}
			SetCtrlVal(panelHandle, PANEL_DIFFERENTIAL, diff_mistake);
			
			break;
	}
	return 0;
}

int CVICALLBACK SHOW_ACD (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			FILE* file = fopen("output.txt", "r");
			
			double Vin1;
			int code;
			int step = 0;
			while(!feof(file)){
				fscanf(file, "%lf %d \n", &Vin1, &code);
				adc_voltage_arr[step] = Vin1;
				adc_code_arr[step] = code;
				step++;
			}
			//PlotXY(panelHandle, PANEL_GRAPH_ACD, adc_voltage_arr, adc_code_arr, step, VAL_DOUBLE, VAL_INTEGER, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, 1, VAL_RED);
			
			double x1;
			double x2;
			int flag = 0;
			for(int i = 0; i < step; i++){
				if(adc_code_arr[i] == 0){
					x1 = adc_voltage_arr[i];
				}
				if(!flag && adc_code_arr[i] == 1023){
					x2 = adc_voltage_arr[i];
					flag = 1;
				}
			}
			double LSB = 2.56 / 1024;
			double k0 = 1 / (1 * LSB);
			double k_our = k0 * ( 2.56 - 2 * LSB) / (x2 - x1);
			double b = k0 * LSB - k_our * x1;
			double shift = - b / k_our;
			SetCtrlVal(panelHandle, PANEL_DIFFERENCE_2, shift);
			SetCtrlVal(panelHandle, PANEL_DIFFERENCE2_2, (2.56 * k0 - (2.56 * k_our + b)) );
			for(int i = 0; i < step; i++){
				adc_code_arr[i] = (k0 / k_our) * adc_code_arr[i] - b; 
			}
			//PlotXY(panelHandle, PANEL_GRAPH_ACD, adc_voltage_arr, adc_code_arr, step, VAL_DOUBLE, VAL_INTEGER, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, 1, VAL_BLUE);
						   
			
			for(int i = 0; i < step; i++){
				adc_code_arr[i] -= k0 * adc_voltage_arr[i];
			}
			PlotXY(panelHandle, PANEL_GRAPH_ACD, adc_voltage_arr, adc_code_arr, step, VAL_DOUBLE, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, 1, VAL_GREEN);  
			
					
				
			break;
	}
	return 0;
}
