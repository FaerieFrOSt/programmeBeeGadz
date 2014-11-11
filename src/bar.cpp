#include "bar.h"

Bar::Bar(Printer *printer, NfcDevice *device, Mysql *sql) : Mode(printer, device, sql)
{}

Bar::~Bar()
{}

bool	Bar::run()
{
	Card	*card = nullptr;
	bool	end = false;
	/* bool	admin = false; */
	uint8_t	uid[] = {0xd2, 0x3f, 0x77, 0x5b};
	m_printer->printInfo("Bar is running\n");
	while (!end)
	{
		bool isSame = true;
		card = m_device->findCard();
		for (size_t i = 0; i < card->sizeUid(); ++i)
			if (card->getUid()[i] != uid[i])
			{
				isSame = false;
				break;
			}
		if (isSame)
			end = true;
	}
	return true;
}
