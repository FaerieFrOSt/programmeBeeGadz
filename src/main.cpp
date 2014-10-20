#include "nfc_driver.h"
#include "print_driver.h"

int main()
{
	Printer p(true);
	NfcDevice   nfc(&p);

	if (!nfc.init())
		return 1;
	nfc.findCard();
	nfc.readCard();
	return 0;
}
