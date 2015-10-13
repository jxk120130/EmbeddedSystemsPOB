/** \anchor form0
 *
 * @brief Form recognition. 
 *
 * @file

 Example of form recognition and print the result on UART and the LCD Screen.

 * @author Foucault Alisson
 */

#include <pob-eye.h>
#include <unistd.h>

#include "Bitmap.h" 
#include "pattern.h"

#define RUN 0x05
#define STOP 0
#define LEFT 0x09
#define RIGHT 0x06
#define BACK 0x0A

// Used to initialize the bot motors
void InitPobProto (void)
{
	PobProto	Proto; // struct to set the pob-proto
	SetPobProto(&Proto);	//set the pob proto
}

// Used to make the project move
void MoveBot(UInt8 Way)
{
	SetPortD(Way);
}

int main (void)
{	
	UInt8 i=0,Nb_Identify = 0, Nb_Last = 0; 
	// List of form
	Form ListOfForm[MAX_OF_FORM];

	// Struct of three pointers on the RGB components
	UInt8 * FrameFromCam;

	//The pixels will be stocked in the LCD buffer
	UInt8 LCD_Buffer [LCD_WIDTH*LCD_HEIGHT*BITS] ;
	GraphicBuffer ScreenBuffer ;

	// System and LCD screen initialization
	InitPobeye2();
	InitI2C(I2C_100_KHZ);
	InitCameraPobeye2();
	InitLCD();	
	InitPobProto();
	SwitchOnAllServo();
	
	// Get the pointer of the red,green and blue video buffer
	FrameFromCam = GetRGBFrame();
		
	// Init the Graphic buffer with 128 per 64, one pixel per bit and LCD_Buffer
	InitGraphicBuffer( &ScreenBuffer, LCD_WIDTH,LCD_HEIGHT,ONE_BIT,LCD_Buffer);

	// clear the graphic buffer
	ClearGraphicBuffer(&ScreenBuffer);

	while(1)
	{
		// grab the RGB components
		GrabRGBFrame();				
		
		// Binary the three RGB Buffer
		BinaryRGBFrame(FrameFromCam); 
		
		// Try to identify the forms and make a list of it
		Nb_Identify = IdentifyForm(FrameFromCam,ListOfForm,pattern);	

	
		// Parse the list of the form and print result on the Pob-Terminal and the LCD Screen
		for (i=0;i<Nb_Identify;i++)
		{
			switch (ListOfForm[i].id)
			{
				case IDP_6_CIRCLE:
					// Draw bitmap on the buffer and the LCD screen
					DrawBitmap(0,0,IDB_KING,bitmap,&ScreenBuffer);
					DrawLCD(&ScreenBuffer);
					MoveBot(RUN);
					// Nb_Last = RUN;
				break;

				case IDP_0_CROSS:
					DrawBitmap(0,0,IDB_CROSS,bitmap,&ScreenBuffer);
					DrawLCD(&ScreenBuffer);
					MoveBot(LEFT);
					Nb_Last = LEFT;
				break;
				
				case IDP_1_BIGA:
					DrawBitmap(0,0,IDB_BIGA,bitmap,&ScreenBuffer);
					DrawLCD(&ScreenBuffer);
					MoveBot(RUN);
				break;				
				
				case IDP_5_TRIANGLE:
					DrawBitmap(0,0,IDB_TRIANGLE,bitmap,&ScreenBuffer);
					DrawLCD(&ScreenBuffer);
					MoveBot(RIGHT);
					Nb_Last = RIGHT;
				break;					

				default:
				break;
			}				
		}		
		
		if (Nb_Identify == 0)
		{
			DrawBitmap(0,0,IDB_NOFORMS,bitmap,&ScreenBuffer);
			DrawLCD(&ScreenBuffer);
			MoveBot(Nb_Last);
		}
	}

	return 0;
}

