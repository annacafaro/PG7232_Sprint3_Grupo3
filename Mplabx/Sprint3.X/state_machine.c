#include "mcc_generated_files/mcc.h"

#define WS281x_PATTERN_LIGHT 0x00       // Simple light
#define WS281x_PATTERN_BLINK_SLOW 0x01       // Blink 50/50
#define WS281x_PATTERN_BLINK_FAST 0x02       
#define WS281x_PATTERN_SOUND 0x03
#define WS281x_PATTERN_SOUND_LOW 0x04
#define WS281x_PATTERN_SOUND_MEDIUM 0x05
#define WS281x_PATTERN_SOUND_HIGH 0x06
#define WS281x_PATTERN_DMX 0x07

/* Seccion ADC */
adc_result_t resultado1;
adc_result_t resultado2;
adc_result_t resultado3;

void adc_conversion(void){
    resultado1 = ADC_GetConversion(channel_AN0)*8/1023;
    resultado2 = ADC_GetConversion(channel_AN1)*8/1023;
    resultado3 = ADC_GetConversion(channel_AN2)*8/1023;
}

/* Seccion de Neopixel */
#define numPixels 8
#define lineqty 3

unsigned char Red[numPixels*lineqty];
unsigned char Green[numPixels*lineqty];
unsigned char Blue[numPixels*lineqty];
unsigned char TempR[numPixels*lineqty];
unsigned char TempG[numPixels*lineqty];
unsigned char TempB[numPixels*lineqty];

#define writePixel(r,g,b)\
do{\
    SSP1BUF = g;\
    while(!SSP1STATbits.BF);\
    SSP1BUF = r;\
    while(!SSP1STATbits.BF);\
    SSP1BUF = b;\
    while(!SSP1STATbits.BF);\
} while(0);

void show(void){
    for(int i = 0; i < numPixels*lineqty; i++) {
        writePixel(Red[i],Green[i],Blue[i]);
    }
}

void single_color_line(char color, int lineNumber){
    int offset;
    unsigned char r,g,b;
    if(color == 'r'){
        r = 1;
        g = 0;
        b = 0;
    }
    else if(color == 'g'){
        r = 0;
        g = 1;
        b = 0;
    }
    else if(color == 'b'){
        r = 0;
        g = 0;
        b = 1;
    }
    else{
        r = 0;
        g = 0;
        b = 0;
    }
    switch(lineNumber){
        case 1:
            offset = 0;
            break;
        case 2:
            offset = 8;
            break;
        case 3:
            offset = 16;
            break;
        default:
            offset = 0;
            break;
    }
    for(int i = offset; i < numPixels+offset; i++){
        Red[i] = 255*r;
        Green[i] = 255*g;
        Blue[i] = 255*b;
    }
}

void single_color_line_adc(char color, int lineNumber, uint16_t qty){
    int offset;
    unsigned char r,g,b;
    if(color == 'r'){
        r = 1;
        g = 0;
        b = 0;
    }
    else if(color == 'g'){
        r = 0;
        g = 1;
        b = 0;
    }
    else if(color == 'b'){
        r = 0;
        g = 0;
        b = 1;
    }
    else{
        r = 0;
        g = 0;
        b = 0;
    }
    switch(lineNumber){
        case 1:
            offset = 0;
            break;
        case 2:
            offset = 8;
            break;
        case 3:
            offset = 16;
            break;
        default:
            offset = 0;
            break;
    }
    for(int i = offset; i < numPixels+offset; i++){
        if(i-offset < qty){
            Red[i] = 255*r;
            Green[i] = 255*g;
            Blue[i] = 255*b;
        }
        else{
            Red[i] = 0;
            Green[i] = 0;
            Blue[i] = 0;
        }
    }
}

void single_color_array(void){
    single_color_line('r',1);
    single_color_line('g',2);
    single_color_line('b',3);
}

void clear_pixels(void){
    single_color_line('d',1);
    single_color_line('d',2);
    single_color_line('d',3);
}

/* Seccion Maquina de Estados */
unsigned char numEstados = 0x08;
unsigned char current_state = 0x00;
uint16_t Buf[ERASE_FLASH_BLOCKSIZE];
int interrupt_flag = 0;

