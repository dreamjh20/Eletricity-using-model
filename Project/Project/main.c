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


int fnds(int count){
	unsigned char fnd[10] = {0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xd8, 0x80, 0x90};
	
	int i = count / 1000;
	int j = count / 100 % 10;
	int k = count / 10 % 10;
	int m = count % 10;

	for(int h = 0; h < 5; h++){
		PORTB=fnd[i];
		PORTD=0x08; //왼쪽에서 첫번째 FND 지정
		_delay_ms(1);
		PORTB=fnd[j];
		PORTD=0x04; //두번째 FND 지정
		_delay_ms(1);
		PORTB=fnd[k];
		PORTD=0x02; //세번째 FND 지정
		_delay_ms(1);
		PORTB=fnd[m];
		PORTD=0x01; //네번째 FND 지정
		_delay_ms(1);
	}
	
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
	int leds[7] = {0, 0, 0, 0, 0, 0, 0};
	unsigned int t = 1;
	unsigned int count = 0;
	unsigned int countL = 0;
	DDRC = 0xff; //lcd
	DDRE = 0xFF; //led
	DDRA = 0xFF;
	PORTA = 0x00;
	PORTC = 0x00; //lcd
	DDRE = 0x00; //스위치
	
	
	DDRB=0xFF;
	DDRD=0x0F;
	PORTB=0xFF;
	PORTD=0x0F;
	
	LCD_INIT(); //lcd초기
	
	while(1)
	{
		/*
		for(digit=0;digit<10;digit++)
		{
			PORTD=fnd_position;
			fnd_position<<=1;
			
			if(fnd_position==0x10)
			fnd_position=0x01;
			
			PORTB=fnd[digit];
			_delay_ms(1000);
		}*/
		
		
		sw7=0;
		countL = 0;
		//스위치
		if((PINE&0x01) == 0x00) {
			sw1 = 1 - sw1;
			count++;
		}
		if((PINE&0x02) == 0x00) {
			sw2 = 1 - sw2;
			count++;
		}
		if((PINE&0x04) == 0x00) {
			sw3 = 1 - sw3;
			count++;
		}
		if((PINE&0x08) == 0x00) {
			sw4 = 1 - sw4;
			count++;
		}
		if((PINE&0x10) == 0x00) {
			sw5 = 1 - sw5;
			count++;
		}
		if((PINE&0x20) == 0x00) {
			sw6 = 1 - sw6;
			count++;
		}
		if((PINE&0x40) == 0x00) {
			sw7 = 1;
			count++;
		}
		
		
		if(sw1 == 1){
			leds[0] = 1;
			PORTA|=0x01;
			//_delay_ms(t);
		}
		
		else if(sw1 == 0) {
			leds[0] = 0;
			PORTA &= 0xFE;
			//_delay_ms(t);
		}
		
		if(sw2 == 1){
			leds[1] = 1;
			PORTA|=0x02;
			//_delay_ms(t);
		}
		else if(sw2 == 0) {
			leds[1] = 0;
			PORTA &= 0xfd;
			//_delay_ms(t);
		}
		
		
		if(sw3 == 1){
			leds[2] = 1;
			PORTA |= 0x04;
			//_delay_ms(t);
		}
		else if(sw3 == 0) {
			leds[2] = 0;
			PORTA &= 0xFB;
			//_delay_ms(t);
		}
		if(sw4 == 1){
			leds[3] = 1;
			PORTA |= 0x08;
			//_delay_ms(t);
		}
		else if(sw4 == 0) {
			leds[3] = 0;
			PORTA &= 0xF7;
			//_delay_ms(t);
		}
		if(sw5 == 1){
			leds[4] = 1;
			PORTA |= 0x10;
			//_delay_ms(t);
		}
		else if(sw5 == 0) {
			leds[4] = 0;
			PORTA &= 0xEF;
			//_delay_ms(t);
		}
		if(sw6 == 1){
			leds[5] = 1;
			PORTA |= 0x20;
			//_delay_ms(t);
		}
		else if(sw6 == 0) {
			leds[5] = 0;
			PORTA &= 0xDF;
			//_delay_ms(t);
		}
		//led off switch
		if(sw7 == 1){
			sw1 = 0;
			sw2 = 0;
			sw3 = 0;
			sw4 = 0;
			sw5 = 0;
			sw6 = 0;
		}
		
		fnds(count);
		
		/*if(check(leds) == 1){ //상태 led 켜기
			PORTA = 0xFF; //A8
		} else{ //상태 led 끄기
			PORTA = 0xFF;
		}*/
		
		for(int i = 0; i < 8; i++){
			if(leds[i] == 1) {
				PORTA = 0x40;
				//_delay_ms(t);
				break;
			}
			else PORTA = 0x00;
		}
		
		//lcd에 on/off 출력
		for(int l = 5; l >= 0; l--){
			
			MOVE(1,11-l);
			if(leds[l] == 1){
				DATA('O');
				countL++;
			}
			else{
				DATA('X');
			}
		}
		
		MOVE(2, 1);
		DATA('U');
		DATA('s');
		DATA('i');
		DATA('n');
		DATA('g');
		DATA(' ');
		DATA('E');
		DATA('l');
		DATA('e');
		DATA('c');
		DATA(':');
		MOVE(2, 13);
		DATA('0');
		DATA('.');
		DATA(countL+0x30);
		DATA('W');
		_delay_ms(70);
		COMMAND(0b00000001);		
	}	
}