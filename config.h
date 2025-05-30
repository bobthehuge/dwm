/* See LICENSE file for copyright and license details. */

#include <stdlib.h>
#define FONT "DejaVuSansMNerdFontMono-Regular"

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { FONT":size=11" };
static const char dmenufont[]       = FONT":size=11";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#005577";
static const char col_cyan2[]       = "#00fff7";
static const char *colors[][3]      = {
    /*               fg         bg         border   */
    [SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
    [SchemeSel]  = { col_gray4, col_cyan,  col_cyan2 },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
    /* xprop(1):
     *  WM_CLASS(STRING) = instance, class
     *  WM_NAME(STRING) = title
     */
    /* class      instance    title       tags mask     isfloating   monitor */
    /* { "Gimp",     NULL,       NULL,       0,            1,           -1 }, */
    { "Firefox",  NULL,       NULL,       0,       0,           -1 },
};

/* layout(s) */
static const float mfact     = 0.5; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
    /* symbol     arrange function */
    { "[M]",        monocle },
    { "Tiled",      tile },    /* first entry is default */
    { "Float",      NULL },    /* no layout function means floating behavior */
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
    { MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
    { MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
    { MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
    { MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

#define NO_MOD 0

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *termcmd[]  = { "st", "-e", "tmux", NULL };
static const char *brilloup[] = { "brillo", "-A", "5", NULL };
static const char *brillodown[] = { "brillo", "-U", "5", NULL };
static const char *powermenu[] = { "/home/bobthehuge/scripts/power-menu", NULL };
static const char *audiotoggle[] = { "amixer", "set", "Master", "toggle", NULL };
static const char *audioup[] = { "amixer", "set", "Master", "1%+", NULL };
static const char *audiodown[] = { "amixer", "set", "Master", "1%-", NULL };

static const Key fr_keys[] = {
    /* modifier                     key             function            argument */
    { MODKEY,                       XK_d,           spawn,              {.v = dmenucmd } },
    { MODKEY,                       XK_Return,      spawn,              {.v = termcmd } },
    { MODKEY,                       XK_b,           togglebar,          {0} },
    { MODKEY,                       0xFF52,         focusstack,         {.i = +1 } },
    { MODKEY,                       0xFF54,         focusstack,         {.i = -1 } },
    { MODKEY,                       XK_i,           incnmaster,         {.i = +1 } },
    { MODKEY,                       XK_p,           incnmaster,         {.i = -1 } },
    { MODKEY,                       0xFF51,         setmfact,           {.f = -0.05} },
    { MODKEY,                       0xFF53,         setmfact,           {.f = +0.05} },
    { MODKEY|ShiftMask,             XK_Return,      zoom,               {0} },
    { MODKEY,                       XK_Tab,         view,               {0} },
    { MODKEY|ShiftMask,             XK_a,           killclient,         {0} },
    { MODKEY,                       XK_t,           setlayout,          {.v = &layouts[0]} },
    { MODKEY,                       XK_f,           setlayout,          {.v = &layouts[1]} },
    { MODKEY,                       XK_m,           setlayout,          {.v = &layouts[2]} },
    { MODKEY,                       XK_space,       setlayout,          {0} },
    { MODKEY|ShiftMask,             XK_space,       togglefloating,     {0} },
    { MODKEY,                       0xE0,           view,               {.ui = ~0 } },
    { MODKEY|ShiftMask,             0xE0,           tag,                {.ui = ~0 } },
    { MODKEY,                       XK_comma,       focusmon,           {.i = -1 } },
    { MODKEY,                       XK_semicolon,   focusmon,           {.i = +1 } },
    { MODKEY,                       XK_colon,       tagmon,             {.i = -1 } },
    { MODKEY,                       XK_exclam,      tagmon,             {.i = +1 } },
    { NO_MOD,                       0x1008FF02,     spawn,              {.v = brilloup } },
    { NO_MOD,                       0x1008FF03,     spawn,              {.v = brillodown } },
    { NO_MOD,                       0x1008FF12,     spawn,              {.v = audiotoggle } },
    { NO_MOD,                       0x1008FF13,     spawn,              {.v = audioup } },
    { NO_MOD,                       0x1008FF11,     spawn,              {.v = audiodown } },
    TAGKEYS(                        0x26,                               0)
    TAGKEYS(                        0xE9,                               1)
    TAGKEYS(                        0x22,                               2)
    TAGKEYS(                        0x27,                               3)
    TAGKEYS(                        0x28,                               4)
    TAGKEYS(                        0x2D,                               5)
    TAGKEYS(                        0xE8,                               6)
    TAGKEYS(                        0x5F,                               7)
    TAGKEYS(                        0xE7,                               8)
    { MODKEY|ShiftMask,             XK_r,           quit,               {0} },
    { MODKEY|ShiftMask,             XK_e,           spawn,              {.v = powermenu } },
    { MODKEY|ControlMask,           XK_space,       my_rotkblayout,     {0} },
};

static const Key us_keys[] = {
    /* modifier                     key             function            argument */
    { MODKEY,                       XK_d,           spawn,              {.v = dmenucmd } },
    { MODKEY,                       XK_Return,      spawn,              {.v = termcmd } },
    { MODKEY,                       XK_b,           togglebar,          {0} },
    { MODKEY,                       XK_Right,       focusstack,         {.i = +1 } },
    { MODKEY,                       XK_Left,        focusstack,         {.i = -1 } },
    { MODKEY,                       XK_i,           incnmaster,         {.i = +1 } },
    { MODKEY,                       XK_p,           incnmaster,         {.i = -1 } },
    { MODKEY|ShiftMask,             XK_Left,        setmfact,           {.f = -0.05} },
    { MODKEY|ShiftMask,             XK_Right,       setmfact,           {.f = +0.05} },
    { MODKEY|ShiftMask,             XK_Return,      zoom,               {0} },
    { MODKEY,                       XK_Tab,         view,               {0} },
    { MODKEY|ShiftMask,             XK_q,           killclient,         {0} },
    { MODKEY,                       XK_t,           setlayout,          {.v = &layouts[0]} },
    { MODKEY,                       XK_f,           setlayout,          {.v = &layouts[1]} },
    { MODKEY,                       XK_m,           setlayout,          {.v = &layouts[2]} },
    { MODKEY,                       XK_space,       setlayout,          {0} },
    { MODKEY|ShiftMask,             XK_space,       togglefloating,     {0} },
    { MODKEY,                       XK_c,           resizemouse,        {0} },
    { MODKEY,                       XK_0,           view,               {.ui = ~0 } },
    { MODKEY|ShiftMask,             XK_0,           tag,                {.ui = ~0 } },
    { MODKEY,                       XK_comma,       focusmon,           {.i = -1 } },
    { MODKEY,                       XK_period,      focusmon,           {.i = +1 } },
    { MODKEY|ShiftMask,             XK_comma,       tagmon,             {.i = -1 } },
    { MODKEY|ShiftMask,             XK_period,      tagmon,             {.i = +1 } },
    { NO_MOD,                       0x1008FF02,     spawn,              {.v = brilloup } },
    { NO_MOD,                       0x1008FF03,     spawn,              {.v = brillodown } },
    { NO_MOD,                       0x1008FF12,     spawn,              {.v = audiotoggle } },
    { NO_MOD,                       0x1008FF13,     spawn,              {.v = audioup } },
    { NO_MOD,                       0x1008FF11,     spawn,              {.v = audiodown } },
    TAGKEYS(                        XK_1,                               0)
    TAGKEYS(                        XK_2,                               1)
    TAGKEYS(                        XK_3,                               2)
    TAGKEYS(                        XK_4,                               3)
    TAGKEYS(                        XK_5,                               4)
    TAGKEYS(                        XK_6,                               5)
    TAGKEYS(                        XK_7,                               6)
    TAGKEYS(                        XK_8,                               7)
    TAGKEYS(                        XK_9,                               8)
    { MODKEY|ShiftMask,             XK_r,           my_restart,         {0} },
    { MODKEY|ShiftMask,             XK_e,           spawn,              {.v = powermenu } },
    { MODKEY|ControlMask,           XK_space,       my_rotkblayout,     {0} },
};

static const char *kbmap_fr[] = { "setxkbmap", "fr", NULL };
static const char *kbmap_us[] = { "setxkbmap", "us", NULL };

static const KbLayout kb_layouts[] = {
    { .kbmap = kbmap_us,    .keys = us_keys,    .keys_len = LENGTH(us_keys) },
    { .kbmap = kbmap_fr,    .keys = fr_keys,    .keys_len = LENGTH(fr_keys) },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
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
