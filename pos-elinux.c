#include <gtk/gtk.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <sys/stat.h>

#include "pos-elinux.h"
#include "co-proc-lib.h"

void close_app(GtkWidget* widget,gpointer user_data)
{
	printf("quit\n");
	gtk_main_quit();
}

int serial_init(void)
{
	struct termios tio;
	struct termios old_stdio;
	int ttyFd;
	char *ttyDevPath;

	tcgetattr(STDOUT_FILENO, &old_stdio);

	memset(&tio,0,sizeof(tio));
	tio.c_iflag = 0;
	tio.c_oflag = 0;
	/* 8n1, see termios.h for more information */
	tio.c_cflag = CS8 |CREAD | CLOCAL;
	tio.c_lflag = 0;
	tio.c_cc[VMIN] = 1;
	tio.c_cc[VTIME] = 5;

	switch (getPROCID()){
		case TYPE_PXA270:
		case TYPE_PXA320:
		case TYPE_PXA300:
		case TYPE_PXA310:
		break;
		case TYPE_TEGRA2:
			ttyDevPath = "/dev/ttyHS3";
		break;
		case TYPE_TEGRA3:
			ttyDevPath = "/dev/ttyHS3";
		break;
		case TYPE_VYBRID:
			ttyDevPath = "/dev/ttyLP2";
		break;
		case TYPE_IMX6:
			ttyDevPath = "/dev/ttymxc2";
		break;
	}

	ttyFd = open(ttyDevPath, O_RDWR | O_NONBLOCK);

	if (ttyFd < 0)
		perror("tty open failed!");

	cfsetospeed(&tio, BAUD_RATE);            /* 9600 baud */
	cfsetispeed(&tio, BAUD_RATE);            /* 9600 baud */
	tcsetattr(ttyFd, TCSANOW, &tio);
	
	return ttyFd;
}


void serial_print(char *filePath)
{
	FILE *fileDescriptor;
	int fileSize, tty_fd;
	unsigned char c = 'D';
	struct stat st;	/* for finding the size of the file */

	fileDescriptor = fopen(filePath, "r+b");
	stat(filePath, &st);
	fileSize = st.st_size;

	tty_fd = serial_init();	

	while (1) {
		if (fileSize > 1) {
			fread(&c, 1, 1, (FILE *) fileDescriptor);
			write(tty_fd, &c, 1);
			fileSize -= 1;
		} else {
			fread(&c, 1, 1, fileDescriptor);
			write(tty_fd, &c, 1);
			fileSize = 0;
			break;
		}
	}

	fclose(fileDescriptor);
	close(tty_fd);
	//tcsetattr(STDOUT_FILENO, TCSANOW, &old_stdio);
}

void pxa270_clicked_cb(GtkButton *button, gpointer user_data)
{
	serial_print(PXA270);
	serial_print(END);
	g_print("pxa270 clicked\n");
}

void pxa300_clicked_cb(GtkButton *button, gpointer user_data)
{
	serial_print(PXA300);
	serial_print(END);
	g_print("pxa300 clicked\n");
}

void pxa310_clicked_cb(GtkButton *button, gpointer user_data)
{
	serial_print(PXA310);
	serial_print(END);
	g_print("pxa310 clicked\n");
}

void pxa320_clicked_cb(GtkButton *button, gpointer user_data)
{
	serial_print(PXA320);
	serial_print(END);
	g_print("pxa320 clicked\n");
}

void colibri_t20_clicked_cb(GtkButton *button, gpointer user_data)
{
	serial_print(COLIBRIT20);
	serial_print(END);
	g_print("colibriT20 clicked\n");
}

void colibri_t30_clicked_cb(GtkButton *button, gpointer user_data)
{
	serial_print(COLIBRIT30);
	serial_print(END);
	g_print("colibriT30 clicked\n");
}

