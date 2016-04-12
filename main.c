/*
 * This code is a skeleton for a Manchester Phase Encoded Transmitter
 * It may transmit any number of bits up to 32. A separate start bit is sent,
 * with a rising edge at mid-bit. After this, normal transmission is sent.
 * This version defines a 1 as a rising edge at mid-bit. Transmission rate is 1 mS per bit.
 * There is an inter-transmission time of 5 mS, then repeat transmission.
 * A 500uS clock tick is interrupt is required.
 */

#include <msp430.h>
#include "configure.h"
#include "timerA.h"
#include "transmit.h"
#include "receive.h"

// Function Prototypes
void InitializeVariables(void);
void InitializeHardware(void);
void ConfigureClockModule(void);

//Global Variables Section
TransmitterData Xmit1 ;  //This declares an instance of the transmitter data structure.
ManchesterReceiver Rcv1 ;
Event_Queue Receiver_Events; //Receiver Global Variables

void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;		// Stop watchdog timer
    ConfigureClockModule();

    InitializeHardware();
  	InitializeVariables();

  	_BIS_SR(GIE);		// interrupts enabled
	while (1) {
#if (TRANSMIT_RECEIVE_MODE == RECEIVE)
		rcv();
#endif
	}
}

//Set up globals, including the transmitter structure.
//Comment Well
void InitializeVariables(void){
	//Here is an example. This is just sample data, the final application determines what is to be sent.
		Xmit1.Bits_Remaining = BITS_IN_TRANSMISSION ;
		Xmit1.Transmit_Data_Buffer = 0xAA55AA55 ;
		Xmit1.Transmit_Data = 0xAA55AA55        ;
		Xmit1.Transmit_Clock_Phase = Low ;
		Xmit1.Transmitter_State = StartBit ;
	//etc. .....
		Receiver_Events.Get_Index = 0 ;
		Receiver_Events.Put_index = 0 ;
		Receiver_Events.QueSize   = 0 ;

	//etc.........
		Rcv1.CurrentRcvState = Initial_Expect_Rising ;
		Rcv1.CurrentRecvdData =      0 ;
		Rcv1.FallingEdgeTimeStamp =  0 ;
		Rcv1.RisingEdgeTimeStamp =   0 ;
		Rcv1.MidBitTimeStamp =       0 ;
		Rcv1.PulseWidth =            0 ;
		Rcv1.CurrentRecvdData =      0 ;
		Rcv1.LastValidReceived =     0 ;

}

void InitializeHardware(void)
{
	ConfigureTimerA();
	InitializeTransmit();
	InitializeReceive();

	SET_TR_AS_AN_OUTPUT;
}
/*
 *  ======== BCSplus_init ========
 *  Initialize MSP430 Basic Clock System
 */
void ConfigureClockModule(void)
{
    /*
     * Basic Clock System Control 2
     *
     * SELM_0 -- DCOCLK
     * DIVM_0 -- Divide by 1
     * ~SELS -- DCOCLK
     * DIVS_0 -- Divide by 1
     * ~DCOR -- DCO uses internal resistor
     *
     * Note: ~<BIT> indicates that <BIT> has value zero
     */
    BCSCTL2 = SELM_0 + DIVM_0 + DIVS_0;

    if (CALBC1_8MHZ != 0xFF) {
        /* Adjust this accordingly to your VCC rise time */
        __delay_cycles(100000);

        // Follow recommended flow. First, clear all DCOx and MODx bits. Then
        // apply new RSELx values. Finally, apply new DCOx and MODx bit values.
        DCOCTL = 0x00;
        BCSCTL1 = CALBC1_8MHZ;      /* Set DCO to 8MHz */
        DCOCTL = CALDCO_8MHZ;
    }

    /*
     * Basic Clock System Control 1
     *
     * XT2OFF -- Disable XT2CLK
     * ~XTS -- Low Frequency
     * DIVA_0 -- Divide by 1
     *
     * Note: ~XTS indicates that XTS has value zero
     */
    BCSCTL1 |= XT2OFF + DIVA_0;

    /*
     * Basic Clock System Control 3
     *
     * XT2S_0 -- 0.4 - 1 MHz
     * LFXT1S_2 -- If XTS = 0, XT1 = VLOCLK ; If XTS = 1, XT1 = 3 - 16-MHz crystal or resonator
     * XCAP_1 -- ~6 pF
     */
    BCSCTL3 = XT2S_0 + LFXT1S_2 + XCAP_1;
}

