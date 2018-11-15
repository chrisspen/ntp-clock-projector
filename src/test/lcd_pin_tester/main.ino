/*
Attach a 30k-50k resistor to between each Com pin and ground.
// 50k=>2.7V avg
// 30k=>2.3V avg
// http://ww1.microchip.com/downloads/en/AppNotes/doc8103.pdf
// http://www.arduinoos.com/2014/08/lcd-direct-driving/

The connector has 12 pins, but one appears to be disconnected, and has no signal, leaving effectively just 11 pins.
We likely have 2 backplanes, which leaves 9 segment driver pins.

Minimum pulse width appears to be 4ms = 0.004sec => would need frequency of 250 Hz

*/

#define LED_PIN 13

#define COM0_PIN 3 //b0 orig
//#define COM0_PIN 12 //b0
//#define COM0_PIN 13 //b0
#define COM1_PIN 4 //b5 orig
//#define COM1_PIN 12 //b5
//#define COM1_PIN 13 //b5
#define COM2_PIN 5 //r1 orig
//#define COM2_PIN 12 //r1
//#define COM2_PIN 13 //r1
#define COM3_PIN 6 //r5 orig
//#define COM3_PIN 12 //r5
//#define COM3_PIN 13 //r5

#define SEG0_PIN 7 //b1 = confirmed control
#define SEG1_PIN 8 //b2 = confirmed control
#define SEG2_PIN 9 //b3 = confirmed control
#define SEG3_PIN 10 //r3 = confirmed control
#define SEG4_PIN 11 //r4 = confirmed control

#define SEG5_PIN 12 //b4 = swap?
//swap with b5=bad
//#define SEG5_PIN 4 //b4
//swap with b0=bad
//#define SEG5_PIN 3 //b4
//swap with r1=bad
//#define SEG5_PIN 5 //b4
//swap with r5=bad
//#define SEG5_PIN 6 //b4

#define SEG6_PIN 13 //r2 = swap?
//swap with b5=bad
//#define SEG6_PIN 4 //r2
//swap with b0=bad
//#define SEG6_PIN 3 //r2
//swap with r1=bad
//#define SEG6_PIN 5 //r2
//swap with r5=
//#define SEG6_PIN 6 //r2

//#define SEG7_PIN 2 //r0 //unused?

#define FRAME_MS 16
#define PULSE_MS 2
#define UPDATE_MS 1

#define NUM_BACKPLANES 4
#define NUM_CONTROLS 7

int backplane_pins[NUM_BACKPLANES] = {COM0_PIN, COM1_PIN, COM2_PIN, COM3_PIN};
int control_pins[NUM_CONTROLS] = {SEG0_PIN, SEG1_PIN, SEG2_PIN, SEG3_PIN, SEG4_PIN, SEG5_PIN, SEG6_PIN};//, SEG7_PIN};

enum PinState {
	ON,
	OFF,
	DISCONNECTED
};

unsigned long last_blink = 0;

unsigned long last_clock_update = 0;

//unsigned long last_update = 0;
//PinState state = DISCONNECTED; // 0=low, 1=high, 2=disconnected
//unsigned long offset = 0; // should be in multiples of PULSE_MS

//bool segment0_state = true;

int current_backplane_index = 0;
PinState current_backplane_state = OFF;
unsigned long last_backplane_update = 0;

//bool control0_status[NUM_BACKPLANES] = {1,1,1,1};//1st
//bool control1_status[NUM_BACKPLANES] = {1,1,1,1};//2nd "1:"
//bool control2_status[NUM_BACKPLANES] = {1,1,1,1};//3rd,4th lower right
//bool control3_status[NUM_BACKPLANES] = {1,1,1,1};
//bool control4_status[NUM_BACKPLANES] = {1,1,1,1};
//bool control5_status[NUM_BACKPLANES] = {1,1,1,1};
//bool control6_status[NUM_BACKPLANES] = {1,0,1,1};
//bool control7_status[NUM_BACKPLANES] = {1,1,1,1};//na

