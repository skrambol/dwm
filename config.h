#include <X11/XF86keysym.h>
/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx = 1; /* border pixel of windows */
static const unsigned int snap = 32;    /* snap pixel */
static const int showbar = 1;           /* 0 means no bar */
static const int topbar = 1;            /* 0 means bottom bar */
static const char *fonts[] = {"JetBrainsMonoNL Nerd Font Mono:size=14"};
static const char dmenufont[] = "JetBrainsMonoNL Nerd Font Mono:size=14";
static const char col_gray2[] = "#444444";
static const char col_black[] = "#000000";
static const char col_white[] = "#ffffff";
static const char *colors[][3] = {
    /*               fg         bg         border   */
    [SchemeNorm] = {col_white, col_black, col_black},
    [SchemeSel] = {col_white, col_black, col_black},
    [SchemeStatus] = {col_white, col_black, col_black},
    [SchemeTagsSel] = {col_white, col_black, col_black},
    [SchemeTagsNorm] = {col_gray2, col_black, col_black},
    [SchemeInfoSel] = {col_white, col_black, col_black},
    [SchemeInfoNorm] = {col_gray2, col_black, col_black},
};

/* tagging */
static const char *tags[] = {"1", "2", "3", "4", "5", "6", "7", "8", "9"};

static const Rule rules[] = {
    /* xprop(1):
     *  WM_CLASS(STRING) = instance, class
     *  WM_NAME(STRING) = title
     */
    /* class      instance    title       tags mask     isfloating   monitor */
    {"st-256color", NULL, NULL, 1 << 0, 0, -1},
    {"Firefox", NULL, NULL, 1 << 1, 0, -1},
    {"Slack", NULL, NULL, 1 << 2, 0, -1},
};

/* layout(s) */
static const float mfact = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster = 1;    /* number of clients in master area */
static const int resizehints =
    0; /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen =
    1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
    /* symbol     arrange function */
    {"[]", tile}, /* first entry is default */
    {"][", NULL}, /* no layout function means floating behavior */
    {"()", monocle},
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY, TAG)                                                      \
  {MODKEY, KEY, view, {.ui = 1 << TAG}},                                       \
      {MODKEY | ControlMask, KEY, toggleview, {.ui = 1 << TAG}},               \
      {MODKEY | ShiftMask, KEY, tag, {.ui = 1 << TAG}}, {                      \
    MODKEY | ControlMask | ShiftMask, KEY, toggletag, { .ui = 1 << TAG }       \
  }

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd)                                                             \
  {                                                                            \
    .v = (const char *[]) { "/bin/sh", "-c", cmd, NULL }                       \
  }

#define STATUSBAR "dwmblocks"

/* commands */
static char dmenumon[2] =
    "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = {
    "dmenu_run", "-m",  dmenumon,  "-fn", dmenufont, "-nb", col_black, "-nf",
    col_white,   "-sb", col_gray2, "-sf", col_white, NULL};
static const char *cmd_term[] = {"st", NULL};
static const char *cmd_web[] = {"firefox", NULL};
static const char *cmd_volume_inc[] = {"status_volume_set", "+", NULL};
static const char *cmd_volume_dec[] = {"status_volume_set", "-", NULL};
static const char *cmd_volume_toggle[] = {"status_volume_set", "toggle", NULL};
static const char *cmd_screenshot_area[] = {"screenshot_area", NULL};
static const char *cmd_lockscreen[] = {"lockscreen", NULL};

static Key keys[] = {
    /* modifier, key, function, argument */
    {MODKEY, XK_d, spawn, {.v = dmenucmd}},
    {MODKEY, XK_Return, spawn, {.v = cmd_term}},
    {MODKEY, XK_f, spawn, {.v = cmd_web}},
    {MODKEY, XK_grave, spawn, {.v = cmd_lockscreen}},
    {0, XF86XK_AudioRaiseVolume, spawn, {.v = cmd_volume_inc}},
    {0, XF86XK_AudioLowerVolume, spawn, {.v = cmd_volume_dec}},
    {0, XF86XK_AudioMute, spawn, {.v = cmd_volume_toggle}},
    {MODKEY | ShiftMask, XK_s, spawn, {.v = cmd_screenshot_area}},

    {MODKEY, XK_b, togglebar, {0}},
    {MODKEY, XK_j, focusstack, {.i = +1}},
    {MODKEY, XK_k, focusstack, {.i = -1}},
    {MODKEY, XK_i, incnmaster, {.i = +1}},
    {MODKEY | ShiftMask, XK_i, incnmaster, {.i = -1}},
    {MODKEY, XK_Left, setmfact, {.f = -0.05}},
    {MODKEY, XK_Right, setmfact, {.f = +0.05}},
    {MODKEY | ShiftMask, XK_Return, zoom, {0}},
    {MODKEY, XK_Tab, view, {0}},
    {MODKEY, XK_q, killclient, {0}},
    {MODKEY, XK_bracketleft, setlayout, {.v = &layouts[0]}},
    {MODKEY, XK_bracketright, setlayout, {.v = &layouts[1]}},
    {MODKEY, XK_m, setlayout, {.v = &layouts[2]}},
    {MODKEY, XK_space, setlayout, {0}},
    {MODKEY | ShiftMask, XK_space, togglefloating, {0}},
    {MODKEY, XK_0, view, {.ui = ~0}},
    {MODKEY | ShiftMask, XK_0, tag, {.ui = ~0}},
    {MODKEY, XK_h, focusmon, {.i = -1}},
    {MODKEY, XK_l, focusmon, {.i = +1}},
    {MODKEY | ShiftMask, XK_h, tagmon, {.i = -1}},
    {MODKEY | ShiftMask, XK_l, tagmon, {.i = +1}},
    TAGKEYS(XK_1, 0),
    TAGKEYS(XK_2, 1),
    TAGKEYS(XK_3, 2),
    TAGKEYS(XK_4, 3),
    TAGKEYS(XK_5, 4),
    TAGKEYS(XK_6, 5),
    TAGKEYS(XK_7, 6),
    TAGKEYS(XK_8, 7),
    TAGKEYS(XK_9, 8),
    {MODKEY | ShiftMask, XK_q, quit, {0}},
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle,
 * ClkClientWin, or ClkRootWin */
static Button buttons[] = {
    /* click, event mask, button, function argument */
    {ClkLtSymbol, 0, Button1, setlayout, {0}},
    {ClkLtSymbol, 0, Button3, setlayout, {.v = &layouts[2]}},
    {ClkWinTitle, 0, Button2, zoom, {0}},
    {ClkStatusText, 0, Button1, sigstatusbar, {.i = 1}},
    {ClkStatusText, 0, Button2, sigstatusbar, {.i = 2}},
    {ClkStatusText, 0, Button3, sigstatusbar, {.i = 3}},
    {ClkClientWin, MODKEY, Button1, movemouse, {0}},
    {ClkClientWin, MODKEY, Button2, togglefloating, {0}},
    {ClkClientWin, MODKEY, Button3, resizemouse, {0}},
    {ClkTagBar, 0, Button1, view, {0}},
    {ClkTagBar, 0, Button3, toggleview, {0}},
    {ClkTagBar, MODKEY, Button1, tag, {0}},
    {ClkTagBar, MODKEY, Button3, toggletag, {0}},
};
