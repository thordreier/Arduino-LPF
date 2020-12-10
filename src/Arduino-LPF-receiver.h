/*
 * https://github.com/thordreier/Arduino-LPF
 */

#ifndef Arduino_LPF_receiver_h
#define Arduino_LPF_receiver_h

#include <Arduino.h>
#include <stdlib.h>
#include "Arduino-LPF-common.h"
#include "Arduino-LPF-types.h"
#include "Arduino-LPF-values.h"

#define LPF_QUEUE_SIZE 8

void lpfReceiverSetPin(byte pin);
void lpfReceiverSetSanitize(byte sanitize);
lpf_msg lpfReceiverGetNextMessage();
void lpfReceiverInterruptHandler();

#endif  // Arduino_LPF_receiver_h
