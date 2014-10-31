#include "nfc_driver.h"
#include "print_driver.h"
#include <iostream>

int main()
{
	Printer p(true);
	NfcDevice   nfc(&p);
	Card		*tmp = nullptr;

	tmp = nfc.findCard();
	tmp->readSector(1);
	return 0;
}
