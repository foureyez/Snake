/*
 *  Terminal.c
 *  CSOM
 *
 *
 */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>


/*************************************************/
#pragma mark * Included Headers                  *
/*************************************************/


#include <vmobjects/VMObject.h>
#include <vmobjects/VMFrame.h>
#include <vmobjects/VMString.h>
#include <vm/Universe.h>


#include <termios.h>
#include <fcntl.h>

/*************************************************/
#pragma mark * Primitive Foreward Declaration    *
/*************************************************/

void _Terminal_sleepFor_(pVMObject object, pVMFrame frame);
void _Terminal_getChar(pVMObject object, pVMFrame frame);
void _Terminal_uninit(pVMObject object, pVMFrame frame);
void _Terminal_init(pVMObject object, pVMFrame frame);


/*************************************************/
#pragma mark * Internal functions and init.      *
/*************************************************/

/*** Lib initialization **/
#ifdef __GNUC__
void init(void) __attribute__((constructor));
void fini(void) __attribute__((destructor));
#elif
void _init(void);
void _fini(void);
#pragma init _init
#pragma fini _fini
#endif

#ifdef __GNUC__
void init(void)
#elif
void _init(void)
#endif
{
	// Call init funcions.
	

}

#ifdef __GNUC__
void fini(void)
#elif
void _fini(void)
#endif
{
	
}

// Classes supported by this lib.
static char *supported_classes[] = {
    "Terminal",
    NULL
};


/*************************************************/
#pragma mark * Exported functions starting here  *
/*************************************************/

// returns, whether this lib is responsible for a specific class
bool		supports_class(const char* name) {
	
	char **iter=supported_classes;
	while(*iter)
		if (strcmp(name,*iter++)==0)
			return true;
	return false;
	
}



/*************************************************/
/*************************************************/
/*************************************************/
#pragma mark * Primitive Implementatition here   *
/*************************************************/
/*************************************************/
/*************************************************/

int terminalStream;
struct termios old_tty;

void Terminal_getChar(pVMObject object, pVMFrame frame) {
  char chr;
  char result[2];
  pString str = NULL;
  pVMObject vmStr = NULL;
  
  pVMObject self __attribute__((unused)) = SEND(frame, pop);
  
  if (read(terminalStream, &chr, sizeof(chr)) > 0) {
	  result[0] = chr;
	  result[1] = 0;
	  
	  str = String_new(result);
	  vmStr = (pVMObject)Universe_new_string(str);
	  SEND(frame, push, vmStr);
  } else {
	  SEND(frame, push, nil_object);
  }
}

void Terminal_uninit(pVMObject object, pVMFrame frame) {
	close(terminalStream);
	tcsetattr(0, TCSANOW, &old_tty);
}

void Terminal_init(pVMObject object, pVMFrame frame) {
	struct termios tty;
	
	// Perpare terminal settings and change to non-canonical mode for char-wise input
	tcgetattr(0, &old_tty);
	tty = old_tty;
	tty.c_lflag = tty.c_lflag & ~(ECHO | ECHOK | ICANON);
	tty.c_cc[VTIME] = 1;
	tcsetattr(0, TCSANOW, &tty);
	
	terminalStream = open("/dev/tty", O_RDONLY | O_NONBLOCK);
	if (terminalStream < 0) {
		Universe_error_exit("Could not open /dev/tty for read\n");
	}
}

void Terminal_sleepFor_(pVMObject object, pVMFrame frame) {
    pVMInteger miliSeconds = (pVMInteger)SEND(frame, pop);
    int64_t sec = (int64_t)SEND(miliSeconds, get_embedded_integer) * 1000;
    sync();
    usleep(sec);
}

/*************************************************/
/*************************************************/
/*************************************************/
#pragma mark * EOF                               *
/*************************************************/
/*************************************************/
/*************************************************/

