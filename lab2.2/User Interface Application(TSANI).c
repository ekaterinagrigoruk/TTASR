//==============================================================================
//
// Title:       User Interface Application(TSANI)
// Purpose:     A short description of the application.
//
// Created on:  23.04.2024 at 12:54:18 by TC.
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
//==============================================================================
// Global functions
void Avalon_init(){
	ni6251Slot(2);
	portMask(2, 7);
	portMask(0, 0xFF);
	portMask(1, 0xFF);
	portOut(2, 0x06);
}

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

void Avalon_adc(int* code){
	
	
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

int CVICALLBACK WRITE_CALLBACK (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal(panelHandle, PANEL_NUMERIC_ADDR, &address);
			GetCtrlVal(panelHandle, PANEL_SUBADDR, &subaddress);
			GetCtrlVal(panelHandle, PANEL_DATA, &data);
			Avalon_write(address, subaddress, data);
			break;
	}
	return 0;
}

int CVICALLBACK READ_CALLBACK (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal(panelHandle, PANEL_NUMERIC_ADDR, &address);
			GetCtrlVal(panelHandle, PANEL_SUBADDR, &subaddress);
			Avalon_read(address, subaddress, &data);
			SetCtrlVal(panelHandle, PANEL_DATA_2, data);
			break;
	}
	return 0;
}

int CVICALLBACK DAC_VOLTAGE_CALLBACK (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			Avalon_write(2, 0, 3);
			GetCtrlVal(panelHandle, PANEL_DAC_VOLTAGE, &voltage);
			dac_code = Avalon_dac_voltage_to_code(voltage);
			SetCtrlVal(panelHandle, PANEL_DAC_CODE, dac_code);
			Avalon_dac(dac_code);
			break;
	}
	return 0;
}

int CVICALLBACK DAC_CODE_CALLBACK (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			Avalon_write(2, 0, 3);
			GetCtrlVal(panelHandle, PANEL_DAC_CODE, &dac_code);
			voltage = Avalon_dac_code_to_voltage(dac_code);
			SetCtrlVal(panelHandle, PANEL_DAC_VOLTAGE, voltage);
			Avalon_dac(dac_code);
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
			Avalon_write(2, 0x12, 0);
			Avalon_write(2, 0x13, 0);
			Avalon_write(2, 0x14, 0xffff);
			
			Avalon_write(2, 0x10, 15);
			
			Avalon_write(2, 0x11, 3);
			
			while (start != 0) {
				Avalon_read(2, 0x11, &start);
			};
			start = 1;
			
			Avalon_read(2, 0x16, &Vin1);
			Avalon_read(2, 0x17, &Vin2);
			
			SetCtrlVal(panelHandle, PANEL_Vin1_VOLTAGE, Avalon_adc_code_to_voltage(Vin1));
			SetCtrlVal(panelHandle, PANEL_Vin2_VOLTAGE_2, Avalon_adc_code_to_voltage(Vin2));
			SetCtrlVal(panelHandle, PANEL_Vin1_CODE, Vin1);
			SetCtrlVal(panelHandle, PANEL_Vin2_CODE, Vin2);
			break;
	}
	return 0;
}
