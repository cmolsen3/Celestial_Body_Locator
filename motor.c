/*
 * motor.c
 *
 */

#include "motor.h"
#include "msp.h"

void configure_timer(void) {
	TA0CCR0 = 3000;                    	  // Capture Compare Value
	// up mode, SMCLK, enable
	TA0CTL = TIMER_A_CTL_MC__UP | TIMER_A_CTL_TASSEL_2 | TIMER_A_CTL_IE;
	TA0R = 0;
	/* NVIC Port Interrupt Enable Code */  // Enable interrupt in NVIC
	NVIC_EnableIRQ(TA0_0_IRQn);


}

void configure_pins(void){
	//altitude motor P2.4 - dir, 2.5 - step, 2.6 - enable
	P2DIR |= BIT4; //direction pin
	P2DIR |= BIT5; //step pin
	P2OUT &= ~BIT5;
	P2DIR |= BIT6; //enable pin
	P2OUT |= BIT6;

	//azimuth motor P2.4 - dir, 2.5 - step, 2.6 - enable
	P3DIR |= BIT5; //direction pin
	P3DIR |= BIT6; //step pin
	P3OUT &= ~BIT6;
	P3DIR |= BIT7; //enable pin
	P3OUT |= BIT7;

	//laser pin
	P1DIR |= BIT7;
	P1OUT &= ~BIT7;

}

void start_alt_motor(void){
	P2OUT |= BIT4; //set direction
	P2OUT &= ~BIT6; //set enable
	TA0CCTL0 |= TIMER_A_CCTLN_CCIE; //enable interrupts for pulses from timer
}
void stop_alt_motor(void){
	P2OUT |= BIT4; //set direction
	P2OUT |= BIT6; //set enable
	TA0CCTL0 &= ~TIMER_A_CCTLN_CCIE; //enable interrupts for pulses from timer
}

void start_azi_motor(void){
	P3OUT &= ~BIT5; //set direction
	P3OUT &= ~BIT7; //set enable
	TA0CCTL0 |= TIMER_A_CCTLN_CCIE; //enable interrupts for pulses from timer
}
void stop_azi_motor(void){
	P3OUT &= ~BIT5; //set direction
	//P3OUT |= BIT7; //disable
	TA0CCTL0 &= ~TIMER_A_CCTLN_CCIE; //enable interrupts for pulses from timer
}

void toggle_laser(void){
	P1OUT ^= BIT7;
}
