#include "stm32f4xx.h"
#include "KEY&LED.h"
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <math.h>
#include <stdlib.h>
#incldue "gcode_sample_480_string.h"

double xy_pos[480][2] = {
66.268 , 82.377,
66.527 , 82.217,
66.796 , 82.073,
67.072 , 81.946,
67.356 , 81.836,
67.647 , 81.743,
67.942 , 81.669,
68.241 , 81.613,
68.544 , 81.576,
68.848 , 81.557,
131.152, 81.557,
131.456, 81.576,
131.759, 81.613,
132.058, 81.669,
132.353, 81.743,
132.644, 81.836,
132.928, 81.946,
133.204, 82.073,
133.473, 82.217,
133.732, 82.377,
133.980, 82.553,
134.218, 82.744,
134.443, 82.950,
134.655, 83.168,
134.853, 83.400,
135.036, 83.643,
135.205, 83.897,
135.357, 84.160,
135.493, 84.433,
135.611, 84.714,
135.713, 85.001,
135.796, 85.294,
135.861, 85.591,
135.908, 85.892,
135.936, 86.196,
135.945, 86.500,
135.936, 86.804,
135.908, 87.108,
135.861, 87.409,
135.796, 87.706,
135.713, 87.999,
135.611, 88.286,
135.493, 88.567,
135.357, 88.840,
135.205, 89.103,
135.036, 89.357,
134.853, 89.600,
134.664, 89.821,
134.225, 90.251,
103.227, 117.248,
102.980, 117.446,
102.732, 117.622,
102.473, 117.783,
102.204, 117.927,
101.928, 118.054,
101.644, 118.164,
101.353, 118.257,
101.058, 118.331,
100.759, 118.387,
100.456, 118.424,
100.152, 118.443,
99.848 , 118.443,
99.544 , 118.424,
99.241 , 118.387,
98.942 , 118.331,
98.647 , 118.257,
98.356 , 118.164,
98.072 , 118.054,
97.796 , 117.927,
97.527 , 117.783,
97.268 , 117.622,
97.021 , 117.447,
96.737 , 117.217,
65.775 , 90.251,
65.336 , 89.821,
65.147 , 89.600,
64.964 , 89.357,
64.795 , 89.103,
64.643 , 88.840,
64.507 , 88.567,
64.389 , 88.286,
64.287 , 87.999,
64.204 , 87.706,
64.139 , 87.409,
64.092 , 87.108,
64.064 , 86.804,
64.055 , 86.500,
64.064 , 86.196,
64.092 , 85.892,
64.139 , 85.591,
64.204 , 85.294,
64.287 , 85.001,
64.389 , 84.714,
64.507 , 84.433,
64.643 , 84.160,
64.795 , 83.897,
64.964 , 83.643,
65.147 , 83.400,
65.345 , 83.168,
65.557 , 82.950,
65.782 , 82.745,
66.020 , 82.553,
65.586 , 81.979,
65.870 , 81.777,
66.167 , 81.593,
66.475 , 81.428,
66.792 , 81.282,
67.117 , 81.156,
67.449 , 81.051,
67.788 , 80.965,
68.131 , 80.901,
68.477 , 80.858,
68.826 , 80.837,
131.174, 80.837,
131.523, 80.858,
131.869, 80.901,
132.212, 80.965,
132.551, 81.051,
132.883, 81.156,
133.208, 81.282,
133.525, 81.428,
133.833, 81.593,
134.130, 81.777,
134.414, 81.979,
134.686, 82.197,
134.944, 82.433,
135.187, 82.683,
135.414, 82.948,
135.624, 83.227,
135.817, 83.517,
135.992, 83.820,
136.147, 84.132,
136.283, 84.453,
136.399, 84.782,
136.495, 85.118,
136.569, 85.459,
136.623, 85.804,
136.655, 86.151,
136.666, 86.500,
136.655, 86.849,
136.623, 87.196,
136.569, 87.541,
136.495, 87.882,
136.399, 88.218,
136.283, 88.547,
136.147, 88.868,
135.992, 89.180,
135.817, 89.483,
135.624, 89.773,
135.414, 90.052,
135.190, 90.313,
134.714, 90.780,
103.689, 117.800,
103.414, 118.021,
103.130, 118.223,
102.833, 118.407,
102.525, 118.572,
102.208, 118.718,
101.883, 118.844,
101.551, 118.949,
101.212, 119.035,
100.869, 119.099,
100.523, 119.142,
100.174, 119.163,
99.826 , 119.163,
99.477 , 119.142,
99.131 , 119.099,
98.788 , 119.035,
98.449 , 118.949,
98.117 , 118.844,
97.792 , 118.718,
97.475 , 118.572,
97.167 , 118.407,
96.870 , 118.223,
96.586 , 118.022,
96.274 , 117.769,
65.286 , 90.780,
64.810 , 90.313,
64.586 , 90.052,
64.376 , 89.773,
64.183 , 89.483,
64.008 , 89.180,
63.853 , 88.868,
63.717 , 88.547,
63.601 , 88.218,
63.505 , 87.882,
63.431 , 87.541,
63.377 , 87.196,
63.345 , 86.849,
63.334 , 86.500,
63.345 , 86.151,
63.377 , 85.804,
63.431 , 85.459,
63.505 , 85.118,
63.601 , 84.782,
63.717 , 84.453,
63.853 , 84.132,
64.008 , 83.820,
64.183 , 83.517,
64.376 , 83.227,
64.586 , 82.948,
64.813 , 82.683,
65.056 , 82.433,
65.314 , 82.197,
65.586 , 81.979,
68.037 , 85.535,
67.847 , 85.916,
67.731 , 86.326,
67.691 , 86.750,
67.731 , 87.174,
67.847 , 87.584,
68.037 , 87.965,
68.304 , 88.319,
68.710 , 88.667,
69.100 , 88.876,
69.506 , 89.005,
69.896 , 89.054,
130.104, 89.054,
130.494, 89.005,
130.900, 88.876,
131.275, 88.675,
131.607, 88.407,
131.884, 88.084,
132.097, 87.715,
132.239, 87.313,
132.304, 86.892,
132.291, 86.466,
132.196, 86.034,
131.973, 85.548,
131.706, 85.195,
131.391, 84.908,
131.029, 84.683,
130.632, 84.529,
130.246, 84.457,
130.071, 84.446,
69.896 , 84.446,
69.506 , 84.495,
69.100 , 84.624,
68.710 , 84.833,
68.304 , 85.181,
68.652 , 85.915,
68.521 , 86.177,
68.441 , 86.459,
68.414 , 86.750,
68.441 , 87.041,
68.521 , 87.323,
68.652 , 87.585,
68.832 , 87.823,
69.119 , 88.069,
69.382 , 88.210,
69.661 , 88.299,
69.940 , 88.334,
130.060, 88.334,
130.339, 88.299,
130.618, 88.210,
130.876, 88.072,
131.104, 87.888,
131.294, 87.666,
131.440, 87.413,
131.538, 87.137,
131.583, 86.848,
131.574, 86.555,
131.510, 86.264,
131.352, 85.920,
131.172, 85.682,
130.956, 85.485,
130.707, 85.331,
130.434, 85.225,
130.157, 85.173,
130.049, 85.166,
69.940 , 85.166,
69.661 , 85.201,
69.382 , 85.290,
69.119 , 85.431,
68.832 , 85.677,
92.904 , 101.189,
92.683 , 101.633,
92.489 , 102.090,
92.324 , 102.558,
92.189 , 103.035,
92.083 , 103.520,
92.006 , 104.010,
91.961 , 104.504,
91.945 , 105.000,
91.961 , 105.496,
92.006 , 105.990,
92.083 , 106.480,
92.189 , 106.965,
92.324 , 107.442,
92.489 , 107.910,
92.683 , 108.367,
92.904 , 108.811,
93.152 , 109.240,
93.426 , 109.654,
93.725 , 110.050,
94.048 , 110.426,
94.393 , 110.783,
94.760 , 111.117,
95.146 , 111.428,
95.551 , 111.714,
95.973 , 111.975,
96.410 , 112.210,
96.860 , 112.418,
97.323 , 112.597,
97.796 , 112.747,
98.277 , 112.868,
98.765 , 112.959,
99.257 , 113.020,
99.752 , 113.051,
100.248, 113.051,
100.743, 113.020,
101.235, 112.959,
101.723, 112.868,
102.204, 112.747,
102.677, 112.597,
103.140, 112.418,
103.590, 112.210,
104.027, 111.975,
104.449, 111.714,
104.854, 111.428,
105.240, 111.117,
105.607, 110.783,
105.952, 110.426,
106.275, 110.050,
106.574, 109.654,
106.848, 109.240,
107.096, 108.811,
107.317, 108.367,
107.511, 107.910,
107.676, 107.442,
107.811, 106.965,
107.917, 106.480,
107.994, 105.990,
108.039, 105.496,
108.055, 105.000,
108.039, 104.504,
107.994, 104.010,
107.917, 103.520,
107.811, 103.035,
107.676, 102.558,
107.511, 102.090,
107.317, 101.633,
107.096, 101.189,
106.848, 100.760,
106.574, 100.346,
106.275, 99.950,
105.952, 99.574,
105.607, 99.217,
105.240, 98.883,
104.854, 98.572,
104.449, 98.286,
104.027, 98.025,
103.590, 97.790,
103.140, 97.582,
102.677, 97.403,
102.204, 97.253,
101.723, 97.132,
101.235, 97.041,
100.743, 96.980,
100.248, 96.949,
99.752 , 96.949,
99.257 , 96.980,
98.765 , 97.041,
98.277 , 97.132,
97.796 , 97.253,
97.323 , 97.403,
96.860 , 97.582,
96.410 , 97.790,
95.973 , 98.025,
95.551 , 98.286,
95.146 , 98.572,
94.760 , 98.883,
94.393 , 99.217,
94.048 , 99.574,
93.725 , 99.950,
93.426 , 100.346,
93.152 , 100.760,
93.538 , 101.530,
93.337 , 101.935,
93.161 , 102.351,
93.011 , 102.777,
92.887 , 103.211,
92.791 , 103.652,
92.721 , 104.099,
92.680 , 104.548,
92.666 , 105.000,
92.680 , 105.451,
92.721 , 105.901,
92.791 , 106.348,
92.887 , 106.789,
93.011 , 107.223,
93.161 , 107.649,
93.337 , 108.065,
93.538 , 108.470,
93.764 , 108.861,
94.014 , 109.238,
94.286 , 109.598,
94.580 , 109.941,
94.894 , 110.265,
95.228 , 110.570,
95.580 , 110.853,
95.949 , 111.114,
96.333 , 111.352,
96.731 , 111.565,
97.141 , 111.754,
97.562 , 111.917,
97.993 , 112.054,
98.431 , 112.164,
98.875 , 112.247,
99.323 , 112.303,
99.774 , 112.331,
100.226, 112.331,
100.677, 112.303,
101.125, 112.247,
101.569, 112.164,
102.007, 112.054,
102.438, 111.917,
102.859, 111.754,
103.269, 111.565,
103.667, 111.352,
104.051, 111.114,
104.420, 110.853,
104.772, 110.570,
105.106, 110.265,
105.420, 109.941,
105.714, 109.598,
105.986, 109.238,
106.236, 108.861,
106.462, 108.470,
106.663, 108.065,
106.839, 107.649,
106.989, 107.223,
107.113, 106.789,
107.209, 106.348,
107.279, 105.901,
107.320, 105.451,
107.334, 105.000,
107.320, 104.548,
107.279, 104.099,
107.209, 103.652,
107.113, 103.211,
106.989, 102.777,
106.839, 102.351,
106.663, 101.935,
106.462, 101.530,
106.236, 101.139,
105.986, 100.762,
105.714, 100.402,
105.420, 100.059,
105.106, 99.735,
104.772, 99.430,
104.420, 99.147,
104.051, 98.886,
103.667, 98.648,
103.269, 98.435,
102.859, 98.246,
102.438, 98.083,
102.007, 97.946,
101.569, 97.836,
101.125, 97.752,
100.677, 97.697,
100.226, 97.669,
99.774 , 97.669,
99.323 , 97.697,
98.875 , 97.752,
98.431 , 97.836,
97.993 , 97.946,
97.562 , 98.083,
97.141 , 98.246,
96.731 , 98.435,
96.333 , 98.648,
95.949 , 98.886,
95.580 , 99.147,
95.228 , 99.430,
94.894 , 99.735,
94.580 , 100.059,
94.286 , 100.402,
94.014 , 100.762,
93.764 , 101.139,
93.764 , 101.139
}; 