void apalis_t30_clicked_cb(GtkButton *button, gpointer user_data)
{
	serial_print(APALIST30);
	serial_print(END);
	g_print("apalisT30 clicked\n");
}

void z530l_clicked_cb(GtkButton *button, gpointer user_data)
{
	serial_print(ROBINZ530L);
	serial_print(END);
	g_print("robinz530L clicked\n");
}

void z530m_clicked_cb(GtkButton *button, gpointer user_data)
{
	serial_print(ROBINZ530M);
	serial_print(END);
	g_print("robinz510S clicked\n");
}

void z510s_clicked_cb(GtkButton *button, gpointer user_data)
{
	serial_print(ROBINZ510S);
	serial_print(END);
	g_print("robinz510S clicked\n");
}

void done_clicked_cb(GtkWidget *widget, customData *data)
{	
	g_print("Quotation\n");
}

void tb_good_item_toggled_cb(GtkToggleButton *tButton, customData *data)
{
	if(gtk_toggle_button_get_active (tButton)) {
		gtk_label_set_text(data->good_item, "Good Item");
	} else {
		gtk_label_set_text(data->good_item, "Bad Item");
	}
}

void select_colibri_clicked_cb(GtkWidget *widget, customData *data)
{
	gtk_toggle_button_set_active((GtkToggleButton *)data->cb_colibri_pxa270, TRUE);
	gtk_toggle_button_set_active((GtkToggleButton *)data->cb_colibri_pxa300, TRUE);
	gtk_toggle_button_set_active((GtkToggleButton *)data->cb_colibri_pxa310, TRUE);
	gtk_toggle_button_set_active((GtkToggleButton *)data->cb_colibri_pxa320, TRUE);
	gtk_toggle_button_set_active((GtkToggleButton *)data->cb_colibri_t20, TRUE);	
	gtk_toggle_button_set_active((GtkToggleButton *)data->cb_colibri_t30, TRUE);
	gtk_toggle_button_set_active((GtkToggleButton *)data->cb_colibri_eval_board, TRUE);	
	gtk_toggle_button_set_active((GtkToggleButton *)data->cb_iris_board, TRUE);
}

void select_apalis_clicked_cb(GtkWidget *widget, customData *data)
{

	gtk_toggle_button_set_active((GtkToggleButton *)data->cb_apalis_t30, TRUE);	
	gtk_toggle_button_set_active((GtkToggleButton *)data->cb_apalis_eval_board, TRUE);
}

void select_robin_clicked_cb(GtkWidget *widget, customData *data)
{
	gtk_toggle_button_set_active((GtkToggleButton *)data->cb_robin_z530l, TRUE);	
	gtk_toggle_button_set_active((GtkToggleButton *)data->cb_robin_z530m, TRUE);
	gtk_toggle_button_set_active((GtkToggleButton *)data->cb_robin_z510s, TRUE);	
	gtk_toggle_button_set_active((GtkToggleButton *)data->cb_daisy_board, TRUE);
}

void select_all_clicked_cb(GtkWidget *widget, customData *data)
{
	gtk_toggle_button_set_active((GtkToggleButton *)data->cb_colibri_pxa270, TRUE);
	gtk_toggle_button_set_active((GtkToggleButton *)data->cb_colibri_pxa300, TRUE);
	gtk_toggle_button_set_active((GtkToggleButton *)data->cb_colibri_pxa310, TRUE);
	gtk_toggle_button_set_active((GtkToggleButton *)data->cb_colibri_pxa320, TRUE);
	gtk_toggle_button_set_active((GtkToggleButton *)data->cb_colibri_t20, TRUE);	
	gtk_toggle_button_set_active((GtkToggleButton *)data->cb_colibri_t30, TRUE);
	gtk_toggle_button_set_active((GtkToggleButton *)data->cb_colibri_eval_board, TRUE);	
	gtk_toggle_button_set_active((GtkToggleButton *)data->cb_iris_board, TRUE);

	gtk_toggle_button_set_active((GtkToggleButton *)data->cb_apalis_t30, TRUE);	
	gtk_toggle_button_set_active((GtkToggleButton *)data->cb_apalis_eval_board, TRUE);

	gtk_toggle_button_set_active((GtkToggleButton *)data->cb_robin_z530l, TRUE);	
	gtk_toggle_button_set_active((GtkToggleButton *)data->cb_robin_z530m, TRUE);
	gtk_toggle_button_set_active((GtkToggleButton *)data->cb_robin_z510s, TRUE);	
	gtk_toggle_button_set_active((GtkToggleButton *)data->cb_daisy_board, TRUE);
}

