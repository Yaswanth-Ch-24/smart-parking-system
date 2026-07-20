#include "parking.h"
#include "main.h"

static uint8_t slots[TOTAL_SLOTS + 1]; /* 1=free 0=occupied, index 1-3 */
static uint8_t available;

static const GPIO_TypeDef* greenPorts[] = {NULL, SLOT1_GREEN_PORT, SLOT2_GREEN_PORT, SLOT3_GREEN_PORT};
static const uint16_t      greenPins[]  = {0,    SLOT1_GREEN_PIN,  SLOT2_GREEN_PIN,  SLOT3_GREEN_PIN};
static const GPIO_TypeDef* redPorts[]   = {NULL, SLOT1_RED_PORT,   SLOT2_RED_PORT,   SLOT3_RED_PORT};
static const uint16_t      redPins[]    = {0,    SLOT1_RED_PIN,    SLOT2_RED_PIN,    SLOT3_RED_PIN};

void Parking_Init(void)
{
    available = TOTAL_SLOTS;
    for (uint8_t i = 1; i <= TOTAL_SLOTS; i++)
    {
        slots[i] = 1;
        Parking_SetSlotLED(i, 1); /* All green at start */
    }
}

uint8_t Parking_GetAvailable(void) { return available; }

uint8_t Parking_AssignSlot(void)
{
    for (uint8_t i = 1; i <= TOTAL_SLOTS; i++)
    {
        if (slots[i] == 1)
        {
            slots[i] = 0;
            available--;
            return i;
        }
    }
    return 0;
}

uint8_t Parking_ReleaseSlot(void)
{
    for (uint8_t i = 1; i <= TOTAL_SLOTS; i++)
    {
        if (slots[i] == 0)
        {
            slots[i] = 1;
            available++;
            return i;
        }
    }
    return 0;
}

void Parking_SetSlotLED(uint8_t slot, uint8_t free)
{
    if (slot < 1 || slot > TOTAL_SLOTS) return;
    HAL_GPIO_WritePin((GPIO_TypeDef*)greenPorts[slot], greenPins[slot],
                      free ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin((GPIO_TypeDef*)redPorts[slot],   redPins[slot],
                      free ? GPIO_PIN_RESET : GPIO_PIN_SET);
}
