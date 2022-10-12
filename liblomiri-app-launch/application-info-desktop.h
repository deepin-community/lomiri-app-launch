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

#pragma once

#include "application.h"
#include "registry-impl.h"
#include "registry.h"
#include <bitset>
#include <glib.h>
#include <mutex>

namespace lomiri
{
namespace app_launch
{
namespace app_info
{

namespace DesktopFlags
{
static const std::bitset<2> NONE{"00"};
static const std::bitset<2> ALLOW_NO_DISPLAY{"01"};
static const std::bitset<2> XMIR_DEFAULT{"10"};
}

class Desktop : public Application::Info
{
public:
    Desktop(const AppID& appid,
            const std::shared_ptr<GKeyFile>& keyfile,
            const std::string& basePath,
            const std::string& rootDir,
            std::bitset<2> flags,
            const std::shared_ptr<Registry::Impl>& registry);

    const Application::Info::Name& name() override
    {
        return _name;
    }
    const Application::Info::Description& description() override
    {
        return _description;
    }
    const Application::Info::IconPath& iconPath() override
    {
        return _iconPath;
    }
    const Application::Info::DefaultDepartment& defaultDepartment() override
    {
        return _defaultDepartment;
    }
    const Application::Info::IconPath& screenshotPath() override
    {
        return _screenshotPath;
    }
    const Application::Info::Keywords& keywords() override
    {
        return _keywords;
    }
    const Application::Info::Popularity& popularity() override
    {
        return _popularity;
    }

    Application::Info::Splash splash() override
    {
        return _splashInfo;
    }

    Application::Info::Orientations supportedOrientations() override
    {
        return _supportedOrientations;
    }

    Application::Info::RotatesWindow rotatesWindowContents() override
    {
        return _rotatesWindow;
    }

    Application::Info::LomiriLifecycle supportsLomiriLifecycle() override
    {
        return _lomiriLifecycle;
    }

    struct XMirEnableTag;
    typedef TypeTagger<XMirEnableTag, bool> XMirEnable;
    virtual XMirEnable xMirEnable()
    {
        return _xMirEnable;
    }

    struct ExecTag;
    typedef TypeTagger<ExecTag, std::string> Exec;
    virtual Exec execLine()
    {
        return _exec;
    }

    struct SingleInstanceTag;
    typedef TypeTagger<SingleInstanceTag, bool> SingleInstance;
    virtual SingleInstance singleInstance()
    {
        return _singleInstance;
    }

protected:
    std::shared_ptr<GKeyFile> _keyfile;
    std::string _basePath;
    std::string _rootDir;

    Application::Info::Name _name;
    Application::Info::Description _description;
    Application::Info::IconPath _iconPath;
    Application::Info::DefaultDepartment _defaultDepartment;
    Application::Info::IconPath _screenshotPath;
    Application::Info::Keywords _keywords;
    Application::Info::Popularity _popularity;

    Application::Info::Splash _splashInfo;
    Application::Info::Orientations _supportedOrientations;
    Application::Info::RotatesWindow _rotatesWindow;
    Application::Info::LomiriLifecycle _lomiriLifecycle;

    XMirEnable _xMirEnable;
    Exec _exec;
    SingleInstance _singleInstance;
};

}  // namespace AppInfo
}  // namespace AppLaunch
}  // namespace Lomiri
