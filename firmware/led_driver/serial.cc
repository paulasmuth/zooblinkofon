#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <string.h>
#include "serial.h"

namespace iot9000 {
namespace avr {

SerialPort::SerialPort() {
  UBRR0L = 0x19;
  UBRR0H = 0;
	UCSR0B = (1 << RXEN0);
	UCSR0C = (3 << UCSZ00);
}

bool SerialPort::recvPacket(uint8_t* pkt, uint16_t len) {
  for (uint8_t i = 0; i < len; ++i) {
    if (!recv(&pkt[i])) {
      return false;
    }
  }

  return true;
}

bool SerialPort::recv(uint8_t* byte) {
  for (uint32_t i = 0; i < F_CPU / 16384; ++i) {
    if (UCSR0A & (1 << RXC0)) {
      *byte = UDR0;
      return true;
    }
  }

  return false;
}

bool SerialPort::recvNonblock(uint8_t* byte) {
  if (hasPendingData()) {
    *byte = UDR0;
    return true;
  } else {
    return false;
  }
}

bool SerialPort::hasPendingData() const {
  return UCSR0A & (1 << RXC0);
}

void SerialPort::wait(uint32_t t) const {
  for (uint32_t i = 0; i < t * (F_CPU / 16384) && !(UCSR0A & (1 << RXC0)); ++i);
}

} // namespace avr
} // namespace iot9000
