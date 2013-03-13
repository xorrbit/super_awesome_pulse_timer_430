/* super awesome pulse timer msp430 v1
 * by Andrew Orr
 */
#include <msp430.h>

unsigned int g_rising_edge;
unsigned int g_falling_edge;

int main(void)
{
	unsigned int pulse_ticks;
	
	// kill the watchdog
	WDTCTL = WDTPW + WDTHOLD;

	// lock up if we don't have a calibration constant
	if (CALBC1_16MHZ==0xFF) {
		while(1);
	}

	// load our 16 mhz calibration
	DCOCTL = 0;
	BCSCTL1 = CALBC1_16MHZ;
	DCOCTL = CALDCO_16MHZ;

#ifdef ENERGIA
	// start our serial junk for energia
	Serial.begin(9600);
#endif

	// set up P1.2 as capture
	P1DIR &= ~BIT2;
	P1SEL |= BIT2;

	// set up timer_a 0 as capture, both edges, and interrupts
	TA0CCTL1 = CAP + CM_3 + CCIE + SCS + CCIS_0;
	TA0CTL |= TASSEL_2 + MC_2 + TACLR;

	for(;;) {
		// sleep LPM0
		__bis_SR_register(LPM0_bits + GIE);
		__no_operation();

		pulse_ticks = g_falling_edge - g_rising_edge;

#ifdef ENERGIA
		Serial.print("Pulse ticks: ");
		Serial.print(pulse_ticks);
		Serial.print(", ns: ");
		Serial.println(pulse_ticks * 0.0000625);
#endif		
  }
}

// timer 0 a 1 interrupt
#pragma vector = TIMER0_A1_VECTOR
__interrupt void TIMER0_A1_ISR (void)
{
	// check if our CCR1 interrupt was fired
	if (TA0IV == TA0IV_TACCR1) {

		// check which edge we're on
		if (TA0CCTL1 & CCI) {
			// rising edge
			g_rising_edge = TA0CCR1;

		} else {
			// falling edge
			g_falling_edge = TA0CCR1;

			// exit LPM0
			__bic_SR_register_on_exit(LPM0_bits + GIE);
		}
	}
}

