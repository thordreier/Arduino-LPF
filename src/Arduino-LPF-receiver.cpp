/*
 * https://github.com/thordreier/Arduino-LPF
 */

#include "Arduino-LPF-receiver.h"
#include <Arduino.h>
#include <stdlib.h>

static volatile byte _pin = 0;
static volatile byte _sanitize = 0;
static volatile lpf_msg _lpf_msg_queue[LPF_QUEUE_SIZE] = {0};

void lpfReceiverSetPin(byte pin) {
  _pin = pin;
}

void lpfReceiverSetSanitize(byte sanitize) {
  _sanitize = sanitize;
}

lpf_msg lpfReceiverGetNextMessage() {
  static byte queue_ptr = 0;
  lpf_msg msg = 0;
  if (_lpf_msg_queue[queue_ptr]) {
    // fixme: maybe the next two lines should be run with interrupts disabled
    msg = _lpf_msg_queue[queue_ptr];
    _lpf_msg_queue[queue_ptr] = 0;
    queue_ptr = (queue_ptr + 1) % LPF_QUEUE_SIZE;
    if (!lpfLrcCheck(msg)) {
      msg = 0;
    }
  }
  return msg;
}

void lpfReceiverInterruptHandler() {
  static unsigned long time_prev = 0;
  static unsigned long time_diff = 0;
  static byte irvalue_prev = 0;
  static byte transmitting_bitcount = 0;
  static lpf_msg transmitting_msg = 0;
  static byte queue_ptr = LPF_QUEUE_SIZE - 1;
  static byte start_bit_received = 0;

  // get time and pin value
  byte irvalue_new =
      _sanitize ? digitalRead(_pin)
                : !digitalRead(_pin);  // fixme: i don't know if I should remove
                                       // this sanitize stuff
  unsigned long time_new = micros();

  if (irvalue_prev != irvalue_new) {  // IR signal has changed
    if (irvalue_new) {                // IR signal has changed to from on to off
      // time between last change (from off to on) and this change (from on to
      // off) - we don't check any time here - it should be arount 160 micro
      // seconds
      time_diff = time_new - time_prev;
    } else {  // IR signal has changed from off to on
      // time between last time it change from off to on to this time it changed
      // from off to on - a full PF bit
      time_diff = time_new - time_prev + time_diff;

      // find out if it's low bit (0), high bit (1) or start stop bit
      if (start_bit_received && time_diff > LPF_IR_LOW_MIN &&
          time_diff < LPF_IR_LOW_MAX) {  // low bit range (0)
        transmitting_bitcount++;
        transmitting_msg = transmitting_msg << 1;
      } else if (start_bit_received && time_diff > LPF_IR_HIGH_MIN &&
                 time_diff < LPF_IR_HIGH_MAX) {  // high bit range (1)
        transmitting_bitcount++;
        transmitting_msg = (transmitting_msg << 1) | 0b1;
      } else if (time_diff > LPF_IR_STARTSTOP_MIN &&
                 time_diff < LPF_IR_STARTSTOP_MAX) {  // start stop bit range
        transmitting_bitcount = 0;
        transmitting_msg = 0;
        start_bit_received = 1;
      } else {  // something else
        transmitting_bitcount = 0;
        transmitting_msg = 0;
        start_bit_received = 0;
      }

      // We are cheating here. When we have received 16 bits we put it in the
      // queue without waiting for the last start/stop bit under test I had some
      // problems with not receiving the last SS-bit
      if (start_bit_received && transmitting_bitcount == LPF_MSG_BITCOUNT) {
        queue_ptr = (queue_ptr + 1) % LPF_QUEUE_SIZE;
        _lpf_msg_queue[queue_ptr] = transmitting_msg;
        transmitting_bitcount = 0;
        transmitting_msg = 0;
      }
    }

    // set "previous" values - used next thime the function is run
    time_prev = time_new;
    irvalue_prev = irvalue_new;
  }
}
