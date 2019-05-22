/**
 ****************************************************************************************
 *
 * @file crc16.h
 *
 * @brief Header file of crc16.
 *
 * Copyright (C) Quintic 2014-2014
 *
 * $Rev: 5338 $
 *
 ****************************************************************************************
 */

#ifndef _CRC16_H_
#define _CRC16_H_

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
 
#include <stdint.h>


/*
 * GLOBAL VARIABLE DECLARATIONS
 ****************************************************************************************
 */

extern const uint16_t crc16_table[256];


/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */


extern uint16_t ycm_crc16( uint8_t * pucFrame, uint16_t usLen );
extern unsigned short CRC16BigEndian(uint8_t *data, int size);
#endif 


