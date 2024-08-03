//Modify this file to change what commands output to your statusbar, and recompile using the make command.
static const Block blocks[] = {
	/*Icon*/	/*Command*/		/*Update Interval*/	/*Update Signal*/
	{"󰎇 ",  "/home/beau/.local/share/dwmblocks/scripts/mpdsong", 1,		0},
    {"󰕾 ",  "/home/beau/.local/share/dwmblocks/scripts/volume",  1,     0},
    //{" ",  "/home/beau/.local/share/dwmblocks/scripts/kernel",  1000,     0},
    {"󰍛 ",  "/home/beau/.local/share/dwmblocks/scripts/cpu",     1,     0},
    {"󰘚 ",  "/home/beau/.local/share/dwmblocks/scripts/ram",     1,     0},
    //{"󰅟  ", "/home/beau/.local/share/dwmblocks/scripts/weather", 120,     0},
    {"󰥔 ",  "/home/beau/.local/share/dwmblocks/scripts/clock",   1,     0},

};

//sets delimeter between status commands. NULL character ('\0') means no delimeter.
static char delim[] = "   ";
//static char delim[] = "  ";
static unsigned int delimLen = 5;
