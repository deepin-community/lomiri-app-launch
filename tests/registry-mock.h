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
#include "info-watcher-zg.h"
#include "registry-impl.h"
#include "registry.h"

#include <gmock/gmock.h>

class MockStore : public lomiri::app_launch::app_store::Base
{
public:
    MockStore(const std::shared_ptr<lomiri::app_launch::Registry::Impl>& registry)
        : lomiri::app_launch::app_store::Base(registry)
    {
    }

    ~MockStore()
    {
    }

    MOCK_METHOD1(verifyPackage, bool(const lomiri::app_launch::AppID::Package&));
    MOCK_METHOD2(verifyAppname,
                 bool(const lomiri::app_launch::AppID::Package&, const lomiri::app_launch::AppID::AppName&));
    MOCK_METHOD2(findAppname,
                 lomiri::app_launch::AppID::AppName(const lomiri::app_launch::AppID::Package&,
                                                    lomiri::app_launch::AppID::ApplicationWildcard));
    MOCK_METHOD2(findVersion,
                 lomiri::app_launch::AppID::Version(const lomiri::app_launch::AppID::Package&,
                                                    const lomiri::app_launch::AppID::AppName&));
    MOCK_METHOD1(hasAppId, bool(const lomiri::app_launch::AppID&));

    /* Possible apps */
    MOCK_METHOD0(list, std::list<std::shared_ptr<lomiri::app_launch::Application>>());

    /* Application Creation */
    MOCK_METHOD1(create, std::shared_ptr<lomiri::app_launch::app_impls::Base>(const lomiri::app_launch::AppID&));

    void mock_signalAppAdded(const std::shared_ptr<lomiri::app_launch::Application>& app)
    {
        appAdded_(app);
    }
    void mock_signalAppRemoved(const lomiri::app_launch::AppID& appid)
    {
        appRemoved_(appid);
    }
    void mock_signalAppInfoChanged(const std::shared_ptr<lomiri::app_launch::Application>& app)
    {
        infoChanged_(app);
    }
};

class MockApp : public lomiri::app_launch::app_impls::Base
{
public:
    lomiri::app_launch::AppID appid_;

    MockApp(const lomiri::app_launch::AppID& appid, const std::shared_ptr<lomiri::app_launch::Registry::Impl>& reg)
        : lomiri::app_launch::app_impls::Base(reg)
        , appid_(appid)
    {
    }
    ~MockApp()
    {
    }

    lomiri::app_launch::AppID appId() const override
    {
        return appid_;
    }

    MOCK_METHOD1(findInstance, std::shared_ptr<lomiri::app_launch::Application::Instance>(const std::string&));
    MOCK_METHOD1(findInstance, std::shared_ptr<lomiri::app_launch::Application::Instance>(const pid_t&));
    MOCK_METHOD0(info, std::shared_ptr<lomiri::app_launch::Application::Info>());
    MOCK_METHOD0(hasInstances, bool());
    MOCK_METHOD0(instances, std::vector<std::shared_ptr<lomiri::app_launch::Application::Instance>>());
    MOCK_METHOD1(launch,
                 std::shared_ptr<lomiri::app_launch::Application::Instance>(
                     const std::vector<lomiri::app_launch::Application::URL>&));
    MOCK_METHOD1(launchTest,
                 std::shared_ptr<lomiri::app_launch::Application::Instance>(
                     const std::vector<lomiri::app_launch::Application::URL>&));
};

class MockInst : public lomiri::app_launch::jobs::instance::Base
{
public:
    MockInst(const lomiri::app_launch::AppID& appId,
             const std::string& job,
             const std::string& instance,
             const std::vector<lomiri::app_launch::Application::URL>& urls,
             const std::shared_ptr<lomiri::app_launch::Registry::Impl>& registry)
        : lomiri::app_launch::jobs::instance::Base(appId, job, instance, urls, registry){};
    ~MockInst(){};

    MOCK_METHOD0(pids, std::vector<pid_t>());
    MOCK_METHOD0(primaryPid, pid_t());
    MOCK_METHOD0(stop, void());
};

