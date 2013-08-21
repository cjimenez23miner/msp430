#include <print.h>

static void (*_print_send_handler)(char *str, unsigned int len);
static void (*_print_send_byes_handler)(char ch);

void print_init(void (*send_handler)(char *str, unsigned int len), 
				void (*send_byes_handler)(char ch))
{
	_print_send_byes_handler = send_byes_handler;
	_print_send_handler = send_handler;
}

void print(char* str)
{
	int len = 0;
	while(*(str+len) != '\0'){
		len++;
	}
	(*_print_send_handler)(str, len);
}

void printInt(unsigned long int num)
{
	unsigned int num_digit = 0;
	unsigned char digits[8] = {0};
	int i;
	
	if(num == 0){
		(*_print_send_byes_handler)('0');
		return;
	}

	while(num){
		digits[num_digit] = num&0xf;
		num >>= 4;
		num_digit++;
	}

	for(i=num_digit-1; i>=0; i--){
		unsigned char c = '?';
		if (digits[i]<=9) {
			c = digits[i] + '0';
		}else if (digits[i] >= 10 && digits[i]<=15) {
			c = digits[i] - 10 + 'A';
		}
		
		(*_print_send_byes_handler)(c);
	}
}
