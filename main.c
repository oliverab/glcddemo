/*
 * File:   main.c
 * Author: Oliver
 *
 * Created on 16 December 2018, 13:17
 */


#include <xc.h>
#include <stdint.h>

#include "glcd128x64/glcd_lib.h"
#include "lgfont.h"
#include "glcd_hardware.h"
#define glcd_cs1 0b00000001
#define glcd_cs2 0b00000010
#define glcd_rs  0b00000100
#define glcd_rw  0b00001000
#define glcd_e   0b00010000
#define glcd_rst 0b00100000

#define glcdc_off 0b00111110
#define glcdc_on 0b00111111
#define glcdc_y  0b01000000
#define glcdc_x  0b10111000
#define glcd_setstart 0b11000000

/* GLCD details for EasyPIC
 * 
 *  1	CS1	RB0	Active low
 *  2	CS2	RB1	Active low
 *  3	GND
 *  4	VCC
 *  5	Vo
 *  6	RS	RB2	0=Control
 *  7	R/W	RB3	0=Write
 *  8	E	RB4	Active High
 *  9	D0	RD0
 *  10	D1	RD1
 *  11	D2	RD2
 *  12	D3	RD3
 *  13	D4	RD4
 *  14	D5	RD5
 *  15	D6	RD6
 *  16	D7	RD7
 *  17	RST	RB5	Active Low
 *  18	VEE	
 *  19	LED+
 *  20	LED-
 */

