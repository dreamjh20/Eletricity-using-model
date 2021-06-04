#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

void COMMAND(unsigned char byte);
void DATA(unsigned char byte);
void LCD_INIT(void);

void STRING(unsigned char font[], unsigned char n)
{
	unsigned char i;
	for(i=0;i<n;i++)
	{
		DATA(font[i]);
	}
}

void MOVE(int y, int x)     //커서 이동
{
	unsigned char data;
	if(y==1) 	data=0x80+x-1;           //1행
	else     	data=0xc0+x-1;            //2행
	COMMAND(data);
}

void COMMAND(unsigned char byte)	// 명령어 함수
{
	_delay_ms(2);

	PORTC = byte&0xf0;		// 명령어 쓰기
	PORTC &= 0b11111110;		// RS = 0 명령어선택 설정
	PORTC &= 0b11111101;		// RW = 0 쓰기 설정
	_delay_us(1);           		// RS & RW setup time 40ns 지연
	PORTC |= 0b00000100;		// E = 1, lcd 동작
	_delay_us(1);          		// E pulse width time 230ns 지연
	PORTC &= 0b11111011;		// E = 0

	PORTC = (byte<<4)&0xf0;	// 명령어 쓰기
	PORTC &= 0b11111110;		// RS = 0 명령어선택 설정
	PORTC &= 0b11111101;		// RW = 0 쓰기 설정
	_delay_us(1);           		// RS & RW setup time 40ns 지연
	PORTC |= 0b00000100;		// E = 1, lcd 동작
	_delay_us(1);          		// E pulse width time 230ns 지연
	PORTC &= 0b11111011;		// E = 0
}

void LCD_INIT(void)			// 초기설정 함수
{
	_delay_ms(30);		// 전원 투입 후 30ms 이상 지연
	
	//Function set
	COMMAND(0b00101000);	// 인터페이스(DL)=0(4bit), 라인(N)=1(2라인), 폰트(F)=0(5*8 dot)	_delay_us(39);		// 39us 이상 지연

	//Display ON/OFF Control
	COMMAND(0b00001100);	// 화면 표시(D)=1(on), 커서(C)=0(off), 블링크(B)=0(off)	_delay_us(39);		// 39us 이상 지연

	//Clear Display
	COMMAND(0b00000001);	// 화면을 클리어하고 , 커서가 홈위치인 0번지로 돌아감.
	_delay_ms(1.53);		// 1.53ms 이상 지연

	//Entry Mode Set
	COMMAND(0b00000110);	// 커서방향(I/D)=1(address증가), 표시이동(S)=0(이동하지 않음)
}

void DATA(unsigned char byte)		// 데이터 함수x`
{
	_delay_ms(2);

	PORTC = byte&0xf0;		// 데이터 쓰기
	PORTC |= 0b00000001;		// RS = 1, 데이터를 쓰도록 설정
	PORTC &= 0b11111101;		// RW = 0, 데이터를 쓰도록 설정
	_delay_us(1);           		// RS & RW setup time 40ns 지연
	PORTC |= 0b00000100;		// E = 1, lcd 동작
	_delay_us(1);          		// E pulse width time 230ns 지연
	PORTC &= 0b11111011;		// E = 0

	PORTC = (byte<<4)&0xf0;	// 데이터 쓰기
	PORTC |= 0b00000001;		// RS = 1, 데이터를 쓰도록 설정
	PORTC &= 0b11111101;		// RW = 0, 데이터를 쓰도록 설정
	_delay_us(1);           		// RS & RW setup time 40ns 지연
	PORTC |= 0b00000100;		// E = 1, lcd 동작
	_delay_us(1);          		// E pulse width time 230ns 지연
	PORTC &= 0b11111011;		// E = 0
}

int check(int leds[]){
	for(int i = 0; i < 8; i++){
		if(leds[i] == 1){
			return 0;
		}
	}
	return 1;
}

int main(void)
{
	unsigned char sw1=0;
	unsigned char sw2=0;
	unsigned char sw3=0;
	unsigned char sw4=0;
	unsigned char sw5=0;
	unsigned char sw6=0;
	unsigned char sw7=0;
	unsigned char sw8=0;
	int leds[7] = {0, 0, 0, 0, 0, 0, 0};
	unsigned int t = 5;
	DDRC = 0xff; //lcd
	DDRE = 0xFF; //led
	DDRA = 0xFF;
	PORTA = 0x00;
	PORTC = 0x00; //lcd
	DDRE = 0x00; //스위치
	
	LCD_INIT(); //lcd초기
	while(1)
	{
		//스위치
		if((PINE&0x01) == 0x00) {
			sw1 = 1 - sw1;
		}
		if((PINE&0x02) == 0x00) {
			sw2 = 1 - sw2;
		}
		if((PINE&0x04) == 0x00) {
			sw3 = 1 - sw3;
		}
		if((PINE&0x08) == 0x00) {
			sw4 = 1 - sw4;
		}
		if((PINE&0x10) == 0x00) {
			sw5 = 1 - sw5;
		}
		if((PINE&0x20) == 0x00) {
			sw6 = 1 - sw6;
		}
		if((PINE&0x40) == 0x00) {
			sw7 = 1 - sw7;
		}
		
		if(sw1 == 1){
			leds[0] = 1;
			PORTA=0x01;
			_delay_ms(t);
		}
		else if(sw1 == 0) {
			leds[0] = 0;
			PORTA = 0x00;
			_delay_ms(t);
		}	
		
		if(sw2 == 1){
			leds[1] = 1;
			PORTA=0x02;
			_delay_ms(t);
		}
		else if(sw2 == 0) {
			leds[1] = 0;
			PORTA = 0x00;
			_delay_ms(t);
		}
		
		if(sw3 == 1){
			leds[2] = 1;
			PORTA = 0x04;
			_delay_ms(t);
		}
		else if(sw3 == 0) {
			leds[2] = 0;
			PORTA = 0x00;
			_delay_ms(t);
		}
		if(sw4 == 1){
			leds[3] = 1;
			PORTA = 0x08;
			_delay_ms(t);
		}
		else if(sw4 == 0) {
			leds[3] = 0;
			PORTA = 0x00;
			_delay_ms(t);
		}
		if(sw5 == 1){
			leds[4] = 1;
			PORTA = 0x10;
			_delay_ms(t);
		}
		else if(sw5 == 0) {
			leds[4] = 0;
			PORTA = 0x00;
			_delay_ms(t);
		}
		if(sw6 == 1){
			leds[5] = 1;
			PORTA = 0x20;
			_delay_ms(t);
		}
		else if(sw6 == 0) {
			leds[5] = 0;
			PORTA = 0x00;
			_delay_ms(t);
		}
		if(sw7 == 1){
			leds[6] = 1;
			PORTA = 0x40;
			_delay_ms(t);
		}
		else if(sw7 == 0) {
			leds[6] = 0;
			PORTA = 0x00;
			_delay_ms(t);
		}
		
		if(check(leds) == 1){ //상태 led 켜기
			PORTA = 0x80; //A8
		} else{ //상태 led 끄기
			PORTA = 0x00;
		}
	}	
}