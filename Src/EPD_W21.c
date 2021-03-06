#include "EPD_W21.h"

const unsigned char lut_vcomDC[] =
{
0x00	,0x1A	,0x1A	,0x00	,0x00	,0x01,		
0x00	,0x0A	,0x0A	,0x00	,0x00	,0x08,		
0x00	,0x0E	,0x01	,0x0E	,0x01	,0x10,		
0x00	,0x0A	,0x0A	,0x00	,0x00	,0x08,		
0x00	,0x04	,0x10	,0x00	,0x00	,0x05,		
0x00	,0x03	,0x0E	,0x00	,0x00	,0x0A,		
0x00	,0x23	,0x00	,0x00	,0x00	,0x01	,0x00	,0x00
};

const unsigned char lut_ww[] ={
0x90	,0x1A	,0x1A	,0x00	,0x00	,0x01,
0x40	,0x0A	,0x0A	,0x00	,0x00	,0x08,
0x84	,0x0E	,0x01	,0x0E	,0x01	,0x10,
0x80	,0x0A	,0x0A	,0x00	,0x00	,0x08,
0x00	,0x04	,0x10	,0x00	,0x00	,0x05,
0x00	,0x03	,0x0E	,0x00	,0x00	,0x0A,
0x00	,0x23	,0x00	,0x00	,0x00	,0x01
};

const unsigned char lut_r[] ={
0xA0	,0x1A	,0x1A	,0x00	,0x00	,0x01,
0x00	,0x0A	,0x0A	,0x00	,0x00	,0x08,
0x84	,0x0E	,0x01	,0x0E	,0x01	,0x10,
0x90	,0x0A	,0x0A	,0x00	,0x00	,0x08,
0xB0	,0x04	,0x10	,0x00	,0x00	,0x05,
0xB0	,0x03	,0x0E	,0x00	,0x00	,0x0A,
0xC0	,0x23	,0x00	,0x00	,0x00	,0x01
};

const unsigned char lut_w[] ={
0x90	,0x1A	,0x1A	,0x00	,0x00	,0x01,
0x40	,0x0A	,0x0A	,0x00	,0x00	,0x08,
0x84	,0x0E	,0x01	,0x0E	,0x01	,0x10,
0x80	,0x0A	,0x0A	,0x00	,0x00	,0x08,
0x00	,0x04	,0x10	,0x00	,0x00	,0x05,
0x00	,0x03	,0x0E	,0x00	,0x00	,0x0A,
0x00	,0x23	,0x00	,0x00	,0x00	,0x01
};

const unsigned char lut_b[] ={
0x90	,0x1A	,0x1A	,0x00	,0x00	,0x01,
0x20	,0x0A	,0x0A	,0x00	,0x00	,0x08,
0x84	,0x0E	,0x01	,0x0E	,0x01	,0x10,
0x10	,0x0A	,0x0A	,0x00	,0x00	,0x08,
0x00	,0x04	,0x10	,0x00	,0x00	,0x05,
0x00	,0x03	,0x0E	,0x00	,0x00	,0x0A,
0x00	,0x23	,0x00	,0x00	,0x00	,0x01
};

static void Usr_SPI_OpraDelay(void)
{
	int i,j;
	for(i=0;i<1;i++)
		for(j=0;j<1;j++);
}
/*
void Delay_ms(int delaytime)
{
	HAL_Delay(delaytime);
}
*/

static void Usr_SPI_Wirte(unsigned char C_0_D_1, unsigned char data) {
	unsigned char TEMPCOM;
	unsigned char scnt;
	TEMPCOM=data;
	nCS_H
	nCS_L
	SCLK_L 
	if (C_0_D_1) {
		nDC_H
	}
	else {
		nDC_L
	}
	for(scnt=0;scnt<8;scnt++)
	{
		if(TEMPCOM&0x80)
			SDA_H
		else
			SDA_L
		Usr_SPI_OpraDelay();
		SCLK_H  
		Usr_SPI_OpraDelay();
		SCLK_L     
		TEMPCOM=TEMPCOM<<1;
		Usr_SPI_OpraDelay();
	}
	nCS_H 
}
static void SPI4W_WRITECOM(unsigned char INIT_COM)
{
	Usr_SPI_Wirte(0, INIT_COM);
}
static void SPI4W_WRITEDATA(unsigned char INIT_DATA)
{
	Usr_SPI_Wirte(1, INIT_DATA);
}
static void EPD_W21_CheckStatus(void)
{
	unsigned char busy;
	do
	{
		SPI4W_WRITECOM(0x71);
		busy = nBUSY;
		busy =!(busy & 0x01);        
	}
	while(busy);   
	Delay_ms(20);
}

void EPD_W21_Init(void)
{
	nBS_L;				//4 wire spi mode selected
	nRST_L;				//module reset	
	Delay_ms(10);
	nRST_H;
	Delay_ms(10);
}


