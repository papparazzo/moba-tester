/*
 *  Project:    Client-Application
 *
 *  Version:    1.0.0
 *  History:    V1.0    15/10/2013  SP - created
 *
 *  Copyright (C) 2013 Stefan Paproth <pappi-@gmx.de>
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

#include <stdio.h>
#include <stdlib.h>
#include <libgen.h>
#include <unistd.h>
#include <vector>
#include <string>
#include <iomanip>
#include <iostream>
#include <fstream>

#include <ctime>
#include <sys/timeb.h>

#include <thread>
#include <mutex>

#include <moba/log.h>
#include <moba/msghandler.h>
#include <moba/helper.h>

#include <moba/version.h>
#include <moba/message.h>
#include <moba/jsonabstractitem.h>

namespace {
    moba::AppData appData = {
        "",
        moba::Version(1, 0, 0),
        __DATE__,
        __TIME__,
        "localhost",
        7000
    };

    struct MsgEnumClearTextStruct {
        moba::Message::MessageType type;
        std::string msg;
    } msgEnumClearTextStruct[] = {
        {moba::Message::MT_VOID,                    "VOID"                   },
        {moba::Message::MT_ECHO_REQ,                "ECHO_REQ"               },

        {moba::Message::MT_RESET_CLIENT,            "RESET_CLIENT"           },
        {moba::Message::MT_SERVER_INFO_REQ,         "SERVER_INFO_REQ"        },
        {moba::Message::MT_CON_CLIENTS_REQ,         "CON_CLIENTS_REQ"        },
        {moba::Message::MT_SELF_TESTING_CLIENT,     "SELF_TESTING_CLIENT"    },

        {moba::Message::MT_GET_GLOBAL_TIMER,        "GET_GLOBAL_TIMER"       },
        {moba::Message::MT_SET_GLOBAL_TIMER,        "SET_GLOBAL_TIMER"       },
        {moba::Message::MT_GET_ENVIRONMENT,         "GET_ENVIRONMENT"        },
        {moba::Message::MT_SET_ENVIRONMENT,         "SET_ENVIRONMENT"        },
        {moba::Message::MT_GET_AMBIENCE,            "GET_AMBIENCE"           },
        {moba::Message::MT_SET_AMBIENCE,            "SET_AMBIENCE"           },
        {moba::Message::MT_GET_AUTO_MODE,           "GET_AUTO_MODE"          },
        {moba::Message::MT_SET_AUTO_MODE,           "SET_AUTO_MODE"          },
        {moba::Message::MT_GET_COLOR_THEME,         "GET_COLOR_THEME"        },
        {moba::Message::MT_SET_COLOR_THEME,         "SET_COLOR_THEME"        },
        {moba::Message::MT_GET_AMBIENT_LIGHT,       "GET_AMBIENT_LIGHT"      },
        {moba::Message::MT_SET_AMBIENT_LIGHT,       "SET_AMBIENT_LIGHT"      },

        {moba::Message::MT_EMERGENCY_STOP,          "EMERGENCY_STOP"         },
        {moba::Message::MT_EMERGENCY_STOP_CLEARING, "EMERGENCY_STOP_CLEARING"},
        {moba::Message::MT_GET_HARDWARE_STATE,      "GET_HARDWARE_STATE"     },
        {moba::Message::MT_SET_HARDWARE_STATE,      "SET_HARDWARE_STATE"     },
        {moba::Message::MT_HARDWARE_SHUTDOWN,       "HARDWARE_SHUTDOWN"      },
        {moba::Message::MT_HARDWARE_RESET,          "HARDWARE_RESET"         },
        {moba::Message::MT_HARDWARE_SWITCH_STANDBY, "HARDWARE_SWITCH_STANDBY"},
    };
}

void prettyPrint(moba::JsonItemPtr ptr, std::ofstream &out) {

    if(!ptr) {
        out << "{}";
        return;
    }

    std::string str = ptr->getJsonString();
    unsigned int indent = 0;

    for(size_t i = 0; i < str.length(); ++i) {
        switch(str[i]) {
            case '{':
            case '[':
                out << str[i] << std::endl << std::setw(++indent * 4);
                break;

            case ',':
                out << str[i] << std::endl << std::setw(indent * 4);
                break;

            case '}':
            case ']':
                out << std::endl << std::setw(--indent * 4) << str[i];
                break;

            default:
                out << str[i];
        }
    }
    out << std::endl;
    return;
}

void printCommands() {
    int size =
        sizeof(msgEnumClearTextStruct) / sizeof(msgEnumClearTextStruct[0]);

    for(int i = 0; i < size; ++i) {
        std::cout <<
            msgEnumClearTextStruct[i].type << ": " <<
            msgEnumClearTextStruct[i].msg << std::endl;
    }
    std::cout << "98: Puffer leeren" << std::endl;
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

template<typename T>
void getData(const std::string &s, T &data) {
    std::cout << s << "?";
    std::cin >> data;
    std::cout << std::endl;
}

std::string getString(const std::string &s) {
    std::string str;
    std::cout << s << "?";
    char input[256];
    std::cin.getline(input, sizeof(input));
    return input;
}

bool getBool(const std::string &s) {
    std::string str;
    getData(s + "[t|f]", str);
    if(str == "t") {
        return true;
    }
    return false;
}

moba::JsonThreeState::ThreeState getThreeState(const std::string &s) {
    std::string str;
    getData(s + "[on|off|auto|unset]", str);
    if(str == "on") {
        return moba::JsonThreeState::ON;
    }
    if(str == "off") {
        return moba::JsonThreeState::OFF;
    }
    if(str == "auto") {
        return moba::JsonThreeState::AUTO;
    }
    return moba::JsonThreeState::UNSET;
}

moba::JsonToggleState::ToggleState getToggleState(const std::string &s) {
    std::string str;
    getData(s + "[on|off|unset]", str);
    if(str == "on") {
        return moba::JsonToggleState::ON;
    }
    if(str == "off") {
        return moba::JsonToggleState::OFF;
    }
    return moba::JsonToggleState::UNSET;
}

moba::JsonSwitch::Switch getSwitchState(const std::string &s) {
    std::string str;
    getData(s + "[on|off|auto|unset|trigger]", str);
    if(str == "on") {
        return moba::JsonSwitch::ON;
    }
    if(str == "off") {
        return moba::JsonSwitch::OFF;
    }
    if(str == "auto") {
        return moba::JsonSwitch::AUTO;
    }
    if(str == "trigger") {
        return moba::JsonSwitch::TRIGGER;
    }
    return moba::JsonSwitch::UNSET;
}

int main(int argc, char* argv[]) {

    std::cerr << moba::baseName("/bima/buu");

    switch(argc) {
        case 3:
            appData.port = atoi(argv[2]);

        case 2:
            appData.host = std::string(argv[1]);

        case 1:
            appData.appName = basename(argv[0]);

        default:
            break;
    }
    printAppData(appData);
    moba::setCoreFileSizeToULimit();

    moba::MsgHandler msgHandler;
    moba::MessagePtr msg;
    std::ofstream myfile;
    myfile.open("response.log", std::ios::app);

    try {
        msgHandler.connect(appData.host, appData.port);
        moba::JsonArrayPtr groups(new moba::JsonArray());
        groups->push_back(moba::toJsonStringPtr("BASE"));
        groups->push_back(moba::toJsonStringPtr("ENV"));
        groups->push_back(moba::toJsonStringPtr("SERV"));
        groups->push_back(moba::toJsonStringPtr("SYSTEM"));

        msgHandler.registerApp(
            appData.appName,
            moba::Version(appData.version),
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
            moba::MessagePtr msg = msgHandler.recieveMsg();
            if(msg) {
                printTimeStamp(myfile);
                myfile << " Response: " << msg->msgTypeAsString() << std::endl;
                prettyPrint(msg->getData(), myfile);
                myfile << "******************************************************" << std::endl;
            }
            myMutex.unlock();
            usleep(250);
        }
    });

    while(1) {
        try {
            printCommands();

            int cmd = 0;
            std::cin >> cmd;

            myMutex.lock();
            printTimeStamp(myfile);
            myfile <<
                " Request: " <<
                *moba::Message::convertToString(static_cast<moba::Message::MessageType>(cmd)) <<
                std::endl;
            myfile << "******************************************************" << std::endl;

            switch(cmd) {
                case moba::Message::MT_VOID:
                    msgHandler.sendVoid();
                    break;

                case moba::Message::MT_ECHO_REQ: {
                    std::string str;
                    getData("Data", str);
                    msgHandler.sendEchoReq(str);
                    break;
                }

                case moba::Message::MT_RESET_CLIENT: {
                    int id = 0;
                    getData("AppId", id);
                    msgHandler.sendResetClient(id);
                    break;
                }

                case moba::Message::MT_SELF_TESTING_CLIENT: {
                    int id = 0;
                    getData("AppId", id);
                    msgHandler.sendSelfTestingClient(id);
                    break;
                }

                case moba::Message::MT_SERVER_INFO_REQ: {
                    msgHandler.sendServerInfoReq();
                    break;
                }

                case moba::Message::MT_CON_CLIENTS_REQ: {
                    msgHandler.sendConClientsReq();
                    break;
                }

                case moba::Message::MT_GET_GLOBAL_TIMER: {
                    msgHandler.sendGetGlobalTimer();
                    break;
                }

                case moba::Message::MT_SET_GLOBAL_TIMER: {
                    std::string date = "Sa 10:00";  // FIXME getString("DD hh:mm (e.g. Sa 10:00)");
                    unsigned int intervall = 0;
                    getData("Seconds (modulo 5)", intervall);
                    unsigned int multiplicator = 0;
                    getData("Multiplicator (realtime * multiplicator = modeltime)", multiplicator);
                    msgHandler.sendSetGlobalTimer(date, intervall, multiplicator);
                    break;
                }

                case moba::Message::MT_GET_ENVIRONMENT:
                    msgHandler.sendGetEnvironment();
                    break;

                case moba::Message::MT_SET_ENVIRONMENT:
                    msgHandler.sendSetEnvironment(
                        getSwitchState("thunder"),
                        getSwitchState("wind"),
                        getSwitchState("rain"),
                        getSwitchState("environmentSound"),
                        getSwitchState("aux01"),
                        getSwitchState("aux02"),
                        getSwitchState("aux03")
                    );
                    break;

                case moba::Message::MT_GET_AMBIENCE: {
                    msgHandler.sendGetAmbience();
                    break;
                }

                case moba::Message::MT_SET_AMBIENCE: {
                    msgHandler.sendSetAmbience(
                        getToggleState("curtainUp"),
                        getToggleState("mainLightOn")
                    );
                    break;
                }

                case moba::Message::MT_GET_AMBIENT_LIGHT: {
                    msgHandler.sendGetAmbientLight();
                    break;
                }

                case moba::Message::MT_SET_AMBIENT_LIGHT: {
                    std::vector<moba::MsgHandler::AmbientLightData> v;
                    moba::MsgHandler::AmbientLightData ald;
                    for(int i = 0; i < 3; ++i) {
                        getData("red", ald.red);
                        getData("blue", ald.blue);
                        getData("white", ald.white);
                        v.push_back(ald);
                    }
                    msgHandler.sendSetAmbientLight(v);
                    break;
                }

                case moba::Message::MT_GET_AUTO_MODE: {
                    msgHandler.sendGetAutoMode();
                    break;
                }

                case moba::Message::MT_SET_AUTO_MODE: {
                    msgHandler.sendSetAutoMode(getBool(""));
                    break;
                }

                case moba::Message::MT_GET_COLOR_THEME: {
                    msgHandler.sendGetColorTheme();
                    break;
                }

                case moba::Message::MT_SET_COLOR_THEME: {
                    msgHandler.sendSetColorTheme(
                        getString("dimTime [hh:mm]"),
                        getString("brightTime [hh:mm]"),
                        getThreeState("condition")
                    );
                    break;
                }

                case moba::Message::MT_EMERGENCY_STOP: {
                    msgHandler.sendEmergencyStop();
                    break;
                }

                case moba::Message::MT_EMERGENCY_STOP_CLEARING: {
                    msgHandler.sendEmergencyStopClearing();
                    break;
                }

                case moba::Message::MT_GET_HARDWARE_STATE: {
                    msgHandler.sendGetHardwareState();
                    break;
                }

                case moba::Message::MT_SET_HARDWARE_STATE: {
                    moba::MsgHandler::HardwareState hs = moba::MsgHandler::HS_ERROR;
                    std::string s;
                    getData("ready, error, standby, power off [r|e|s|p]", s);
                    if(s == "r") {
                        hs = moba::MsgHandler::HS_READY;
                    } else if(s == "s") {
                        hs = moba::MsgHandler::HS_STANDBY;
                    } else if(s == "p") {
                        hs = moba::MsgHandler::HS_POWER_OFF;
                    }
                    msgHandler.sendSetHardwareState(hs);
                    break;
                }

                case moba::Message::MT_HARDWARE_SHUTDOWN: {
                    msgHandler.sendHardwareShutdown();
                    break;
                }

                case moba::Message::MT_HARDWARE_RESET: {
                    msgHandler.sendHardwareReset();
                    break;
                }

                case moba::Message::MT_HARDWARE_SWITCH_STANDBY: {
                    msgHandler.sendHardwareSwitchStandby();
                    break;
                }

                case 98: {
                    break;
                }

                case 99: {
                    msgHandler.sendClientClose();
                    myMutex.unlock();
                    running = false;
                    t.join();
                    return EXIT_SUCCESS;
                }
            }
            myMutex.unlock();
        } catch(std::exception &e) {
            std::cerr << e.what() << std::endl;
            return (EXIT_FAILURE);
        }
    }
 }

