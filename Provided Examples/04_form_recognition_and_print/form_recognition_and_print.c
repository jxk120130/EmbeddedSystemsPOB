
#include <pob-eye.h>

// Include bitmap list and dictionnary of forms
#include "Bitmap.h" 
#include "pattern.h"

#define RUN 0x05
#define STOP 0
#define LEFT 0x09
#define RIGHT 0x06
#define BACK 0x0A

// Used to initialize the motors
void InitPobProto (void)
{
	PobProto	Proto; // struct to set the pob-proto
	
	Proto.porta=ALL_PORTA_AS_ANA;	//to get the position of the analogic joystick, you have to set the PORTA as analogic input
	Proto.portc=RC7_AS_DO| RC6_AS_DO |RC3_AS_DO |RC2_AS_SERVO|RC1_AS_SERVO |RC0_AS_SERVO;	//all pin of PORTC are configured to manage servomotors
	Proto.portd=RD7_AS_DI	| RD6_AS_DI	|RD5_AS_DI |RD4_AS_DI|RD3_AS_DO	|RD2_AS_DO	|RD1_AS_DO	|RD0_AS_DO;		//RD0 RD1 RD2 RD3 are configured as digitals output to gear DC motor, RD4 RD5 RD6 RD7 are configured as digitals input
	SetPobProto(&Proto);	//set the pob proto
}


// Used to make the project move
void MoveBot(UInt8 Way)
{
	SetPortD(Way);
}

void turnRight(){
	MoveBot(RIGHT);
	WaitUs(3100000);
	MoveBot(STOP);
}

void turnLeft(){
	MoveBot(LEFT);
	WaitUs(3800000);
	MoveBot(STOP);
}

void runForward(int inches){
	int time = 237500 * inches;
	MoveBot(RUN);
	WaitUs(time);
	MoveBot(STOP);
}


int main (void)
{	
	UInt8 i=0,Nb_Identify=0;
	
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

	runForward(18);
	turnLeft();
	runForward(36);
	turnRight();
	//SIGNAL HERE
	runForward(24);
	//end

	/*while(1)
	{		
		// grab the RGB components
		GrabRGBFrame();				
		
		// Binary the three RGB Buffer
		BinaryRGBFrame(FrameFromCam); 
		
		// Try to identify the forms and make a list of it
		Nb_Identify=IdentifyForm(FrameFromCam,ListOfForm,pattern);	

		// Parse the list of the form and print result on the Pob-Terminal and the LCD Screen
		for (i=0;i<Nb_Identify;i++)
		{
			switch (ListOfForm[i].id)
			{
			case IDP_0_CROSS:
				// Draw bitmap on the buffer and the LCD screen
				DrawBitmap(0,0,IDB_CROSS,bitmap,&ScreenBuffer);
				DrawLCD(&ScreenBuffer);
				MoveBot(RUN);
			break;
				
			case IDP_1_BIGA:
				DrawBitmap(0,0,IDB_BIGA,bitmap,&ScreenBuffer);
				DrawLCD(&ScreenBuffer);
			break;

			case IDP_2_KING:
				DrawBitmap(0,0,IDB_KING,bitmap,&ScreenBuffer);
				DrawLCD(&ScreenBuffer);
				MoveBot(LEFT);
			break;

			case IDP_3_TOWER:
				DrawBitmap(0,0,IDB_TOWER,bitmap,&ScreenBuffer);
				DrawLCD(&ScreenBuffer);
			break;
			
			case IDP_4_TREFLE:
				DrawBitmap(0,0,IDB_BIGA,bitmap,&ScreenBuffer);
				DrawLCD(&ScreenBuffer);
			break;
			
			case IDP_5_TRIANGLE:
				DrawBitmap(0,0,IDB_TRIANGLE,bitmap,&ScreenBuffer);
				DrawLCD(&ScreenBuffer);
			break;
			
			default:
				MoveBot(STOP);
			break;
			
			}				
		}
		
	if (Nb_Identify == 0)
		{
		DrawBitmap(0,0,IDB_NOFORMS,bitmap,&ScreenBuffer);
		DrawLCD(&ScreenBuffer);
		}
	}
	*/
	return 0;
}

