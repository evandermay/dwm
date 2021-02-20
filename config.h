/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 2;        /* border pixel of windows */
static const Gap default_gap        = {.isgap = 1, .realgap = 10, .gappx = 10};
static const unsigned int snap      = 32;       /* snap pixel */
static const int swallowfloating    = 0;        /* 1 means swallow floating windows by default */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "monospace:size=12", "fontawesome:size=12", "Joypixels:size=12" };
static const char dmenufont[]       = "monospace:size=11";
static char normbgcolor[]           = "#282828";
static char normbordercolor[]       = "#504945";
static char normfgcolor[]           = "#ebdbb2";
static char selfgcolor[]            = "#282828";
static char selbordercolor[]        = "#d79921";
static char selbgcolor[]            = "#d79921";
static char *colors[][3] = {   
     /*               fg           bg           border   */        
	[SchemeNorm] = { normfgcolor, normbgcolor, normbordercolor },        
	[SchemeSel]  = { selfgcolor,  selbgcolor,  selbordercolor  }, 
};

static const char *const autostart[] = {
    NULL /* terminate */
};

typedef struct {
    	const char *name;
   	const void *cmd;
} Sp;
const char *spcmd1[] = {"st", "-n", "spterm", NULL };
static Sp scratchpads[] = {
    	/* name          cmd  */
    		{"spterm",      spcmd1},
};

