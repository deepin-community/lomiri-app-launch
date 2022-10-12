/*
 * Copyright 2016 Canonical Ltd.
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

#include "liblomiri-app-launch/application.h"
#include "liblomiri-app-launch/registry.h"
#include <iostream>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " (appid)" << std::endl;
        exit(1);
    }

    auto appid = lomiri::app_launch::AppID::find(argv[1]);
    if (appid.empty())
    {
        std::cerr << "Unable to find app for appid: " << argv[1] << std::endl;
        return 1;
    }

    std::shared_ptr<lomiri::app_launch::Application> app;
    try
    {
        app = lomiri::app_launch::Application::create(appid, lomiri::app_launch::Registry::getDefault());
        if (!app)
            throw std::runtime_error("Application object is nullptr");
    }
    catch (std::runtime_error &e)
    {
        std::cerr << "Unable to find application for AppID: " << argv[1] << std::endl;
        exit(1);
    }

    try
    {
        auto info = app->info();

        std::cout << "Name:             " << info->name().value() << std::endl;
        std::cout << "Description:      " << info->description().value() << std::endl;
        std::cout << "Icon Path:        " << info->iconPath().value() << std::endl;
        std::cout << "Splash:           " << std::endl;
        std::cout << "  Title:          " << info->splash().title.value() << std::endl;
        std::cout << "  Image:          " << info->splash().image.value() << std::endl;
        std::cout << "  BG Color:       " << info->splash().backgroundColor.value() << std::endl;
        std::cout << "  Header Color:   " << info->splash().headerColor.value() << std::endl;
        std::cout << "  Footer Color:   " << info->splash().footerColor.value() << std::endl;
        std::cout << "  Show Header:    " << info->splash().showHeader.value() << std::endl;
        std::cout << "Orientations:     " << std::endl;
        std::cout << "  Portrait:       " << info->supportedOrientations().portrait << std::endl;
        std::cout << "  Landscape:      " << info->supportedOrientations().landscape << std::endl;
        std::cout << "  Inv Portrait:   " << info->supportedOrientations().invertedPortrait << std::endl;
        std::cout << "  Inv Landscape:  " << info->supportedOrientations().invertedLandscape << std::endl;
        std::cout << "Rotates:          " << info->rotatesWindowContents().value() << std::endl;
        std::cout << "Lomiri Lifecycle: " << info->supportsLomiriLifecycle().value() << std::endl;
    }
    catch (std::runtime_error &e)
    {
        std::cerr << "Unable to parse Application info for application '" << std::string(appid) << "': " << e.what()
                  << std::endl;
        exit(1);
    }

    return 0;
}
