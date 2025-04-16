/*
 *  Project:    moba-tester
 *
 *  Copyright (C) 2016 Stefan Paproth <pappi-@gmx.de>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Affero General Public License as
 *  published by the Free Software Foundation, either version 3 of the
 *  License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU Affero General Public License for more details.
 *
 *  You should have received a copy of the GNU Affero General Public License
 *  along with this program. If not, see <http://www.gnu.org/licenses/agpl.txt>.
 *
 */

#include <iostream>

#include <gtkmm/application.h>

#include <moba-common/helper.h>

#include "frmmain.h"
#include "config.h"
#include "moba/endpoint.h"
#include "moba/socket.h"
#include "moba/nlohmann/json.hpp"

namespace {
    moba::AppData appData = {
        PACKAGE_NAME,
        moba::Version{PACKAGE_VERSION},
        __DATE__,
        __TIME__,
        "::1",
        7000
    };
}

int main(int argc, char *argv[]) {
    if(argc == 2) {
        appData.host = std::string(argv[1]);
    }

    auto socket = std::make_shared<Socket>(appData.host, appData.port);
    auto endpoint = std::make_shared<Endpoint>(socket, appData.appName, appData.version);
    auto app = Gtk::Application::create("org.moba.tester");

    return app->make_window_and_run<FrmMain>(--argc, argv, endpoint);
}
