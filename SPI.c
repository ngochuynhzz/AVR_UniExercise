//Nhấn button1 master truyền đến slave giá trị "9" tức 09h
//Nhấn button2 master truyền đến slave giá trị "134" tức 86h
//Giá trị xung clock Fosc/16
#define F_CPU 1000000UL  // fosc = 1MHz
#include <avr/io.h>
#include <util/delay.h>

// Định nghĩa các chân SPI
#define SS        PB0
#define SCK_PIN   PB1
#define MOSI_PIN  PB2
#define MISO_PIN  PB3

// Hàm gửi dữ liệu SPI
void SPI_Send(uint8_t data) {
	SPDR = data;  // Gửi data đi
	// Chờ cho đến khi data được gửi xong
	while (bit_is_clear(SPSR, SPIF)) { }
}

int main(void) {
	// PD0, PD1 là input
	DDRD &= ~((1 << PD0) | (1 << PD1));
	// Điện trở nội kéo lên
	PORTD |= (1 << PD0) | (1 << PD1);

	// SPI Master
	// SCK_PIN, MOSI_PIN, SS là output
	DDRB |= (1 << SCK_PIN) | (1 << MOSI_PIN) | (1 << SS);

	// Enable SPI, bit order MSB, chọn Master, mode 0, fosc/16
	SPCR |= (1 << SPE) | (1 << MSTR) | (1 << SPR0);

	// Set bit để Inactive Slave khi chưa giao tiếp
	PORTB |= (1 << SS);

	while (1) {
		if (bit_is_clear(PIND, 0)) {
			PORTB &= ~(1 << SS);     // Active Slave
			SPI_Send(9);            // data = 00001001 = 09h
			_delay_ms(100);
			PORTB |= (1 << SS);     // Inactive Slave
		}

		if (bit_is_clear(PIND, 1)) {
			PORTB &= ~(1 << SS);     // Active Slave
			SPI_Send(134);         // data = 10000110 = 86h
			_delay_ms(100);
			PORTB |= (1 << SS);     // Inactive Slave
		}
	}
}
