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
#include "pad.h"


#define RUN 0x05
#define STOP 0
#define LEFT 0x09
#define RIGHT 0x06
#define BACK 0x0A

#define BUFFER_WIDTH	64
#define BUFFER_HEIGHT	64

//function to draw picture on both screen, when a form is recognize
static void DrawWhatIsKnown(Form What);	
//function to draw rectangle arround the form known
static UInt8 DrawRect (Rect Rectangle,const GraphicBuffer *Target);	
//extern void PrintTextOnPobLCD(int row, int col, char *string, UInt8 *Screen_Buffer);
//extern void InitAsciiBuffer();
//extern void PrintTextOnPobLCD(int row, int col, char *string, UInt8 *Screen_Buffer);




UInt8 LCD_Left_Buffer [BUFFER_WIDTH*BUFFER_HEIGHT*BYTES];  // array to store the left lcd screen, one byte per pixel
UInt8 LCD_Right_Buffer [BUFFER_WIDTH*BUFFER_HEIGHT*BITS];  // array to store the right lcd screen one bit per pixel.

GraphicBuffer LCD_Left ;  // left graphic buffer 
GraphicBuffer LCD_Right ; // right graphic buffer 
UInt8 X_Pic=5,Y_Pic=5;	  // x,y to draw pictures of form known on the right LCD screen 

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

void turnRight(){
	MoveBot(RIGHT);
	WaitUs(1500000);
	MoveBot(STOP);
}

void turnLeft(){
	MoveBot(LEFT);
	WaitUs(1500000);
	MoveBot(STOP);
}

void runForward(int inches){
	int time = 237500 * inches;
	MoveBot(RUN);
	WaitUs(time);
	MoveBot(STOP);
}

void signal(){
	//do somthing
}

int main (void)
{	
	UInt8 i=0,Nb_Identify = 0, Nb_Last = 0;
	UInt8 state = 0;
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
	//InitAsciiBuffer(); 
	SwitchOnAllServo();
	SetDCSpeed(5);
	
	InitGraphicBuffer( &LCD_Left,BUFFER_WIDTH,BUFFER_HEIGHT,EIGHT_BITS,LCD_Left_Buffer);
	InitGraphicBuffer( &LCD_Right,BUFFER_WIDTH,BUFFER_HEIGHT,ONE_BIT,LCD_Right_Buffer);

	// Clear the Left and Right screen
	ClearGraphicBuffer(&LCD_Left); 
	ClearGraphicBuffer(&LCD_Right);
	
	// Get the pointer of the red,green and blue video buffer
	FrameFromCam = GetRGBFrame();
		
	// Init the Graphic buffer with 128 per 64, one pixel per bit and LCD_Buffer
	//InitGraphicBuffer( &ScreenBuffer, LCD_WIDTH,LCD_HEIGHT,ONE_BIT,LCD_Buffer);

	// clear the graphic buffer
	//ClearGraphicBuffer(&ScreenBuffer);

	while(1)
	{
		// grab the RGB components
		GrabRGBFrame();
		//PrintTextOnPobLCD(1, 2, "Top", &LCD_Right);
				
		
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
					DrawBitmap(0,0,IDB_KING,bitmap,&LCD_Left);
					DrawLeftLCD(&LCD_Left);
					MoveBot(RUN);
					// Nb_Last = RUN;
					break;

				case IDP_0_CROSS:
					DrawBitmap(0,0,IDB_CROSS,bitmap,&LCD_Left);
					DrawLeftLCD(&LCD_Left);
					if(state == 0){
						turnLeft();
						state++;					
					} else if (state == 2){
						MoveBot(STOP);
						return; //end of execution.
					}
					//MoveBot(LEFT);
					Nb_Last = LEFT;
					break;
				
				case IDP_1_BIGA:
					DrawBitmap(0,0,IDB_BIGA,bitmap,&LCD_Left);
					DrawLeftLCD(&LCD_Left);
					//MoveBot(RUN);
					break;				
				
				case IDP_5_TRIANGLE:
					DrawBitmap(0,0,IDB_TRIANGLE,bitmap,&LCD_Left);
					DrawLeftLCD(&LCD_Left);
					if(state == 1){
						turnRight();
						state++;
						signal();
					}
					//MoveBot(RIGHT);
					Nb_Last = RIGHT;
					break;					

				default:
				break;
			}				
		}		
		
		if (Nb_Identify == 0)
		{
			DrawBitmap(0,0,IDB_NOFORMS,bitmap,&LCD_Left);
			DrawLeftLCD(&LCD_Left);
			//MoveBot(Nb_Last);
			if(state == 2){
				//return;
			}
		}
	}

	return 0;
}