void glcd_lgtext(uint8_t x,uint8_t y,const char *tx,uint8_t c)
{
    //uint8_t d;
    if (x>0x7f || y>0x3f) return;
    if (x&0x40) {
        glcdcont_set(glcd_cs1);
        glcdcont_unset(glcd_cs2);
    } else {
        glcdcont_unset(glcd_cs1);
        glcdcont_set(glcd_cs2);
    }
    glcdcont_unset(glcd_rs);
    glcddata_write((uint8_t)(glcdc_x|((y>>3)&7)));
    glcdcont_set(glcd_e);
    glcd_wait();
    glcdcont_unset(glcd_e);
    glcd_wait();
    glcddata_write((uint8_t)(glcdc_y|(x&0x3f)));
    glcdcont_set(glcd_e);
    glcd_wait();
    glcdcont_unset(glcd_e);
    glcd_wait();

    glcdcont_set(glcd_rs);
    uint8_t xx=x;
    const char* ttx = tx;
    uint8_t cc=c;
    for (;cc>0;cc--)
    {
        uint8_t* addr=&Terminal12x16[(unsigned short) (*ttx-32)*(12*2+1)+1];
        for(uint8_t d=0;d<=11;d++ )
        {
            glcddata_write(*addr);
            addr+=2;
            glcdcont_set(glcd_e);
            glcd_wait();
            glcdcont_unset(glcd_e);
            glcd_wait();
            xx++;
            if (xx==0x40)
            {
                glcdcont_set(glcd_cs1);
                glcdcont_unset(glcd_cs2);
                glcdcont_unset(glcd_rs);
                glcddata_write((uint8_t)(glcdc_x|((y>>3)&7)));
                glcdcont_set(glcd_e);
                glcd_wait();
                glcdcont_unset(glcd_e);
                glcd_wait();
                glcddata_write((uint8_t)(glcdc_y|0x00));
                glcdcont_set(glcd_e);
                glcd_wait();
                glcdcont_unset(glcd_e);
                glcd_wait();
                
                glcdcont_set(glcd_rs);                               
            }
            if (xx==0x80) break;
        }
        if (xx==0x80) break;
        ttx++;
    }
    glcdcont_unset(glcd_rs);
    y+=8;
    if (x>0x7f || y>0x3f) return;
    if (x&0x40) {
        glcdcont_set(glcd_cs1);
        glcdcont_unset(glcd_cs2);
    } else {
        glcdcont_unset(glcd_cs1);
        glcdcont_set(glcd_cs2);
    }
    glcdcont_unset(glcd_rs);
    glcddata_write((uint8_t)(glcdc_x|((y>>3)&7)));
    glcdcont_set(glcd_e);
    glcd_wait();
    glcdcont_unset(glcd_e);
    glcd_wait();
    glcddata_write((uint8_t)(glcdc_y|(x&0x3f)));
    glcdcont_set(glcd_e);
    glcd_wait();
    glcdcont_unset(glcd_e);
    glcd_wait();

    glcdcont_set(glcd_rs);
    xx=x;
    ttx=tx;
    cc=c;

    for (;cc>0;cc--)
    {
        uint8_t* addr=&Terminal12x16[(unsigned short) (*ttx-32)*(12*2+1)+2];
        for(uint8_t d=0;d<=11;d++ )
        {
            glcddata_write(*addr);
            addr+=2;
            glcdcont_set(glcd_e);
            glcd_wait();
            glcdcont_unset(glcd_e);
            glcd_wait();
            xx++;
            if (xx==0x40)
            {
                glcdcont_set(glcd_cs1);
                glcdcont_unset(glcd_cs2);
                glcdcont_unset(glcd_rs);
                glcddata_write((uint8_t)(glcdc_x|((y>>3)&7)));
                glcdcont_set(glcd_e);
                glcd_wait();
                glcdcont_unset(glcd_e);
                glcd_wait();
                glcddata_write((uint8_t)(glcdc_y|0x00));
                glcdcont_set(glcd_e);
                glcd_wait();
                glcdcont_unset(glcd_e);
                glcd_wait();
                
                glcdcont_set(glcd_rs);                               
            }
            if (xx==0x80) break;
        }
        if (xx==0x80) break;
        ttx++;
    }
    glcdcont_unset(glcd_rs);

}
void glcd_adv_lgtext(uint8_t x,uint8_t y,const char *tx,uint8_t c)
{
    uint8_t buf[5];
    //uint8_t d;
    if (x>0x7f || y>0x3f) return;

    for (; c > 0; c--) {
        if ((y & 0b111) && (y<0x38))
        {
        if (x & 0x40) {
            glcdcont_set(glcd_cs1);
            glcdcont_unset(glcd_cs2);
        } else {
            glcdcont_unset(glcd_cs1);
            glcdcont_set(glcd_cs2);
        }
        glcdcont_unset(glcd_rs);
        glcddata_write((uint8_t) (glcdc_x | (((y >> 3)&7))+1));
        glcdcont_set(glcd_e);
        glcd_wait();
        glcdcont_unset(glcd_e);
        glcd_wait();
        glcddata_write((uint8_t) (glcdc_y | (x & 0x3f)));
        glcdcont_set(glcd_e);
        glcd_wait();
        glcdcont_unset(glcd_e);
        glcd_wait();

        glcdcont_set(glcd_rs);
        glcddata_wr_tris(0xff);
        glcdcont_set(glcd_rw);
        glcdcont_set(glcd_e);
        glcd_wait();
        glcdcont_unset(glcd_e);
        glcd_wait();

        for(uint8_t d=0;d<=5;d++ )
        {
            glcdcont_set(glcd_e);
            glcd_wait();
            buf[d]=glcddata_read();
            glcdcont_unset(glcd_e);
            glcd_wait();
            if (x+d==0x3f)
            {
                glcdcont_unset(glcd_rw);
                glcddata_wr_tris(0x00);
                glcdcont_set(glcd_cs1);
                glcdcont_unset(glcd_cs2);
                glcdcont_unset(glcd_rs);
                glcddata_write((uint8_t)(glcdc_x|(((y>>3)&7)+1)));
                glcdcont_set(glcd_e);
                glcd_wait();
                glcdcont_unset(glcd_e);
                glcd_wait();
                glcddata_write((uint8_t)(glcdc_y|0x00));
                glcdcont_set(glcd_e);
                glcd_wait();
                glcdcont_unset(glcd_e);
                glcd_wait();
                
                glcdcont_set(glcd_rs);   
                glcddata_wr_tris(0xff);
                glcdcont_set(glcd_rw);
                glcdcont_set(glcd_e);
                glcd_wait();
                glcdcont_unset(glcd_e);
                glcd_wait();

            }
            if (x+d==0xff) break;
        }
        glcdcont_unset(glcd_rw);
        glcddata_wr_tris(0x00);

        if (x & 0x40) {
            glcdcont_set(glcd_cs1);
            glcdcont_unset(glcd_cs2);
        } else {
            glcdcont_unset(glcd_cs1);
            glcdcont_set(glcd_cs2);
        }
        glcdcont_unset(glcd_rs);
        glcddata_write((uint8_t) (glcdc_x | (((y >> 3)&7)+1)));
        glcdcont_set(glcd_e);
        glcd_wait();
        glcdcont_unset(glcd_e);
        glcd_wait();
        glcddata_write((uint8_t) (glcdc_y | (x & 0x3f)));
        glcdcont_set(glcd_e);
        glcd_wait();
        glcdcont_unset(glcd_e);
        glcd_wait();

        glcdcont_set(glcd_rs);

        for(uint8_t d=0;d<=5;d++ )
        {
            uint8_t yy=8-(y&0b111);
            uint8_t bb=buf[d];
            bb &=~(0xff>>yy);
            bb |=Terminal12x16[(unsigned short) (*tx-32)*6+d]>>yy;
            
            glcddata_write(bb);
            glcdcont_set(glcd_e);
            glcd_wait();
            glcdcont_unset(glcd_e);
            glcd_wait();
            if (x+d==0x3f)
            {
                glcdcont_set(glcd_cs1);
                glcdcont_unset(glcd_cs2);
                glcdcont_unset(glcd_rs);
                glcddata_write((uint8_t)(glcdc_x|(((y>>3)&7)+1)));
                glcdcont_set(glcd_e);
                glcd_wait();
                glcdcont_unset(glcd_e);
                glcd_wait();
                glcddata_write((uint8_t)(glcdc_y|0x00));
                glcdcont_set(glcd_e);
                glcd_wait();
                glcdcont_unset(glcd_e);
                glcd_wait();
                
                glcdcont_set(glcd_rs);                               
            }
            if (x+d==0xff) break;
        }
  
        }
        
        
        
        if (x & 0x40) {
            glcdcont_set(glcd_cs1);
            glcdcont_unset(glcd_cs2);
        } else {
            glcdcont_unset(glcd_cs1);
            glcdcont_set(glcd_cs2);
        }
        glcdcont_unset(glcd_rs);
        glcddata_write((uint8_t) (glcdc_x | ((y >> 3)&7)));
        glcdcont_set(glcd_e);
        glcd_wait();
        glcdcont_unset(glcd_e);
        glcd_wait();
        glcddata_write((uint8_t) (glcdc_y | (x & 0x3f)));
        glcdcont_set(glcd_e);
        glcd_wait();
        glcdcont_unset(glcd_e);
        glcd_wait();

        glcdcont_set(glcd_rs);
        glcddata_wr_tris(0xff);
        glcdcont_set(glcd_rw);
        glcdcont_set(glcd_e);
        glcd_wait();
        glcdcont_unset(glcd_e);
        glcd_wait();

        for(uint8_t d=0;d<=5;d++ )
        {
            glcdcont_set(glcd_e);
            glcd_wait();
            buf[d]=glcddata_read();
            glcdcont_unset(glcd_e);
            glcd_wait();
            if (x+d==0x40)
            {
                glcdcont_unset(glcd_rw);
                glcddata_wr_tris(0x00);
                glcdcont_set(glcd_cs1);
                glcdcont_unset(glcd_cs2);
                glcdcont_unset(glcd_rs);
                glcddata_write((uint8_t)(glcdc_x|((y>>3)&7)));
                glcdcont_set(glcd_e);
                glcd_wait();
                glcdcont_unset(glcd_e);
                glcd_wait();
                glcddata_write((uint8_t)(glcdc_y|0x00));
                glcdcont_set(glcd_e);
                glcd_wait();
                glcdcont_unset(glcd_e);
                glcd_wait();
                
                glcdcont_set(glcd_rs);   
                glcddata_wr_tris(0xff);
                glcdcont_set(glcd_rw);
                glcdcont_set(glcd_e);
                glcd_wait();
                glcdcont_unset(glcd_e);
                glcd_wait();

            }
            if (x+d==0x80) break;
        }
        glcdcont_unset(glcd_rw);
        glcddata_wr_tris(0x00);

        if (x & 0x40) {
            glcdcont_set(glcd_cs1);
            glcdcont_unset(glcd_cs2);
        } else {
            glcdcont_unset(glcd_cs1);
            glcdcont_set(glcd_cs2);
        }
        glcdcont_unset(glcd_rs);
        glcddata_write((uint8_t) (glcdc_x | ((y >> 3)&7)));
        glcdcont_set(glcd_e);
        glcd_wait();
        glcdcont_unset(glcd_e);
        glcd_wait();
        glcddata_write((uint8_t) (glcdc_y | (x & 0x3f)));
        glcdcont_set(glcd_e);
        glcd_wait();
        glcdcont_unset(glcd_e);
        glcd_wait();

        glcdcont_set(glcd_rs);

        for(uint8_t d=0;d<=5;d++ )
        {
            uint8_t yy=y&0b111;
            uint8_t bb=buf[d];
            bb &=~(0xff<<yy);
            bb |=Terminal12x16[(unsigned short) (*tx-32)*6+d]<<yy;
            
            glcddata_write(bb);
            glcdcont_set(glcd_e);
            glcd_wait();
            glcdcont_unset(glcd_e);
            glcd_wait();
            x++;
            if (x==0x40)
            {
                glcdcont_set(glcd_cs1);
                glcdcont_unset(glcd_cs2);
                glcdcont_unset(glcd_rs);
                glcddata_write((uint8_t)(glcdc_x|((y>>3)&7)));
                glcdcont_set(glcd_e);
                glcd_wait();
                glcdcont_unset(glcd_e);
                glcd_wait();
                glcddata_write((uint8_t)(glcdc_y|0x00));
                glcdcont_set(glcd_e);
                glcd_wait();
                glcdcont_unset(glcd_e);
                glcd_wait();
                
                glcdcont_set(glcd_rs);                               
            }
            if (x==0x80) break;
        }
        if (x==0x80) break;
        tx++;
    }
    glcdcont_unset(glcd_rs);

}




