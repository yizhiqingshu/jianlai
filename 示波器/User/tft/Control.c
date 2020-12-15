#include "Control.h"
Grid Display_Grid={10,10};
u16 GRID_ARR[2]={10,20};
void Frame(void)
{
LCD_DrawLine(0,0,240,0);
LCD_DrawLine(0,0,10,10);
LCD_DrawLine(10,10,230,10);

LCD_DrawLine(240,0,240,320);
LCD_DrawLine(240,0,230,10);
LCD_DrawLine(230,10,230,310);

LCD_DrawLine(240,320,0,320);
LCD_DrawLine(240,320,230,310);
LCD_DrawLine(230,310,10,310);
	
LCD_DrawLine(0,320,0,0);
LCD_DrawLine(0,320,10,310);
LCD_DrawLine(10,310,10,10);
	
LCD_DrawLine(10,160,230,160);
LCD_DrawLine(120,10,120,310);
for(int j=1;j<=15;j++)
{
for(int i=1;i<=10;i++)
	{
LCD_DrawPoint(Display_Grid.Grid_X+i*20,Display_Grid.Grid_Y+j*20);
	}
}
}

