#include "mode.h"

Mode::Mode(Printer *printer, NfcDevice *device, Mysql *sql) : m_printer(printer),
	m_device(device), m_sql(sql)
{}

Mode::~Mode()
{}
