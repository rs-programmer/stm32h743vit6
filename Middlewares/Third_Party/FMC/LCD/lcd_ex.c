/************************************************ 
* lcd_ex.c存放各个LCD驱动IC的寄存器初始化部分代码,以简化lcd.c,该.c文件
* 不直接加入到工程里面,只有lcd.c会用到,所以通过include的形式添加.(不要在
* 其他文件再包含该.c文件!!否则会报错!)							  
************************************************/	
 
#include "delay.h"
#include "lcd.h"

#define delay_ms(x) HAL_Delay(x)
#define delay_us(x) MX_Delay_us(x)

/**
 * @brief       ST7789 寄存器初始化代码
 * @param       无
 * @retval      无
 */
void lcd_ex_st7789_reginit(void)
{
    LCD_WriteReg(0x11);

    delay_ms(120); 

    LCD_WriteReg(0x36);
    LCD_WriteData(0x00);

    LCD_WriteReg(0x3A);
    LCD_WriteData(0X05);

    LCD_WriteReg(0xB2);
    LCD_WriteData(0x0C);
    LCD_WriteData(0x0C);
    LCD_WriteData(0x00);
    LCD_WriteData(0x33);
    LCD_WriteData(0x33);

    LCD_WriteReg(0xB7);
    LCD_WriteData(0x35);

    LCD_WriteReg(0xBB); /* vcom */
    LCD_WriteData(0x32);  /* 30 */

    LCD_WriteReg(0xC0);
    LCD_WriteData(0x0C);

    LCD_WriteReg(0xC2);
    LCD_WriteData(0x01);

    LCD_WriteReg(0xC3); /* vrh */
    LCD_WriteData(0x10);  /* 17 0D */

    LCD_WriteReg(0xC4); /* vdv */
    LCD_WriteData(0x20);  /* 20 */

    LCD_WriteReg(0xC6);
    LCD_WriteData(0x0f);

    LCD_WriteReg(0xD0);
    LCD_WriteData(0xA4); 
    LCD_WriteData(0xA1); 

    LCD_WriteReg(0xE0); /* Set Gamma  */
    LCD_WriteData(0xd0);
    LCD_WriteData(0x00);
    LCD_WriteData(0x02);
    LCD_WriteData(0x07);
    LCD_WriteData(0x0a);
    LCD_WriteData(0x28);
    LCD_WriteData(0x32);
    LCD_WriteData(0X44);
    LCD_WriteData(0x42);
    LCD_WriteData(0x06);
    LCD_WriteData(0x0e);
    LCD_WriteData(0x12);
    LCD_WriteData(0x14);
    LCD_WriteData(0x17);

    LCD_WriteReg(0XE1); /* Set Gamma */
    LCD_WriteData(0xd0);
    LCD_WriteData(0x00);
    LCD_WriteData(0x02);
    LCD_WriteData(0x07);
    LCD_WriteData(0x0a);
    LCD_WriteData(0x28);
    LCD_WriteData(0x31);
    LCD_WriteData(0x54);
    LCD_WriteData(0x47);
    LCD_WriteData(0x0e);
    LCD_WriteData(0x1c);
    LCD_WriteData(0x17);
    LCD_WriteData(0x1b); 
    LCD_WriteData(0x1e);

    LCD_WriteReg(0x2A);
    LCD_WriteData(0x00);
    LCD_WriteData(0x00);
    LCD_WriteData(0x00);
    LCD_WriteData(0xef);

    LCD_WriteReg(0x2B);
    LCD_WriteData(0x00);
    LCD_WriteData(0x00);
    LCD_WriteData(0x01);
    LCD_WriteData(0x3f);

    LCD_WriteReg(0x29); /* display on */
}

/**
 * @brief       NT35310寄存器初始化代码 
 * @param       无
 * @retval      无
 */
