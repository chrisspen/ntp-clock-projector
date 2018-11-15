/*
    3-20-14
    Copyright Steven Mullins 2014
    
    ATTiny45
	
	60Khz Transmitter
	Modulated with WWVB signal
	
*/

#include <avr/io.h>
#include <avr/interrupt.h> 
#include <util/delay.h>

//==============================

// Hours in UTC time
#define hour_tens 2
#define hour_ones 2

#define minute_tens 2
// Minute ones default to zero

volatile uint8_t timer;
volatile uint8_t slot;
volatile uint8_t signal;

volatile uint8_t minute_ones;

int main (void)
{	
	/* Initalize Fast PWM on OCR1A*/
	DDRB |= _BV(PB1); // Set PWM pin as output
	PLLCSR |= _BV(PLLE); // Start PLL
	_delay_us(100);      // Wait till PLL stablizes p. 9
	PLLCSR |= _BV(PCKE); // Set Clock source to PLL
	OCR1C = 132; // Set OCR1C to top p. 91 (60kkHz)
	OCR1A = 66;  // Set beginning OCR1A value (50% duty cycle)
	TCCR1 	|= _BV(CS12); 	 /* Set clock prescaler to 8   */ 
	TCCR1	|= _BV(PWM1A) 	 /* Enable PWM based on OCR1A  */ \
			|  _BV(COM1A0) 	 /* Set PWM compare mode p. 89 */ \
			;
	
	/* Initalize CTC interupt on timer0 at 61hz */
	TCCR0A 	|=	_BV(WGM01); //pg. 82  Mode 2 CTC OCR0A TOP
	OCR0A	|=	127; 		// 8mhz / ((127+1) * 1024 prescale) = 61hz
	TCCR0B	|=	_BV(CS00) | _BV(CS02); 	// set prescaler to 1024
	TIMSK	|=	_BV(OCIE0A);// enable compare match interrupt
	
	sei();	// Enable interupts
	
	while(1);
}

ISR(TIMER0_COMPA_vect){
	
	switch (slot) {
		
		case 0 : { signal = 2;break;} 
		
		case 1 : { signal = ((minute_tens >> 2) & 1);break;} // min 40
		case 2 : { signal = ((minute_tens >> 1) & 1);break;} // min 20
		case 3 : { signal = ((minute_tens >> 0) & 1);break;} // min 10
		
		case 5 : { signal = ((minute_ones >> 4) & 1);break;} // min 8
		case 6 : { signal = ((minute_ones >> 2) & 1);break;} // min 4
		case 7 : { signal = ((minute_ones >> 1) & 1);break;} // min 2
		case 8 : { signal = (minute_ones & 1);break;}		 // min 1
		
		case 9 : { signal = 2;break;}
		
		case 12 : { signal = ((hour_tens >> 1) & 1);break;} // hour 20
		case 13 : { signal = ((hour_tens >> 0) & 1);break;} // hour 10
		
		case 15 : { signal = ((hour_ones >> 4) & 1);break;} // hour 8
		case 16 : { signal = ((hour_ones >> 2) & 1);break;} // hour 4
		case 17 : { signal = ((hour_ones >> 1) & 1);break;} // hour 2
		case 18 : { signal = (hour_ones & 1);break;}		// hour 1
		
		case 19: { signal = 2;break;}
		
		case 26: { signal = 1;break;}  //
		case 27: { signal = 1;break;}  // Day of year 60
		case 29: { signal = 2;break;}  //
		
		case 31: { signal = 1;break;}  //
		case 32: { signal = 1;break;}  // Day of year 6
		case 37: { signal = 1;break;}  //
		case 39: { signal = 2;break;}
		
		case 42: { signal = 1;break;}  //
		case 43: { signal = 1;break;}  // DUT1 = 0.3
		case 49: { signal = 2;break;}
		
		case 50: { signal = 1;break;}  // Year = 08
		case 55: { signal = 1;break;}  // Leap year = True
		case 59: { signal = 2;break;}
		
		default: { signal = 0;break;}	
	}
	
	switch (signal) {
		
		case 0: {
		// 0 (0.2s reduced power)
		if (timer < 12) {OCR1A = 6;} 
			else {OCR1A = 66;}
		} break;
		case 1: {
		// 1 (0.5s reduced power)
		if (timer < 30) {OCR1A = 6;} 
			else {OCR1A = 66;}
		} break;
		case 2: {
		// Marker (0.8s reduced power)
		if (timer < 48) {OCR1A = 6;} 
			else {OCR1A = 66;}
		} break;
	}
	
	timer++;			// Advance timer
	if (timer == 61) {	// Check to see if at end of second
		timer = 0; 		// If so reset timer
		slot++;			// Advance data slot in minute data packet
		if (slot == 60) {
			slot = 0; // Reset slot to 0 if at 60 seconds
			minute_ones++; // Advance minute count
		}
	}
}
