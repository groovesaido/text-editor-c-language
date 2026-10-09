#include <unistd.h>
#include <termios.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <errno.h>

#define CTRL_KEY(k) ((k) & 0x1f)
struct termios org_terminos;
//error checking
void die (const char *s){
    perror(s);
    exit(1);
}
void disableRawMode(){
 tcsetattr(STDIN_FILENO,TCSAFLUSH,&org_terminos);
 if (tcsetattr(STDIN_FILENO,TCSAFLUSH,&org_terminos) == -1) die("tcsetattr");
}
void enableRawMode(){
  
    tcgetattr(STDIN_FILENO,&org_terminos);
    if (tcgetattr(STDIN_FILENO,&org_terminos) == -1) die("tcgetattr");
    atexit(disableRawMode);
    struct termios raw = org_terminos;
    //input flags
    raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    //output flags
    raw.c_oflag &= ~(OPOST);
    //control flag
    raw.c_cflag |= (CS8);
    //local mode flag
    raw.c_lflag &= ~(ECHO | ICANON | ISIG | IEXTEN);
    //control characters
    raw.c_cc[VMIN] =0; //minimum
    raw.c_cc[VTIME] =1; //timer
    //set terminal setting
    tcsetattr(STDIN_FILENO,TCSAFLUSH,&raw);
    if (tcsetattr(STDIN_FILENO,TCSAFLUSH,&raw) == -1) die("tcsetattr");
}
char editorReadKey(){
    int nread;
    char c;
    while ((nread = read(STDIN_FILENO,&c,1)) !=1){
        if(nread == -1 && errno != EAGAIN) die("read");
    }
    return c;
    
}
void editorProcessrKeyPress(){
    char c = editorReadKey();
    switch (c)
    {
    case CTRL_KEY('q'):
        exit(0);
        break;
    
    default:
        break;
    }
}

int main(){
    enableRawMode();
   
    while(1){
       editorProcessrKeyPress();
    }
    return 0;
}