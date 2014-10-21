#include "nfc_driver.h"
#include "print_driver.h"

int main()
{
	Printer p(true);
	NfcDevice   nfc(&p);
	/* uint8_t     data[4] = {0xda, 0xbb, 0xcc, 0xdd }; */

	if (!nfc.init())
		return 1;
	nfc.findCard();
	nfc.readCard(0);
	return 0;
}
