/* Used to remember window positions and layouts after restartsig */
#define SESSION_FILE "/tmp/dwm-session"

/* Media keys */
#include <X11/XF86keysym.h>

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int swallowfloating    = 0;        /* 1 means swallow floating windows by default */
static const unsigned int gappih    = 20;       /* horiz inner gap between windows */
static const unsigned int gappiv    = 20;       /* vert inner gap between windows */
static const unsigned int gappoh    = 20;       /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = 20;       /* vert outer gap between windows and screen edge */
static const int smartgaps          = 0;        /* 1 means no outer gap when there is only one window */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const int user_bh            = 30;        /* 0 means that dwm will calculate bar height, >= 1 means dwm will user_bh as bar height */
static const int horizpadbar        = 6;        /* horizontal padding for statusbar */
static const int vertpadbar         = 10;        /* vertical padding for statusbar */
static const char *fonts[]          = { "Hack:size=8", "Hack Nerd Font:size=11" };
static const char dmenufont[]       = "Hack:size=8";
static const char col_gray1[]       = "#080808";
static const char col_gray2[]       = "#080808";
static const char col_gray3[]       = "#bdbdbd";
static const char col_gray4[]       = "#bdbdbd";
static const char col_cyan[]        = "#121212";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]  = { col_gray4, col_cyan,  col_cyan  },
    [SchemeStatus]  = { col_gray3, col_gray1,  "#080808"  }, // Statusbar right {text,background,not used but
    [SchemeTagsSel]  = { col_gray4, col_cyan,  "#080808"  }, // Tagbar left selected {text,background,not use
    [SchemeTagsNorm]  = { col_gray3, col_gray1,  "#080808"  }, // Tagbar left unselected {text,background,no
    [SchemeInfoSel]  = { col_gray4, col_gray1,  "#080808"  }, // infobar middle  selected {text,background,no
    [SchemeInfoNorm]  = { col_gray3, col_gray1,  "#080808"  }, // infobar middle  unselected {text,backgroun
};

static const char *const autostart[] = {
  "picom", NULL,
  "dwmblocks", NULL,
  "pulseaudio", NULL,
  "dunst", NULL,
  "mpd", NULL,
  "xwallpaper", "--zoom", "/home/beau/.local/wall.jpg", NULL,
  NULL /* terminate */
};

typedef struct {
	const char *name;
	const void *cmd;
} Sp;
const char *spcmd1[] = {"st", "-n", "spterm", "-g", "120x34", NULL };
const char *spcmd2[] = {"st", "-n", "spfm", "-g", "144x41", "-e", "ranger", NULL };
const char *spcmd3[] = {"keepassxc", NULL };
static Sp scratchpads[] = {
	/* name          cmd  */
	{"spterm",      spcmd1},
	{"spranger",    spcmd2},
	{"keepassxc",   spcmd3},
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };
static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Gimp",	  NULL,			NULL,		0,				1,			 -1 },
	{ NULL,		  "spterm",		NULL,		SPTAG(0),		1,			 -1 },
	{ NULL,		  "spfm",		NULL,		SPTAG(1),		1,			 -1 },
	{ NULL,		  "keepassxc",	NULL,		SPTAG(2),		0,			 -1 },


    /* class     instance       title                       tags mask  isfloating  isterminal  noswallow  monitor */
    { "Gimp",    NULL,          NULL,                       0,         1,          0,           0,        -1 },
    { "St",      NULL,          NULL,                       0,         0,          1,           0,        -1 },
    { NULL,      NULL,          "Event Tester",             0,         0,          0,           1,        -1 }, /* xev */

    { NULL,	"spterm",	 NULL,		SPTAG(0),		1,		   1,	       1,           1,        -1 },
    { NULL,	"spfm",		 NULL,		SPTAG(1),		1,		   1,	       1,           1,        -1 },
    { NULL,	"keepassxc", NULL,		SPTAG(2),		0,		   0,	       1,           1,        -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
};

/* key definitions */
#define MODKEY Mod1Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, topbar ? NULL : "-b", NULL };
static const char *termcmd[]  = { "st", NULL };
static const char *slockcmd[] = { "slock", NULL  };
static const char *flamecmd[] = { "flameshot", "gui", NULL };
static const char *pwrcmd[]   = { "/home/beau/.local/bin/dm-scripts/powermenu", NULL };
static const char *appcmd[]   = { "/home/beau/.local/bin/dm-scripts/appmenu", NULL };
static const char *passcmd[]  = { "/home/beau/.local/bin/dm-scripts/passmenu", "--type", NULL };