static void lut(void)
{
	unsigned int i;
	unsigned char *pt;
	
	pt = (unsigned char *)lut_vcomDC;
	i = 44;
	SPI4W_WRITECOM(0x20);
	while (i--)
		SPI4W_WRITEDATA(*pt++);
	
	pt = (unsigned char *)lut_r;
	i = 42;
	SPI4W_WRITECOM(0x22);
	while (i--)
		SPI4W_WRITEDATA(*pt++);
	
	pt = (unsigned char *)lut_w;
	i = 42;
	SPI4W_WRITECOM(0x23);
	while (i--)
		SPI4W_WRITEDATA(*pt++);
	
	pt = (unsigned char *)lut_b;
	i = 42;
	SPI4W_WRITECOM(0x24);
	while (i--)
		SPI4W_WRITEDATA(*pt++);
}	

static void Load2Chip(const unsigned char *src) {
	unsigned int i;
	for (i = 0; i < 15000; i++) {
		SPI4W_WRITEDATA(*src++);
	}
	Delay_ms(2);
}

void EPD_W21_Display(const unsigned char *src_black, const unsigned char *src_red, unsigned char SleepEnable)
{ 
		extern void convert(unsigned char *);
		convert((unsigned char *)src_black);
		convert((unsigned char *)src_red);
		EPD_W21_Init();
#if 0
		SPI4W_WRITECOM(0x01); 
		SPI4W_WRITEDATA (0x03);    
		SPI4W_WRITEDATA (0x00);
		SPI4W_WRITEDATA (0x2b);
		SPI4W_WRITEDATA (0x2b);
		SPI4W_WRITEDATA (0x12);

		SPI4W_WRITECOM(0x06);
		SPI4W_WRITEDATA (0x17);
		SPI4W_WRITEDATA (0x17);
		SPI4W_WRITEDATA (0x17);

		SPI4W_WRITECOM(0x06);
		SPI4W_WRITEDATA (0x17);
		SPI4W_WRITEDATA (0x17);
		SPI4W_WRITEDATA (0x17);

		SPI4W_WRITECOM(0x04);  
		EPD_W21_CheckStatus();
		SPI4W_WRITECOM(0x00);
		SPI4W_WRITEDATA(0xaf);
		SPI4W_WRITEDATA(0x0b);

		SPI4W_WRITECOM(0x30);
		SPI4W_WRITEDATA (0x3a);
		SPI4W_WRITECOM(0x61);
		SPI4W_WRITEDATA (0x01); 
		SPI4W_WRITEDATA (0x90);
		SPI4W_WRITEDATA (0x01);
		SPI4W_WRITEDATA (0x2c);
		SPI4W_WRITECOM(0x82);
		SPI4W_WRITEDATA (0x12);				   
		Delay_ms(2);	              
		SPI4W_WRITECOM(0X50);
		SPI4W_WRITEDATA(0x77);
#else
		SPI4W_WRITECOM(0x06);         //boost��		SPI4W_WRITEDATA (0x17);
		SPI4W_WRITEDATA (0x17);
		SPI4W_WRITEDATA (0x17);       //07 0f 17 1f 27 2F 37 2f

		SPI4W_WRITECOM(0x04);  
		EPD_W21_CheckStatus();
		SPI4W_WRITECOM(0x00);
		SPI4W_WRITEDATA(0x0f);		// LUT from OTP 
    
    SPI4W_WRITECOM(0x30);
		SPI4W_WRITEDATA (0x3a);
		SPI4W_WRITECOM(0x61);
		SPI4W_WRITEDATA (0x01); 
		SPI4W_WRITEDATA (0x90);
		SPI4W_WRITEDATA (0x01);
		SPI4W_WRITEDATA (0x2c);
		SPI4W_WRITECOM(0x82);
		SPI4W_WRITEDATA (0x12);				   
		Delay_ms(2);	              
		SPI4W_WRITECOM(0X50);
		SPI4W_WRITEDATA(0x77);
#endif
		/*pic_display();
		*/
		SPI4W_WRITECOM(0x10);	  	
		Load2Chip(src_black);
		SPI4W_WRITECOM(0x13);
		Load2Chip(src_red);
		lut();
		SPI4W_WRITECOM(0x12);
		Delay_ms(50);	              
		EPD_W21_CheckStatus();
		
		if (SleepEnable) {
		/************************power off Sequence************************/
			SPI4W_WRITECOM(0X50);
			SPI4W_WRITEDATA(0xf7);		//border floating	

			SPI4W_WRITECOM(0X82);  		//VCOM to 0V
			SPI4W_WRITECOM(0X00);  		//

			Delay_ms(100);	  	

			SPI4W_WRITECOM(0x01);   		//VG&VS to 0V fast
			SPI4W_WRITEDATA (0x00);		
			SPI4W_WRITEDATA (0x00);		
			SPI4W_WRITEDATA (0x00);		      
			SPI4W_WRITEDATA (0x00);		
			SPI4W_WRITEDATA (0x00);
						 
			Delay_ms(100);
				
			SPI4W_WRITECOM(0X02);  		//power off
			EPD_W21_CheckStatus();
			SPI4W_WRITECOM(0X07);		  	//deep sleep
			SPI4W_WRITEDATA(0xA5);
	}
///************************power off Sequence************************/
}

