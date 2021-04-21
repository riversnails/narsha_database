/* Includes ------------------------------------------------------------------*/
#include "VS1003/vs1003.h"

/* Private variables ---------------------------------------------------------*/

/* VS1003设置参数 */ 
/* 0 , henh.1 , hfreq.2 , lenh.3 , lfreq 5 ,主音量 */
uint8_t vs1003ram[5] = { 0 , 0 , 0 , 0 , 250 };

/*******************************************************************************
* Function Name  : delay_ms
* Description    : delay time
* Input          : - ms: time
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
static void delay_ms(uint32_t ms)
{
    uint32_t len;
    for (;ms > 0; ms --)
        for (len = 0; len < 100; len++ );
}
		
/*******************************************************************************
* Function Name  : VS1003_SPI_Init
* Description    : VS1003 SPI Init
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
static void VS1003_SPI_Init(void)
{
  	SPI_InitTypeDef  SPI_InitStructure;

	RCC_APB1PeriphClockCmd(Open_SPIx_CLK,ENABLE);
	SPI_I2S_DeInit(Open_SPIx);
	SPI_InitStructure.SPI_Direction= SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b; 
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(Open_SPIx, &SPI_InitStructure);

	SPI_Cmd(Open_SPIx, ENABLE);
}

/*******************************************************************************
* Function Name  : VS1003_SPI_SetSpeed
* Description    : None
* Input          : - SpeedSet: SPI_SPEED_HIGH 1  SPI_SPEED_LOW 0
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
static void VS1003_SPI_SetSpeed( uint8_t SpeedSet)
{
  SPI_InitTypeDef SPI_InitStructure ;
    
  SPI_InitStructure.SPI_Direction=SPI_Direction_2Lines_FullDuplex ;
  SPI_InitStructure.SPI_Mode=SPI_Mode_Master ;
  SPI_InitStructure.SPI_DataSize=SPI_DataSize_8b ;
  SPI_InitStructure.SPI_CPOL=SPI_CPOL_High ;
  SPI_InitStructure.SPI_CPHA=SPI_CPHA_2Edge ;
  SPI_InitStructure.SPI_NSS=SPI_NSS_Soft ;

  if( SpeedSet == SPI_SPEED_LOW )
  {
      SPI_InitStructure.SPI_BaudRatePrescaler=SPI_BaudRatePrescaler_128;
  }
  else 
  {
      SPI_InitStructure.SPI_BaudRatePrescaler=SPI_BaudRatePrescaler_8;
  }
  SPI_InitStructure.SPI_FirstBit=SPI_FirstBit_MSB ;
  SPI_InitStructure.SPI_CRCPolynomial=7 ;
  SPI_Init(Open_SPIx,&SPI_InitStructure);
}

/*******************************************************************************
* Function Name  : VS1003_SPI_ReadWriteByte
* Description    : None
* Input          : - TxData: Write Byte
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
static uint16_t VS1003_SPI_ReadWriteByte( uint16_t TxData )
{
	while(SPI_I2S_GetFlagStatus(Open_SPIx, SPI_I2S_FLAG_TXE)==RESET);
	SPI_I2S_SendData(Open_SPIx,TxData);
	
	while(SPI_I2S_GetFlagStatus(Open_SPIx, SPI_I2S_FLAG_RXNE)==RESET);
	return SPI_I2S_ReceiveData(Open_SPIx);
}

/*******************************************************************************
* Function Name  : VS1003_Init
* Description    : VS1003端口初始化
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/	 
void VS1003_Init(void)
{		  
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB , ENABLE);
	
	GPIO_PinAFConfig(Open_SPIx_SCK_GPIO_PORT, Open_SPIx_SCK_SOURCE,  Open_SPIx_SCK_AF);
	GPIO_PinAFConfig(Open_SPIx_MISO_GPIO_PORT, Open_SPIx_MISO_SOURCE, Open_SPIx_MISO_AF);
	GPIO_PinAFConfig(Open_SPIx_MOSI_GPIO_PORT, Open_SPIx_MOSI_SOURCE, Open_SPIx_MOSI_AF);
	
	GPIO_InitStructure.GPIO_Pin = Open_SPIx_SCK_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;  
	GPIO_Init(Open_SPIx_SCK_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = Open_SPIx_MISO_PIN;
	GPIO_Init(Open_SPIx_MISO_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = Open_SPIx_MOSI_PIN;
	GPIO_Init(Open_SPIx_MOSI_GPIO_PORT, &GPIO_InitStructure);

	/* XCS */
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	/* XDCS */
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_4;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/* Reset */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	 
	/* DREQ */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_Init(GPIOA, &GPIO_InitStructure); 
	
	MP3_Reset(0);
	delay_ms(1);
	MP3_Reset(1);
	
	MP3_DCS(1); 
	MP3_CCS(1); 
	
	VS1003_SPI_Init();
}