void lcd_ex_nt35310_reginit(void)
{
    LCD_WriteReg(0xED);
    LCD_WriteData(0x01);
    LCD_WriteData(0xFE);

    LCD_WriteReg(0xEE);
    LCD_WriteData(0xDE);
    LCD_WriteData(0x21);

    LCD_WriteReg(0xF1);
    LCD_WriteData(0x01);
    LCD_WriteReg(0xDF);
    LCD_WriteData(0x10);

    /* VCOMvoltage */
    LCD_WriteReg(0xC4);
    LCD_WriteData(0x8F);  /* 5f */

    LCD_WriteReg(0xC6);
    LCD_WriteData(0x00);
    LCD_WriteData(0xE2);
    LCD_WriteData(0xE2);
    LCD_WriteData(0xE2);
    LCD_WriteReg(0xBF);
    LCD_WriteData(0xAA);

    LCD_WriteReg(0xB0);
    LCD_WriteData(0x0D);
    LCD_WriteData(0x00);
    LCD_WriteData(0x0D);
    LCD_WriteData(0x00);
    LCD_WriteData(0x11);
    LCD_WriteData(0x00);
    LCD_WriteData(0x19);
    LCD_WriteData(0x00);
    LCD_WriteData(0x21);
    LCD_WriteData(0x00);
    LCD_WriteData(0x2D);
    LCD_WriteData(0x00);
    LCD_WriteData(0x3D);
    LCD_WriteData(0x00);
    LCD_WriteData(0x5D);
    LCD_WriteData(0x00);
    LCD_WriteData(0x5D);
    LCD_WriteData(0x00);

    LCD_WriteReg(0xB1);
    LCD_WriteData(0x80);
    LCD_WriteData(0x00);
    LCD_WriteData(0x8B);
    LCD_WriteData(0x00);
    LCD_WriteData(0x96);
    LCD_WriteData(0x00);

    LCD_WriteReg(0xB2);
    LCD_WriteData(0x00);
    LCD_WriteData(0x00);
    LCD_WriteData(0x02);
    LCD_WriteData(0x00);
    LCD_WriteData(0x03);
    LCD_WriteData(0x00);

    LCD_WriteReg(0xB3);
    LCD_WriteData(0x00);
    LCD_WriteData(0x00);
    LCD_WriteData(0x00);
    LCD_WriteData(0x00);
    LCD_WriteData(0x00);
    LCD_WriteData(0x00);
    LCD_WriteData(0x00);
    LCD_WriteData(0x00);
    LCD_WriteData(0x00);
    LCD_WriteData(0x00);
    LCD_WriteData(0x00);
    LCD_WriteData(0x00);
    LCD_WriteData(0x00);
    LCD_WriteData(0x00);
    LCD_WriteData(0x00);
    LCD_WriteData(0x00);
    LCD_WriteData(0x00);
    LCD_WriteData(0x00);
    LCD_WriteData(0x00);
    LCD_WriteData(0x00);
    LCD_WriteData(0x00);
    LCD_WriteData(0x00);
    LCD_WriteData(0x00);
    LCD_WriteData(0x00);

    LCD_WriteReg(0xB4);
    LCD_WriteData(0x8B);
    LCD_WriteData(0x00);
    LCD_WriteData(0x96);
    LCD_WriteData(0x00);
    LCD_WriteData(0xA1);
    LCD_WriteData(0x00);

    LCD_WriteReg(0xB5);
    LCD_WriteData(0x02);
    LCD_WriteData(0x00);
    LCD_WriteData(0x03);
    LCD_WriteData(0x00);
    LCD_WriteData(0x04);
    LCD_WriteData(0x00);

    LCD_WriteReg(0xB6);
    LCD_WriteData(0x00);
    LCD_WriteData(0x00);

    LCD_WriteReg(0xB7);
    LCD_WriteData(0x00);
    LCD_WriteData(0x00);
    LCD_WriteData(0x3F);
    LCD_WriteData(0x00);
    LCD_WriteData(0x5E);
    LCD_WriteData(0x00);
    LCD_WriteData(0x64);
    LCD_WriteData(0x00);
    LCD_WriteData(0x8C);
    LCD_WriteData(0x00);
    LCD_WriteData(0xAC);
    LCD_WriteData(0x00);
    LCD_WriteData(0xDC);
    LCD_WriteData(0x00);
    LCD_WriteData(0x70);
    LCD_WriteData(0x00);
    LCD_WriteData(0x90);
    LCD_WriteData(0x00);
    LCD_WriteData(0xEB);
    LCD_WriteData(0x00);
    LCD_WriteData(0xDC);
    LCD_WriteData(0x00);

    LCD_WriteReg(0xB8);
    LCD_WriteData(0x00);
    LCD_WriteData(0x00);
    LCD_WriteData(0x00);
    LCD_WriteData(0x00);
    LCD_WriteData(0x00);
    LCD_WriteData(0x00);
    LCD_WriteData(0x00);
    LCD_WriteData(0x00);

    LCD_WriteReg(0xBA);
    LCD_WriteData(0x24);
    LCD_WriteData(0x00);
    LCD_WriteData(0x00);
    LCD_WriteData(0x00);

    LCD_WriteReg(0xC1);
    LCD_WriteData(0x20);
    LCD_WriteData(0x00);
    LCD_WriteData(0x54);
    LCD_WriteData(0x00);
    LCD_WriteData(0xFF);
    LCD_WriteData(0x00);

    LCD_WriteReg(0xC2);
    LCD_WriteData(0x0A);
    LCD_WriteData(0x00);
    LCD_WriteData(0x04);
    LCD_WriteData(0x00);

    LCD_WriteReg(0xC3);
    LCD_WriteData(0x3C);
    LCD_WriteData(0x00);
    LCD_WriteData(0x3A);
    LCD_WriteData(0x00);
    LCD_WriteData(0x39);
    LCD_WriteData(0x00);
    LCD_WriteData(0x37);
    LCD_WriteData(0x00);
    LCD_WriteData(0x3C);
    LCD_WriteData(0x00);
    LCD_WriteData(0x36);
    LCD_WriteData(0x00);
    LCD_WriteData(0x32);
    LCD_WriteData(0x00);
    LCD_WriteData(0x2F);
    LCD_WriteData(0x00);
    LCD_WriteData(0x2C);
    LCD_WriteData(0x00);
    LCD_WriteData(0x29);
    LCD_WriteData(0x00);
    LCD_WriteData(0x26);
    LCD_WriteData(0x00);
    LCD_WriteData(0x24);
    LCD_WriteData(0x00);
    LCD_WriteData(0x24);
    LCD_WriteData(0x00);
    LCD_WriteData(0x23);
    LCD_WriteData(0x00);
    LCD_WriteData(0x3C);
    LCD_WriteData(0x00);
    LCD_WriteData(0x36);
    LCD_WriteData(0x00);
    LCD_WriteData(0x32);
    LCD_WriteData(0x00);
    LCD_WriteData(0x2F);
    LCD_WriteData(0x00);
    LCD_WriteData(0x2C);
    LCD_WriteData(0x00);
    LCD_WriteData(0x29);
    LCD_WriteData(0x00);
    LCD_WriteData(0x26);
    LCD_WriteData(0x00);
    LCD_WriteData(0x24);
    LCD_WriteData(0x00);
    LCD_WriteData(0x24);
    LCD_WriteData(0x00);
    LCD_WriteData(0x23);
    LCD_WriteData(0x00);

    LCD_WriteReg(0xC4);
    LCD_WriteData(0x62);
    LCD_WriteData(0x00);
    LCD_WriteData(0x05);
    LCD_WriteData(0x00);
    LCD_WriteData(0x84);
    LCD_WriteData(0x00);
    LCD_WriteData(0xF0);
    LCD_WriteData(0x00);
    LCD_WriteData(0x18);
    LCD_WriteData(0x00);
    LCD_WriteData(0xA4);
    LCD_WriteData(0x00);
    LCD_WriteData(0x18);
    LCD_WriteData(0x00);
    LCD_WriteData(0x50);
    LCD_WriteData(0x00);
    LCD_WriteData(0x0C);
    LCD_WriteData(0x00);
    LCD_WriteData(0x17);
    LCD_WriteData(0x00);
    LCD_WriteData(0x95);
    LCD_WriteData(0x00);
    LCD_WriteData(0xF3);
    LCD_WriteData(0x00);
    LCD_WriteData(0xE6);
    LCD_WriteData(0x00);

    LCD_WriteReg(0xC5);
    LCD_WriteData(0x32);
    LCD_WriteData(0x00);
    LCD_WriteData(0x44);
    LCD_WriteData(0x00);
    LCD_WriteData(0x65);
    LCD_WriteData(0x00);
    LCD_WriteData(0x76);
    LCD_WriteData(0x00);
    LCD_WriteData(0x88);
    LCD_WriteData(0x00);

    LCD_WriteReg(0xC6);
    LCD_WriteData(0x20);
    LCD_WriteData(0x00);
    LCD_WriteData(0x17);
    LCD_WriteData(0x00);
    LCD_WriteData(0x01);
    LCD_WriteData(0x00);

    LCD_WriteReg(0xC7);
    LCD_WriteData(0x00);
    LCD_WriteData(0x00);
    LCD_WriteData(0x00);
    LCD_WriteData(0x00);

    LCD_WriteReg(0xC8);
    LCD_WriteData(0x00);
    LCD_WriteData(0x00);
    LCD_WriteData(0x00);
    LCD_WriteData(0x00);

    LCD_WriteReg(0xC9);
    LCD_WriteData(0x00);
    LCD_WriteData(0x00);
    LCD_WriteData(0x00);
    LCD_WriteData(0x00);
    LCD_WriteData(0x00);
    LCD_WriteData(0x00);
    LCD_WriteData(0x00);
    LCD_WriteData(0x00);
    LCD_WriteData(0x00);
    LCD_WriteData(0x00);
    LCD_WriteData(0x00);
    LCD_WriteData(0x00);
    LCD_WriteData(0x00);
    LCD_WriteData(0x00);
    LCD_WriteData(0x00);
    LCD_WriteData(0x00);

    LCD_WriteReg(0xE0);
    LCD_WriteData(0x16);
    LCD_WriteData(0x00);
    LCD_WriteData(0x1C);
    LCD_WriteData(0x00);
    LCD_WriteData(0x21);
    LCD_WriteData(0x00);
    LCD_WriteData(0x36);
    LCD_WriteData(0x00);
    LCD_WriteData(0x46);
    LCD_WriteData(0x00);
    LCD_WriteData(0x52);
    LCD_WriteData(0x00);
    LCD_WriteData(0x64);
    LCD_WriteData(0x00);
    LCD_WriteData(0x7A);
    LCD_WriteData(0x00);
    LCD_WriteData(0x8B);
    LCD_WriteData(0x00);
    LCD_WriteData(0x99);
    LCD_WriteData(0x00);
    LCD_WriteData(0xA8);
    LCD_WriteData(0x00);
    LCD_WriteData(0xB9);
    LCD_WriteData(0x00);
    LCD_WriteData(0xC4);
    LCD_WriteData(0x00);
    LCD_WriteData(0xCA);
    LCD_WriteData(0x00);
    LCD_WriteData(0xD2);
    LCD_WriteData(0x00);
    LCD_WriteData(0xD9);
    LCD_WriteData(0x00);
    LCD_WriteData(0xE0);
    LCD_WriteData(0x00);
    LCD_WriteData(0xF3);
    LCD_WriteData(0x00);

    LCD_WriteReg(0xE1);
    LCD_WriteData(0x16);
    LCD_WriteData(0x00);
    LCD_WriteData(0x1C);
    LCD_WriteData(0x00);
    LCD_WriteData(0x22);
    LCD_WriteData(0x00);
    LCD_WriteData(0x36);
    LCD_WriteData(0x00);
    LCD_WriteData(0x45);
    LCD_WriteData(0x00);
    LCD_WriteData(0x52);
    LCD_WriteData(0x00);
    LCD_WriteData(0x64);
    LCD_WriteData(0x00);
    LCD_WriteData(0x7A);
    LCD_WriteData(0x00);
    LCD_WriteData(0x8B);
    LCD_WriteData(0x00);
    LCD_WriteData(0x99);
    LCD_WriteData(0x00);
    LCD_WriteData(0xA8);
    LCD_WriteData(0x00);
    LCD_WriteData(0xB9);
    LCD_WriteData(0x00);
    LCD_WriteData(0xC4);
    LCD_WriteData(0x00);
    LCD_WriteData(0xCA);
    LCD_WriteData(0x00);
    LCD_WriteData(0xD2);
    LCD_WriteData(0x00);
    LCD_WriteData(0xD8);
    LCD_WriteData(0x00);
    LCD_WriteData(0xE0);
    LCD_WriteData(0x00);
    LCD_WriteData(0xF3);
    LCD_WriteData(0x00);

    LCD_WriteReg(0xE2);
    LCD_WriteData(0x05);
    LCD_WriteData(0x00);
    LCD_WriteData(0x0B);
    LCD_WriteData(0x00);
    LCD_WriteData(0x1B);
    LCD_WriteData(0x00);
    LCD_WriteData(0x34);
    LCD_WriteData(0x00);
    LCD_WriteData(0x44);
    LCD_WriteData(0x00);
    LCD_WriteData(0x4F);
    LCD_WriteData(0x00);
    LCD_WriteData(0x61);
    LCD_WriteData(0x00);
    LCD_WriteData(0x79);
    LCD_WriteData(0x00);
    LCD_WriteData(0x88);
    LCD_WriteData(0x00);
    LCD_WriteData(0x97);
    LCD_WriteData(0x00);
    LCD_WriteData(0xA6);
    LCD_WriteData(0x00);
    LCD_WriteData(0xB7);
    LCD_WriteData(0x00);
    LCD_WriteData(0xC2);
    LCD_WriteData(0x00);
    LCD_WriteData(0xC7);
    LCD_WriteData(0x00);
    LCD_WriteData(0xD1);
    LCD_WriteData(0x00);
    LCD_WriteData(0xD6);
    LCD_WriteData(0x00);
    LCD_WriteData(0xDD);
    LCD_WriteData(0x00);
    LCD_WriteData(0xF3);
    LCD_WriteData(0x00);
    LCD_WriteReg(0xE3);
    LCD_WriteData(0x05);
    LCD_WriteData(0x00);
    LCD_WriteData(0xA);
    LCD_WriteData(0x00);
    LCD_WriteData(0x1C);
    LCD_WriteData(0x00);
    LCD_WriteData(0x33);
    LCD_WriteData(0x00);
    LCD_WriteData(0x44);
    LCD_WriteData(0x00);
    LCD_WriteData(0x50);
    LCD_WriteData(0x00);
    LCD_WriteData(0x62);
    LCD_WriteData(0x00);
    LCD_WriteData(0x78);
    LCD_WriteData(0x00);
    LCD_WriteData(0x88);
    LCD_WriteData(0x00);
    LCD_WriteData(0x97);
    LCD_WriteData(0x00);
    LCD_WriteData(0xA6);
    LCD_WriteData(0x00);
    LCD_WriteData(0xB7);
    LCD_WriteData(0x00);
    LCD_WriteData(0xC2);
    LCD_WriteData(0x00);
    LCD_WriteData(0xC7);
    LCD_WriteData(0x00);
    LCD_WriteData(0xD1);
    LCD_WriteData(0x00);
    LCD_WriteData(0xD5);
    LCD_WriteData(0x00);
    LCD_WriteData(0xDD);
    LCD_WriteData(0x00);
    LCD_WriteData(0xF3);
    LCD_WriteData(0x00);

    LCD_WriteReg(0xE4);
    LCD_WriteData(0x01);
    LCD_WriteData(0x00);
    LCD_WriteData(0x01);
    LCD_WriteData(0x00);
    LCD_WriteData(0x02);
    LCD_WriteData(0x00);
    LCD_WriteData(0x2A);
    LCD_WriteData(0x00);
    LCD_WriteData(0x3C);
    LCD_WriteData(0x00);
    LCD_WriteData(0x4B);
    LCD_WriteData(0x00);
    LCD_WriteData(0x5D);
    LCD_WriteData(0x00);
    LCD_WriteData(0x74);
    LCD_WriteData(0x00);
    LCD_WriteData(0x84);
    LCD_WriteData(0x00);
    LCD_WriteData(0x93);
    LCD_WriteData(0x00);
    LCD_WriteData(0xA2);
    LCD_WriteData(0x00);
    LCD_WriteData(0xB3);
    LCD_WriteData(0x00);
    LCD_WriteData(0xBE);
    LCD_WriteData(0x00);
    LCD_WriteData(0xC4);
    LCD_WriteData(0x00);
    LCD_WriteData(0xCD);
    LCD_WriteData(0x00);
    LCD_WriteData(0xD3);
    LCD_WriteData(0x00);
    LCD_WriteData(0xDD);
    LCD_WriteData(0x00);
    LCD_WriteData(0xF3);
    LCD_WriteData(0x00);
    LCD_WriteReg(0xE5);
    LCD_WriteData(0x00);
    LCD_WriteData(0x00);
    LCD_WriteData(0x00);
    LCD_WriteData(0x00);
    LCD_WriteData(0x02);
    LCD_WriteData(0x00);
    LCD_WriteData(0x29);
    LCD_WriteData(0x00);
    LCD_WriteData(0x3C);
    LCD_WriteData(0x00);
    LCD_WriteData(0x4B);
    LCD_WriteData(0x00);
    LCD_WriteData(0x5D);
    LCD_WriteData(0x00);
    LCD_WriteData(0x74);
    LCD_WriteData(0x00);
    LCD_WriteData(0x84);
    LCD_WriteData(0x00);
    LCD_WriteData(0x93);
    LCD_WriteData(0x00);
    LCD_WriteData(0xA2);
    LCD_WriteData(0x00);
    LCD_WriteData(0xB3);
    LCD_WriteData(0x00);
    LCD_WriteData(0xBE);
    LCD_WriteData(0x00);
    LCD_WriteData(0xC4);
    LCD_WriteData(0x00);
    LCD_WriteData(0xCD);
    LCD_WriteData(0x00);
    LCD_WriteData(0xD3);
    LCD_WriteData(0x00);
    LCD_WriteData(0xDC);
    LCD_WriteData(0x00);
    LCD_WriteData(0xF3);
    LCD_WriteData(0x00);

    LCD_WriteReg(0xE6);
    LCD_WriteData(0x11);
    LCD_WriteData(0x00);
    LCD_WriteData(0x34);
    LCD_WriteData(0x00);
    LCD_WriteData(0x56);
    LCD_WriteData(0x00);
    LCD_WriteData(0x76);
    LCD_WriteData(0x00);
    LCD_WriteData(0x77);
    LCD_WriteData(0x00);
    LCD_WriteData(0x66);
    LCD_WriteData(0x00);
    LCD_WriteData(0x88);
    LCD_WriteData(0x00);
    LCD_WriteData(0x99);
    LCD_WriteData(0x00);
    LCD_WriteData(0xBB);
    LCD_WriteData(0x00);
    LCD_WriteData(0x99);
    LCD_WriteData(0x00);
    LCD_WriteData(0x66);
    LCD_WriteData(0x00);
    LCD_WriteData(0x55);
    LCD_WriteData(0x00);
    LCD_WriteData(0x55);
    LCD_WriteData(0x00);
    LCD_WriteData(0x45);
    LCD_WriteData(0x00);
    LCD_WriteData(0x43);
    LCD_WriteData(0x00);
    LCD_WriteData(0x44);
    LCD_WriteData(0x00);

    LCD_WriteReg(0xE7);
    LCD_WriteData(0x32);
    LCD_WriteData(0x00);
    LCD_WriteData(0x55);
    LCD_WriteData(0x00);
    LCD_WriteData(0x76);
    LCD_WriteData(0x00);
    LCD_WriteData(0x66);
    LCD_WriteData(0x00);
    LCD_WriteData(0x67);
    LCD_WriteData(0x00);
    LCD_WriteData(0x67);
    LCD_WriteData(0x00);
    LCD_WriteData(0x87);
    LCD_WriteData(0x00);
    LCD_WriteData(0x99);
    LCD_WriteData(0x00);
    LCD_WriteData(0xBB);
    LCD_WriteData(0x00);
    LCD_WriteData(0x99);
    LCD_WriteData(0x00);
    LCD_WriteData(0x77);
    LCD_WriteData(0x00);
    LCD_WriteData(0x44);
    LCD_WriteData(0x00);
    LCD_WriteData(0x56);
    LCD_WriteData(0x00);
    LCD_WriteData(0x23);
    LCD_WriteData(0x00);
    LCD_WriteData(0x33);
    LCD_WriteData(0x00);
    LCD_WriteData(0x45);
    LCD_WriteData(0x00);

    LCD_WriteReg(0xE8);
    LCD_WriteData(0x00);
    LCD_WriteData(0x00);
    LCD_WriteData(0x99);
    LCD_WriteData(0x00);
    LCD_WriteData(0x87);
    LCD_WriteData(0x00);
    LCD_WriteData(0x88);
    LCD_WriteData(0x00);
    LCD_WriteData(0x77);
    LCD_WriteData(0x00);
    LCD_WriteData(0x66);
    LCD_WriteData(0x00);
    LCD_WriteData(0x88);
    LCD_WriteData(0x00);
    LCD_WriteData(0xAA);
    LCD_WriteData(0x00);
    LCD_WriteData(0xBB);
    LCD_WriteData(0x00);
    LCD_WriteData(0x99);
    LCD_WriteData(0x00);
    LCD_WriteData(0x66);
    LCD_WriteData(0x00);
    LCD_WriteData(0x55);
    LCD_WriteData(0x00);
    LCD_WriteData(0x55);
    LCD_WriteData(0x00);
    LCD_WriteData(0x44);
    LCD_WriteData(0x00);
    LCD_WriteData(0x44);
    LCD_WriteData(0x00);
    LCD_WriteData(0x55);
    LCD_WriteData(0x00);

    LCD_WriteReg(0xE9);
    LCD_WriteData(0xAA);
    LCD_WriteData(0x00);
    LCD_WriteData(0x00);
    LCD_WriteData(0x00);

    LCD_WriteReg(0x00);
    LCD_WriteData(0xAA);

    LCD_WriteReg(0xCF);
    LCD_WriteData(0x00);
    LCD_WriteData(0x00);
    LCD_WriteData(0x00);
    LCD_WriteData(0x00);
    LCD_WriteData(0x00);
    LCD_WriteData(0x00);
    LCD_WriteData(0x00);
    LCD_WriteData(0x00);
    LCD_WriteData(0x00);
    LCD_WriteData(0x00);
    LCD_WriteData(0x00);
    LCD_WriteData(0x00);
    LCD_WriteData(0x00);
    LCD_WriteData(0x00);
    LCD_WriteData(0x00);
    LCD_WriteData(0x00);
    LCD_WriteData(0x00);

    LCD_WriteReg(0xF0);
    LCD_WriteData(0x00);
    LCD_WriteData(0x50);
    LCD_WriteData(0x00);
    LCD_WriteData(0x00);
    LCD_WriteData(0x00);

    LCD_WriteReg(0xF3);
    LCD_WriteData(0x00);

    LCD_WriteReg(0xF9);
    LCD_WriteData(0x06);
    LCD_WriteData(0x10);
    LCD_WriteData(0x29);
    LCD_WriteData(0x00);

    LCD_WriteReg(0x3A);
    LCD_WriteData(0x55);  /* 66 */

    LCD_WriteReg(0x11);
    delay_ms(100);
    LCD_WriteReg(0x29);
    LCD_WriteReg(0x35);
    LCD_WriteData(0x00);

    LCD_WriteReg(0x51);
    LCD_WriteData(0xFF);
    LCD_WriteReg(0x53);
    LCD_WriteData(0x2C);
    LCD_WriteReg(0x55);
    LCD_WriteData(0x82);
    LCD_WriteReg(0x2c);
}

