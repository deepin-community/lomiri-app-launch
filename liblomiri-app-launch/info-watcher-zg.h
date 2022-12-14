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

#include "application.h"
#include "info-watcher.h"
#include "registry.h"

#include <core/signal.h>
#include <set>

namespace lomiri
{
namespace app_launch
{
namespace info_watcher
{

class Zeitgeist : public Base
{
public:
    Zeitgeist(const std::shared_ptr<Registry::Impl>& registry);
    virtual ~Zeitgeist() = default;

    virtual Application::Info::Popularity lookupAppPopularity(const AppID& appid);
};

}  // namespace info_watcher
}  // namespace app_launch
}  // namespace lomiri
