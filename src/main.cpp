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

#include "helper.h"
#include "environmenthandler.h"
#include "systemhandler.h"
#include "basehandler.h"
#include "layouthandler.h"

#include <string>
#include <iomanip>
#include <iostream>
#include <fstream>

#include <ctime>
#include <sys/timeb.h>

#include <config.h>

#include <moba/message.h>
#include <moba/helper.h>

#include <moba/msghandler.h>

#include <thread>
#include <mutex>

namespace {
    moba::AppData appData = {
        PACKAGE_NAME,
        moba::Version(PACKAGE_VERSION),
        __DATE__,
        __TIME__,
        "localhost",
        7000
    };

    moba::Message::MessageType type[] = {

        moba::Message::MT_VOID,
        moba::Message::MT_ECHO_REQ,

        moba::Message::MT_RESET_CLIENT,
        moba::Message::MT_SERVER_INFO_REQ,
        moba::Message::MT_CON_CLIENTS_REQ,
        moba::Message::MT_SELF_TESTING_CLIENT,

        moba::Message::MT_GET_GLOBAL_TIMER,
        moba::Message::MT_SET_GLOBAL_TIMER,
        moba::Message::MT_GET_ENVIRONMENT,
        moba::Message::MT_SET_ENVIRONMENT,
        moba::Message::MT_GET_AMBIENCE,
        moba::Message::MT_SET_AMBIENCE,
        moba::Message::MT_GET_AUTO_MODE,
        moba::Message::MT_SET_AUTO_MODE,
        moba::Message::MT_GET_COLOR_THEME,
        moba::Message::MT_SET_COLOR_THEME,
        moba::Message::MT_GET_AMBIENT_LIGHT,
        moba::Message::MT_SET_AMBIENT_LIGHT,

        moba::Message::MT_EMERGENCY_STOP,
        moba::Message::MT_EMERGENCY_STOP_CLEARING,
        moba::Message::MT_GET_HARDWARE_STATE,
        moba::Message::MT_SET_HARDWARE_STATE,
        moba::Message::MT_HARDWARE_SHUTDOWN,
        moba::Message::MT_HARDWARE_RESET,
        moba::Message::MT_HARDWARE_SWITCH_STANDBY,

        moba::Message::MT_GET_LAYOUTS_REQ,

        moba::Message::MT_DEL_LAYOUT,
        moba::Message::MT_CREATE_LAYOUT_REQ,
        moba::Message::MT_UPDATE_LAYOUT,
        moba::Message::MT_UNLOCK_LAYOUT,
        moba::Message::MT_GET_LAYOUT_REQ

    };
}

void printCommands() {
    int size =
        sizeof(type) / sizeof(type[0]);

    for(int i = 0; i < size; ++i) {
        std::cout << type[i] << ": " << *moba::Message::convertToString(type[i]) << std::endl;
    }
    std::cout << "99: Programmende" << std::endl;
    std::cout << "?";
}

void printTimeStamp(std::ofstream &out) {
    char buffer[25] = "";
    timeb sTimeB;

    ftime(&sTimeB);

    strftime(buffer, 21, "%d.%m.%Y %H:%M:%S.", localtime(&sTimeB.time));
    out <<
        buffer << std::setw(4) << std::setfill('0') <<
        sTimeB.millitm << std::setfill(' ');
}

int main(int argc, char* argv[]) {
    switch(argc) {
        case 3:
            appData.port = atoi(argv[2]);

        case 2:
            appData.host = std::string(argv[1]);

        default:
            break;
    }
    printAppData(appData);
    moba::setCoreFileSizeToULimit();

    moba::MsgHandlerPtr msgHandler(new moba::MsgHandler());

    moba::MessagePtr msg;
    std::ofstream myfile;
    myfile.open("response.log", std::ios::app);

    try {
        msgHandler->connect(appData.host, appData.port);
        moba::JsonArrayPtr groups(new moba::JsonArray());
        groups->push_back(moba::toJsonStringPtr("BASE"));
        groups->push_back(moba::toJsonStringPtr("ENV"));
        groups->push_back(moba::toJsonStringPtr("SERV"));
        groups->push_back(moba::toJsonStringPtr("SYSTEM"));

        msgHandler->registerApp(
            appData.appName,
            appData.version,
            groups
        );
        myfile << "######################################################" << std::endl;
        myfile << "######             Application start            ######" << std::endl;
        myfile << "######################################################" << std::endl;
    } catch(moba::MsgHandlerException &e) {
        std::cerr << e.what() << std::endl;
        return (EXIT_FAILURE);
    }

    bool running = true;

    std::mutex myMutex;

    std::thread t([&] {
        while(running) {
            myMutex.lock();
            moba::MessagePtr msg = msgHandler->recieveMsg();
            if(msg) {
                printTimeStamp(myfile);
                myfile << " Response: " << msg->msgTypeAsString() << std::endl;
                moba::prettyPrint(msg->getData(), myfile);
                myfile << "******************************************************" << std::endl;
            }
            myMutex.unlock();
            usleep(250);
        }
    });

    EnvironmentHandler envHandler(msgHandler);
    SystemHandler      sysHandler(msgHandler);
    BaseHandler        basHandler(msgHandler);
    LayoutHandler      tloHandler(msgHandler);

    while(1) {
        try {
            printCommands();

            int in = 0;
            std::cin >> in;
            moba::Message::MessageType cmd = static_cast<moba::Message::MessageType>(in);
            myMutex.lock();

            if(cmd == 99) {
                msgHandler->sendClientClose();
                myMutex.unlock();
                running = false;
                t.join();
                return EXIT_SUCCESS;
            }

            printTimeStamp(myfile);
            myfile <<
                " Request: " <<
                *moba::Message::convertToString(static_cast<moba::Message::MessageType>(cmd)) <<
                std::endl;
            myfile << "******************************************************" << std::endl;

            envHandler.handle(cmd);
            sysHandler.handle(cmd);
            basHandler.handle(cmd);
            tloHandler.handle(cmd);
            myMutex.unlock();
        } catch(std::exception &e) {
            std::cerr << e.what() << std::endl;
            return (EXIT_FAILURE);
        }
    }
 }
