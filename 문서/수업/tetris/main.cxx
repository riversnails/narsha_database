#include "graphics.h"

unsigned int size;
char *buf_background;
char *buf_red;
char *buf_white;
int init_offset = 2;
int x = 3;
int y = 0;
int overlap = 0;
int rotate = 0;
int overlap_rotate = 0;

char background[22][12] = { {1,1,1,1,1,1,1,1,1,1,1,1},
							{1,0,0,0,0,0,0,0,0,0,0,1},
							{1,0,0,0,0,0,0,0,0,0,0,1},
							{1,0,0,0,0,0,0,0,0,0,0,1},
							{1,0,0,0,0,0,0,0,0,0,0,1},
							{1,0,0,0,0,0,0,0,0,0,0,1},
							{1,0,0,0,0,0,0,0,0,0,0,1},
							{1,0,0,0,0,0,0,0,0,0,0,1},
							{1,0,0,0,0,0,0,0,0,0,0,1},
							{1,0,0,0,0,0,0,0,0,0,0,1},
							{1,0,0,0,0,0,0,0,0,0,0,1},
							{1,0,0,0,0,0,0,0,0,0,0,1},
							{1,0,0,0,0,0,0,0,0,0,0,1},
							{1,0,0,0,0,0,0,0,0,0,0,1},
							{1,0,0,0,0,0,0,0,0,0,0,1},
							{1,0,0,0,0,0,0,0,0,0,0,1},
							{1,0,0,0,0,0,0,0,0,0,0,1},
							{1,0,0,0,0,0,0,0,0,0,0,1},
							{1,0,0,0,0,0,0,0,0,0,0,1},
							{1,0,0,0,0,0,0,0,0,0,0,1},
							{1,0,0,0,0,0,0,0,0,0,0,1},
							{1,1,1,1,1,1,1,1,1,1,1,1} };

char block_l[4][4][4] = { { {0,0,0,0},
							{0,1,0,0},
							{0,1,1,1},
							{0,0,0,0}, },
							{ 
							{0,0,0,0},
							{0,1,1,0},
							{0,1,0,0},
							{0,1,0,0}, },
							{ 
							{0,0,0,0},
							{1,1,1,0},
							{0,0,1,0},
							{0,0,0,0}, },
							{ 
							{0,0,1,0},
							{0,0,1,0},
							{0,1,1,0},
							{0,0,0,0}, } };

void make_block(int x, int y)
{
	for (int j = 0; j < 4; j++)
	{
		for (int i = 0; i < 4; i++)
		{
			if (block_l[rotate][j][i] == 1) putimage(init_offset + 22 * (x + i), init_offset + 22 * (y + j), buf_red, 0);
		}
	}
}

void delete_block(int x, int y)
{
	for (int j = 0; j < 4; j++)
	{
		for (int i = 0; i < 4; i++)
		{
			if (block_l[rotate][j][i] == 1) putimage(init_offset + 22 * (x + i), init_offset + 22 * (y + j), buf_white, 0);
		}
	}
}

//void draw_background()
//{
//	for (int j = 0; j < 22; j++)
//	{
//		for (int i = 0; i < 12; i++)
//		{
//			if(background[j][i])
//		}
//	}
//}

void overlap_check(int x, int y)
{
	overlap = 0;
	for (int j = 0; j < 4; j++)
	{
		for (int i = 0; i < 4; i++)
		{
			if (block_l[rotate][j][i] == 1 && background[j + y + 1][i + x + 1] == 1)
			{
				overlap++;
			}
		}
	}
}

void overlap_check_rotate(int rotate)
{
	overlap_rotate = 0;
	int tmp_rotate1 = rotate, tmp_rotate2 = rotate;
	tmp_rotate1++;
	if (tmp_rotate1 == 4) tmp_rotate1 = 0;

	tmp_rotate2--;
	if (tmp_rotate2 == -1) tmp_rotate2 = 3;

	for (int j = 0; j < 4; j++)
	{
		for (int i = 0; i < 4; i++)
		{
			if (block_l[tmp_rotate1][j][i] == 1 && background[j + y + 1][i + x + 1] == 1)
			{
				overlap_rotate++;
			}
			if (block_l[tmp_rotate2][j][i] == 1 && background[j + y + 1][i + x + 1] == 1)
			{
				overlap_rotate++;
			}
		}
	}
}

void insert_block(int x, int y)
{
	for (int j = 0; j < 4; j++)
	{
		for (int i = 0; i < 4; i++)
		{
			if (block_l[rotate][j][i] == 1)
			{
				background[j + y + 1][i + x + 1] = 1;
			}
		}
	}
}

int main()
{
	int ch = 0, count = 0;

	initwindow(232, 452, "Tetris"); 

	
	readimagefile("block_red.jpg", 0, 0, 19, 19);
	size = imagesize(0, 0, 19, 19);
	buf_red = (char*)malloc(size);
	getimage(0, 0, 19, 19, buf_red);

	readimagefile("block_white.jpg", 0, 0, 19, 19);
	size = imagesize(0, 0, 19, 19);
	buf_white = (char*)malloc(size);
	getimage(0, 0, 19, 19, buf_white);

	readimagefile("background.jpg", 0, 0, 222, 442);
	size = imagesize(0, 0, 222, 442);
	buf_background = (char*)malloc(size);
	getimage(0, 0, 222, 442, buf_background);
	putimage(0, 0, buf_background, 0);

	while (1)
	{
		ch = 0;
		if (kbhit()) ch = getch();

		if (count == 20)
		{
			count = 0;
			
			overlap_check(x, y + 1);

			if (overlap == 0)
			{
				delete_block(x, y);
				y += 1;
				make_block(x, y);
			}
			else
			{
				insert_block(x, y);
				x = 3;
				y = 1;
			}
		}

		if (ch == 'a')
		{
			overlap_check(x - 1, y);

			if (overlap == 0)
			{
				delete_block(x, y);
				x -= 1; 
				make_block(x, y);
			}
			
		}
		else if (ch == 'd')
		{
			overlap_check(x + 1, y);

			if (overlap == 0)
			{
				delete_block(x, y);
				x += 1;
				make_block(x, y);
			}
		}
		else if (ch == 's')
		{
			overlap_check(x, y + 1);

			if (overlap == 0)
			{
				delete_block(x, y);
				y += 1;
				make_block(x, y);
			}
		}
		else if (ch == 'w')
		{
			overlap_check(x, y - 1);

			if (overlap == 0)
			{
				delete_block(x, y);
				y -= 1;
				make_block(x, y);
			}
		}

		else if (ch == 'q')
		{
			overlap_check_rotate(rotate);

			if (overlap_rotate == 0)
			{
				delete_block(x, y);
				rotate--;
				if (rotate == -1) rotate = 3;
				make_block(x, y);
			}
			
		}

		else if (ch == 'e' || ch == 'r')
		{
			overlap_check_rotate(rotate);

			if (overlap_rotate == 0)
			{
				delete_block(x, y);
				rotate++;
				if (rotate == 4) rotate = 0; 
				make_block(x, y);
			}
			
		}

		count++;
		Sleep(50);
	}


	while (1);
	closegraph();
	return 0;
}