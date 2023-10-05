#ifndef __WALKMOTOR_H
#define __WALKMOTOR_H

#include "gpio.h"
#include "main.h"

enum dir{Pos,Neg};
void stepper(uint8_t dir,int speed);
                                                  

#endif
