/*
 * Copyright 2013 Canonical Ltd.
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 3, as published
 * by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranties of
 * MERCHANTABILITY, SATISFACTORY QUALITY, or FITNESS FOR A PARTICULAR
 * PURPOSE.  See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Authors:
 *     Ted Gould <ted.gould@canonical.com>
 */

#include <json-glib/json-glib.h>
#include "utils.h"

/* Take an app ID and validate it and then break it up
   and spit it out.  These are newly allocated strings */
gboolean
app_id_to_triplet (const gchar * app_id, gchar ** package, gchar ** application, gchar ** version)
{
	/* 'Parse' the App ID */
	gchar ** app_id_segments = g_strsplit(app_id, "_", 4);
	if (g_strv_length(app_id_segments) != 3) {
		g_debug("Unable to parse Application ID: %s", app_id);
		g_strfreev(app_id_segments);
		return FALSE;
	}

	if (package != NULL) {
		*package = app_id_segments[0];
	} else {
		g_free(app_id_segments[0]);
	}

	if (application != NULL) {
		*application = app_id_segments[1];
	} else {
		g_free(app_id_segments[1]);
	}

	if (version != NULL) {
		*version = app_id_segments[2];
	} else {
		g_free(app_id_segments[2]);
	}

	g_free(app_id_segments);
	return TRUE;
}

/* Take a desktop file, make sure that it makes sense and
   then return the exec line */
gchar *
desktop_to_exec (GKeyFile * desktop_file, const gchar * from)
{
	GError * error = NULL;

	if (!g_key_file_has_group(desktop_file, "Desktop Entry")) {
		g_warning("Desktop file '%s' does not have a 'Desktop Entry' group", from);
		return NULL;
	}

	gchar * type = g_key_file_get_string(desktop_file, "Desktop Entry", "Type", &error);
	if (error != NULL) {
		g_warning("Desktop file '%s' unable to get type: %s", from, error->message);
		g_error_free(error);
		g_free(type);
		return NULL;
	}

	if (g_strcmp0(type, "Application") != 0) {
		g_warning("Desktop file '%s' has a type of '%s' instead of 'Application'", from, type);
		g_free(type);
		return NULL;
	}
	g_free(type);

	if (g_key_file_has_key(desktop_file, "Desktop Entry", "NoDisplay", NULL)) {
		gboolean nodisplay = g_key_file_get_boolean(desktop_file, "Desktop Entry", "NoDisplay", NULL);
		if (nodisplay) {
			g_warning("Desktop file '%s' is set to not display, not copying", from);
			return NULL;
		}
	}

	if (g_key_file_has_key(desktop_file, "Desktop Entry", "Hidden", NULL)) {
		gboolean hidden = g_key_file_get_boolean(desktop_file, "Desktop Entry", "Hidden", NULL);
		if (hidden) {
			g_warning("Desktop file '%s' is set to be hidden, not copying", from);
			return NULL;
		}
	}

	if (g_key_file_has_key(desktop_file, "Desktop Entry", "Terminal", NULL)) {
		gboolean terminal = g_key_file_get_boolean(desktop_file, "Desktop Entry", "Terminal", NULL);
		if (terminal) {
			g_warning("Desktop file '%s' is set to run in a terminal, not copying", from);
			return NULL;
		}
	}

	if (!g_key_file_has_key(desktop_file, "Desktop Entry", "Exec", NULL)) {
		g_warning("Desktop file '%s' has no 'Exec' key", from);
		return NULL;
	}

	gchar * exec = g_key_file_get_string(desktop_file, "Desktop Entry", "Exec", NULL);
	return exec;
}

/* Convert a URI into a file */
static gchar *
uri2file (const gchar * uri)
{
	GError * error = NULL;
	gchar * retval = g_filename_from_uri(uri, NULL, &error);

	if (error != NULL) {
		g_warning("Unable to resolve '%s' to a filename: %s", uri, error->message);
		g_error_free(error);
	}

	if (retval == NULL) {
		retval = g_strdup("");
	}

	g_debug("Converting URI '%s' to file '%s'", uri, retval);
	return retval;
}