void deselect_all_clicked_cb(GtkWidget *widget, customData *data)
{
	gtk_toggle_button_set_active((GtkToggleButton *)data->cb_colibri_pxa270, FALSE);
	gtk_toggle_button_set_active((GtkToggleButton *)data->cb_colibri_pxa300, FALSE);
	gtk_toggle_button_set_active((GtkToggleButton *)data->cb_colibri_pxa310, FALSE);
	gtk_toggle_button_set_active((GtkToggleButton *)data->cb_colibri_pxa320, FALSE);
	gtk_toggle_button_set_active((GtkToggleButton *)data->cb_colibri_t20, FALSE);	
	gtk_toggle_button_set_active((GtkToggleButton *)data->cb_colibri_t30, FALSE);
	gtk_toggle_button_set_active((GtkToggleButton *)data->cb_colibri_eval_board, FALSE);	
	gtk_toggle_button_set_active((GtkToggleButton *)data->cb_iris_board, FALSE);

	gtk_toggle_button_set_active((GtkToggleButton *)data->cb_apalis_t30, FALSE);	
	gtk_toggle_button_set_active((GtkToggleButton *)data->cb_apalis_eval_board, FALSE);

	gtk_toggle_button_set_active((GtkToggleButton *)data->cb_robin_z530l, FALSE);	
	gtk_toggle_button_set_active((GtkToggleButton *)data->cb_robin_z530m, FALSE);
	gtk_toggle_button_set_active((GtkToggleButton *)data->cb_robin_z510s, FALSE);	
	gtk_toggle_button_set_active((GtkToggleButton *)data->cb_daisy_board, FALSE);
}

void colorMe(GtkWidget *objectId)
{
	GdkColor color;
	gdk_color_parse( "#000052528888", &color );
	gtk_widget_modify_bg(GTK_WIDGET(objectId), GTK_STATE_NORMAL, &color);
	gtk_widget_modify_bg(GTK_WIDGET(objectId), GTK_STATE_PRELIGHT, &color);
	gtk_widget_modify_bg(GTK_WIDGET(objectId), GTK_STATE_ACTIVE, &color);
}

