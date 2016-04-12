#include "transmit.h"

void InitializeTransmit(void)
{
	SET_TR_AS_AN_OUTPUT;
	SET_TX_DATA_AS_AN_OUTPUT;
}

//This routine manages the actual transmitter and is called every 500uS by a periodic interrupt.
void Xmit(TransmitterData* TData)
{
	enum XmitClockPhase Phase;

	if (TData->Transmit_Clock_Phase == Low) {
		TData->Transmit_Clock_Phase = High;
	}
	else {
		TData->Transmit_Clock_Phase = Low;
	}
	Phase = TData->Transmit_Clock_Phase;

	// Now, begin the transmitter state machine.
	switch(TData->Transmitter_State) {

		case StartBit :
			switch(Phase) {
				case Low :

				break ;
				case High :

				break ;
			}
		break ;

		case NormalXmit :
			switch(Phase) {
				case Low :

				break ;

				case High :

				break ;
			}

		break ;
		case InterWord :
			switch(Phase) {
				case Low :

				break ;
				case High :

				break ;
			}

		break ;
		default :
			TData->Transmitter_State = StartBit ;
			//Other intitialization here.....
		break ;

	}
}
