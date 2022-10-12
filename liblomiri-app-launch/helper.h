/*
 * Copyright © 2016 Canonical Ltd.
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

#include <memory>
#include <vector>

#include <mir_toolkit/mir_prompt_session.h>

#include "appid.h"
#include "type-tagger.h"

#pragma once
#pragma GCC visibility push(default)

namespace lomiri
{
namespace app_launch
{

class Registry;

/** Class representing an untrusted helper in the system.  Untrusted
    helpers are used by trusted helpers to get some amount of functionality
    from a package in the system. Typically this is via a Click hook
    in a Click package.

    In order to setup a untrusted helper the trusted helper needs to
    install a small executable that gives the equivallent of a Desktop
    Exec string to the system. This is done by installing the executable
    in the ``/usr/lib/$(arch)/lomiri-app-launch/$(helper type)/exec-tool``.
    A simple example can be seen in [URL Dispatcher's URL Overlay
   helper](http://bazaar.launchpad.net/~indicator-applet-developers/url-dispatcher/trunk.15.10/view/head:/service/url-overlay.c).
    It is important to note that the helper will be confined with the apparmor
    profile associated with the AppID that is being used. For Click based
    applications this means that an untrusted helper should be its own
    stanza in the Click manifest with its own ``apparmor`` hook. This will
    configure the confinement for the helper.

    Many times an untrusted helper runs in a non-user-facing mode, it is
    important that UAL **DOES NOT** implement a lifecycle for the helper. It
    is the responsibility of the trusted helper to do that. Many times this
    is a timeout or other similar functionality. These are the tools to
    implement those in a reasonable fashion (services don't have to worry
    about AppArmor, cgroups, or jobs) but it doesn't not implement them
    by itself.
*/
class Helper
{
protected:
    Helper() = default;
    virtual ~Helper() = default;

public:
    /** \private */
    struct TypeTag;
    /** \private */
    struct URLTag;

    /** \private */
    typedef TypeTagger<TypeTag, std::string> Type;
    /** \private */
    typedef TypeTagger<URLTag, std::string> URL;

    /** Create a new helper object from an AppID

        \param type Type of untrusted helper
        \param appid AppID of the helper
        \param registry Shared registry instance
    */
    static std::shared_ptr<Helper> create(Type type, AppID appid, std::shared_ptr<Registry> registry);

    /** Get the AppID for this helper */
    virtual AppID appId() const = 0;

    /** Running instance of a a Helper */
    class Instance
    {
    protected:
        Instance() = default;
        virtual ~Instance() = default;

    public:
        /** Check to see if this instance is running */
        virtual bool isRunning() = 0;

        /** Stop a running helper */
        virtual void stop() = 0;

        bool operator==(const Instance& b) const;
        bool operator!=(const Instance& b) const;
    };

    /** Check to see if there are any instances of this untrusted helper */
    virtual bool hasInstances() = 0;
    /** Get the list of instances of this helper */
    virtual std::vector<std::shared_ptr<Instance>> instances() = 0;

    /** Launch an instance of a helper with an optional set of URLs
        that get passed to the helper.

        \param urls List of URLs to passed to the untrusted helper
    */
    virtual std::shared_ptr<Instance> launch(std::vector<URL> urls = {}) = 0;
    /** Launch an instance of a helper that is run in a Mir Trusted
        Prompt session. The session should be created by the trusted
        helper using the Mir function ``mir_connection_create_prompt_session_sync()``.

        \param session Mir trusted prompt session
        \param urls List of URLs to passed to the untrusted helper
    */
    virtual std::shared_ptr<Instance> launch(MirPromptSession* session, std::vector<URL> urls = {}) = 0;
    /** Set the exec from a helper utility. This function should only
        be used inside a helper exec util.

        \param exec The exec line to use for the helper with the AppID given
    */
    static void setExec(std::vector<std::string> exec);

    bool operator==(const Helper& b) const;
    bool operator!=(const Helper& b) const;
};

}  // namespace app_launch
}  // namespace lomiri

#pragma GCC visibility pop