class MockJobsManager : public lomiri::app_launch::jobs::manager::Base
{
public:
    MockJobsManager(const std::shared_ptr<lomiri::app_launch::Registry::Impl>& reg)
        : lomiri::app_launch::jobs::manager::Base(reg)
    {
    }
    ~MockJobsManager()
    {
    }

    MOCK_METHOD6(launch,
                 std::shared_ptr<lomiri::app_launch::Application::Instance>(
                     const lomiri::app_launch::AppID&,
                     const std::string&,
                     const std::string&,
                     const std::vector<lomiri::app_launch::Application::URL>&,
                     lomiri::app_launch::jobs::manager::launchMode,
                     std::function<std::list<std::pair<std::string, std::string>>(void)>&));

    MOCK_METHOD4(existing,
                 std::shared_ptr<lomiri::app_launch::Application::Instance>(
                     const lomiri::app_launch::AppID&,
                     const std::string&,
                     const std::string&,
                     const std::vector<lomiri::app_launch::Application::URL>&));

    MOCK_METHOD0(runningApps, std::list<std::shared_ptr<lomiri::app_launch::Application>>());
    MOCK_METHOD1(runningHelpers,
                 std::list<std::shared_ptr<lomiri::app_launch::Helper>>(const lomiri::app_launch::Helper::Type&));

    MOCK_METHOD1(runningAppIds, std::list<std::string>(const std::list<std::string>&));

    MOCK_METHOD2(instances,
                 std::vector<std::shared_ptr<lomiri::app_launch::jobs::instance::Base>>(
                     const lomiri::app_launch::AppID& appID, const std::string& job));

    /* Job signals from implementations */
    MOCK_METHOD0(jobStarted, core::Signal<const std::string&, const std::string&, const std::string&>&());
    MOCK_METHOD0(jobStopped, core::Signal<const std::string&, const std::string&, const std::string&>&());
    MOCK_METHOD0(jobFailed,
                 core::Signal<const std::string&,
                              const std::string&,
                              const std::string&,
                              lomiri::app_launch::Registry::FailureType>&());
};

class zgWatcherMock : public lomiri::app_launch::info_watcher::Zeitgeist
{
public:
    zgWatcherMock(const std::shared_ptr<lomiri::app_launch::Registry::Impl>& registry)
        : lomiri::app_launch::info_watcher::Zeitgeist(registry)
    {
    }

    virtual ~zgWatcherMock()
    {
    }

    MOCK_METHOD1(lookupAppPopularity,
                 lomiri::app_launch::Application::Info::Popularity(const lomiri::app_launch::AppID&));
};

class RegistryImplMock : public lomiri::app_launch::Registry::Impl
{
public:
    RegistryImplMock()
        : lomiri::app_launch::Registry::Impl()
    {
        g_debug("Registry Mock Implementation Created");
    }

    ~RegistryImplMock()
    {
        g_debug("Registry Mock Implementation taken down");
    }

    MOCK_METHOD2(zgSendEvent, void(lomiri::app_launch::AppID, const std::string& eventtype));
};

class RegistryMock : public lomiri::app_launch::Registry
{
public:
    RegistryMock()
        : Registry(std::make_shared<RegistryImplMock>())
    {
        auto zgWatcher = std::make_shared<zgWatcherMock>(impl);
        ON_CALL(*zgWatcher, lookupAppPopularity(testing::_))
            .WillByDefault(testing::Return(lomiri::app_launch::Application::Info::Popularity::from_raw(1u)));
        impl->setZgWatcher(zgWatcher);

        g_debug("Registry Mock Created");
    }

    RegistryMock(std::list<std::shared_ptr<lomiri::app_launch::app_store::Base>> appStores,
                 std::shared_ptr<lomiri::app_launch::jobs::manager::Base> jobManager)
        : Registry(std::make_shared<RegistryImplMock>())
    {
        impl->setAppStores(appStores);
        impl->setJobs(jobManager);

        auto zgWatcher = std::make_shared<zgWatcherMock>(impl);
        ON_CALL(*zgWatcher, lookupAppPopularity(testing::_))
            .WillByDefault(testing::Return(lomiri::app_launch::Application::Info::Popularity::from_raw(1u)));
        impl->setZgWatcher(zgWatcher);

        g_debug("Registry Mock Created");
    }

    ~RegistryMock()
    {
        g_debug("Registry Mock taken down");
    }
};
