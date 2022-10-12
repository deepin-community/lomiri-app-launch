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

#include <algorithm>
#include <cerrno>
#include <cstring>
#include <map>
#include <numeric>

#include "application-impl-base.h"
#include "registry-impl.h"
#include "second-exec-core.h"
#include "string-util.h"
#include "utils.h"

extern "C" {
#include "lomiri-app-launch-trace.h"
}

namespace lomiri
{
namespace app_launch
{
namespace app_impls
{

Base::Base(const std::shared_ptr<Registry::Impl>& registry)
    : registry_(registry)
{
    g_debug("Application construction:   %p", static_cast<void*>(this));
}

Base::~Base()
{
    g_debug("Application deconstruction: %p", static_cast<void*>(this));
}

bool Base::hasInstances()
{
    return !instances().empty();
}

/** Function to create all the standard environment variables that we're
    building for everyone. Mostly stuff involving paths.

    \param package Name of the package
    \param pkgdir Directory that the package lives in
*/
std::list<std::pair<std::string, std::string>> Base::confinedEnv(const std::string& package, const std::string& pkgdir)
{
    std::list<std::pair<std::string, std::string>> retval{{"LOMIRI_APPLICATION_ISOLATION", "1"}};

    /* C Funcs can return null, which offends std::string */
    auto cset = [&retval](const gchar* key, const gchar* value) {
        if (value != nullptr)
        {
            g_debug("Setting '%s' to '%s'", key, value);
            retval.emplace_back(std::make_pair(key, value));
        }
    };

    cset("XDG_CACHE_HOME", g_get_user_cache_dir());
    cset("XDG_CONFIG_HOME", g_get_user_config_dir());
    cset("XDG_DATA_HOME", g_get_user_data_dir());
    cset("XDG_RUNTIME_DIR", g_get_user_runtime_dir());

    /* Add the application's dir to the list of sources for data */
    auto basedatadirs = unique_gchar(g_strjoinv(":", (gchar**)g_get_system_data_dirs()));
    auto datadirs = unique_gchar(g_strjoin(":", pkgdir.c_str(), basedatadirs.get(), nullptr));
    cset("XDG_DATA_DIRS", datadirs.get());

    /* Set TMPDIR to something sane and application-specific */
    auto tmpdir = unique_gchar(g_strdup_printf("%s/confined/%s", g_get_user_runtime_dir(), package.c_str()));
    cset("TMPDIR", tmpdir.get());
    g_debug("Creating '%s'", tmpdir.get());
    g_mkdir_with_parents(tmpdir.get(), 0700);

    /* Do the same for nvidia */
    auto nv_shader_cachedir = unique_gchar(g_strdup_printf("%s/%s", g_get_user_cache_dir(), package.c_str()));
    cset("__GL_SHADER_DISK_CACHE_PATH", nv_shader_cachedir.get());

    return retval;
}

/** Generates an instance string based on the clock if we're a multi-instance
    application. */
std::string Base::getInstance(const std::shared_ptr<app_info::Desktop>& desktop) const
{
    if (!desktop)
    {
        g_warning("Invalid desktop file passed to getInstance");
        return {};
    }

    if (desktop->singleInstance())
    {
        return {};
    }
    else
    {
        return std::to_string(g_get_real_time());
    }
}

std::shared_ptr<Application::Instance> Base::findInstance(const pid_t& pid)
{
    for (auto instance : instances())
    {
        if (instance->hasPid(pid))
        {
            return instance;
        }
    }
    return nullptr;
}

}  // namespace app_impls
}  // namespace app_launch
}  // namespace lomiri
