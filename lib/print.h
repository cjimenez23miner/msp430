#ifndef __PRINT_H__
#define __PRINT_H__

void print_init(void (*send_handler)(char *str, unsigned int len), 
				void (*send_byes_handler)(char ch));
void print(char* str);
void printInt(unsigned long int num);

#endif
