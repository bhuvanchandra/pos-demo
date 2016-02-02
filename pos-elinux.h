#include <stdio.h>

#define BAUD_RATE		B9600
#define PXA270			"/usr/share/doc/POSDemoQuickInfo/ColibriPXA270.txt"
#define PXA300			"/usr/share/doc/POSDemoQuickInfo/ColibriPXA300.txt"
#define PXA310			"/usr/share/doc/POSDemoQuickInfo/ColibriPXA320.txt"
#define PXA320			"/usr/share/doc/POSDemoQuickInfo/ColibriPXA320.txt"
#define COLIBRIT20		"/usr/share/doc/POSDemoQuickInfo/ColibriT20.txt"
#define COLIBRIT30		"/usr/share/doc/POSDemoQuickInfo/ColibriT30.txt"
#define APALIST30		"/usr/share/doc/POSDemoQuickInfo/ApalisT30.txt"
#define ROBINZ510S		"/usr/share/doc/POSDemoQuickInfo/RobinZ510S.txt"
#define ROBINZ530L		"/usr/share/doc/POSDemoQuickInfo/RobinZ530L.txt"
#define ROBINZ530M		"/usr/share/doc/POSDemoQuickInfo/RobinZ530M.txt"
#define END			"/usr/share/doc/POSDemoQuickInfo/end.txt"
#define POS_ELINUX_GLADE	"/usr/share/glade/pos-linux.glade"
#define FULL_SCREEN

typedef struct _customData
{
	GtkWidget *cb_colibri_pxa270;
	GtkWidget *cb_colibri_pxa300;
	GtkWidget *cb_colibri_pxa310;
	GtkWidget *cb_colibri_pxa320;
	GtkWidget *cb_colibri_t20;
	GtkWidget *cb_colibri_t30;
	GtkWidget *cb_colibri_eval_board;
	GtkWidget *cb_iris_board;
	GtkWidget *cb_apalis_t30;
	GtkWidget *cb_apalis_eval_board;
	GtkWidget *cb_robin_z530l;
	GtkWidget *cb_robin_z530m;
	GtkWidget *cb_robin_z510s;
	GtkWidget *cb_daisy_board;
	GtkLabel *good_item;
} customData;