int fputc(int ch, FILE * f) {
	while (!(USART1->SR & (0x01 << 7))) {}
	USART1->DR = ch;
	return ch;
}

void TimingDelay_Decrement(void);


volatile unsigned long sys_count;
volatile unsigned int TimingDelay;
volatile unsigned long count_ms = 0;

unsigned long micros_10us()
{
	return sys_count;
}

void Delay(unsigned int nTime)
{ 
  TimingDelay = nTime;

  while(TimingDelay > 0);
}

void delay_ms(unsigned int ms) {
	Delay(ms * 100);
}

 void TimingDelay_Decrement(void)
{
  if (TimingDelay != 0x00)
  { 
    TimingDelay--;
  }
}

unsigned long millis()
{
	return count_ms;
}

unsigned long micros()
{
	int timer1_cnt = 0;
	TIM1->CR1 |= 0x00;
	timer1_cnt = TIM1->CNT;
	TIM1->CR1 |= 0x01;
	return count_ms*1000 + timer1_cnt;
}

enum{
	LEFT = 0,
	RIGHT,
	UP,
	DOWN
};

enum{
	STOPPED = 0,
	RUNNING
};

volatile double x_pos = 0;
volatile double y_pos = 0;

volatile int x_distance = 0;
volatile int y_distance = 0;