/* Audio Control */
static const char *upvol[] = { "pactl", "set-sink-volume", "1", "+5%",      NULL };
static const char *downvol[] = { "pactl", "set-sink-volume", "1", "-5%",    NULL };
static const char *mutevol[] = { "pactl", "set-sink-volume", "1", "toggle", NULL };

/* MPD Control */
static const char *mpdpause[] = { "mpc", "pause", NULL };
static const char *mpdplay[] = { "mpc", "play", NULL };
static const char *mpdnext[] = { "mpc", "next", NULL };
static const char *mpdprev[] = { "mpc", "prev", NULL };


static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_p,      spawn,          {.v = dmenucmd } },
	{ MODKEY|ShiftMask,             XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY|ShiftMask,		        XK_l,      spawn,          {.v = slockcmd} },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY|Mod4Mask,              XK_h,      incrgaps,       {.i = +1 } },
	{ MODKEY|Mod4Mask,              XK_l,      incrgaps,       {.i = -1 } },
	{ MODKEY|Mod4Mask|ShiftMask,    XK_h,      incrogaps,      {.i = +1 } },
	{ MODKEY|Mod4Mask|ShiftMask,    XK_l,      incrogaps,      {.i = -1 } },
	{ MODKEY|Mod4Mask|ControlMask,  XK_h,      incrigaps,      {.i = +1 } },
	{ MODKEY|Mod4Mask|ControlMask,  XK_l,      incrigaps,      {.i = -1 } },
	{ MODKEY|Mod4Mask,              XK_0,      togglegaps,     {0} },
	{ MODKEY|Mod4Mask|ShiftMask,    XK_0,      defaultgaps,    {0} },
	{ MODKEY,                       XK_y,      incrihgaps,     {.i = +1 } },
	{ MODKEY,                       XK_o,      incrihgaps,     {.i = -1 } },
	{ MODKEY|ControlMask,           XK_y,      incrivgaps,     {.i = +1 } },
	{ MODKEY|ControlMask,           XK_o,      incrivgaps,     {.i = -1 } },
	{ MODKEY|Mod4Mask,              XK_y,      incrohgaps,     {.i = +1 } },
	{ MODKEY|Mod4Mask,              XK_o,      incrohgaps,     {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_y,      incrovgaps,     {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_o,      incrovgaps,     {.i = -1 } },
	{ MODKEY,                       XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,             XK_c,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
    { MODKEY,            		    XK_s,  	   togglescratch,  {.ui = 0 } },
    { MODKEY,            		    XK_u,	   togglescratch,  {.ui = 1 } },
    { MODKEY,            		    XK_x,	   togglescratch,  {.ui = 2 } },
    { MODKEY|ShiftMask,             XK_r,      quit,           {1} },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },

    /* PulseAudio Keys */
    { 0,                            XF86XK_AudioMute,        spawn, {.v = downvol } },
    { 0,                            XF86XK_AudioLowerVolume, spawn, {.v = mutevol } },
    { 0,                            XF86XK_AudioRaiseVolume, spawn, {.v = upvol   } },

    /* MPD keys */
    { 0,                            XF86XK_AudioPlay,        spawn, {.v = mpdplay} },
    { 0,        			        XF86XK_AudioStop,        spawn, {.v = mpdpause} },
    { 0,        			        XF86XK_AudioNext,        spawn, {.v = mpdnext} },
    { 0,        			        XF86XK_AudioPrev,        spawn, {.v = mpdprev} },

    /* flameshot bind */
    { MODKEY,                       XK_v,                    spawn, {.v = flamecmd } },

    /* appmenu bind */
    { MODKEY,		                XK_a,                    spawn, { .v = appcmd } },

    /* passmenu bind */
    { MODKEY|ShiftMask,             XK_p,                    spawn, { .v = passcmd } },

    /* powermenu bind */
    { MODKEY|ShiftMask,             XK_s,                    spawn, { .v = pwrcmd } },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button1,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

