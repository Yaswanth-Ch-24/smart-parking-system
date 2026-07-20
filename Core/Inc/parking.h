#ifndef PARKING_H
#define PARKING_H
#include <stdint.h>
void    Parking_Init(void);
uint8_t Parking_GetAvailable(void);
uint8_t Parking_AssignSlot(void);    /* Returns slot number 1-3 */
uint8_t Parking_ReleaseSlot(void);   /* Returns released slot number */
void    Parking_SetSlotLED(uint8_t slot, uint8_t free); /* free=1 Green, 0 Red */
#endif
