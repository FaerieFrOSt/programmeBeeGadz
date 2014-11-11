#include "print_driver.h"
#include "nfc_driver.h"
#include "card.h"

int	main()
{
	Printer		p(true);
	NfcDevice	device(&p);
	Card		*card;
	/* uint8_t		key[] = {0xe3, 0x22, 0x9a, 0x12, 0x6f, 0x10}; */

	card = device.findCard();
	card->readSector(1);
	/* (*card)[1][0][0] = 0xcc; */
	/* (*card)[1].useKeyB(true); */
	/* (*card)[1].setKeyB(key); */
	/* (*card)[1].setPermissions(3, 0, 1, 1); */
	/* card->writeSector(1); */
	return 0;
}
