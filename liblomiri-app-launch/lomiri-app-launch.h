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

#include <glib.h>
#include <mir_toolkit/mir_prompt_session.h>

#ifndef __LOMIRI_APP_LAUNCH_H__
#define __LOMIRI_APP_LAUNCH_H__ 1

#pragma GCC visibility push(default)

#ifdef __cplusplus
extern "C" {
#endif

/**
 * LomiriAppLaunchAppFailed:
 *
 * Types of failure that we report.
 */
typedef enum { /*< prefix=LOMIRI_APP_LAUNCH_APP_FAILED */
	LOMIRI_APP_LAUNCH_APP_FAILED_CRASH,         /*< nick=crash */
	LOMIRI_APP_LAUNCH_APP_FAILED_START_FAILURE  /*< nick=start-failure */
} LomiriAppLaunchAppFailed;

/**
 * LomiriAppLaunchAppObserver:
 *
 * Function prototype for application observers.
 */
typedef void (*LomiriAppLaunchAppObserver) (const gchar * appid, gpointer user_data);

/**
 * LomiriAppLaunchAppFailedObserver:
 *
 * Function prototype for application failed observers.
 */
typedef void (*LomiriAppLaunchAppFailedObserver) (const gchar * appid, LomiriAppLaunchAppFailed failure_type, gpointer user_data);

/**
 * LomiriAppLaunchAppPausedResumedObserver:
 * @appid: App ID of the application being paused
 * @pids: Zero terminated array of PIDs
 *
 * Function prototype for application paused and resumed observers.
 */
typedef void (*LomiriAppLaunchAppPausedResumedObserver) (const gchar * appid, GPid * pids, gpointer user_data);

/**
 * LomiriAppLaunchHelperObserver:
 *
 * Function to watch for helpers that are starting and stopping
 */
typedef void (*LomiriAppLaunchHelperObserver) (const gchar * appid, const gchar * instanceid, const gchar * helpertype, gpointer user_data);

/**
 * lomiri_app_launch_start_application:
 * @appid: ID of the application to launch
 * @uris: (allow-none) (array zero-terminated=1) (element-type utf8) (transfer none): A NULL terminated list of URIs to send to the application
 *
 * Asks upstart to launch an application.
 *
 * Return value: Whether the launch succeeded (may fail later, but upstart
 *    will report the error in that case.
 */
gboolean   lomiri_app_launch_start_application         (const gchar *                     appid,
                                                         const gchar * const *             uris);

/**
 * lomiri_app_launch_start_application_test:
 * @appid: ID of the application to launch
 * @uris: (allow-none) (array zero-terminated=1) (element-type utf8) (transfer none): A NULL terminated list of URIs to send to the application
 *
 * Asks upstart to launch an application with environment variables set
 * to enable testing.  Should only be used in testing.
 *
 * Return value: Whether the launch succeeded (may fail later, but upstart
 *    will report the error in that case.
 */
gboolean   lomiri_app_launch_start_application_test    (const gchar *                     appid,
                                                         const gchar * const *             uris);

/**
 * lomiri_app_launch_stop_application:
 * @appid: ID of the application to launch
 *
 * Asks upstart to stop an application.
 *
 * Return value: Whether we were able to ask Lomiri to stop the process,
 *    used lomiri_app_launch_observer_add_app_stop() to know when it is
 *    finally stopped.
 */
gboolean   lomiri_app_launch_stop_application         (const gchar *                     appid);

/**
 * lomiri_app_launch_pause_application:
 * @appid: ID of the application to pause
 *
 * Sends SIGSTOP to all processes related to the application
 *
 * Return value: Whether we were able to send SIGSTOP to all processes.
 */
gboolean   lomiri_app_launch_pause_application         (const gchar *                     appid);

/**
 * lomiri_app_launch_resume_application:
 * @appid: ID of the application to pause
 *
 * Sends SIGCONT to all processes related to the application
 *
 * Return value: Whether we were able to send SIGCONT to all processes.
 */
gboolean   lomiri_app_launch_resume_application         (const gchar *                     appid);

/**
 * lomiri_app_launch_application_info:
 * @appid: ID of the application
 * @appdir: (allow-none) (transfer full): Directory for the application
 * @appdesktop: (allow-none) (transfer full): Relative path to desktop file
 *
 * Finds a location for information on an application and the relative
 * directory that it was found in. So this should be used to find icons
 * relating to that desktop file.
 *
 * Return value: Whether @appid could be found
 */
gboolean   lomiri_app_launch_application_info         (const gchar *                     appid,
                                                       gchar **                          appdir,
                                                       gchar **                          appdesktop);

/**
 * lomiri_app_launch_observer_add_app_starting:
 * @observer: (scope notified): Callback when an application is about to start
 * @user_data: (closure) (allow-none): Data to pass to the observer
 *
 * Sets up a callback to get called each time an application
 * is about to start.  The application will not start until the
 * function returns.
 *
 * Return value: Whether adding the observer was successful.
 */
gboolean   lomiri_app_launch_observer_add_app_starting (LomiriAppLaunchAppObserver       observer,
                                                         gpointer                          user_data);
/**
 * lomiri_app_launch_observer_add_app_started:
 * @observer: (scope notified): Callback when an application started
 * @user_data: (closure) (allow-none): Data to pass to the observer
 *
 * Sets up a callback to get called each time an application
 * has been started.
 *
 * Return value: Whether adding the observer was successful.
 */
gboolean   lomiri_app_launch_observer_add_app_started  (LomiriAppLaunchAppObserver       observer,
                                                         gpointer                          user_data);
/**
 * lomiri_app_launch_observer_add_app_stop:
 * @observer: (scope notified): Callback when an application stops
 * @user_data: (closure) (allow-none): Data to pass to the observer
 *
 * Sets up a callback to get called each time an application
 * stops.
 *
 * Return value: Whether adding the observer was successful.
 */
gboolean   lomiri_app_launch_observer_add_app_stop     (LomiriAppLaunchAppObserver       observer,
                                                         gpointer                          user_data);

/**
 * lomiri_app_launch_observer_add_app_focus:
 * @observer: (scope notified): Callback when an application is started for the second time
 * @user_data: (closure) (allow-none): Data to pass to the observer
 *
 * Sets up a callback to get called each time an app gets called
 * that is already running, so we request it to be focused again.
 *
 * Return value: Whether adding the observer was successful.
 */
gboolean   lomiri_app_launch_observer_add_app_focus    (LomiriAppLaunchAppObserver       observer,
                                                         gpointer                          user_data);

/**
 * lomiri_app_launch_observer_add_app_resume:
 * @observer: (scope notified): Callback when an application is started and possibly asleep
 * @user_data: (closure) (allow-none): Data to pass to the observer
 *
 * Sets up a callback to get called each time an app gets called
 * that is already running, so we request it to be given CPU time.
 * At the end of the observer running the app as assumed to be active.
 *
 * Return value: Whether adding the observer was successful.
 */
gboolean   lomiri_app_launch_observer_add_app_resume   (LomiriAppLaunchAppObserver       observer,
                                                         gpointer                          user_data);

/**
 * lomiri_app_launch_observer_add_app_failed:
 * @observer: (scope notified): Callback when an application fails
 * @user_data: (allow-none) (closure): Data to pass to the observer
 *
 * Sets up a callback to get called each time an application
 * stops via failure.
 *
 * Return value: Whether adding the observer was successful.
 */
gboolean   lomiri_app_launch_observer_add_app_failed   (LomiriAppLaunchAppFailedObserver observer,
                                                         gpointer                          user_data);

/**
 * lomiri_app_launch_observer_add_app_paused:
 * @observer: (scope notified): Callback when an application is paused
 * @user_data: (allow-none) (closure): Data to pass to the observer
 *
 * Sets up a callback to get called each time an application
 * is paused.
 *
 * Return value: Whether adding the observer was successful.
 */
gboolean   lomiri_app_launch_observer_add_app_paused   (LomiriAppLaunchAppPausedResumedObserver observer,
                                                        gpointer                                user_data);

/**
 * lomiri_app_launch_observer_add_app_resumed:
 * @observer: (scope notified): Callback when an application is resumed
 * @user_data: (allow-none) (closure): Data to pass to the observer
 *
 * Sets up a callback to get called each time an application
 * is resumed. Which is after the SIGCONT has been sent to the pids.
 *
 * Return value: Whether adding the observer was successful.
 */
gboolean   lomiri_app_launch_observer_add_app_resumed  (LomiriAppLaunchAppPausedResumedObserver observer,
                                                        gpointer                                user_data);

/**
 * lomiri_app_launch_observer_delete_app_starting:
 * @observer: (scope notified): Callback to remove
 * @user_data: (closure) (allow-none): Data that was passed to the observer
 *
 * Removes a previously registered callback to ensure it no longer
 * gets signaled.
 *
 * Return value: Whether deleting the observer was successful.
 */
gboolean   lomiri_app_launch_observer_delete_app_starting (LomiriAppLaunchAppObserver       observer,
                                                            gpointer                          user_data);

/**
 * lomiri_app_launch_observer_delete_app_started:
 * @observer: (scope notified): Callback to remove
 * @user_data: (closure) (allow-none): Data that was passed to the observer
 *
 * Removes a previously registered callback to ensure it no longer
 * gets signaled.
 *
 * Return value: Whether deleting the observer was successful.
 */
gboolean   lomiri_app_launch_observer_delete_app_started (LomiriAppLaunchAppObserver       observer,
                                                         gpointer                          user_data);
/**
 * lomiri_app_launch_observer_delete_app_stop:
 * @observer: (scope notified): Callback to remove
 * @user_data: (closure) (allow-none): Data that was passed to the observer
 *
 * Removes a previously registered callback to ensure it no longer
 * gets signaled.
 *
 * Return value: Whether deleting the observer was successful.
 */
gboolean   lomiri_app_launch_observer_delete_app_stop  (LomiriAppLaunchAppObserver       observer,
                                                         gpointer                          user_data);

/**
 * lomiri_app_launch_observer_delete_app_focus:
 * @observer: (scope notified): Callback to remove
 * @user_data: (closure) (allow-none): Data that was passed to the observer
 *
 * Removes a previously registered callback to ensure it no longer
 * gets signaled.
 *
 * Return value: Whether deleting the observer was successful.
 */
gboolean   lomiri_app_launch_observer_delete_app_focus    (LomiriAppLaunchAppObserver       observer,
                                                            gpointer                          user_data);

/**
 * lomiri_app_launch_observer_delete_app_resume:
 * @observer: (scope notified): Callback to remove
 * @user_data: (closure) (allow-none): Data that was passed to the observer
 *
 * Removes a previously registered callback to ensure it no longer
 * gets signaled.
 *
 * Return value: Whether deleting the observer was successful.
 */
gboolean   lomiri_app_launch_observer_delete_app_resume   (LomiriAppLaunchAppObserver       observer,
                                                            gpointer                          user_data);

/**
 * lomiri_app_launch_observer_delete_app_failed:
 * @observer: (scope notified): Callback to remove
 * @user_data: (closure) (allow-none): Data to pass to the observer
 *
 * Removes a previously registered callback to ensure it no longer
 * gets signaled.
 *
 * Return value: Whether deleting the observer was successful.
 */
gboolean   lomiri_app_launch_observer_delete_app_failed (LomiriAppLaunchAppFailedObserver        observer,
                                                          gpointer                                 user_data);

/**
 * lomiri_app_launch_observer_delete_app_paused:
 * @observer: (scope notified): Callback to remove
 * @user_data: (closure) (allow-none): Data to pass to the observer
 *
 * Removes a previously registered callback to ensure it no longer
 * gets signaled.
 *
 * Return value: Whether deleting the observer was successful.
 */
gboolean   lomiri_app_launch_observer_delete_app_paused (LomiriAppLaunchAppPausedResumedObserver observer,
                                                         gpointer                                user_data);

/**
 * lomiri_app_launch_observer_delete_app_resumed:
 * @observer: (scope notified): Callback to remove
 * @user_data: (closure) (allow-none): Data to pass to the observer
 *
 * Removes a previously registered callback to ensure it no longer
 * gets signaled.
 *
 * Return value: Whether deleting the observer was successful.
 */
gboolean   lomiri_app_launch_observer_delete_app_resumed (LomiriAppLaunchAppPausedResumedObserver  observer,
                                                          gpointer                                 user_data);

/**
 * lomiri_app_launch_list_running_apps:
 *
 * Gets the Application IDs of all the running applications
 * in the system.
 *
 * Return value: (transfer full): A NULL terminated list of
 *     application IDs.  Should be free'd with g_strfreev().
 */
gchar **   lomiri_app_launch_list_running_apps         (void);

/**
 * lomiri_app_launch_get_primary_pid:
 * @appid: ID of the application to look for
 *
 * Checks to see if an application is running and returns its
 * main PID if so.
 *
 * Return Value: Either the PID of the application or 0 if it
 *     is not running.
 */
GPid       lomiri_app_launch_get_primary_pid           (const gchar *                     appid);

/**
 * lomiri_app_launch_get_pids:
 * @appid: ID of the application to look for
 *
 * Checks to see if an application is running and returns
 * the PIDs associated with it.
 *
 * Return Value: (transfer full) (element-type GLib.Pid): A list
 *   of PIDs associated with @appid, empty if not running.
 */
GList *     lomiri_app_launch_get_pids                 (const gchar *                     appid);

/**
 * lomiri_app_launch_pid_in_app_id:
 * @pid: Process ID to check on
 * @appid: ID of the application to look in
 *
 * Checks to see if a PID is associated with the current application ID.
 *
 * Currently the implementation just calls lomiri_app_launch_get_primary_pid()
 * and checks to see if they're the same.  But in the future this will check
 * any PID created in the cgroup to see if it is associated.
 *
 * Return Value: Whether @pid is associated with the @appid
 */
gboolean   lomiri_app_launch_pid_in_app_id             (GPid                              pid,
                                                         const gchar *                     appid);

/**
 * lomiri_app_launch_triplet_to_app_id:
 * @pkg: Click package name
 * @app: (allow-none): Application name, see description
 * @version: (allow-none): Specific version or wildcard, see description
 *
 * Constructs an appid from pkg, app, version triple.  Wildcards are allowed
 * for the @app and @version parameters.
 *
 * For the @app parameter the wildcards * "first-listed-app", "last-listed-app"
 * and "only-listed-app" can be used.  A NULL value will default to the
 * first listed app.
 *
 * For the @version parameter only one wildcard is allowed, "current-user-version".
 * If NULL is passed that is the default.
 *
 * Return Value: Either the properly constructed @appid or NULL if it failed 
 *     to construct it.
 */
gchar *     lomiri_app_launch_triplet_to_app_id        (const gchar *                     pkg,
                                                         const gchar *                     app,
                                                         const gchar *                     version);

/**
 * lomiri_app_launch_app_id_parse:
 * @appid: Application ID to parse
 * @package: (out) (transfer full) (allow-none): Package section of @appid
 * @application: (out) (transfer full) (allow-none): Application section of @appid
 * @version: (out) (transfer full) (allow-none): Version section of @appid
 *
 * Takes an application ID @appid and breaks it into its component parts.  Each
 * of them can be NULL if those parts aren't desired.  If all are NULL it will
 * still parse to generate a proper return value check if @appid is valid.
 *
 * Return value: Whether @appid is valid
 */
gboolean    lomiri_app_launch_app_id_parse             (const gchar *                     appid,
                                                         gchar **                          package,
                                                         gchar **                          application,
                                                         gchar **                          version);

/**
 * lomiri_app_launch_start_helper:
 * @type: Type of helper
 * @appid: App ID of the helper
 * @uris: (allow-none) (array zero-terminated=1) (element-type utf8) (transfer none): A NULL terminated list of URIs to send to the helper
 *
 * Start an untrusted helper for a specific @type on a given
 * @appid.  We don't know how that is done specifically, as Upstart
 * will call a helper for that type.  And then execute it under the
 * AppArmor profile for the helper as defined in its manifest.
 *
 * Return value: Whether the helper was able to be started
 */
gboolean   lomiri_app_launch_start_helper              (const gchar *                     type,
                                                         const gchar *                     appid,
                                                         const gchar * const *             uris);

/**
 * lomiri_app_launch_start_multiple_helper:
 * @type: Type of helper
 * @appid: App ID of the helper
 * @uris: (allow-none) (array zero-terminated=1) (element-type utf8) (transfer none): A NULL terminated list of URIs to send to the helper
 *
 * Start an untrusted helper for a specific @type of a given
 * @appid.  We don't know how that is done specifically, as Upstart
 * will call a helper for that type.  And then execute it under the
 * Apparmor profile for that helper type.  This function is different
 * from @lomiri_app_launch_start_helper in that it works for helpers
 * that aren't single instance and the manager will be managing the
 * instances as well.
 *
 * Return value: The generated instance ID or NULL on failure
 */
gchar *    lomiri_app_launch_start_multiple_helper     (const gchar *                     type,
                                                         const gchar *                     appid,
                                                         const gchar * const *             uris);

/**
 * lomiri_app_launch_start_session_helper:
 * @type: Type of helper
 * @session: Mir Trusted Prompt Session to run the helper under
 * @appid: App ID of the helper
 * @uris: (allow-none) (array zero-terminated=1) (element-type utf8) (transfer none): A NULL terminated list of URIs to send to the helper
 *
 * Start an untrusted helper for a specific @type of a given
 * @appid running under a Mir Trusted Prompt Session @session. The
 * helper's MIR_SOCKET environment variable will be set appropriately
 * so that the helper will draw on the correct surfaces. Otherwise this
 * is the same as #lomiri_app_launch_start_multiple_helper.
 *
 * It is important that all exec tools for @type call the function
 * #lomiri_app_launch_helper_set_exec to set the exec line.
 *
 * Return value: The generated instance ID or NULL on failure
 */
gchar *    lomiri_app_launch_start_session_helper  (const gchar *            type,
                                                    MirPromptSession *       session,
                                                    const gchar *            appid,
                                                    const gchar * const *    uris);

/**
 * lomiri_app_launch_stop_helper:
 * @type: Type of helper
 * @appid: App ID of the helper
 *
 * Asks Upstart to kill a helper.  In general, this should be a last resort
 * as we should ask the helper a better way probably with an in-band protocol
 * of use.
 *
 * Return value: Whether the helper is stopped
 */
gboolean   lomiri_app_launch_stop_helper               (const gchar *                     type,
                                                         const gchar *                     appid);

/**
 * lomiri_app_launch_stop_multiple_helper:
 * @type: Type of helper
 * @appid: App ID of the helper
 * @instanceid: The instance ID returned when starting the helper
 *
 * Asks Upstart to kill a helper.  In general, this should be a last resort
 * as we should ask the helper a better way probably with an in-band protocol
 * of use.
 *
 * Return value: Whether the helper is stopped
 */
gboolean   lomiri_app_launch_stop_multiple_helper      (const gchar *                     type,
                                                         const gchar *                     appid,
                                                         const gchar *                     instanceid);

/**
 * lomiri_app_launch_list_helpers:
 * @type: Type of helper
 *
 * List all App IDs of helpers of a given @type.
 *
 * Return value: (transfer full): List of application IDs
 */
gchar **   lomiri_app_launch_list_helpers              (const gchar *                     type);

/**
 * lomiri_app_launch_list_helper_instances:
 * @type: Type of helper
 * @appid: AppID of helper
 *
 * List all the instances for a particular AppID
 *
 * Return value: (transfer full): List of instance IDs
 */
gchar **   lomiri_app_launch_list_helper_instances     (const gchar *                     type,
                                                         const gchar *                     appid);


/**
 * lomiri_app_launch_observer_add_helper_started:
 * @observer: (scope notified): Callback when a helper started
 * @helper_type: (closure) (allow-none): Type of helpers to look for
 * @user_data: (allow-none): Data to pass to the observer
 *
 * Sets up a callback to get called each time a helper of
 * @helper_type has been started.
 *
 * Return value: Whether adding the observer was successful.
 */
gboolean   lomiri_app_launch_observer_add_helper_started  (LomiriAppLaunchHelperObserver    observer,
                                                            const gchar *                     helper_type,
                                                            gpointer                          user_data);
/**
 * lomiri_app_launch_observer_add_helper_stop:
 * @observer: (scope notified): Callback when a helper stops
 * @helper_type: (closure) (allow-none): Type of helpers to look for
 * @user_data: (allow-none): Data to pass to the observer
 *
 * Sets up a callback to get called each time a helper of
 * @helper_type stops.
 *
 * Return value: Whether adding the observer was successful.
 */
gboolean   lomiri_app_launch_observer_add_helper_stop       (LomiriAppLaunchHelperObserver    observer,
                                                              const gchar *                     helper_type,
                                                              gpointer                          user_data);
/**
 * lomiri_app_launch_observer_delete_helper_started:
 * @observer: (scope notified): Callback to remove
 * @helper_type: (closure) (allow-none): Type of helpers it looked for
 * @user_data: (allow-none): Data that was passed to the observer
 *
 * Removes a previously registered callback to ensure it no longer
 * gets signaled.
 *
 * Return value: Whether deleting the observer was successful.
 */
gboolean   lomiri_app_launch_observer_delete_helper_started (LomiriAppLaunchHelperObserver    observer,
                                                              const gchar *                     helper_type,
                                                              gpointer                          user_data);
/**
 * lomiri_app_launch_observer_delete_helper_stop:
 * @observer: (scope notified): Callback to remove
 * @helper_type: (closure) (allow-none): Type of helpers it looked for
 * @user_data: (allow-none): Data that was passed to the observer
 *
 * Removes a previously registered callback to ensure it no longer
 * gets signaled.
 *
 * Return value: Whether deleting the observer was successful.
 */
gboolean   lomiri_app_launch_observer_delete_helper_stop    (LomiriAppLaunchHelperObserver    observer,
                                                              const gchar *                     helper_type,
                                                              gpointer                          user_data);

/**
 * lomiri_app_launch_helper_set_exec:
 * @execline: Exec line to be executed, in Desktop file format
 * @directory: (allow-none): The directory that the exec line should
 *     be executed in.
 *
 * A function to be called by an untrusted helper exec
 * tool to set the exec line. The exec tool should determine
 * what should be executed from some sort of configuration
 * based on its type (usually a configuration file from a click
 * package). Once it determines the exec line it can set it
 * with this function and exit.
 *
 * Return Value: Whether we were able to set the exec line
 */
gboolean   lomiri_app_launch_helper_set_exec       (const gchar *            execline,
                                                    const gchar *            directory);

#ifdef __cplusplus
}
#endif

#pragma GCC visibility pop

#endif /* __LOMIRI_APP_LAUNCH_H__ */
