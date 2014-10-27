#include "nfc_driver.h"
#include "print_driver.h"

int main()
{
	Printer p(true);
	NfcDevice   nfc(&p);
	Card		*card;

	card = nfc.findCard();
	uint8_t **sector = card->read(1);
	return 0;
}