/**
 * @brief       ST7796寄存器初始化代码 
 * @param       无
 * @retval      无
 */
void lcd_ex_st7796_reginit(void)
{
    LCD_WriteReg(0x11);

    delay_ms(120); 

    LCD_WriteReg(0x36); /* Memory Data Access Control MY,MX~~ */
    LCD_WriteData(0x48);
    
    LCD_WriteReg(0x3A);
    LCD_WriteData(0x55);
    
    LCD_WriteReg(0xF0);
    LCD_WriteData(0xC3);
    
    LCD_WriteReg(0xF0);
    LCD_WriteData(0x96);

    LCD_WriteReg(0xB4);
    LCD_WriteData(0x01);
    
    LCD_WriteReg(0xB6); /* Display Function Control */
    LCD_WriteData(0x0A);
    LCD_WriteData(0xA2);

    LCD_WriteReg(0xB7);
    LCD_WriteData(0xC6);

    LCD_WriteReg(0xB9);
    LCD_WriteData(0x02);
    LCD_WriteData(0xE0);

    LCD_WriteReg(0xC0);
    LCD_WriteData(0x80);
    LCD_WriteData(0x16);

    LCD_WriteReg(0xC1);
    LCD_WriteData(0x19);

    LCD_WriteReg(0xC2);
    LCD_WriteData(0xA7);

    LCD_WriteReg(0xC5);
    LCD_WriteData(0x16);   

    LCD_WriteReg(0xE8);
    LCD_WriteData(0x40);
    LCD_WriteData(0x8A);
    LCD_WriteData(0x00);
    LCD_WriteData(0x00);
    LCD_WriteData(0x29);
    LCD_WriteData(0x19);
    LCD_WriteData(0xA5);
    LCD_WriteData(0x33);

    LCD_WriteReg(0xE0);
    LCD_WriteData(0xF0);
    LCD_WriteData(0x07);
    LCD_WriteData(0x0D);
    LCD_WriteData(0x04);
    LCD_WriteData(0x05);
    LCD_WriteData(0x14);
    LCD_WriteData(0x36);
    LCD_WriteData(0x54);
    LCD_WriteData(0x4C);
    LCD_WriteData(0x38);
    LCD_WriteData(0x13);
    LCD_WriteData(0x14);
    LCD_WriteData(0x2E);
    LCD_WriteData(0x34);

    LCD_WriteReg(0xE1);
    LCD_WriteData(0xF0);
    LCD_WriteData(0x10);
    LCD_WriteData(0x14);
    LCD_WriteData(0x0E);
    LCD_WriteData(0x0C);
    LCD_WriteData(0x08);
    LCD_WriteData(0x35);
    LCD_WriteData(0x44);
    LCD_WriteData(0x4C);
    LCD_WriteData(0x26);
    LCD_WriteData(0x10);
    LCD_WriteData(0x12);
    LCD_WriteData(0x2C);
    LCD_WriteData(0x32);

    LCD_WriteReg(0xF0);
    LCD_WriteData(0x3C);

    LCD_WriteReg(0xF0);
    LCD_WriteData(0x69);

    delay_ms(120);

    LCD_WriteReg(0x21);
    LCD_WriteReg(0x29);
}