volatile int x_timer_toggle = 0;
volatile int x_step_count = 0;
volatile int x_motor_move_enable = STOPPED;

volatile int y_timer_toggle = 0;
volatile int y_step_count = 0;
volatile int y_motor_move_enable = STOPPED;

double xy_gcode[480][2];

void TIM1_UP_TIM10_IRQHandler (void) { 

	 if ((TIM1->SR & 0x0001) != 0) { 
		 
		 count_ms++;
		 
		 
		 TIM1->SR &= ~(1<<0); // clear UIF flag
	 }
	 
}

void TIM2_IRQHandler (void) {

	 if ((TIM2->SR & 0x0001) != 0) { 
		 
		 if(x_motor_move_enable)
		 {
			 if(x_timer_toggle == 0)
			 {
				 x_timer_toggle = 1;
				 GPIOE->ODR |= (0x01 << 9);
			 }
			 else
			 {
				 x_timer_toggle = 0;
				 GPIOE->ODR &= ~(0x01 << 9);
				 
				 x_step_count++;
				 if(x_step_count >= x_distance)
				 {
					 x_step_count = 0;
					 x_motor_move_enable = STOPPED;
				 }
			 }
		}
		 
		 
		 TIM2->SR &= ~(1<<0); // clear UIF flag
	 }
	 
}

