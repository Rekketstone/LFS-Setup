/* See LICENSE file for copyright and license details. */
#include <X11/XF86keysym.h>

/* appearance */
static const unsigned int borderpx  = 2;        /* border pixel of windows */
static const unsigned int gappx     = 4;        /* gap pixel between windows */
static const unsigned int snap      = 20;       /* snap pixel */
static const unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayonleft = 1;    /* 0: systray in the right corner, >0: systray on left of status text */
static const unsigned int systrayspacing = 1;   /* systray spacing */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray        = 1;        /* 0 means no systray */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "3270 Nerd Font Mono:size=12:style=Regular:antialias=true:autohint=true", "fontawesome:size=12:antialias=true:autohint=true" };
static const char dmenufont[]       = { "3270 Nerd Font Mono:size=12:style=Regular:antialias=true:autohint=true" };
static const char col_gray1[]       = "#000000";
static const char col_gray2[]       = "#eeeeee";
static const char col_gray3[]       = "#eeeeee";
static const char col_gray4[]       = "#ffffff";
static const char col_cyan[]        = "#000000";
static const char col_border[]      = "#ffffff";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]  = { col_gray4, col_cyan,  col_border  },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const unsigned int ulinepad	= 5;	/* horizontal padding between the underline and tag */
static const unsigned int ulinestroke	= 2;	/* thickness / height of the underline */
static const unsigned int ulinevoffset	= 0;	/* how far above the bottom of the bar the line should appear */
static const int ulineall 		= 0;	/* 1 to show underline on all tags, 0 for just the active ones */

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Gimp",     NULL,       NULL,       0,            1,           -1 },
	{ "Firefox",  NULL,       NULL,       1 << 8,       0,           -1 },
        { "urxvt",    NULL,       NULL,       1 << 8,       0,           -1 },
        { "pavucontrol", NULL,    "control",  1 << 8,       1,           -1 },
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
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static const char *dmenucmd[] = { "dmenu_run", "-fn", dmenufont, "-nb", "#000000", "-nf", "#ffffff", "-sb", "#ffffff", "-sf", "#000000", "-p", "󱓞 Launch:", NULL };
static const char *termcmd[]  = { "st", NULL };
static const char *browscmd[] = { "mercury", NULL};
static const char *clipmenu[] = { "clipmenu", "-i", "-c", "-p", "", "-fn", dmenufont, "-sb", col_cyan, NULL};
static const char *clipurl[]  = { "clipmenu-url", NULL };
static const char *fileman[]  = { "pcmanfm", NULL };
static const char *volume[]   = { "pavucontrol", NULL };
static const char *slock[]    = { "slock", NULL };

static const char *up_vol[]   = { "amixer", "-q", "sset", "'Master'", "10%+", 	NULL };
static const char *down_vol[] = { "amixer", "-q", "sset", "'Master'", "10%-", 	NULL };
static const char *mute_vol[] = { "amixer", "-q", "sset", "'Master'", "toggle", NULL };

static const char *brighter[] = { "brightnessctl", "set", "10%+", NULL };
static const char *dimmer[]   = { "brightnessctl", "set", "10%-", NULL};

static const char *screenie[] = { "/home/rekketstone/.dwm/screenie.sh", NULL };
static const char *power[]    = { "/home/rekketstone/.dwm/powermenu.sh", NULL };

static const Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_p,      spawn,          {.v = dmenucmd } },
	{ MODKEY,     	                XK_t, 	   spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_w,      spawn,          {.v = browscmd } },
	{ MODKEY,		        XK_v, 	   spawn,     	   {.v = clipmenu } },
	{ MODKEY,		        XK_v, 	   spawn,	   {.v = clipurl } },
        { MODKEY|Mod1Mask,              XK_v,      spawn,          {.v = volume } },
	{ MODKEY,     	                XK_e, 	   spawn,          {.v = fileman } },
        { MODKEY|ShiftMask,             XK_l,      spawn,          {.v = slock } },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY, 	                XK_q,      killclient,     {0} },
	{ MODKEY|ShiftMask,             XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY|ShiftMask,             XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
        { MODKEY,                       XK_f,      togglefullscr,  {0} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
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
        { MODKEY|ShiftMask,             XK_e,     spawn,           {.v = power } },
        { 0,                XF86XK_AudioMute,     spawn,           {.v = mute_vol } },
	{ 0,         XF86XK_AudioLowerVolume,     spawn,           {.v = down_vol } },
	{ 0,         XF86XK_AudioRaiseVolume,     spawn,           {.v = up_vol } },
	{ 0,        XF86XK_MonBrightnessDown,     spawn,           {.v = dimmer } },
	{ 0,          XF86XK_MonBrightnessUp,     spawn,           {.v = brighter } },
        { 0,                        XK_Print,     spawn,           {.v = screenie } },
};

/* buttontions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
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

