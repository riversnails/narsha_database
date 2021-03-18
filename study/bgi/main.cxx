//#include "graphics.h"
//
//int main()
//{
//	initwindow(400, 300, "First Sample");
//	circle(100, 50, 40);
//	while (!kbhit())
//	{
//		delay(200);
//	}
//	return 0;
//}




#include "graphics.h"



int g_code_idx = 0;

unsigned int x_pos, y_pos, pre_x_pos, pre_y_pos;

int main()
{
	unsigned int size;
	char *buf;
	char *buf1;
	char *buf2;
	unsigned int count = 0;
	unsigned int y_position = 5;
	unsigned int x_position = 202;
	int ch;
	int speed = 20;

	unsigned int i = 0, j = 0, k = 0, m = 0, n = 0;

	unsigned int color;
	unsigned char r = 0, g = 0, b = 0, g_upper = 0, g_lower = 0;
	unsigned short tmp_rgb;
	unsigned int toggle;
	unsigned char init_value = 0x30;
	int x, y;

	int width, height;

	unsigned char * data;
	FILE * file;

	initwindow(320, 240, "3d_printer_gcode");

	//for (int j = 0; i < 240; j++)
	//	for (int i = 0; i < 320; i++)
	//	{
	//		if (j < 80)
	//			putpixel(i, j, COLOR(255, 0, 0));
	//		else if (j >= 80 && j < 160)
	//			putpixel(i, j, COLOR(0, 255, 0));
	//		else if (j >= 160 && j < 240)
	//			putpixel(i, j, COLOR(0, 0, 255));
	//	}
	
#if 0

	file = fopen("3_tiger.bmp", "rb");

	if (file == NULL) return 0;

	width = 320;
	height = 240;

	data = (unsigned char *)malloc(width * height * 4);
	fread( data, width * height * 3 + 0x36, 1, file);

	fclose(file);


	for (i = 0; i < 16; i++)
		printf(" 0x%02x ", *(data + i));

	printf("\n");


	for (int j = 239; j >= 0; j--)
		for (int i = 319; i >= 0; i--)
		{
			r = *(data + 0x36 + (j * 320 * 3) + (i * 3) + 2);
			g = *(data + 0x36 + (j * 320 * 3) + (i * 3) + 1);
			b = *(data + 0x36 + (j * 320 * 3) + (i * 3) + 0);
			putpixel(i, 239 - j, COLOR(r, g, b));
		}


	while (1);
	//getch();

#endif

	file = fopen("320_240_565_16.bmp", "rb");

	if (file == NULL) return 0;

	width = 320;
	height = 240;

	data = (unsigned char *)malloc(width * height * 4);
	fread(data, width * height * 3 + 0x36, 1, file);

	fclose(file);


	for (i = 0; i < 16; i++)
		printf(" 0x%02x ", *(data + i));

	printf("\n");


	for (int j = 0; j < 240; j++)
		for (int i = 0; i < 320; i++)
		{
			r = *(data + 0x46 + (j * 320 * 2) + (i * 2) + 1) & 0xf1;
			//g = (((*(data + 0x46 + (j * 320 * 2) + (i * 2) + 1) & 0x07) << 3) | ((*(data + 0x46 + (j * 320 * 2) + (i * 2) + 0) & 0xE0) >> 5)) << 2;
			b = (*(data + 0x46 + (j * 320 * 2) + (i * 2) + 0) & 0x1f) << 3;
			g_upper = (*(data + 0x46 + (j * 320 * 2) + (i * 2) + 1) & 0x07) << 3;
			g_lower = (*(data + 0x46 + (j * 320 * 2) + (i * 2) + 0) & 0xE0) >> 5;
			g = (g_upper | g_lower) << 2;

			putpixel(i, j, COLOR(r, g, b));
		}


	while (1);
	//getch();


	closegraph();
	return 0;
}



