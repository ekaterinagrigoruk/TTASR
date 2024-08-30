//==============================================================================
//
// Title:       User Interface Application(TSANI)
// Purpose:     A short description of the application.
//
// Created on:  21.05.2024 at 15:07:39 by TC.
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
#define DELAY 0.00025
int counter = 0;
//==============================================================================
// Types

//==============================================================================
// Static global variables

static int panelHandle;

//==============================================================================
// Static functions

//==============================================================================
// Global variables
unsigned char name[9];
//==============================================================================
// Global functions
void InitPorts(){
	ni6251Slot(2);
	
	portMask(0, 0x7F);
	portMask(1, 0x00);
	
	portOut(0, 0x00);
}

void writeBus(unsigned char sda, unsigned char scl){
	unsigned char data;
	data = ((sda? 1:0) << 5) | ((scl? 1:0) << 6) | (1 << 3) | (1 << 4) | 0x07;
	portOut(0, data);
}

void readBus(unsigned char* sda, unsigned char* scl){
	unsigned char val0, val1;
	//portIn(0, &val0);
	portIn(1, &val1);
	*sda = (val1 >> 1) & 1;
	*scl = (val1 >> 2) & 1;
}

void sendStart(){
	writeBus(1, 1);
	Delay(DELAY);
	writeBus(0, 1);
	Delay(DELAY);
	writeBus(0, 0);
	Delay(DELAY);
}

void sendStop(){
	writeBus(0, 0);
	Delay(DELAY);
	writeBus(0, 1);
	Delay(DELAY);
	writeBus(1, 1);
	Delay(DELAY);
}

void sendBit(unsigned char bit){
	writeBus(bit, 0);
	Delay(DELAY);
	writeBus(bit, 1);
	Delay(DELAY);
	writeBus(bit, 0);
	Delay(DELAY);
}

unsigned char getBit(){
	unsigned char sda, scl;
	writeBus(1, 0);
	Delay(DELAY);
	writeBus(1, 1);
	Delay(DELAY);
	readBus(&sda, &scl);
	writeBus(1, 0);
	Delay(DELAY);
	return sda;
}

void sendByte(unsigned char byte){
	for(int i = 0; i < 8; i++){
		sendBit(byte & (1 << (7 - i)));
	}
}

int getACK(){
}


int send2Byte(unsigned char adr, unsigned char byte1, unsigned char byte2){
	sendStart();
	sendByte(adr << 1 | 0);
	if(getBit()){
		sendStop();
		return -1;
	}
	sendByte(byte1);
	if(getBit()){
		sendStop();
		return -1;
	}
	sendByte(byte2);
	if(getBit()){
		sendStop();
		return -1;
	}
	sendStop();
	return 0;
}

int send1Byte(unsigned char adr, unsigned char byte){
	sendStart();
	sendByte(adr << 1 | 0);
	if(getBit()){
		sendStop();
		return -1;
	}
	sendByte(byte);
	if(getBit()){
		sendStop();
		return -1;
	}
	sendStop();
	return 0;
}

void getByte(unsigned char* byte){
	*byte = 0;
	for(int i = 0; i < 8; i++){
		*byte |= getBit() << (7 - i);
	}
}

int get1Byte(unsigned char adr, unsigned char* byte){
	sendStart();
	sendByte(adr << 1 | 1);
	if(getBit()){
		sendStop();
		return -1;
	}
	getByte(byte);
	sendBit(1);
	sendStop();
	return 0;
}

int get2Byte(unsigned char adr, unsigned char* byte1, unsigned char* byte2){
	sendStart();
	sendByte(adr << 1 | 1);
	if(getBit()){
		sendStop();
		return -1;
	}
	getByte(byte1);
	sendBit(0);
	getByte(byte2);
	sendBit(1);
	sendStop();
	return 0;
}
	
/// HIFN The main entry-point function.
int main (int argc, char *argv[])
{
    int error = 0;
    
    /* initialize and load resources */
    nullChk (InitCVIRTE (0, argv, 0));
    errChk (panelHandle = LoadPanel (0, "User Interface Application(TSANI).uir", PANEL));
    InitPorts();
	

	
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

int CVICALLBACK CALLBACK_START_SNAKE (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			SetCtrlAttribute(panelHandle, PANEL_TIMER, ATTR_ENABLED, 1);
			break;
	}
	return 0;
}

int CVICALLBACK CALLBACK_STOP_SNAKE (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			SetCtrlAttribute(panelHandle, PANEL_TIMER, ATTR_ENABLED, 0);
			break;
	}
	return 0;
}

int CVICALLBACK CALLBACK_TIMER (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_TIMER_TICK:
			
			send2Byte(0x08, 0, 1 << counter);
			counter ++;
			if (counter == 8)
				counter = 0;
			break;
	}
	return 0;
}

int CVICALLBACK CALLBACK_READ (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			unsigned char b;
			
			for(int i = 0; i < 8; i++){
				send1Byte(0x08, 8 + i);
				get1Byte(0x08, &b);
				name[i] = b;
			}
			name[8] = 0;
			SetCtrlVal(panelHandle, PANEL_STRING, name);
			printf("%s\n",name);
			//	b = 0;
			break;
	}
	return 0;
}

int CVICALLBACK CALLBACK_TIMER2 (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_TIMER_TICK:
				unsigned char b1, b2;
				get2Byte(0x28, &b1, &b2);
				double t = b1 + 0.25 * (b2 >> 6);
				if (t >= 256 / 2)
					t -= 256;
				SetCtrlVal(panelHandle, PANEL_NUMERIC, t);
			break;
	}
	return 0;
}
