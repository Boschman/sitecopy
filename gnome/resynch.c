/* 
 *      XSitecopy, for managing remote web sites with a GNOME interface.
 *      Copyright (C) 2000, Lee Mallabone <lee@fonicmonkey.net>
 *                                                                        
 *      This program is free software; you can redistribute it and/or modify
 *      it under the terms of the GNU General Public License as published by
 *      the Free Software Foundation; either version 2 of the License, or
 *      (at your option) any later version.
 *     
 *      This program is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *      GNU General Public License for more details.
 *     
 *      You should have received a copy of the GNU General Public License
 *      along with this program; if not, write to the Free Software
 *      Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *  
 */

/* FIXME! fetch mode needs to be made a little more generic to accomodate all the
 * possible calling contexts; ie. normal, from a druid, from slim mode.
 */

#include "resynch.h"

GtkWidget *fetch_list_window, *fetch_anim, *fetch_message;
GtkWidget *fetch_clist;
GtkWidget *close_fetch_button, *begin_fetch_button;

extern GtkWidget *connection_label, *sitecopy;

extern struct site *selected_site;

/* The user should never close the window during a fetch. */

gboolean delete_fetch(GtkWindow *fnar, gpointer data)
{
    return FALSE;
}

int fetch_site_list_please(GtkWidget * update_button, gpointer data)
{

    GtkWidget *dialog_vbox7;
    GtkWidget *vbox10;
    GtkWidget *hbox4;
    GtkWidget *label17;
    GtkWidget *scrolledwindow1;
    GtkWidget *label18;
    GtkWidget *dialog_action_area7;

    if (!selected_site) {
	gnome_error_dialog("You must select a site before trying to fetch file information");
	return 0;
    }
/** Generated by Glade **/
    fetch_list_window = gnome_dialog_new(NULL, NULL);
    gtk_signal_connect(GTK_OBJECT(fetch_list_window), "delete_event",
		       GTK_SIGNAL_FUNC(delete_fetch), NULL);
    gtk_window_set_title(GTK_WINDOW(fetch_list_window), "XSitecopy: Fetching files");
    gtk_window_set_policy(GTK_WINDOW(fetch_list_window), FALSE, FALSE, FALSE);
    gnome_dialog_set_parent(GNOME_DIALOG(fetch_list_window), GTK_WINDOW(sitecopy));
    dialog_vbox7 = GNOME_DIALOG(fetch_list_window)->vbox;
    gtk_widget_show(dialog_vbox7);

    vbox10 = gtk_vbox_new(FALSE, 5);
    gtk_widget_show(vbox10);
    gtk_box_pack_start(GTK_BOX(dialog_vbox7), vbox10, TRUE, TRUE, 0);

    hbox4 = gtk_hbox_new(FALSE, 0);
    gtk_widget_show(hbox4);
    gtk_box_pack_start(GTK_BOX(vbox10), hbox4, FALSE, FALSE, 0);

    label17 = gtk_label_new("Status: ");
    gtk_widget_show(label17);
    gtk_box_pack_start(GTK_BOX(hbox4), label17, FALSE, FALSE, 0);

    fetch_message = gtk_label_new("Awaiting user input");
    gtk_widget_show(fetch_message);
    gtk_box_pack_start(GTK_BOX(hbox4), fetch_message, TRUE, TRUE, 0);
    gtk_label_set_justify(GTK_LABEL(fetch_message), GTK_JUSTIFY_LEFT);
    gtk_misc_set_alignment(GTK_MISC(fetch_message), 0, 0.5);

    fetch_anim = make_transfer_anim(1);
    gtk_widget_show(fetch_anim);
    gtk_box_pack_start(GTK_BOX(vbox10), fetch_anim, FALSE, FALSE, 0);

    scrolledwindow1 = gtk_scrolled_window_new(NULL, NULL);
    gtk_widget_show(scrolledwindow1);
    gtk_box_pack_start(GTK_BOX(vbox10), scrolledwindow1, TRUE, TRUE, 0);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolledwindow1), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

    fetch_clist = gtk_clist_new(1);
    gtk_widget_show(fetch_clist);
    gtk_container_add(GTK_CONTAINER(scrolledwindow1), fetch_clist);
    gtk_widget_set_usize(fetch_clist, -2, 140);
    gtk_clist_set_column_width(GTK_CLIST(fetch_clist), 0, 80);
    gtk_clist_set_selection_mode(GTK_CLIST(fetch_clist), GTK_SELECTION_BROWSE);
    gtk_clist_column_titles_show(GTK_CLIST(fetch_clist));

    label18 = gtk_label_new("Found information about these files");
    gtk_widget_show(label18);
    gtk_clist_set_column_widget(GTK_CLIST(fetch_clist), 0, label18);

    dialog_action_area7 = GNOME_DIALOG(fetch_list_window)->action_area;
    gtk_widget_show(dialog_action_area7);
    gtk_button_box_set_layout(GTK_BUTTON_BOX(dialog_action_area7), GTK_BUTTONBOX_SPREAD);
    gtk_button_box_set_spacing(GTK_BUTTON_BOX(dialog_action_area7), 8);
    gtk_button_box_set_child_ipadding(GTK_BUTTON_BOX(dialog_action_area7), 0, 0);

    gnome_dialog_append_button_with_pixmap(GNOME_DIALOG(fetch_list_window),
			 "Fetch file states", GNOME_STOCK_PIXMAP_SEARCH);
    begin_fetch_button = g_list_last(GNOME_DIALOG(fetch_list_window)->buttons)->data;
    gtk_widget_show(begin_fetch_button);
    GTK_WIDGET_SET_FLAGS(begin_fetch_button, GTK_CAN_DEFAULT);

    gnome_dialog_append_button(GNOME_DIALOG(fetch_list_window), GNOME_STOCK_BUTTON_CLOSE);
    close_fetch_button = g_list_last(GNOME_DIALOG(fetch_list_window)->buttons)->data;
    gtk_widget_show(close_fetch_button);
    GTK_WIDGET_SET_FLAGS(close_fetch_button, GTK_CAN_DEFAULT);

    gtk_signal_connect(GTK_OBJECT(begin_fetch_button), "clicked",
		       GTK_SIGNAL_FUNC(start_fetch_list),
		       NULL);
    gtk_signal_connect(GTK_OBJECT(close_fetch_button), "clicked",
		       GTK_SIGNAL_FUNC(close_fetch_window),
		       NULL);

