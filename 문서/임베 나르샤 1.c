setup()
{
	//8 4 2 1   8 4 2 1
	//7 6 5 4 | 3 2 1 0
	//0 0 0 0 | 0 0 0 0
	DDRB = 0x20 | 0x10;// 따로 따로 넣으면 덮어쓰기로 되는 샘이니 or로 처리해준다
	//DDRB = 0x10;
}
char a;
loop()
{
	//digitalWrite(13, HIGH);
	PORTB = 0x20 | 0x10;//이것도 마찬가지
	delay(1000);
	//digitalWrite(13, LOW);
	//PORTB = 0x00;
	PORTB = PORTB & (~(0x20 | 0x10));//이것도
	delay(1000);

	a = PINB;
}