/**
 * @brief       NT35510寄存器初始化代码 
 * @param       无
 * @retval      无
 */
void lcd_ex_nt35510_reginit(void)
{
    LCD_Write(0xF000, 0x55);
    LCD_Write(0xF001, 0xAA);
    LCD_Write(0xF002, 0x52);
    LCD_Write(0xF003, 0x08);
    LCD_Write(0xF004, 0x01);
    /* AVDD Set AVDD 5.2V */
    LCD_Write(0xB000, 0x0D);
    LCD_Write(0xB001, 0x0D);
    LCD_Write(0xB002, 0x0D);
    /* AVDD ratio */
    LCD_Write(0xB600, 0x34);
    LCD_Write(0xB601, 0x34);
    LCD_Write(0xB602, 0x34);
    /* AVEE -5.2V */
    LCD_Write(0xB100, 0x0D);
    LCD_Write(0xB101, 0x0D);
    LCD_Write(0xB102, 0x0D);
    /* AVEE ratio */
    LCD_Write(0xB700, 0x34);
    LCD_Write(0xB701, 0x34);
    LCD_Write(0xB702, 0x34);
    /* VCL -2.5V */
    LCD_Write(0xB200, 0x00);
    LCD_Write(0xB201, 0x00);
    LCD_Write(0xB202, 0x00);
    /* VCL ratio */
    LCD_Write(0xB800, 0x24);
    LCD_Write(0xB801, 0x24);
    LCD_Write(0xB802, 0x24);
    /* VGH 15V (Free pump) */
    LCD_Write(0xBF00, 0x01);
    LCD_Write(0xB300, 0x0F);
    LCD_Write(0xB301, 0x0F);
    LCD_Write(0xB302, 0x0F);
    /* VGH ratio */
    LCD_Write(0xB900, 0x34);
    LCD_Write(0xB901, 0x34);
    LCD_Write(0xB902, 0x34);
    /* VGL_REG -10V */
    LCD_Write(0xB500, 0x08);
    LCD_Write(0xB501, 0x08);
    LCD_Write(0xB502, 0x08);
    LCD_Write(0xC200, 0x03);
    /* VGLX ratio */
    LCD_Write(0xBA00, 0x24);
    LCD_Write(0xBA01, 0x24);
    LCD_Write(0xBA02, 0x24);
    /* VGMP/VGSP 4.5V/0V */
    LCD_Write(0xBC00, 0x00);
    LCD_Write(0xBC01, 0x78);
    LCD_Write(0xBC02, 0x00);
    /* VGMN/VGSN -4.5V/0V */
    LCD_Write(0xBD00, 0x00);
    LCD_Write(0xBD01, 0x78);
    LCD_Write(0xBD02, 0x00);
    /* VCOM */
    LCD_Write(0xBE00, 0x00);
    LCD_Write(0xBE01, 0x64);
    /* Gamma Setting */
    LCD_Write(0xD100, 0x00);
    LCD_Write(0xD101, 0x33);
    LCD_Write(0xD102, 0x00);
    LCD_Write(0xD103, 0x34);
    LCD_Write(0xD104, 0x00);
    LCD_Write(0xD105, 0x3A);
    LCD_Write(0xD106, 0x00);
    LCD_Write(0xD107, 0x4A);
    LCD_Write(0xD108, 0x00);
    LCD_Write(0xD109, 0x5C);
    LCD_Write(0xD10A, 0x00);
    LCD_Write(0xD10B, 0x81);
    LCD_Write(0xD10C, 0x00);
    LCD_Write(0xD10D, 0xA6);
    LCD_Write(0xD10E, 0x00);
    LCD_Write(0xD10F, 0xE5);
    LCD_Write(0xD110, 0x01);
    LCD_Write(0xD111, 0x13);
    LCD_Write(0xD112, 0x01);
    LCD_Write(0xD113, 0x54);
    LCD_Write(0xD114, 0x01);
    LCD_Write(0xD115, 0x82);
    LCD_Write(0xD116, 0x01);
    LCD_Write(0xD117, 0xCA);
    LCD_Write(0xD118, 0x02);
    LCD_Write(0xD119, 0x00);
    LCD_Write(0xD11A, 0x02);
    LCD_Write(0xD11B, 0x01);
    LCD_Write(0xD11C, 0x02);
    LCD_Write(0xD11D, 0x34);
    LCD_Write(0xD11E, 0x02);
    LCD_Write(0xD11F, 0x67);
    LCD_Write(0xD120, 0x02);
    LCD_Write(0xD121, 0x84);
    LCD_Write(0xD122, 0x02);
    LCD_Write(0xD123, 0xA4);
    LCD_Write(0xD124, 0x02);
    LCD_Write(0xD125, 0xB7);
    LCD_Write(0xD126, 0x02);
    LCD_Write(0xD127, 0xCF);
    LCD_Write(0xD128, 0x02);
    LCD_Write(0xD129, 0xDE);
    LCD_Write(0xD12A, 0x02);
    LCD_Write(0xD12B, 0xF2);
    LCD_Write(0xD12C, 0x02);
    LCD_Write(0xD12D, 0xFE);
    LCD_Write(0xD12E, 0x03);
    LCD_Write(0xD12F, 0x10);
    LCD_Write(0xD130, 0x03);
    LCD_Write(0xD131, 0x33);
    LCD_Write(0xD132, 0x03);
    LCD_Write(0xD133, 0x6D);
    LCD_Write(0xD200, 0x00);
    LCD_Write(0xD201, 0x33);
    LCD_Write(0xD202, 0x00);
    LCD_Write(0xD203, 0x34);
    LCD_Write(0xD204, 0x00);
    LCD_Write(0xD205, 0x3A);
    LCD_Write(0xD206, 0x00);
    LCD_Write(0xD207, 0x4A);
    LCD_Write(0xD208, 0x00);
    LCD_Write(0xD209, 0x5C);
    LCD_Write(0xD20A, 0x00);

    LCD_Write(0xD20B, 0x81);
    LCD_Write(0xD20C, 0x00);
    LCD_Write(0xD20D, 0xA6);
    LCD_Write(0xD20E, 0x00);
    LCD_Write(0xD20F, 0xE5);
    LCD_Write(0xD210, 0x01);
    LCD_Write(0xD211, 0x13);
    LCD_Write(0xD212, 0x01);
    LCD_Write(0xD213, 0x54);
    LCD_Write(0xD214, 0x01);
    LCD_Write(0xD215, 0x82);
    LCD_Write(0xD216, 0x01);
    LCD_Write(0xD217, 0xCA);
    LCD_Write(0xD218, 0x02);
    LCD_Write(0xD219, 0x00);
    LCD_Write(0xD21A, 0x02);
    LCD_Write(0xD21B, 0x01);
    LCD_Write(0xD21C, 0x02);
    LCD_Write(0xD21D, 0x34);
    LCD_Write(0xD21E, 0x02);
    LCD_Write(0xD21F, 0x67);
    LCD_Write(0xD220, 0x02);
    LCD_Write(0xD221, 0x84);
    LCD_Write(0xD222, 0x02);
    LCD_Write(0xD223, 0xA4);
    LCD_Write(0xD224, 0x02);
    LCD_Write(0xD225, 0xB7);
    LCD_Write(0xD226, 0x02);
    LCD_Write(0xD227, 0xCF);
    LCD_Write(0xD228, 0x02);
    LCD_Write(0xD229, 0xDE);
    LCD_Write(0xD22A, 0x02);
    LCD_Write(0xD22B, 0xF2);
    LCD_Write(0xD22C, 0x02);
    LCD_Write(0xD22D, 0xFE);
    LCD_Write(0xD22E, 0x03);
    LCD_Write(0xD22F, 0x10);
    LCD_Write(0xD230, 0x03);
    LCD_Write(0xD231, 0x33);
    LCD_Write(0xD232, 0x03);
    LCD_Write(0xD233, 0x6D);
    LCD_Write(0xD300, 0x00);
    LCD_Write(0xD301, 0x33);
    LCD_Write(0xD302, 0x00);
    LCD_Write(0xD303, 0x34);
    LCD_Write(0xD304, 0x00);
    LCD_Write(0xD305, 0x3A);
    LCD_Write(0xD306, 0x00);
    LCD_Write(0xD307, 0x4A);
    LCD_Write(0xD308, 0x00);
    LCD_Write(0xD309, 0x5C);
    LCD_Write(0xD30A, 0x00);

    LCD_Write(0xD30B, 0x81);
    LCD_Write(0xD30C, 0x00);
    LCD_Write(0xD30D, 0xA6);
    LCD_Write(0xD30E, 0x00);
    LCD_Write(0xD30F, 0xE5);
    LCD_Write(0xD310, 0x01);
    LCD_Write(0xD311, 0x13);
    LCD_Write(0xD312, 0x01);
    LCD_Write(0xD313, 0x54);
    LCD_Write(0xD314, 0x01);
    LCD_Write(0xD315, 0x82);
    LCD_Write(0xD316, 0x01);
    LCD_Write(0xD317, 0xCA);
    LCD_Write(0xD318, 0x02);
    LCD_Write(0xD319, 0x00);
    LCD_Write(0xD31A, 0x02);
    LCD_Write(0xD31B, 0x01);
    LCD_Write(0xD31C, 0x02);
    LCD_Write(0xD31D, 0x34);
    LCD_Write(0xD31E, 0x02);
    LCD_Write(0xD31F, 0x67);
    LCD_Write(0xD320, 0x02);
    LCD_Write(0xD321, 0x84);
    LCD_Write(0xD322, 0x02);
    LCD_Write(0xD323, 0xA4);
    LCD_Write(0xD324, 0x02);
    LCD_Write(0xD325, 0xB7);
    LCD_Write(0xD326, 0x02);
    LCD_Write(0xD327, 0xCF);
    LCD_Write(0xD328, 0x02);
    LCD_Write(0xD329, 0xDE);
    LCD_Write(0xD32A, 0x02);
    LCD_Write(0xD32B, 0xF2);
    LCD_Write(0xD32C, 0x02);
    LCD_Write(0xD32D, 0xFE);
    LCD_Write(0xD32E, 0x03);
    LCD_Write(0xD32F, 0x10);
    LCD_Write(0xD330, 0x03);
    LCD_Write(0xD331, 0x33);
    LCD_Write(0xD332, 0x03);
    LCD_Write(0xD333, 0x6D);
    LCD_Write(0xD400, 0x00);
    LCD_Write(0xD401, 0x33);
    LCD_Write(0xD402, 0x00);
    LCD_Write(0xD403, 0x34);
    LCD_Write(0xD404, 0x00);
    LCD_Write(0xD405, 0x3A);
    LCD_Write(0xD406, 0x00);
    LCD_Write(0xD407, 0x4A);
    LCD_Write(0xD408, 0x00);
    LCD_Write(0xD409, 0x5C);
    LCD_Write(0xD40A, 0x00);
    LCD_Write(0xD40B, 0x81);

    LCD_Write(0xD40C, 0x00);
    LCD_Write(0xD40D, 0xA6);
    LCD_Write(0xD40E, 0x00);
    LCD_Write(0xD40F, 0xE5);
    LCD_Write(0xD410, 0x01);
    LCD_Write(0xD411, 0x13);
    LCD_Write(0xD412, 0x01);
    LCD_Write(0xD413, 0x54);
    LCD_Write(0xD414, 0x01);
    LCD_Write(0xD415, 0x82);
    LCD_Write(0xD416, 0x01);
    LCD_Write(0xD417, 0xCA);
    LCD_Write(0xD418, 0x02);
    LCD_Write(0xD419, 0x00);
    LCD_Write(0xD41A, 0x02);
    LCD_Write(0xD41B, 0x01);
    LCD_Write(0xD41C, 0x02);
    LCD_Write(0xD41D, 0x34);
    LCD_Write(0xD41E, 0x02);
    LCD_Write(0xD41F, 0x67);
    LCD_Write(0xD420, 0x02);
    LCD_Write(0xD421, 0x84);
    LCD_Write(0xD422, 0x02);
    LCD_Write(0xD423, 0xA4);
    LCD_Write(0xD424, 0x02);
    LCD_Write(0xD425, 0xB7);
    LCD_Write(0xD426, 0x02);
    LCD_Write(0xD427, 0xCF);
    LCD_Write(0xD428, 0x02);
    LCD_Write(0xD429, 0xDE);
    LCD_Write(0xD42A, 0x02);
    LCD_Write(0xD42B, 0xF2);
    LCD_Write(0xD42C, 0x02);
    LCD_Write(0xD42D, 0xFE);
    LCD_Write(0xD42E, 0x03);
    LCD_Write(0xD42F, 0x10);
    LCD_Write(0xD430, 0x03);
    LCD_Write(0xD431, 0x33);
    LCD_Write(0xD432, 0x03);
    LCD_Write(0xD433, 0x6D);
    LCD_Write(0xD500, 0x00);
    LCD_Write(0xD501, 0x33);
    LCD_Write(0xD502, 0x00);
    LCD_Write(0xD503, 0x34);
    LCD_Write(0xD504, 0x00);
    LCD_Write(0xD505, 0x3A);
    LCD_Write(0xD506, 0x00);
    LCD_Write(0xD507, 0x4A);
    LCD_Write(0xD508, 0x00);
    LCD_Write(0xD509, 0x5C);
    LCD_Write(0xD50A, 0x00);
    LCD_Write(0xD50B, 0x81);

    LCD_Write(0xD50C, 0x00);
    LCD_Write(0xD50D, 0xA6);
    LCD_Write(0xD50E, 0x00);
    LCD_Write(0xD50F, 0xE5);
    LCD_Write(0xD510, 0x01);
    LCD_Write(0xD511, 0x13);
    LCD_Write(0xD512, 0x01);
    LCD_Write(0xD513, 0x54);
    LCD_Write(0xD514, 0x01);
    LCD_Write(0xD515, 0x82);
    LCD_Write(0xD516, 0x01);
    LCD_Write(0xD517, 0xCA);
    LCD_Write(0xD518, 0x02);
    LCD_Write(0xD519, 0x00);
    LCD_Write(0xD51A, 0x02);
    LCD_Write(0xD51B, 0x01);
    LCD_Write(0xD51C, 0x02);
    LCD_Write(0xD51D, 0x34);
    LCD_Write(0xD51E, 0x02);
    LCD_Write(0xD51F, 0x67);
    LCD_Write(0xD520, 0x02);
    LCD_Write(0xD521, 0x84);
    LCD_Write(0xD522, 0x02);
    LCD_Write(0xD523, 0xA4);
    LCD_Write(0xD524, 0x02);
    LCD_Write(0xD525, 0xB7);
    LCD_Write(0xD526, 0x02);
    LCD_Write(0xD527, 0xCF);
    LCD_Write(0xD528, 0x02);
    LCD_Write(0xD529, 0xDE);
    LCD_Write(0xD52A, 0x02);
    LCD_Write(0xD52B, 0xF2);
    LCD_Write(0xD52C, 0x02);
    LCD_Write(0xD52D, 0xFE);
    LCD_Write(0xD52E, 0x03);
    LCD_Write(0xD52F, 0x10);
    LCD_Write(0xD530, 0x03);
    LCD_Write(0xD531, 0x33);
    LCD_Write(0xD532, 0x03);
    LCD_Write(0xD533, 0x6D);
    LCD_Write(0xD600, 0x00);
    LCD_Write(0xD601, 0x33);
    LCD_Write(0xD602, 0x00);
    LCD_Write(0xD603, 0x34);
    LCD_Write(0xD604, 0x00);
    LCD_Write(0xD605, 0x3A);
    LCD_Write(0xD606, 0x00);
    LCD_Write(0xD607, 0x4A);
    LCD_Write(0xD608, 0x00);
    LCD_Write(0xD609, 0x5C);
    LCD_Write(0xD60A, 0x00);
    LCD_Write(0xD60B, 0x81);

    LCD_Write(0xD60C, 0x00);
    LCD_Write(0xD60D, 0xA6);
    LCD_Write(0xD60E, 0x00);
    LCD_Write(0xD60F, 0xE5);
    LCD_Write(0xD610, 0x01);
    LCD_Write(0xD611, 0x13);
    LCD_Write(0xD612, 0x01);
    LCD_Write(0xD613, 0x54);
    LCD_Write(0xD614, 0x01);
    LCD_Write(0xD615, 0x82);
    LCD_Write(0xD616, 0x01);
    LCD_Write(0xD617, 0xCA);
    LCD_Write(0xD618, 0x02);
    LCD_Write(0xD619, 0x00);
    LCD_Write(0xD61A, 0x02);
    LCD_Write(0xD61B, 0x01);
    LCD_Write(0xD61C, 0x02);
    LCD_Write(0xD61D, 0x34);
    LCD_Write(0xD61E, 0x02);
    LCD_Write(0xD61F, 0x67);
    LCD_Write(0xD620, 0x02);
    LCD_Write(0xD621, 0x84);
    LCD_Write(0xD622, 0x02);
    LCD_Write(0xD623, 0xA4);
    LCD_Write(0xD624, 0x02);
    LCD_Write(0xD625, 0xB7);
    LCD_Write(0xD626, 0x02);
    LCD_Write(0xD627, 0xCF);
    LCD_Write(0xD628, 0x02);
    LCD_Write(0xD629, 0xDE);
    LCD_Write(0xD62A, 0x02);
    LCD_Write(0xD62B, 0xF2);
    LCD_Write(0xD62C, 0x02);
    LCD_Write(0xD62D, 0xFE);
    LCD_Write(0xD62E, 0x03);
    LCD_Write(0xD62F, 0x10);
    LCD_Write(0xD630, 0x03);
    LCD_Write(0xD631, 0x33);
    LCD_Write(0xD632, 0x03);
    LCD_Write(0xD633, 0x6D);
    /* LV2 Page 0 enable */
    LCD_Write(0xF000, 0x55);
    LCD_Write(0xF001, 0xAA);
    LCD_Write(0xF002, 0x52);
    LCD_Write(0xF003, 0x08);
    LCD_Write(0xF004, 0x00);
    /* Display control */
    LCD_Write(0xB100, 0xCC);
    LCD_Write(0xB101, 0x00);
    /* Source hold time */
    LCD_Write(0xB600, 0x05);
    /* Gate EQ control */
    LCD_Write(0xB700, 0x70);
    LCD_Write(0xB701, 0x70);
    /* Source EQ control (Mode 2) */
    LCD_Write(0xB800, 0x01);
    LCD_Write(0xB801, 0x03);
    LCD_Write(0xB802, 0x03);
    LCD_Write(0xB803, 0x03);
    /* Inversion mode (2-dot) */
    LCD_Write(0xBC00, 0x02);
    LCD_Write(0xBC01, 0x00);
    LCD_Write(0xBC02, 0x00);
    /* Timing control 4H w/ 4-delay */
    LCD_Write(0xC900, 0xD0);
    LCD_Write(0xC901, 0x02);
    LCD_Write(0xC902, 0x50);
    LCD_Write(0xC903, 0x50);
    LCD_Write(0xC904, 0x50);
    LCD_Write(0x3500, 0x00);
    LCD_Write(0x3A00, 0x55); /* 16-bit/pixel */
    LCD_WriteReg(0x1100);
    delay_us(120);
    LCD_WriteReg(0x2900);
}












