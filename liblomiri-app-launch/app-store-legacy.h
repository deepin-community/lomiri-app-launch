/*
 * Copyright © 2017 Canonical Ltd.
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

#include "app-store-base.h"

#include <lomiri/util/GObjectMemory.h>

namespace lomiri
{
namespace app_launch
{
namespace app_store
{

class Legacy : public Base
{
public:
    Legacy(const std::shared_ptr<Registry::Impl>& registry);
    virtual ~Legacy();

    /* Discover tools */
    virtual bool verifyPackage(const AppID::Package& package) override;
    virtual bool verifyAppname(const AppID::Package& package, const AppID::AppName& appname) override;
    virtual AppID::AppName findAppname(const AppID::Package& package, AppID::ApplicationWildcard card) override;
    virtual AppID::Version findVersion(const AppID::Package& package, const AppID::AppName& appname) override;
    virtual bool hasAppId(const AppID& appid) override;

    /* Possible apps */
    virtual std::list<std::shared_ptr<Application>> list() override;

    /* Application Creation */
    virtual std::shared_ptr<app_impls::Base> create(const AppID& appid) override;

    /* Info watching */
    virtual core::Signal<const std::shared_ptr<Application>&>& infoChanged() override;
    virtual core::Signal<const std::shared_ptr<Application>&>& appAdded() override;
    virtual core::Signal<const AppID&>& appRemoved() override;

private:
    std::set<std::unique_ptr<GFileMonitor, lomiri::util::GObjectDeleter>> monitors_;
    std::once_flag monitorsSetup_;

    void directoryChanged(GFile* file, GFileMonitorEvent type);
    void setupMonitors();
};

}  // namespace app_store
}  // namespace app_launch
}  // namespace lomiri