/*******************************************************************************
* Function Name  : VS1003_WriteReg
* Description    : VS1003写寄存器
* Input          : - reg: 命令地址
*				   - value: 命令数据
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void VS1003_WriteReg( uint8_t reg,uint16_t value )
{  
   while(  MP3_DREQ ==0 );           /* 等待空闲 */
   
   VS1003_SPI_SetSpeed( SPI_SPEED_LOW );	 
   MP3_DCS(1); 
   MP3_CCS(0); 
   VS1003_SPI_ReadWriteByte(VS_WRITE_COMMAND); /* 发送VS1003的写命令 */
   VS1003_SPI_ReadWriteByte(reg);             
   VS1003_SPI_ReadWriteByte(value>>8);        
   VS1003_SPI_ReadWriteByte(value);	          
   MP3_CCS(1);         
   VS1003_SPI_SetSpeed( SPI_SPEED_HIGH );
} 

/*******************************************************************************
* Function Name  : VS1003_ReadReg
* Description    : VS1003读寄存器
* Input          : - reg: 命令地址
* Output         : None
* Return         : - value: 数据
* Attention		 : None
*******************************************************************************/
uint16_t VS1003_ReadReg( uint8_t reg)
{ 
   uint16_t value;
    
   while(  MP3_DREQ ==0 );           /* 等待空闲 */
   VS1003_SPI_SetSpeed( SPI_SPEED_LOW );
   MP3_DCS(1);     
   MP3_CCS(0);     
   VS1003_SPI_ReadWriteByte(VS_READ_COMMAND);/* 发送VS1003的读命令 */
   VS1003_SPI_ReadWriteByte( reg );   
   value = VS1003_SPI_ReadWriteByte(0xff);		
   value = value << 8;
   value |= VS1003_SPI_ReadWriteByte(0xff); 
   MP3_CCS(1);   
   VS1003_SPI_SetSpeed( SPI_SPEED_HIGH );
   return value; 
} 

/*******************************************************************************
* Function Name  : VS1003_ResetDecodeTime
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/                       
void VS1003_ResetDecodeTime(void)
{
   VS1003_WriteReg(SPI_DECODE_TIME,0x0000);
   VS1003_WriteReg(SPI_DECODE_TIME,0x0000); /* 操作两次 */
}

