#include <unistd.h>
#include <termios.h>
#include <stdlib.h>
struct termios org_terminos;
void disableRawMode(){
 tcsetattr(STDIN_FILENO,TCSAFLUSH,&org_terminos);
}
void enableRawMode(){
  
    tcgetattr(STDIN_FILENO,&org_terminos);
    atexit(disableRawMode);
    struct termios raw = org_terminos;
    raw.c_lflag &= ~(ECHO | ICANON);
    tcsetattr(STDERR_FILENO,TCSAFLUSH,&raw);
}
int main(){
    enableRawMode();
    char c;
    while(read(STDIN_FILENO,&c,1)==1 && c != 'q');
    return 0;
}