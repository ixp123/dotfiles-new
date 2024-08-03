static const char *colorname[] = {
	/* 8 normal colors */
	"#282828",
	"#cc6666",
	"#b5bd68",
	"#f0c674",
	"#81a2be",
	"#fe8989",
	"#a1c8ea",
	"#373b41",

	/* 8 bright colors */
	"#282828",
	"#cc6666",
	"#b5bd68",
	"#f0c674",
	"#81a2be",
	"#fe8989",
	"#a1c8ea",
	"#373b41",

	[255] = 0,

	/* more colors can be added after 255 to use with DefaultXX */
	"#cccccc",
	"#555555",
	"#c5c8c6", /* default foreground colour */
	"#101112", /* default background colour */
};

unsigned int defaultfg = 258; 
unsigned int defaultbg = 259;
unsigned int defaultcs = 256; 
unsigned int defaultrcs = 257; 