/*******************************************************************************
* Function Name  : VS1003_GetDecodeTime
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/    
uint16_t VS1003_GetDecodeTime(void)
{ 
   return VS1003_ReadReg(SPI_DECODE_TIME);   
} 

/*******************************************************************************
* Function Name  : VS1003_SoftReset
* Description    : VS1003软复位
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void VS1003_SoftReset(void)
{
   uint8_t retry; 	
   			   
   while(  MP3_DREQ ==0 );   /* 等待软件复位结束 */
   VS1003_SPI_ReadWriteByte(0xff);  /* 启动传输 */
   retry = 0;
   while( VS1003_ReadReg(0) != 0x0804 )   /* 软件复位,新模式 */  
   {
	   VS1003_WriteReg(SPI_MODE,0x0804);  /* 软件复位,新模式 */
	   delay_ms(2);                        /* 等待至少1.35ms */
	   if( retry++ > 100 )
	   { 
	      break; 
	   }
   }	 	

   while(  MP3_DREQ ==0 );   /* 等待软件复位结束 */

   retry = 0;

   while( VS1003_ReadReg(SPI_CLOCKF) != 0X9800 ) /* 设置vs1003的时钟,3倍频 ,1.5xADD */ 
   {
	   VS1003_WriteReg(SPI_CLOCKF,0X9800);       /* 设置vs1003的时钟,3倍频 ,1.5xADD */
	   if( retry++ > 100 )
	   { 
	      break; 
	   }
   }		   
   retry = 0;

   while( VS1003_ReadReg(SPI_AUDATA) != 0XBB81 ) /* 设置vs1003的时钟,3倍频 ,1.5xADD */
   {
	   VS1003_WriteReg(SPI_AUDATA,0XBB81);
	   if( retry++ > 100 )
	   { 
	      break; 
	   }   
   }

   VS1003_WriteReg(11,0x2020);	 /* 音量 */			 
   VS1003_ResetDecodeTime();     /* 复位解码时间 */	     
   /* 向vs1003发送4个字节无效数据，用以启动SPI发送 */	
   MP3_DCS(0);  /* 选中数据传输 */
   VS1003_SPI_ReadWriteByte(0XFF);
   VS1003_SPI_ReadWriteByte(0XFF);
   VS1003_SPI_ReadWriteByte(0XFF);
   VS1003_SPI_ReadWriteByte(0XFF);
   MP3_DCS(1);  /* 取消数据传输 */
   delay_ms(20);
} 

/*******************************************************************************
* Function Name  : VS1003_Reset
* Description    : VS1003复位
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void VS1003_Reset(void)
{
   delay_ms(20);
   VS1003_SPI_ReadWriteByte(0XFF);
   MP3_DCS(1);              /* 取消数据传输 */
   MP3_CCS(1);              /* 取消数据传输 */

   while(  MP3_DREQ ==0 );  /* 等待DREQ为高 */
   delay_ms(20);			 
}

/*******************************************************************************
* Function Name  : VS1003_SineTest
* Description    : VS1003正弦测试
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void VS1003_SineTest(void)
{	
   VS1003_WriteReg(0x0b,0X2020);	  /* 设置音量 */	 
   VS1003_WriteReg(SPI_MODE,0x0820); /* 进入vs1003的测试模式 */	    
   while(  MP3_DREQ ==0 );  /* 等待DREQ为高 */
 	
   /* 向vs1003发送正弦测试命令：0x53 0xef 0x6e n 0x00 0x00 0x00 0x00 */
   MP3_DCS(0);/* 选中数据传输 */
   VS1003_SPI_ReadWriteByte(0x53);
   VS1003_SPI_ReadWriteByte(0xef);
   VS1003_SPI_ReadWriteByte(0x6e);
   VS1003_SPI_ReadWriteByte(0x24);
   VS1003_SPI_ReadWriteByte(0x00);
   VS1003_SPI_ReadWriteByte(0x00);
   VS1003_SPI_ReadWriteByte(0x00);
   VS1003_SPI_ReadWriteByte(0x00);
   delay_ms(100);
   MP3_DCS(1); 
   /* 退出正弦测试 */
   MP3_DCS(0);/* 选中数据传输 */
   VS1003_SPI_ReadWriteByte(0x45);
   VS1003_SPI_ReadWriteByte(0x78);
   VS1003_SPI_ReadWriteByte(0x69);
   VS1003_SPI_ReadWriteByte(0x74);
   VS1003_SPI_ReadWriteByte(0x00);
   VS1003_SPI_ReadWriteByte(0x00);
   VS1003_SPI_ReadWriteByte(0x00);
   VS1003_SPI_ReadWriteByte(0x00);
   delay_ms(100);
   MP3_DCS(1);		 

   /* 再次进入正弦测试并设置n值为0x44，即将正弦波的频率设置为另外的值 */
   MP3_DCS(0);/* 选中数据传输 */      
   VS1003_SPI_ReadWriteByte(0x53);
   VS1003_SPI_ReadWriteByte(0xef);
   VS1003_SPI_ReadWriteByte(0x6e);
   VS1003_SPI_ReadWriteByte(0x44);
   VS1003_SPI_ReadWriteByte(0x00);
   VS1003_SPI_ReadWriteByte(0x00);
   VS1003_SPI_ReadWriteByte(0x00);
   VS1003_SPI_ReadWriteByte(0x00);
   delay_ms(100);
   MP3_DCS(1);
   /* 退出正弦测试 */
   MP3_DCS(0);/* 选中数据传输 */      
   VS1003_SPI_ReadWriteByte(0x45);
   VS1003_SPI_ReadWriteByte(0x78);
   VS1003_SPI_ReadWriteByte(0x69);
   VS1003_SPI_ReadWriteByte(0x74);
   VS1003_SPI_ReadWriteByte(0x00);
   VS1003_SPI_ReadWriteByte(0x00);
   VS1003_SPI_ReadWriteByte(0x00);
   VS1003_SPI_ReadWriteByte(0x00);
   delay_ms(100);
   MP3_DCS(1);	 
}	 

