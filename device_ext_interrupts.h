/*
 * device_pin_change.h
 *
 * Created: 7/29/2013 8:14:35 PM
 *  Author: Greg Cook
 */ 


#ifndef DEVICE_EXT_INTERRUPTS_H_
#define DEVICE_EXT_INTERRUPTS_H_

#include "types.h"

typedef struct {
  register_t *PCMSKx;
  uint8_t PCIFx;
  uint8_t PCINTx;
  void (*callback)(void);
} exti_pc_t;


#endif /* DEVICE_PIN_CHANGE_H_ */