void CustomInterruptHandler(void){
    if(current_state  ==  numEstados){
        current_state = 0x00;
    }
    else{
        current_state += 1;
    }
    interrupt_flag = 1;
}

void WS2812B_PATTERN_LIGHT(void){
    single_color_array();
    show();
    while(!interrupt_flag){
    }
    interrupt_flag = 0;
}

void WS2812B_PATTERN_BLINK_SLOW(void){
    do{
        single_color_array();
        show();
        __delay_ms(100); // Reset delay
        clear_pixels();
        show();
        __delay_ms(100); // Reset delay
    }while(!interrupt_flag);
    interrupt_flag = 0;
}

void WS2812B_PATTERN_BLINK_FAST(void){
    do{
        single_color_array();
        show();
        __delay_ms(10); // Reset delay
        clear_pixels();
        show();
        __delay_ms(10); // Reset delay
    }while(!interrupt_flag);
    interrupt_flag = 0;
}

void WS2812B_PATTERN_SOUND(void){
    do{
        resultado1 = ADC_GetConversion(channel_AN0)*8/1023;
        resultado2 = ADC_GetConversion(channel_AN1)*8/1023;
        resultado3 = ADC_GetConversion(channel_AN2)*8/1023;
        single_color_line_adc('r',1,resultado1);
        single_color_line_adc('g',2,resultado2);
        single_color_line_adc('b',3,resultado3);
        show();
        __delay_ms(10); // Reset delay
    }while(!interrupt_flag);
    interrupt_flag = 0;
}

void WS2812B_PATTERN_SOUND_LOW(void){
    do{
        resultado1 = ADC_GetConversion(channel_AN0)*8/1023;
        single_color_line_adc('r',1,resultado1);
        single_color_line_adc('g',2,resultado1);
        single_color_line_adc('b',3,resultado1);
        show();
        __delay_ms(10); // Reset delay
    }while(!interrupt_flag);
    interrupt_flag = 0;
}

void WS2812B_PATTERN_SOUND_MEDIUM(void){
    do{
        resultado2 = ADC_GetConversion(channel_AN1)*8/1023;
        single_color_line_adc('r',1,resultado2);
        single_color_line_adc('g',2,resultado2);
        single_color_line_adc('b',3,resultado2);
        show();
        __delay_ms(10); // Reset delay
    }while(!interrupt_flag);
    interrupt_flag = 0;
}

void WS2812B_PATTERN_SOUND_HIGH(void){
    do{
        resultado3 = ADC_GetConversion(channel_AN2)*8/1023;
        single_color_line_adc('r',1,resultado3);
        single_color_line_adc('g',2,resultado3);
        single_color_line_adc('b',3,resultado3);
        show();
        __delay_ms(10); // Reset delay
    }while(!interrupt_flag);
    interrupt_flag = 0;
}

void WS2812B_PATTERN_DMX(void){
    single_color_array();
    show();
    while(!interrupt_flag){
    }
    interrupt_flag = 0;
}

void state_machine_init(void){
    IOCCF0_SetInterruptHandler(CustomInterruptHandler);
    // Clear all the pixels
    clear_pixels();
    current_state = FLASH_ReadWord(0x1F90) & 0x07;
}

void run_lights(void){
    switch(current_state){
        case WS281x_PATTERN_LIGHT:
            WS2812B_PATTERN_LIGHT();
            break;
        case WS281x_PATTERN_BLINK_SLOW:
            WS2812B_PATTERN_BLINK_SLOW();
            break;
        case WS281x_PATTERN_BLINK_FAST:
            WS2812B_PATTERN_BLINK_FAST();
            break;
        case WS281x_PATTERN_SOUND:
            WS2812B_PATTERN_SOUND();
            break;
        case WS281x_PATTERN_SOUND_LOW:
            WS2812B_PATTERN_SOUND_LOW();
            break;
        case WS281x_PATTERN_SOUND_MEDIUM:
            WS2812B_PATTERN_SOUND_MEDIUM();
            break;
        case WS281x_PATTERN_SOUND_HIGH:
            WS2812B_PATTERN_SOUND_HIGH();
            break;
        case WS281x_PATTERN_DMX:
            WS2812B_PATTERN_DMX();
            break;
        default:
            WS2812B_PATTERN_LIGHT();
            break;
    }
}

void state_machine(void){
    PORTD = current_state;
    run_lights();
}