/*******************************************************************************
* Function Name  : VS1003_RamTest
* Description    : VS1003 RAM测试
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/																			 
void VS1003_RamTest(void)
{
   volatile uint16_t value;
     
   VS1003_Reset();     
   VS1003_WriteReg(SPI_MODE,0x0820);/* 进入vs1003的测试模式 */
   while(  MP3_DREQ ==0 );           /* 等待DREQ为高 */
   MP3_DCS(0);	       			     /* xDCS = 1，选择vs1003的数据接口 */
   VS1003_SPI_ReadWriteByte(0x4d);
   VS1003_SPI_ReadWriteByte(0xea);
   VS1003_SPI_ReadWriteByte(0x6d);
   VS1003_SPI_ReadWriteByte(0x54);
   VS1003_SPI_ReadWriteByte(0x00);
   VS1003_SPI_ReadWriteByte(0x00);
   VS1003_SPI_ReadWriteByte(0x00);
   VS1003_SPI_ReadWriteByte(0x00);
   delay_ms(50);  
   MP3_DCS(1);
   value = VS1003_ReadReg(SPI_HDAT0); /* 如果得到的值为0x807F，则表明完好 */
}     
		 				
/*******************************************************************************
* Function Name  : VS1003_SetVol
* Description    : 设定vs1003播放的音量和高低音
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/   
void VS1003_SetVol(void)
{
   uint8_t i;
   uint16_t bass=0;  /* 暂存音调寄存器值 */
   uint16_t volt=0;  /* 暂存音量值 */
   uint8_t  vset=0;  /* 暂存音量值 */ 
		 
   vset = 255 - vs1003ram[4]; /* 取反得到最大值 */ 
   volt =vset;
   volt <<= 8;
   volt += vset;/* 得到音量设置后大小 */
   /* 0,henh.1,hfreq.2,lenh.3,lfreq */      
   for( i = 0; i < 4; i++ )
   {
       bass <<= 4;
       bass += vs1003ram[i]; 
   }     
   VS1003_WriteReg(SPI_BASS,bass);/* BASS */   
   VS1003_WriteReg(SPI_VOL,volt); /* 设音量 */ 
}    

/*******************************************************************************
* Function Name  : VS1003_WriteData
* Description    : VS1003写数据
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/  
void VS1003_WriteData( uint8_t * buf)
{		
   uint8_t  count = 32;

   MP3_DCS(0);   
   while( count-- )
   {
	  VS1003_SPI_ReadWriteByte( *buf++ );
   }
   MP3_DCS(1);  
   MP3_CCS(1);  
} 


/*********************************************************************************************************
      END FILE
*********************************************************************************************************/

