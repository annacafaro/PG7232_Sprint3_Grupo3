/* 
 * File:   state_machine.h
 * Author: DEV
 *
 * Created on April 10, 2021, 6:53 PM
 */

#ifndef STATE_MACHINE_H
#define	STATE_MACHINE_H

#ifdef	__cplusplus
extern "C" {
#endif

 void check_temperature(void);
    
/* Seccion de Neopixel */
void show(void);
void single_color_line(char color, int lineNumber);
void single_color_line_adc(char color, int lineNumber, uint16_t qty);
void single_color_array(void);
void clear_pixels(void);

/* Seccion Maquina de Estados */
unsigned char numEstados;
unsigned char current_state;
int interrupt_flag;
void CustomInterruptHandler(void);
void state_machine_init(void);
void state_machine(void);
void run_lights(void);
void WS2812B_PATTERN_LIGHT(void);
void WS2812B_PATTERN_BLINK_SLOW(void);
void WS2812B_PATTERN_BLINK_FAST(void);
void WS2812B_PATTERN_SOUND(void);
void WS2812B_PATTERN_SOUND_LOW(void);
void WS2812B_PATTERN_SOUND_MEDIUM(void);
void WS2812B_PATTERN_SOUND_HIGH(void);
void WS2812B_PATTERN_DMX(void);


#ifdef	__cplusplus
}
#endif

#endif	/* STATE_MACHINE_H */