/** End Glade generated code **/

    connection_label = fetch_message;
    gtk_window_set_modal(GTK_WINDOW(fetch_list_window), TRUE);
    gtk_widget_show(fetch_list_window);
    return 1;
}

void close_fetch_window(void)
{
    gtk_window_set_modal(GTK_WINDOW(fetch_list_window), FALSE);
    gtk_widget_destroy(fetch_list_window);
    connection_label = NULL;
}

int start_fetch_list(void)
{
    extern sem_t *fetch_semaphore;
    sem_post(fetch_semaphore);
    return 1;
}

void *fetch_thread(void *no_data)
{
    int ret;
    extern struct site *selected_site;
    extern sem_t *fetch_semaphore;
    
    pthread_detach(pthread_self());    
    for(;;)
      {
	  NE_DEBUG(DEBUG_GNOME, "fetch_thread: sleeping...\n");
	  sem_wait(fetch_semaphore);

	  gdk_threads_enter();
	    gtk_widget_set_sensitive(begin_fetch_button, FALSE);
	    gtk_widget_set_sensitive(close_fetch_button, FALSE);
	    ret = verifysite_gnome(selected_site);
	  gdk_threads_leave();
	  
	  if (ret)
	    continue;

	  /* start animation */
	  gdk_threads_enter();
	    NE_DEBUG (DEBUG_GNOME, "Verified correctness, beginning fetch...\n");
	    gnome_animator_start(GNOME_ANIMATOR(fetch_anim));
	  gdk_threads_leave();
	  
	  /* commence the fetch */
	  ret = my_abortable_transfer_wrapper(selected_site,
					      site_op_fetch);
	  NE_DEBUG (DEBUG_GNOME, "Fetch terminated, checking return code...\n");

	  gdk_threads_enter();

	  /* Set the main label to indicate the terminated status. */
	  set_status_after_operation(ret, GTK_LABEL(fetch_message));

	  /* Stop the animation and restore standard dialog state. */
	  gtk_widget_set_sensitive(close_fetch_button, TRUE);
	  gtk_widget_set_sensitive(begin_fetch_button, TRUE);
	  gnome_animator_stop(GNOME_ANIMATOR(fetch_anim));
	  /* It's no longer modal when it's closed, right? */
/*	  gtk_window_set_modal(GTK_WINDOW(fetch_list_window), FALSE);*/

	  gdk_threads_leave();
      }
}


void fe_fetch_found(const struct site_file *file)
{
    gchar *info;
    gchar *col[1];

    info = g_strdup_printf("Found information for %s.", file_name(file));
    
    gdk_threads_enter();
      gtk_label_set(GTK_LABEL(fetch_message), info);
      g_free(info);

      col[0] = file_name(file);
      gtk_clist_append(GTK_CLIST(fetch_clist), col);
    gdk_threads_leave();
    
    return;
}

void fe_checksumming(const char *filename)
{
    printf("Xsitecopy: checksumming %s...", filename);
}

void fe_checksummed(const char *filename, int success,
		    const char *error)
{
    if (success)
      printf("done.\n");
    else
      printf("failed: %s.\n", error);
}



void *sync_thread(void *no_data)
{
    return NULL;
}