void TIM3_IRQHandler (void) {
	 if ((TIM3->SR & 0x0001) != 0) { 
		 if(y_motor_move_enable)
		 {
			 if(y_timer_toggle == 0)
			 {
				 y_timer_toggle = 1;
				 GPIOE->ODR |= (0x01 << 11);
			 }
			 else
			 {
				 y_timer_toggle = 0;
				 GPIOE->ODR &= ~(0x01 << 11);
				 
				 y_step_count++;
				 if(y_step_count >= y_distance)
				 {
					 y_step_count = 0;
					 y_motor_move_enable = STOPPED;
				 }
			 }
		}
		 
		 
		 TIM3->SR &= ~(1<<0); // clear UIF flag
	 }
	 
}

int stop_check()
{
	return x_motor_move_enable | y_motor_move_enable;
}

void direction_set(char dir)
{
	if(dir == LEFT)
	{
		GPIOF->ODR &= ~(0x01 << 1);
	}
	else if(dir == RIGHT)
	{
		GPIOF->ODR |= (0x01 << 1);
	}
	else if(dir == UP)
	{
		GPIOE->ODR |= (0x01 << 8);
	}
	else if(dir == DOWN)
	{
		GPIOE->ODR &= ~(0x01 << 8);
	}
}

void x_motor_move(char dir, int speed)
{
	direction_set(dir);
	TIM2->ARR = speed-1;
	TIM2->CNT = 0x00;
	x_motor_move_enable = RUNNING;
}

