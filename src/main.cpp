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

#include <moba/helper.h>

#include "frmmain.h"
#include "config.h"
#include "moba/endpoint.h"
#include "moba/socket.h"

namespace {
    moba::AppData appData = {
        PACKAGE_NAME,
        moba::Version(PACKAGE_VERSION),
        __DATE__,
        __TIME__,
        "localhost",
        7000
    };
}

int main(int argc, char *argv[]) {
    moba::setCoreFileSizeToULimit();

    SocketPtr   socket(new Socket{appData.host, appData.port});
    EndpointPtr endpoint(new Endpoint{socket});

    try {
        endpoint->connect(
            appData.appName,
            appData.version,
            moba::JsonArrayPtr{new moba::JsonArray()}
        );
    } catch(SocketException &e) {
        std::cerr << e.what() << std::endl;
    }

    auto app = Gtk::Application::create(argc, argv, "org.moba.tester");

    FrmMain frmMain{endpoint};
    frmMain.set_title(appData.appName);
    frmMain.set_border_width(10);
    frmMain.set_default_size(800, 600);

    return app->run(frmMain);
}