bool control_status[NUM_BACKPLANES*NUM_CONTROLS] = {
	//2A:PA pm
	//1,1,1,1,
	//1,1,1,1,
	//1,1,1,1,
	//1,1,1,1,
	//1,1,1,1,
	//1,1,1,1,
	//1,1,1,1
	//oA:PA pm
	//0,1,1,1,
	//1,1,1,1,
	//1,1,1,1,
	//1,1,1,1,
	//1,1,1,1,
	//1,1,1,1,
	//1,1,1,1
	//dA:PA
	//1,0,1,1,
	//1,1,1,1,
	//1,1,1,1,
	//1,1,1,1,
	//1,1,1,1,
	//1,1,1,1,
	//1,1,1,1
	//1A:PA pm
	//1,1,0,1,
	//1,1,1,1,
	//1,1,1,1,
	//1,1,1,1,
	//1,1,1,1,
	//1,1,1,1,
	//1,1,1,1
	//dA:PA pm
	//1,1,1,0,
	//1,1,1,1,
	//1,1,1,1,
	//1,1,1,1,
	//1,1,1,1,
	//1,1,1,1,
	//1,1,1,1
	//dA:PA pm fading
	//1,0,1,0,
	//1,1,1,1,
	//1,1,1,1,
	//1,1,1,1,
	//1,1,1,1,
	//1,1,1,1,
	//1,1,1,1
	//2P:PA pm
	//1,1,1,1,
	//0,1,1,1,
	//1,1,1,1,
	//1,1,1,1,
	//1,1,1,1,
	//1,1,1,1,
	//1,1,1,1
	//dP:PA
	//1,0,1,1,
	//0,1,1,1,
	//1,1,1,1,
	//1,1,1,1,
	//1,1,1,1,
	//1,1,1,1,
	//1,1,1,1
	//2h:PA
	//1,1,1,1,
	//1,0,1,1,
	//1,1,1,1,
	//1,1,1,1,
	//1,1,1,1,
	//1,1,1,1,
	//1,1,1,1
	//2A PA
	//1,1,1,1,
	//1,1,0,1,
	//1,1,1,1,
	//1,1,1,1,
	//1,1,1,1,
	//1,1,1,1,
	//1,1,1,1
	//2A:PA pm
	//1,1,1,1,
	//1,1,1,0,
	//1,1,1,1,
	//1,1,1,1,
	//1,1,1,1,
	//1,1,1,1,
	//1,1,1,1
	//2A:NA pm
	//1,1,1,1,
	//1,1,1,1,
	//0,1,1,1,
	//1,1,1,1,
	//1,1,1,1,
	//1,1,1,1,
	//1,1,1,1
	//2A:Ah pm
	//1,1,1,1,
	//1,1,1,1,
	//1,0,1,1,
	//1,1,1,1,
	//1,1,1,1,
	//1,1,1,1,
	//1,1,1,1
	//2A:hA pm
	//1,1,1,1,
	//1,1,1,1,
	//1,1,0,1,
	//1,1,1,1,
	//1,1,1,1,
	//1,1,1,1,
	//1,1,1,1
	//2A:AH pm
	//1,1,1,1,
	//1,1,1,1,
	//1,1,1,0,
	//1,1,1,1,
	//1,1,1,1,
	//1,1,1,1,
	//1,1,1,1
	//2A:9A pm broken
	//1,1,1,1,
	//1,1,1,1,
	//1,1,1,1,
	//0,1,1,1,
	//1,1,1,1,
	//1,1,1,1,
	//1,1,1,1
	//2A:2A pm
	//1,1,1,1,
	//1,1,1,1,
	//1,1,1,1,
	//1,0,1,1,
	//1,1,1,1,
	//1,1,1,1,
	//1,1,1,1
	//C
	//0,0,0,0,
	//0,0,0,0,
	//0,0,0,0,
	//0,0,1,0,
	//0,0,0,0,
	//0,0,0,0,
	//0,0,0,0,
	//C
	//0,0,0,0,
	//0,0,0,0,
	//0,0,0,0,
	//0,0,0,1,
	//0,0,0,0,
	//0,0,0,0,
	//0,0,0,0,
	//C
	//0,0,0,0,
	//0,0,0,0,
	//0,0,0,0,
	//1,1,1,1,
	//0,0,0,0,
	//0,0,0,0,
	//0,0,0,0,
	//part __45
	//0,0,0,0,
	//0,0,0,0,
	//1,0,0,0,
	//0,0,0,0,
	//0,0,0,0,
	//0,0,0,0,
	//0,0,0,0,
	//
	0,0,0,0,
	0,0,0,0,
	0,0,0,1,
	0,0,0,1,
	1,0,0,0,
	0,0,0,0,
	0,0,0,0,
	//
	//0,0,0,0,
	//0,0,0,0,
	//1,0,0,0,
	//0,0,0,0,
	//0,0,0,0,
	//0,0,0,0,
	//0,0,0,0,
	////
	//0,0,0,0,
	//0,0,0,0,
	//1,0,0,0,
	//0,0,0,0,
	//0,0,0,0,
	//0,0,0,0,
	//0,0,0,0,
	////
	//0,0,0,0,
	//0,0,0,0,
	//1,0,0,0,
	//0,0,0,0,
	//0,0,0,0,
	//0,0,0,0,
	//0,0,0,0,
	////
	//0,0,0,0,
	//0,0,0,0,
	//1,0,0,0,
	//0,0,0,0,
	//0,0,0,0,
	//0,0,0,0,
	//0,0,0,0,
	//
	//1,1,1,1,
	//1,1,1,1,
	//1,1,1,1,
	//1,1,1,1,
	//1,1,1,1,
	//1,1,1,1,
	//1,1,1,1
	//
	//1,1,1,1,
	//1,1,1,1,
	//1,1,1,1,
	//1,1,1,1,
	//1,1,1,1,
	//1,1,1,1,
	//1,1,1,1
	//
	//1,1,1,1,
	//1,1,1,1,
	//1,1,1,1,
	//1,1,1,1,
	//1,1,1,1,
	//1,1,1,1,
	//1,1,1,1
	//
	//1,1,1,1,
	//1,1,1,1,
	//1,1,1,1,
	//1,1,1,1,
	//1,1,1,1,
	//1,1,1,1,
	//1,1,1,1
	//
	//1,1,1,1,
	//1,1,1,1,
	//1,1,1,1,
	//1,1,1,1,
	//1,1,1,1,
	//1,1,1,1,
	//1,1,1,1
	//
	//1,1,1,1,
	//1,1,1,1,
	//1,1,1,1,
	//1,1,1,1,
	//1,1,1,1,
	//1,1,1,1,
	//1,1,1,1
	//
	//1,1,1,1,
	//1,1,1,1,
	//1,1,1,1,
	//1,1,1,1,
	//1,1,1,1,
	//1,1,1,1,
	//1,1,1,1
};

