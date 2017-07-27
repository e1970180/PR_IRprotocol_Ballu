#include "PR_IRprotocol_Ballu.h"


//**********************************************************  SENDER	*****************************************	

bool	PR_IRprotocol_Ballu::makeRaw(const uint16_t &payload, const uint16_t &payLenght, _ 
									 irBuferRaw_t &rawBufer, uint16_t &rawLenght) {
	
	int16_t			countRaw = 0;
	uint8_t			data;
	// Header
	if ( rawLenght < (2 + payLenght *2 * 8) ) return false;
	
	rawBufer[countRaw++] = headerMark;
	rawBufer[countRaw++] = headerSpace;

	// Data
	for (int i = 0;  i < payLenght;  i++) {
	
		data = payload[i];
		mask = 0b00000001;
		for (uint8_t j = 0;  j < 8;  j++) {	//throw bits in byte
			rawBufer[countRaw++] = dataMark;
			if (data & mask)  rawBufer[countRaw++] = dataOneSpace;
				else          rawBufer[countRaw++] = dataZeroSpace;
			mask = mask << 1;
		}
	} //for(i)
	//footer
		//empty
	rawLenght = countRaw;	
}

void	PR_IRprotocol_Ballu::send(const uint16_t &payload, const uint16_t &payLenght) {
	
	//int16_t			countRaw = 0;
	uint8_t			data;
	
	// Header
	_irTranceiver->sendMark(headerMark + dataMarkCorr);
	_irTranceiver->sendSpace(headerSpace + dataSpaceCorr);

	// Data
	for (int i = 0;  i < payLenght;  i++) {

		data = payload[i];
		mask = 0b00000001;
		for (uint8_t j = 0;  j < 8;  j++) {	//throw bits in byte
			_irTranceiver->sendMark(dataMark + dataMarkCorr);
			if (data & mask)  _irTranceiver->sendSpace(dataOneSpace + dataMarkCorr);
				else          _irTranceiver->sendSpace(dataZeroSpace + dataMarkCorr);
			mask = mask << 1;
		}
	} //for(i)
	//footer
		//empty
}

void	PR_IRprotocol_Ballu::sendRaw(const irBuferRaw_t &rawBufer, const uint16_t &rawLenght) {
	
	_irTranceiver->sendRaw(rawBufer, rawLenght);
}

//********************************************* 	RECIVER		*********************************
bool	PR_IRprotocol_Ballu::receive(int &payload, int lenght);
void	PR_IRprotocol_Ballu::onReceived();

bool	PR_IRprotocol_Ballu::decodeRaw(uint8_t &payload, uint16_t &payBuferLenght, _ 
									   const irBuferRaw_t &rawBufer, const uint16_t &rawLenght) {

	int16_t			countRaw = 0;
	//Header
	if ( !isMatch(headerMark,  rawBufer[countRaw++]) ) return false;
	if (rawLenght < countRaw) return false;
	if ( !isMatch(headerSpace, rawBufer[countRaw++]) ) return false;	
	if (rawLenght < countRaw) return false;
	
	if ( payBuferLenght < (rawLenght - countRaw)/16) )  return false; //too short bufer
	
    // Data
	for (int i = 0;  i < (rawLenght - countRaw)/16;  i++) { // (rawBufer - countRaw)/16 is number of bytes in payload
		data=0;
		for (int j = 0;  j < 8;  j++) {
			if ( !isMatch(dataMark, rawBufer[countRaw++]) )  return false;
			
			if ( isMatch(dataOneSpace, rawBufer[countRaw++]) ) 			data = (data >> 1) | 0b10000000 ;
			else if ( isMatch(dataZeroSpace, rawBufer[countRaw++]) ) 	data = (data >> 1) | 0b00000000 ;
			else 	 return false;
			
			if      (MATCH_SPACE(results->rawbuf[offset],BALLU_ONE_SPACE ))  data = (data >> 1) | 0b10000000 ;
			else if (MATCH_SPACE(results->rawbuf[offset],BALLU_ZERO_SPACE))  data = (data >> 1) | 0b00000000 ;
			else                                                                 return false ;
			offset++;
    }
	results->valueBallu[j]       = data;

									   
}

bool	isMatch(const irMicrosec_t reference, const irMicrosec_t data)	{
	//bool match = (abs((int16_t)(reference - data)) <= durationTolerance);
	bool match = ( (data >= (reference - durationTolerance) ) && (data <= (reference + durationTolerance) );
	return match;
}