int main (int argc, char **argv)
{
	GtkBuilder *gtkBuilder;
	GtkWidget *mainwin;
	GtkWidget *pxa270, *pxa300, *pxa310, *pxa320, *colibriT20, *colibriT30, *apalisT30, *z530L, *z530M, *z510S;
	GtkWidget *select_colibri, *select_apalis, *select_robin, *select_all, *deselect_all, *done;
	GtkWidget *tb_good_item, *remove_item, *bad_item, *print_bill, *quit, *clear_good_stock, *clear_bad_stock, *g_item, *g_in_stock, *b_item, *b_in_stock;
	GtkWidget *c_pxa270, *c_pxa300, *c_pxa310, *c_pxa320, *c_t20_v1, *c_t20_v2, *c_t30_v1, *c_t30_v2;
	customData data;

	gtk_set_locale();
	/* Initialize the widget set */
	gtk_init (&argc, &argv);

	/* Create the main window */
	gtkBuilder= gtk_builder_new();
	gtk_builder_add_from_file(gtkBuilder, POS_ELINUX_GLADE, NULL);
	gtk_builder_connect_signals ( gtkBuilder, &data);
	mainwin= GTK_WIDGET(gtk_builder_get_object(gtkBuilder, "window"));

	pxa270 = GTK_WIDGET(gtk_builder_get_object(gtkBuilder, "pxa270"));
	colorMe(pxa270);
	pxa300 = GTK_WIDGET(gtk_builder_get_object(gtkBuilder, "pxa300"));
	colorMe(pxa300);
	pxa310 = GTK_WIDGET(gtk_builder_get_object(gtkBuilder, "pxa310"));
	colorMe(pxa310);
	pxa320 = GTK_WIDGET(gtk_builder_get_object(gtkBuilder, "pxa320"));
	colorMe(pxa320);
	colibriT20 = GTK_WIDGET(gtk_builder_get_object(gtkBuilder, "colibri-t20"));
	colorMe(colibriT20);
	colibriT30 = GTK_WIDGET(gtk_builder_get_object(gtkBuilder, "colibri-t30"));
	colorMe(colibriT30);
	apalisT30 = GTK_WIDGET(gtk_builder_get_object(gtkBuilder, "apalis-t30"));
	colorMe(apalisT30);
	z530L = GTK_WIDGET(gtk_builder_get_object(gtkBuilder, "z530l"));
	colorMe(z530L);
	z530M = GTK_WIDGET(gtk_builder_get_object(gtkBuilder, "z530m"));
	colorMe(z530M);
	z510S = GTK_WIDGET(gtk_builder_get_object(gtkBuilder, "z510s"));
	colorMe(z510S);
	select_colibri = GTK_WIDGET(gtk_builder_get_object(gtkBuilder, "select-colibri"));
	colorMe(select_colibri);
	select_apalis = GTK_WIDGET(gtk_builder_get_object(gtkBuilder, "select-apalis"));
	colorMe(select_apalis);
	select_robin = GTK_WIDGET(gtk_builder_get_object(gtkBuilder, "select-robin"));
	colorMe(select_robin);
	select_all = GTK_WIDGET(gtk_builder_get_object(gtkBuilder, "select-all"));
	colorMe(select_all);
	deselect_all = GTK_WIDGET(gtk_builder_get_object(gtkBuilder, "deselect-all"));
	colorMe(deselect_all);
	done = GTK_WIDGET(gtk_builder_get_object(gtkBuilder, "done"));
	colorMe(done);
	remove_item = GTK_WIDGET(gtk_builder_get_object(gtkBuilder, "remove-item"));
	colorMe(remove_item);
	bad_item = GTK_WIDGET(gtk_builder_get_object(gtkBuilder, "bad-item"));
	colorMe(bad_item);
	print_bill = GTK_WIDGET(gtk_builder_get_object(gtkBuilder, "print-bill"));
	colorMe(print_bill);
	clear_good_stock = GTK_WIDGET(gtk_builder_get_object(gtkBuilder, "clear-good-stock"));
	colorMe(clear_good_stock);
	clear_bad_stock = GTK_WIDGET(gtk_builder_get_object(gtkBuilder, "clear-bad-stock"));
	colorMe(clear_bad_stock);
	quit = GTK_WIDGET(gtk_builder_get_object(gtkBuilder, "quit"));
	colorMe(quit);
	tb_good_item = GTK_WIDGET(gtk_builder_get_object(gtkBuilder, "tb-good-item"));
	colorMe(tb_good_item);
	data.good_item = GTK_LABEL(gtk_builder_get_object(gtkBuilder,"good-item"));
	c_pxa270 = GTK_WIDGET(gtk_builder_get_object(gtkBuilder, "c-pxa270"));
	colorMe(c_pxa270);
	c_pxa300 = GTK_WIDGET(gtk_builder_get_object(gtkBuilder, "c-pxa300"));
	colorMe(c_pxa300);
	c_pxa310 = GTK_WIDGET(gtk_builder_get_object(gtkBuilder, "c-pxa310"));
	colorMe(c_pxa310);
	c_pxa320 = GTK_WIDGET(gtk_builder_get_object(gtkBuilder, "c-pxa320"));
	colorMe(c_pxa320);
	c_t20_v1 = GTK_WIDGET(gtk_builder_get_object(gtkBuilder, "c-t20-v1"));
	colorMe(c_t20_v1);
	c_t20_v2 = GTK_WIDGET(gtk_builder_get_object(gtkBuilder, "c-t20-v2"));
	colorMe(c_t20_v2);
	c_t30_v1 = GTK_WIDGET(gtk_builder_get_object(gtkBuilder, "c-t30-v1"));
	colorMe(c_t30_v1);
	c_t30_v2 = GTK_WIDGET(gtk_builder_get_object(gtkBuilder, "c-t30-v2"));
	colorMe(c_t30_v2);
	g_item = GTK_WIDGET(gtk_builder_get_object(gtkBuilder, "g_item"));
	colorMe(g_item);
	g_in_stock = GTK_WIDGET(gtk_builder_get_object(gtkBuilder, "g_in-stock"));
	colorMe(g_in_stock);
	b_item = GTK_WIDGET(gtk_builder_get_object(gtkBuilder, "b_item"));
	colorMe(b_item);
	b_in_stock = GTK_WIDGET(gtk_builder_get_object(gtkBuilder, "b_in-stock"));
	colorMe(b_in_stock);

	data.cb_colibri_pxa270 = GTK_WIDGET(gtk_builder_get_object(gtkBuilder, "cb-colibri-pxa270"));
	data.cb_colibri_pxa300 = GTK_WIDGET(gtk_builder_get_object(gtkBuilder, "cb-colibri-pxa300"));
	data.cb_colibri_pxa310 = GTK_WIDGET(gtk_builder_get_object(gtkBuilder, "cb-colibri-pxa310"));
	data.cb_colibri_pxa320 = GTK_WIDGET(gtk_builder_get_object(gtkBuilder, "cb-colibri-pxa320"));
	data.cb_colibri_t20 = GTK_WIDGET(gtk_builder_get_object(gtkBuilder, "cb-colibri-t20"));
	data.cb_colibri_t30 = GTK_WIDGET(gtk_builder_get_object(gtkBuilder, "cb-colibri-t30"));
	data.cb_colibri_eval_board = GTK_WIDGET(gtk_builder_get_object(gtkBuilder, "cb-colibri-eval-board-v3.1"));
	data.cb_iris_board = GTK_WIDGET(gtk_builder_get_object(gtkBuilder, "cb-iris-base-board-v1.1"));
	data.cb_apalis_t30 = GTK_WIDGET(gtk_builder_get_object(gtkBuilder, "cb-apalis-t30"));
	data.cb_apalis_eval_board = GTK_WIDGET(gtk_builder_get_object(gtkBuilder, "cb-apalis-eval-board-v1.0"));
	data.cb_robin_z530l = GTK_WIDGET(gtk_builder_get_object(gtkBuilder, "cb-robin-z530l"));
	data.cb_robin_z530m = GTK_WIDGET(gtk_builder_get_object(gtkBuilder, "cb-robin-z530m"));
	data.cb_robin_z510s = GTK_WIDGET(gtk_builder_get_object(gtkBuilder, "cb-robin-z510s"));
	data.cb_daisy_board = GTK_WIDGET(gtk_builder_get_object(gtkBuilder, "cb-daisy-pico-itx-carrier-board"));

	g_object_unref ( G_OBJECT(gtkBuilder) );

	/* Show the application window */
	#ifdef FULL_SCREEN
	gtk_window_fullscreen((GtkWindow *) mainwin);
	#endif
	gtk_widget_show(mainwin);

	/* Enter the main event loop, and wait for user interaction */
	gtk_main();

	return 0;
}