void y_motor_move(char dir, int speed)
{
	direction_set(dir);
	TIM3->ARR = speed-1;
	TIM3->CNT = 0x00;
	y_motor_move_enable = RUNNING;
}




void motor_move(double x, double y, int speed)
{
	double x_dis = (double)(x - x_pos);
	double y_dis = (double)(y - y_pos);
	double angle;
	double x_speed;
	double y_speed;
	double distance_per_sec;
	char x_dir = RIGHT;
	char y_dir = UP;
	
	 x_speed = speed;
   y_speed = speed;
	angle = atan2(y_dis, x_dis);
	
	if(x_dis < 0) 
	{
		x_dis = fabs(x_dis);
		x_dir = LEFT;
	}
	if(y_dis < 0) 
	{
		y_dis = fabs(y_dis);
		y_dir = DOWN;
	}
	
	x_distance = (int)(x_dis * 80);
	y_distance = (int)(y_dis * 80);
	
	distance_per_sec = ((1/ (((double)speed / 60.0)*80) )* 1000000) / 2;
	
	if(x_dis == 0)
	{
		y_speed = distance_per_sec * (double)(1 / fabs(sin(angle)) );
		y_motor_move(y_dir, (int)y_speed);
	}
	else if(y_dis == 0)
	{
		x_speed = distance_per_sec * (double)(1 / fabs(cos(angle)) );
		x_motor_move(x_dir, (int)x_speed);
	}
	else if(x_dis != 0 && y_dis != 0)
	{
		x_speed = (int)(distance_per_sec * (double)(1 / fabs(cos(angle)) ));
		y_speed = (int)(distance_per_sec * (double)(1 / fabs(sin(angle)) ));
		x_motor_move(x_dir, (int)x_speed);
		y_motor_move(y_dir, (int)y_speed);
	}
	
	x_pos = x;
	y_pos = y;
}

void timer_set()
{
	//timer1
	RCC->APB2ENR |= 0x01 << 0;
	TIM1->CR1 |= 0x01;
	TIM1->PSC = 168 - 1;
	TIM1->ARR = 1000 - 1;
	TIM1->CNT = 0x00;
	TIM1->DIER |= 0x01;
	NVIC->ISER[0] |= (0x01 << 25);
	
	//timer2
	RCC->APB1ENR |= 0x01 << 0;
	TIM2->CR1 |= 0x01;
	TIM2->PSC = 84 - 1;
	TIM2->ARR = 200 - 1;
	TIM2->CNT = 0x00;
	TIM2->DIER |= 0x01;
	NVIC->ISER[0] |= (0x01 << 28);
	
	//timer3
	RCC->APB1ENR |= 0x01 << 1;
	TIM3->CR1 |= 0x01;
	TIM3->PSC = 84 - 1;
	TIM3->ARR = 200 - 1;
	TIM3->CNT = 0x00;
	TIM3->DIER |= 0x01;
	NVIC->ISER[0] |= (0x01 << 29);
}

GPIO_InitTypeDef  GPIO_InitStructure;
USART_InitTypeDef USART_InitStructure;

void motor_set()
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);

	//led
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	//x_step
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOE, &GPIO_InitStructure);

//x_dir, x_en
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOF, &GPIO_InitStructure);

//y_step, y_dir
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOE, &GPIO_InitStructure);

//y_en
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

//z_step
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOE, &GPIO_InitStructure);

//z_dir, z_en
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIOF->ODR |= ~(0x01 << 2);
	GPIOD->ODR |= ~(0x01 << 7);
	GPIOC->ODR |= ~(0x01 << 0);
}

#define SIZE_POINT 480
double pos_array[5][3] = {{50,0,2400}, {50,25,2400}, {0,0,2400}};

unsigned long p_millis = 0;
unsigned long c_millis = 0;
int toggle = 0;
int state = 0;
int i = 0;