/* Put the list of files into the argument array */
static inline void
file_list_handling (GArray * outarray, gchar ** list, gchar * (*dup_func) (const gchar * in))
{
	/* No URLs, cool, this is a noop */
	if (list == NULL || list[0] == NULL) {
		return;
	}

	int i;
	for (i = 0; list[i] != NULL; i++) {
		gchar * entry = dup_func(list[i]);

		/* No NULLs */
		if (entry != NULL && entry[0] != '\0') {
			g_array_append_val(outarray, entry);
		} else {
			g_free(entry);
		}
	}
}

/* Parse a desktop exec line and return the next string */
static void
desktop_exec_segment_parse (GArray * finalarray, const gchar * execsegment, gchar ** uri_list)
{
	/* No NULL strings */
	if (execsegment == NULL || execsegment[0] == '\0')
		return;

	/* Handle %F and %U as an argument on their own as per the spec */
	if (g_strcmp0(execsegment, "%U") == 0) {
		file_list_handling(finalarray, uri_list, g_strdup);
		return;
	}
	if (g_strcmp0(execsegment, "%F") == 0) {
		file_list_handling(finalarray, uri_list, uri2file);
		return;
	}

	/* Start looking at individual codes */
	gchar ** execsplit = g_strsplit(execsegment, "%", 0);

	/* If we didn't have any codes, just exit here */
	if (execsplit[1] == NULL) {
		g_strfreev(execsplit);
		gchar * dup = g_strdup(execsegment);
		g_array_append_val(finalarray, dup);
		return;
	}

	int i;

	gboolean previous_percent = FALSE;
	GArray * outarray = g_array_new(TRUE, FALSE, sizeof(const gchar *));
	g_array_append_val(outarray, execsplit[0]);
	gchar * single_file = NULL;

	/* The variables allowed in an exec line from the Freedesktop.org Desktop
	   File specification: http://standards.freedesktop.org/desktop-entry-spec/desktop-entry-spec-latest.html#exec-variables */
	for (i = 1; execsplit[i] != NULL; i++) {
		const gchar * skipchar = &(execsplit[i][1]);

		/* Handle the case of %%F printing "%F" */
		if (previous_percent) {
			g_array_append_val(outarray, execsplit[i]);
			previous_percent = FALSE;
			continue;
		}

		switch (execsplit[i][0]) {
		case '\0': {
			const gchar * percent = "%";
			g_array_append_val(outarray, percent); /* %% is the literal */
			previous_percent = TRUE;
			break;
		}
		case 'd':
		case 'D':
		case 'n':
		case 'N':
		case 'v':
		case 'm':
			/* Deprecated */
			g_array_append_val(outarray, skipchar);
			break;
		case 'f':
			if (uri_list != NULL && uri_list[0] != NULL) {
				if (single_file == NULL)
					single_file = uri2file(uri_list[0]);
				g_array_append_val(outarray, single_file);
			}

			g_array_append_val(outarray, skipchar);
			break;
		case 'F':
			g_warning("Exec line segment has a '%%F' that isn't its own argument '%s', ignoring.", execsegment);
			g_array_append_val(outarray, skipchar);
			break;
		case 'i':
		case 'c':
		case 'k':
			/* Perhaps?  Not sure anyone uses these */
			g_array_append_val(outarray, skipchar);
			break;
		case 'U':
			g_warning("Exec line segment has a '%%U' that isn't its own argument '%s', ignoring.", execsegment);
			g_array_append_val(outarray, skipchar);
			break;
		case 'u':
			if (uri_list != NULL && uri_list[0] != NULL) {
				g_array_append_val(outarray, uri_list[0]);
			}

			g_array_append_val(outarray, skipchar);
			break;
		default:
			g_warning("Desktop Exec line code '%%%c' unknown, skipping.", execsplit[i][0]);
			g_array_append_val(outarray, skipchar);
			break;
		}
	}

	gchar * output = g_strjoinv(NULL, (gchar **)outarray->data);
	g_array_free(outarray, TRUE);

	if (output != NULL && output[0] != '\0') {
		g_array_append_val(finalarray, output);
	} else {
		g_free(output);
	}

	g_free(single_file);
	g_strfreev(execsplit);
}

/* Take a full exec line, split it out, parse the segments and return
   it to the caller */
