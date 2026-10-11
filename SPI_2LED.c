#define F_CPU 1000000UL  // fosc = 1MHz
#include <avr/io.h>
#include <util/delay.h>

// Định nghĩa các chân SPI
#define SS         PB0
#define SCK_PIN    PB1
#define MOSI_PIN   PB2
#define MISO_PIN   PB3

uint8_t data;

// Hàm nhận dữ liệu SPI
char SPI_Receive(void) {
	// Chờ cho đến khi nhận xong dữ liệu thì SPIF = 1
	while (bit_is_clear(SPSR, SPIF)) {}
	// Dữ liệu nhận được lưu vào thanh ghi SPDR
	return SPDR;
}

int main(void) {
	// PA0, PA1 là output (LED điều khiển)
	DDRA |= (1 << PA0) | (1 << PA1);

	// SPI Slave
	DDRB = (1 << MISO_PIN);  // MISO là output

	// Enable SPI, bit order MSB, chọn Slave, mode 0, fosc/16
	SPCR = (1 << SPE) | (1 << SPR0);

	while (1) {
		data = SPI_Receive();  // gán SPDR vào data

		if (data == 1) {
			PORTA |= (1 << PA0);    // sáng LED 1
			PORTA &= ~(1 << PA1);   // tắt LED 2
		}
		else if (data == 2) {
			PORTA &= ~(1 << PA0);   // tắt LED 1
			PORTA |= (1 << PA1);    // sáng LED 2
		}
	}
}