/* tagging */
static const char *tags[] = { "一", "二", "三", "四", "五", "六", "七", "八", "九" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor  scratch key  float x,y,w,h         floatborderpx*/
	{ "Gimp",     NULL,       NULL,       0,            1,           -1,        0,            50,50,500,500,        5 },
	{ "Firefox",  NULL,       NULL,       1 << 8,       0,           -1,        0,            50,50,500,500,        5 },
	{ "floatingwin", NULL,  "floatingwin", 0,         1,              -1,       0,            50,50,500,500, 5 },
	{ "St",      NULL,     NULL,           0,         0,          1,           0,        -1,	50,50,500,500, 5 },
 	{ NULL,      NULL,     "Event Tester", 0,         0,          0,           0,        -1 }, /* xev */
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
#include "fibonacci.c"
static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
        { "[@]",      spiral },
        { "[\\]",      dwindle },
	{ NULL,       NULL },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ 0, MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ 0, MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ 0, MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ 0, MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/bash", "-c", cmd, NULL } }
#include <X11/XF86keysym.h>
/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-c", "-g", "4", "-l", "20", "-bw", "2", "-p", "Run:",  NULL };
/*static const char *dmenucmd[] = { "st", "-T", "floatingwin", "-e", "launcher.sh", "-e",  NULL };*/
static const char *termcmd[]  = { "st", NULL };

/*First arg only serves to match against key in rules*/
static const char *scratchpadcmd[] = {"s", "st", "-t", "scratchpad", NULL}; 

#define MULTIKEY_THRESHOLD_MS_PRESS 400
#define MULTIKEY_THRESHOLD_MS_HOLD 700

#include "selfrestart.c"

static Key keys[] = {
    /* npresses, 	modifier	key	function 	argumen */
    {0,   MODKEY,                       XK_c,      spawn,          {.v = dmenucmd } },
    {0,   MODKEY,                       XK_Return, spawn,          {.v = termcmd } },
    {0,   MODKEY|ShiftMask,		XK_Return, togglescratch,  {.ui = 0} },
    {0,   MODKEY,                       XK_b,      togglebar,      {0} },
    {0,   MODKEY|ShiftMask,		XK_j, 	   rotatestack,    {.i = +1 } },
    {0,   MODKEY|ShiftMask,		XK_k, 	   rotatestack,    {.i = -1 } },
    {0,   MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
    {0,   MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
    {0,   MODKEY|ShiftMask,             XK_l,      incnmaster,     {.i = +1 } },
    {0,   MODKEY|ShiftMask,             XK_h,      incnmaster,     {.i = -1 } },
    {0,   MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
    {0,   MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
    {0,   MODKEY,                       XK_z,      zoom,           {0} },
    {0,   MODKEY,                       XK_Tab,    view,           {0} },
    {0,   MODKEY,                       XK_q,      killclient,     {0} },
        /*{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
        { MODKEY|ShiftMask,             XK_f,      setlayout,      {.v = &layouts[1]} },
        { MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
        { MODKEY,                       XK_r,      setlayout,      {.v = &layouts[3]} },
        { MODKEY|ShiftMask,             XK_r,      setlayout,      {.v = &layouts[4]} },
	{ MODKEY|ControlMask,           XK_comma,  cyclelayout,    {.i = -1 } },
        { MODKEY|ControlMask,           XK_period, cyclelayout,    {.i = +1 } },
        { MODKEY,                       XK_space,  setlayout,      {0} },*/
    {0,   MODKEY,                       XK_space,  cyclelayout,    {.i = +1 } },
    {0,   MODKEY,                       XK_f,      fullscreen,     {0} },
    {0,   MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
    {0,   MODKEY,                       XK_0,      view,           {.ui = ~0 } },
    {0,   MODKEY,                       XK_s,      togglesticky,   {0} },
    {0,   MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
    {0,   MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
    {0,   MODKEY,                       XK_period, focusmon,       {.i = +1 } },
    {0,   MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
    {0,   MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
    {0,   MODKEY,                       XK_minus,  setgaps,        {.i = -5 } },
    {0,   MODKEY,                       XK_equal,  setgaps,        {.i = +5 } },
    {0,   MODKEY|ControlMask,           XK_minus,  setgaps,        {.i = GAP_RESET } },
    {0,   MODKEY|ControlMask,           XK_equal,  setgaps,        {.i = GAP_TOGGLE} },

    {2,   MODKEY,			XK_w,      spawn,	   SHCMD("dmenu_websearch") },
    {1,   MODKEY,	        	XK_w,      spawn,	   SHCMD("firefox") },
    {3,   MODKEY,	        	XK_w,      spawn,	   SHCMD("qutebrowser") },
    {0,   MODKEY,			XK_x,      spawn,	   SHCMD("thunar") },
    {0,   MODKEY|ShiftMask,		XK_z,      spawn,	   SHCMD("pavucontrol") },
    {0,   MODKEY,			XK_F3,     spawn,	   SHCMD("displayselect") },
    {0,   MODKEY,			XK_F9,	   spawn,          SHCMD("dmenumount") },
    {0,   MODKEY,			XK_F10,	   spawn,          SHCMD("dmenuumount") },
    {0,   MODKEY,			XK_grave,  spawn,	   SHCMD("dmenuunicode") },
    {0,   MODKEY,			XK_BackSpace,  spawn,	   SHCMD("sysact") },

    {0,    0,				XK_Print,	spawn,		SHCMD("maim pic-full-$(date '+%y%m%d-%H%M-%S').png") },
    {0,   ShiftMask,			XK_Print,	spawn,		SHCMD("maimpick") },

    {0,   0,                            XF86XK_AudioMute, spawn,   SHCMD("pamixer -t; kill -44 $(pidof dwmblocks)") },
    {0,   0,                            XF86XK_AudioLowerVolume, spawn,          SHCMD("pamixer --allow-boost -d 2; kill -44 $(pidof dwmblocks)") },
    {0,   0,                            XF86XK_AudioRaiseVolume, spawn,   SHCMD("pamixer --allow-boost -i 2; kill -44 $(pidof dwmblocks)") },
    {0,   0,                 		XF86XK_MonBrightnessUp,	spawn,		SHCMD("xbacklight -inc 10") },
    {0,   0, 				XF86XK_MonBrightnessDown, spawn,		SHCMD("xbacklight -dec 10") },
    {0,   0,                            XF86XK_AudioPrev,		spawn,		SHCMD("cmus-remote -r") },
    {0,   0,                            XF86XK_AudioNext,		spawn,		SHCMD("cmus-remote -n") },
    {0,   0,                            XF86XK_AudioPause,		spawn,		SHCMD("cmus-remote -u") },
	TAGKEYS(                        XK_1,                      0)
        TAGKEYS(                        XK_2,                      1)
        TAGKEYS(                        XK_3,                      2)
        TAGKEYS(                        XK_4,                      3)
        TAGKEYS(                        XK_5,                      4)
        TAGKEYS(                        XK_6,                      5)
        TAGKEYS(                        XK_7,                      6)
        TAGKEYS(                        XK_8,                      7)
        TAGKEYS(                        XK_9,                      8)
    {0,  MODKEY|ShiftMask,             XK_q,      quit,           {0} },
    {0,  MODKEY|ShiftMask,             XK_r,      self_restart,   {0} },
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
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};