GArray *
desktop_exec_parse (const gchar * execline, const gchar * urilist)
{
	GError * error = NULL;
	gchar ** splitexec = NULL;
	gchar ** splituris = NULL;
	gint execitems = 0;

	/* This returns from desktop file style quoting to straight strings with
	   the appropriate characters split by the spaces that were meant for
	   splitting.  Trickier than it sounds.  But now we should be able to assume
	   that each string in the array is expected to be its own parameter. */
	g_shell_parse_argv(execline, &execitems, &splitexec, &error);

	if (error != NULL) {
		g_warning("Unable to parse exec line '%s': %s", execline, error->message);
		g_error_free(error);
		return NULL;
	}

	if (urilist != NULL && urilist[0] != '\0') {
		g_shell_parse_argv(urilist, NULL, &splituris, &error);

		if (error != NULL) {
			g_warning("Unable to parse URIs '%s': %s", urilist, error->message);
			g_error_free(error);
			/* Continuing without URIs */
			splituris = NULL;
		}
	}


	GArray * newargv = g_array_new(TRUE, FALSE, sizeof(gchar *));
	int i;
	for (i = 0; i < execitems; i++) {
		desktop_exec_segment_parse(newargv, splitexec[i], splituris);
	}
	g_strfreev(splitexec);

	if (splituris != NULL) {
		g_strfreev(splituris);
	}

	/* Each string here should be its own param */

	return newargv;
}

static void
lomiri_signal_cb (GDBusConnection * con, const gchar * sender, const gchar * path, const gchar * interface, const gchar * signal, GVariant * params, gpointer user_data)
{
	GMainLoop * mainloop = (GMainLoop *)user_data;
	g_main_loop_quit(mainloop);
}

struct _handshake_t {
	GDBusConnection * con;
	GMainLoop * mainloop;
	guint signal_subscribe;
	GSource * timeout;
};

static gboolean
lomiri_too_slow_cb (gpointer user_data)
{
	handshake_t * handshake = (handshake_t *)user_data;
	g_main_loop_quit(handshake->mainloop);
	handshake->timeout = NULL;
	return G_SOURCE_REMOVE;
}

handshake_t *
starting_handshake_start (const gchar * app_id, const gchar * instance_id, int timeout_s)
{
	GError * error = NULL;
	handshake_t * handshake = g_new0(handshake_t, 1);

	GMainContext * context = g_main_context_get_thread_default();

	handshake->mainloop = g_main_loop_new(context, FALSE);
	handshake->con = g_bus_get_sync(G_BUS_TYPE_SESSION, NULL, &error);

	if (error != NULL) {
		g_critical("Unable to connect to session bus: %s", error->message);
		g_error_free(error);
		g_free(handshake);
		return NULL;
	}

	/* Set up listening for the unfrozen signal from Lomiri */
	handshake->signal_subscribe = g_dbus_connection_signal_subscribe(handshake->con,
		NULL, /* sender */
		"com.lomiri.LomiriAppLaunch", /* interface */
		"LomiriStartingSignal", /* signal */
		"/", /* path */
		app_id, /* arg0 */
		G_DBUS_SIGNAL_FLAGS_NONE,
		lomiri_signal_cb, handshake->mainloop,
		NULL); /* user data destroy */

	/* Send unfreeze to to Lomiri */
	g_dbus_connection_emit_signal(handshake->con,
		NULL, /* destination */
		"/", /* path */
		"com.lomiri.LomiriAppLaunch", /* interface */
		"LomiriStartingBroadcast", /* signal */
		g_variant_new("(ss)", app_id, instance_id),
		&error);

	/* Really, Lomiri? */
	handshake->timeout = g_timeout_source_new_seconds(timeout_s);
	g_source_set_callback(handshake->timeout, lomiri_too_slow_cb, handshake, NULL);
	g_source_attach(handshake->timeout, context);

	return handshake;
}

void
starting_handshake_wait (handshake_t * handshake)
{
	if (handshake == NULL)
		return;

	g_main_loop_run(handshake->mainloop);

	if (handshake->timeout != NULL) {
		g_source_destroy(handshake->timeout);
		g_source_unref(handshake->timeout);
		handshake->timeout = NULL;
	}
	g_main_loop_unref(handshake->mainloop);
	g_dbus_connection_signal_unsubscribe(handshake->con, handshake->signal_subscribe);
	g_object_unref(handshake->con);

	g_free(handshake);
}

