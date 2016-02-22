#include "crc.h"

uint32_t crc32_table[256];

uint16_t make_crc16(char * buff, uint32_t size)
{
	uint16_t crctemp, crc = 0xffff;
	uint16_t i, j;
	for (i = 0; i<size; ++i)
	{
		crc ^= buff[i];
		for (j = 0; j<8; ++j)
		{
			crctemp = crc;
			crc >>= 1;
			if (crctemp & 0x0001)
				crc ^= 0xa001;
		}
	}
	return crc;
}

void make_crc32_table()
{
	uint32_t c;
	int i = 0;
	int bit = 0;

	for (i = 0; i < 256; i++)
	{
		c = (uint32_t)i;

		for (bit = 0; bit < 8; bit++)
		{
			if (c & 1) {
				c = (c >> 1) ^ (0xEDB88320);
			} else {
				c = c >> 1;
			}
		}
		crc32_table[i] = c;
	}
}

uint32_t make_crc32(char *string, uint32_t size)
{
	uint32_t crc = 0xffffffff;
	while (size--)
		crc = (crc >> 8) ^ (crc32_table[(crc ^ *string++) & 0xff]);

	return crc;
}
