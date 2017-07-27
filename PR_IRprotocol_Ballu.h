
#include "PR_IRtranceiverESP8266noCarrieer.h"	//we need types declaration




class PR_IRprotocol_Ballu {
	//*******	SENDER	******		
	public:
		PR_IRprotocol_Ballu(PR_IRtranceiver *irTranceiver)	{
			_irTranceiver = irTranceiver;
		}
		~PR_IRprotocol_Ballu();
		bool	makeRaw(const uint16_t &payload, const uint16_t &payLenght, irBuferRaw_t &rawBufer, uint16_t &rawLenght);
		void	send(const uint16_t &payload, const uint16_t &payLenght);
		void	sendRaw(const irBuferRaw_t &rawBufer, const uint16_t &rawLenght);
	
	
	
	//*******	RECEIVER	******	
		bool	receive(int &payload, int lenght);
		void	onReceived();
		
		bool	decodeRaw(uint8_t &payload, uint16_t &payLenght, const irBuferRaw_t &rawBufer, const uint16_t &rawLenght);
		
		
	protected:
		PR_IRtranceiver		*_irTranceiver = null;
	
	private:
		//timings [us]
		const	irMicrosec_t	headerMark		= 2950;		//The duration of the Header:Mark
		const	irMicrosec_t	headerSpace		= 1750;		//The duration of the Header:Space
		const	irMicrosec_t	dataMark		= 500;		//duration of a Bit:Mark
		const	irMicrosec_t	dataOneSpace	= 1150;		//duration of a Bit:Space for 1's
		const	irMicrosec_t	dataZeroSpace	= 500;		//duration of a Bit:Space for 0's		
		//corrections
		const	int8_t			dataMarkCorr	= -0;		//+/- correction [us] to the duration for send()
		const	int8_t			dataSpaceCorr	= -0;		//+/- correction [us] to the duration for send()
		//duration tolerance limit for decoding [us]
		const	int8_t			durationTolerance	= 50;	
		
}


void swapbits(byte x) {
    byte tmp=0;
    if (x & 0b00000001) tmp|=0b10000000;
    if (x & 0b00000010) tmp|=0b01000000;
    if (x & 0b00000100) tmp|=0b00100000;
    if (x & 0b00001000) tmp|=0b00010000;    
    if (x & 0b00010000) tmp|=0b00001000;
    if (x & 0b00100000) tmp|=0b00000100;
    if (x & 0b01000000) tmp|=0b00000010;
    if (x & 0b10000000) tmp|=0b00000001;
    return tmp;     
}