int main(void)
{	
	
	int i = 0;
	
//	if (SysTick_Config(168000000 / 100000)) {
//		while (1);
//	}
	
	
	motor_set();
	timer_set();
	
	for()
	
	
	while(1)
	{
		if(stop_check() == 0)
		{
			motor_move((double)xy_pos[state][0], (double)xy_pos[state][1], 4800);
			state++;
			
			if(state == SIZE_POINT) break;
		}
	}
	
	while(1)
	{
		if(stop_check() == 0)
		{
			motor_move(pos_array[state][0], pos_array[state][1], pos_array[state][2]);
			state++;
			if(state == SIZE_POINT) state = 0;
		}
	}
	
	y_distance = 0.1*80;
	y_motor_move(DOWN, 1125);
	while(1);
	
	
	

	
	while(1);
	
	
	direction_set(RIGHT);
	x_motor_move_enable = RUNNING;
	
	
	
	while(1)
	{
		c_millis = millis();
		if(c_millis - p_millis >= 300)
		{
			p_millis = c_millis;
			if(toggle == 0)
			 {
				 toggle = 1;
				 GPIOA->ODR |= (0x01 << 7);
			 }
			 else
			 {
				 toggle = 0;
				 GPIOA->ODR &= ~(0x01 << 7);
			 }
		 }
		
		 
		 if(state == 0)
		 {
				if(x_motor_move_enable == 0 && y_motor_move_enable == 0) 
				{
					direction_set(UP);
					y_motor_move_enable = RUNNING;
					state++;
				}
		 }
		 else if(state == 1)
		 {
			 if(x_motor_move_enable == 0 && y_motor_move_enable == 0) 
				{
					direction_set(LEFT);
					x_motor_move_enable = RUNNING;
					state++;
				}
		 }
		 else if(state == 2)
		 {
			 if(x_motor_move_enable == 0 && y_motor_move_enable == 0) 
				{
					direction_set(DOWN);
					y_motor_move_enable = RUNNING;
					state++;
				}
		 }
		 else if(state == 3)
		 {
			 if(x_motor_move_enable == 0 && y_motor_move_enable == 0) 
				{
					direction_set(RIGHT);
					x_motor_move_enable = RUNNING;
					state++;
				}
		 }
	}
	
	
	while(1)
	{
		// high 
		GPIOF->ODR |= (0x01 << 1);
	for(i = 0; i < 3200; i++)
	{
		Delay(20);
		GPIOE->ODR |= (0x01 << 9);
		Delay(20);
		GPIOE->ODR &= ~(0x01 << 9);
	}
	
	GPIOE->ODR |= (0x01 << 8);
	for(i = 0; i < 3200; i++)
	{
		Delay(20);
		GPIOE->ODR |= (0x01 << 11);
		Delay(20);
		GPIOE->ODR &= ~(0x01 << 11);
	}
	
	GPIOC->ODR |= (0x01 << 2);
	for(i = 0; i < 3200; i++)
	{
		Delay(20);
		GPIOE->ODR |= (0x01 << 13);
		Delay(20);
		GPIOE->ODR &= ~(0x01 << 13);
	}
		
		// low
	GPIOF->ODR &= ~(0x01 << 1);
	for(i = 0; i < 3200; i++)
	{
		Delay(20);
		GPIOE->ODR |= (0x01 << 9);
		Delay(20);
		GPIOE->ODR &= ~(0x01 << 9);
	}
	
	GPIOE->ODR &= ~(0x01 << 8);
	for(i = 0; i < 3200; i++)
	{
		Delay(20);
		GPIOE->ODR |= (0x01 << 11);
		Delay(20);
		GPIOE->ODR &= ~(0x01 << 11);
	}
	
	GPIOC->ODR &= ~(0x01 << 2);
	for(i = 0; i < 3200; i++)
	{
		Delay(20);
		GPIOE->ODR |= (0x01 << 13);
		Delay(20);
		GPIOE->ODR &= ~(0x01 << 13);
	}
	
	
	while(1);
}
	
	
	while(1) {
		// 200 us
//		Delay(20);
//		GPIOE->ODR |= (0x01 << 9);
//		Delay(20);
//		GPIOE->ODR &= ~(0x01 << 9);
		delay_ms(1000);
		GPIOA->ODR |= (0x01 << 7);
		delay_ms(1000);
		GPIOA->ODR &= ~(0x01 << 7);
	}
	
	
}


// X STEP PE9
// X DIR PF1
// X EN PF2
// Y STEP PE11
// Y DIR PE8
// Y EN PD7
// Z STEP PE13
// Z DIR PC2
// Z EN PC0