#include "mcc_generated_files/mcc.h"

// Se definen los distintos modos bajo los cuales funcionaran las luces
#define WS281x_PATTERN_LIGHT 0x00       // Luces Encendidas
#define WS281x_PATTERN_BLINK_SLOW 0x01  // Blink cada 100ms
#define WS281x_PATTERN_BLINK_FAST 0x02  // Blink cada 10ms     
#define WS281x_PATTERN_SOUND 0x03       // Controla cada tubo de luz a una frecuencia distinta
#define WS281x_PATTERN_SOUND_LOW 0x04   // Controla los 3 tubos utilizando las frecuencias bajas
#define WS281x_PATTERN_SOUND_MEDIUM 0x05 // Controla los 3 tubos utilizando las frecuencias medias
#define WS281x_PATTERN_SOUND_HIGH 0x06   // Controla los 3 tubos utilizando las frecuencias altas
#define WS281x_PATTERN_DMX 0x07          // Reservado para utilizar con el DMX

/* Seccion ADC */
// Se definen las variables que se utilizaran para guardar los resultados de cada canal del ADC
adc_result_t resultado1;
adc_result_t resultado2;
adc_result_t resultado3;
adc_result_t temp_value;

// Funcion utilizada para revisar la temperatura al final de cada rutina. El valor de 900 representa 32grados aproximadamente
void check_temperature(void){
    temp_value = ADC_GetConversion(temp);
    if(temp_value > 57600){
        IO_RD4_SetLow();
    }
    else{
        IO_RD4_SetHigh();
    }
}

/* Seccion de Neopixel */
// Se definen la cantidad de neopixeles por tubo y la cantidad de tubos
#define numPixels 8
#define lineqty 3

// Se definen los array que describiran los colores para cada pixel
unsigned char Red[numPixels*lineqty];
unsigned char Green[numPixels*lineqty];
unsigned char Blue[numPixels*lineqty];

// Se define la funcion principal que hace el envio de un pixel
#define writePixel(r,g,b)\
do{\
    SSP1BUF = g;\
    while(!SSP1STATbits.BF);\
    SSP1BUF = r;\
    while(!SSP1STATbits.BF);\
    SSP1BUF = b;\
    while(!SSP1STATbits.BF);\
} while(0);

// Rutina utilizada para enviar todos los pixeles de todos los tubos
void show(void){
    for(int i = 0; i < numPixels*lineqty; i++) {
        writePixel(Red[i],Green[i],Blue[i]);
    }
}

// Esta rutina permite seleccionar el color que quieres aplicar al tubo que pasas como parametro
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

// Esta rutina es similar a la anterior pero toma en cuenta la lectura de los adc para hacer bailar los tubos de acuerdo a las
// mediciones que se obtienen del adc
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

// Funcion utilizada para pintar cada tubo de los colores basicos
void single_color_array(void){
    single_color_line('r',1);
    single_color_line('g',2);
    single_color_line('b',3);
}

// Funcion utilizada para poner en negro todos los pixeles
void clear_pixels(void){
    single_color_line('d',1);
    single_color_line('d',2);
    single_color_line('d',3);
}

/* Seccion Maquina de Estados */
unsigned char numEstados = 0x08;
unsigned char current_state = 0x00;
//uint16_t Buf[ERASE_FLASH_BLOCKSIZE];
int interrupt_flag = 0;

// Rutina de interrupcion que permite cambiar el estado con un boton externo
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
        check_temperature();
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
        check_temperature();
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
        check_temperature();
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
        check_temperature();
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
        check_temperature();
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
        check_temperature();
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
        check_temperature();
    }while(!interrupt_flag);
    interrupt_flag = 0;
}

void WS2812B_PATTERN_DMX(void){
    single_color_array();
    show();
    while(!interrupt_flag){
        check_temperature();
    }
    interrupt_flag = 0;
}

// Funcion que inicializa la maquina de estados activando la rutina de interrupcion y borrando los pixeles
void state_machine_init(void){
    IOCCF0_SetInterruptHandler(CustomInterruptHandler);
    // Clear all the pixels
    clear_pixels();
    //current_state = FLASH_ReadWord(0x1F90) & 0x07;
}

// Rutina que selecciona el patron a representar de acuerdo al estado actual de los leds
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

// Rutina principal que permite mostrar en los leds el estado actual y posteriormente correr la rutina correspondiente
void state_machine(void){
    PORTD = current_state;
    //FLASH_WriteWord(0x1F90, Buf, current_state);
    run_lights();
}