void high(int _pin){
	pinMode(_pin, OUTPUT);
	digitalWrite(_pin, HIGH);
}

//void com_to_avg(){
	//pinMode(_pin, INPUT_PULLUP);
//}

void disconnect(int _pin){
	pinMode(_pin, INPUT); /* set pin as input */
	digitalWrite(_pin, LOW); /* set to 0 in input mode */
}

void low(int _pin){
	pinMode(_pin, OUTPUT);
	digitalWrite(_pin, LOW);
}

void update_clock(){
	
	// http://www.arduinoos.com/2014/08/lcd-direct-driving/
	// Update backplane pin.
	bool changed = false;	
	if(current_backplane_state == ON && millis() - last_backplane_update >= PULSE_MS){
		// Go low for pulse.
		low(backplane_pins[current_backplane_index]);
		current_backplane_state = OFF;
		last_backplane_update = millis();
		changed = true;
	}else if(current_backplane_state == OFF && millis() - last_backplane_update >= PULSE_MS){
		// Disconnect old pin and switch to new pin.
		disconnect(backplane_pins[current_backplane_index]);
		current_backplane_state = ON;
		current_backplane_index = (current_backplane_index + 1) % NUM_BACKPLANES;
		// Go high for pulse.
		high(backplane_pins[current_backplane_index]);
		last_backplane_update = millis();
		changed = true;
	}
	
	if(changed){
		//if(control0_status[current_backplane_index]){
			//if(current_backplane_state == ON){
				//low(control_pins[0]);
			//}else{
				//high(control_pins[0]);
			//}
		//}else{
			//if(current_backplane_state == ON){
				//high(control_pins[0]);
			//}else{
				//low(control_pins[0]);
			//}
		//}
		
		//if(control1_status[current_backplane_index]){
			//if(current_backplane_state == ON){
				//low(control_pins[1]);
			//}else{
				//high(control_pins[1]);
			//}
		//}else{
			//if(current_backplane_state == ON){
				//high(control_pins[1]);
			//}else{
				//low(control_pins[1]);
			//}
		//}
		
		//if(control2_status[current_backplane_index]){
			//if(current_backplane_state == ON){
				//low(control_pins[2]);
			//}else{
				//high(control_pins[2]);
			//}
		//}else{
			//if(current_backplane_state == ON){
				//high(control_pins[2]);
			//}else{
				//low(control_pins[2]);
			//}
		//}
		
		//if(control3_status[current_backplane_index]){
			//if(current_backplane_state == ON){
				//low(control_pins[3]);
			//}else{
				//high(control_pins[3]);
			//}
		//}else{
			//if(current_backplane_state == ON){
				//high(control_pins[3]);
			//}else{
				//low(control_pins[3]);
			//}
		//}
		
		//if(control4_status[current_backplane_index]){
			//if(current_backplane_state == ON){
				//low(control_pins[4]);
			//}else{
				//high(control_pins[4]);
			//}
		//}else{
			//if(current_backplane_state == ON){
				//high(control_pins[4]);
			//}else{
				//low(control_pins[4]);
			//}
		//}
		
		//if(control5_status[current_backplane_index]){
			//if(current_backplane_state == ON){
				//low(control_pins[5]);
			//}else{
				//high(control_pins[5]);
			//}
		//}else{
			//if(current_backplane_state == ON){
				//high(control_pins[5]);
			//}else{
				//low(control_pins[5]);
			//}
		//}
		
		//if(control6_status[current_backplane_index]){
			//if(current_backplane_state == ON){
				//low(control_pins[6]);
			//}else{
				//high(control_pins[6]);
			//}
		//}else{
			//if(current_backplane_state == ON){
				//high(control_pins[6]);
			//}else{
				//low(control_pins[6]);
			//}
		//}

		for(int i=0; i<NUM_CONTROLS; i++){
			if(control_status[current_backplane_index+i*4]){
				if(current_backplane_state == ON){
					low(control_pins[i]);
				}else{
					high(control_pins[i]);
				}
			}else{
				if(current_backplane_state == ON){
					high(control_pins[i]);
				}else{
					low(control_pins[i]);
				}
			}
		}

		//if(control7_status[current_backplane_index]){
			//if(current_backplane_state == ON){
				//low(control_pins[7]);
			//}else{
				//high(control_pins[7]);
			//}
		//}else{
			//if(current_backplane_state == ON){
				//high(control_pins[7]);
			//}else{
				//low(control_pins[7]);
			//}
		//}
	}
	
	//if(changed){
		
		//if(control0_status[current_backplane_index]){
			//// Active segment by making pin out of phase with the backplane (opposite).
			//if(current_backplane_state == ON){
				//low(control_pins[0]);
			//}else if(current_backplane_state == OFF){
				//high(control_pins[0]);
			//}
		//}else{
			//// Otherwise deactivate segment by making the pin in phase with the backplane (identical).
			//if(current_backplane_state == ON){
				//high(control_pins[0]);
			//}else if(current_backplane_state == OFF){
				//low(control_pins[0]);
			//}
		//}
				
		//if(control1_status[current_backplane_index]){
			//// Active segment by making pin out of phase with the backplane (opposite).
			//if(current_backplane_state == ON){
				//low(control_pins[1]);
			//}else if(current_backplane_state == OFF){
				//high(control_pins[1]);
			//}
		//}else{
			//// Otherwise deactivate segment by making the pin in phase with the backplane (identical).
			//if(current_backplane_state == ON){
				//high(control_pins[1]);
			//}else if(current_backplane_state == OFF){
				//low(control_pins[1]);
			//}
		//}
	//}
	
}

void setup() {

    //pinMode(LED_PIN, OUTPUT);
    
	for(int i=0; i<NUM_BACKPLANES; i++){
		disconnect(backplane_pins[i]);
	}

	for(int i=0; i<NUM_CONTROLS; i++){
		disconnect(control_pins[i]);
	}

}

void loop() {
	
	//if(millis() - last_blink > 1000){
		//last_blink = millis();
		//digitalWrite(LED_PIN, !digitalRead(LED_PIN));
	//}
    
    update_clock();
    	
}