void main(void) {
    ANSELD=0;
    //OSCCON NEEDED FOR PIC18F45K22
#ifdef OSCCON
    OSCCON=0X72;
#endif
    /*
     * Initialize glcd
     */
    glcd_reset();
    __delay_ms(100);
    glcd_on();
    glcd_clear();
    for(uint8_t aa=10;aa<=100;aa+=10)
    {
        glcd_line(aa,10,10,53,1);
    }
    glcd_adv_systext(14,20,"GLCD Draw library",17);
    glcd_systext(28,32,"Oliver Broad",12);
    for(uint8_t d=0;d<=50;d++)
    {
        __delay_ms(100);
    }
    glcd_clear();
    for(uint8_t aa=10;aa<=100;aa+=10)
    {
        glcd_line(10,10,aa,53,1);
    }
    glcd_lgtext(14,16,"Large",5);
    glcd_lgtext(28,32,"Font",4);
   
    while (1)
    {
        PORTD=0x55;
        __delay_ms(200);
        PORTD=~PORTD;
        __delay_ms(200);
    }

    
    
    for(uint8_t d=0;d<=50;d++)
    {
        __delay_ms(100);
    }
    glcd_clear();
    
    for(uint8_t x=0;x<64;x++)
        glcd_vline(x+32,x,63-x,1);
    for(uint8_t x=4;x<28;x++)
        glcd_vline(x+32,x+2,61-x,0);
    
    for(uint8_t x=10;x<=100;x++)
    {
        glcd_setpixel(x,15,1);
        glcd_setpixel(x,50,1);
    }
    for(uint8_t y=16;y<50;y++)
    {
        glcd_setpixel(10,y,1);
        glcd_setpixel(100,y,1);
    }
/*    glcdunset(glcd_cs1);
    glcdset(glcd_cs2);
    glcdset(glcd_rs);
    for(uint8_t aa =0; aa<=0x3f; aa++)
    {
        glcddata=aa;
        glcdset(glcd_e);
        glcd_wait();
        glcdunset(glcd_e);
        glcd_wait();
    }
  */  
    __delay_ms(200);
    __delay_ms(200);
    __delay_ms(200);
    __delay_ms(200);
    __delay_ms(200);
    __delay_ms(200);
    __delay_ms(200);
    __delay_ms(200);
    __delay_ms(200);
    __delay_ms(200);
    __delay_ms(200);
    glcd_clear();
    for(uint8_t aa=10;aa<=100;aa+=10)
    {
        glcd_line(aa,10,10,53,1);
    }
    //glcd_line(73,10,54,53,1);
    __delay_ms(200);
    __delay_ms(200);
    __delay_ms(200);
    __delay_ms(200);
    __delay_ms(200);
    __delay_ms(200);
    __delay_ms(200);
    __delay_ms(200);
    __delay_ms(200);
    __delay_ms(200);
    __delay_ms(200);
    glcd_clear();
    glcd_fill_donut(64,32,30,20,1);
    for(uint8_t l=1;l<=64;l++)
    {
        glcd_set_start_line(l);
            __delay_ms(100);
    }
    __delay_ms(200);
    __delay_ms(200);
    __delay_ms(200);
    __delay_ms(200);
    __delay_ms(200);
    __delay_ms(200);
    __delay_ms(200);
    __delay_ms(200);
    __delay_ms(200);
    __delay_ms(200);
    __delay_ms(200);
    glcd_clear();
    for(uint8_t r=5;r<=30;r+=5)
    {
        glcd_circle(64,32,r,1);
    }
    TRISD=0x00;
    while (1)
    {
        PORTD=0x55;
        __delay_ms(200);
        PORTD=~PORTD;
        __delay_ms(200);
    }
    
    return;
}
