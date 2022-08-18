

#define REG_DISPCTL (*(volatile unsigned short *)0x4000000)
#define MODE3 (3)
#define BG2 (1<<10)

volatile unsigned short *videoBuffer = (unsigned short *)0x6000000;
int main(int argc, char *argv[]) {
	REG_DISPCTL = MODE3 | BG2;

	videoBuffer[1208] = 0x7fff;


}





