/*
 * —≠ª∑»ﬂ”‡–£—È¬Î£®Cyclic Redundancy Check£©
 *
 *				songjun 2013.2.8
 */

#ifndef SHARED_CRC_H
#define SHARED_CRC_H

#include <stdint.h>

// crc16
uint16_t make_crc16(char * buff, uint32_t size);

// crc32
void make_crc32_table();
uint32_t make_crc32(char *string, uint32_t size);


#endif	//SHARED_CRC_H
