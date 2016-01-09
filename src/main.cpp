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

#include <moba/log.h>
#include <moba/msghandler.h>
#include <moba/helper.h>

#include <moba/version.h>

namespace {
    AppData appData = {
        "",
        "1.0.0",
        __DATE__,
        __TIME__,
        "localhost",
        7000
    };

    struct MsgEnumClearTextStruct {
        Message::MessageType type;
        std::string msg;
    } msgEnumClearTextStruct[] = {
        {Message::MT_VOID,                    "VOID"                   },
        {Message::MT_ECHO_REQ,                "ECHO_REQ"               },

        {Message::MT_RESET_CLIENT,            "RESET_CLIENT"           },

        {Message::MT_SERVER_INFO_REQ,         "SERVER_INFO_REQ"        },
        {Message::MT_CON_CLIENTS_REQ,         "CON_CLIENTS_REQ"        },

        {Message::MT_GET_GLOBAL_TIMER,        "GET_GLOBAL_TIMER"       },
        {Message::MT_SET_GLOBAL_TIMER,        "SET_GLOBAL_TIMER"       },
        {Message::MT_GET_ENVIRONMENT,         "GET_ENVIRONMENT"        },
        {Message::MT_SET_ENVIRONMENT,         "SET_ENVIRONMENT"        },
        {Message::MT_SET_AUTO_MODE,           "SET_AUTO_MODE"          },
        {Message::MT_GET_COLOR_THEME,         "GET_COLOR_THEME"        },
        {Message::MT_SET_COLOR_THEME,         "SET_COLOR_THEME"        },

        {Message::MT_EMERGENCY_STOP,          "EMERGENCY_STOP"         },
        {Message::MT_EMERGENCY_STOP_CLEARING, "EMERGENCY_STOP_CLEARING"},
        {Message::MT_GET_HARDWARE_STATE,      "GET_HARDWARE_STATE"     },
        {Message::MT_SET_HARDWARE_STATE,      "SET_HARDWARE_STATE"     },
        {Message::MT_HARDWARE_SHUTDOWN,       "HARDWARE_SHUTDOWN"      },
        {Message::MT_HARDWARE_RESET,          "HARDWARE_RESET"         },
        {Message::MT_HARDWARE_SWITCH_STANDBY, "HARDWARE_SWITCH_STANDBY"},
    };
}

bool prettyPrint(JsonItemPtr ptr) {
    std::ofstream myfile;
    myfile.open("out.txt", std::ios::app);

    if(!ptr) {
        myfile << "{}";
        return false;
    }

    std::string str = ptr->getJsonString();
    unsigned int indent = 0;

    for(size_t i = 0; i < str.length(); ++i) {
        switch(str[i]) {
            case '{':
            case '[':
                myfile << str[i] << std::endl << std::setw(++indent * 4);
                break;

            case ',':
                myfile << str[i] << std::endl << std::setw(indent * 4);
                break;

            case '}':
            case ']':
                myfile << std::endl << std::setw(--indent * 4) << str[i];
                break;

            default:
                myfile << str[i];
        }
    }
    myfile << std::endl;
    return true;
}

void printCommands() {
    system("clear");
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

int main(int argc, char* argv[]) {
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
    setCoreFileSizeToULimit();

    MsgHandler msgHandler;
    MessagePtr msg;
    try {
      msgHandler.connect(appData.host, appData.port);
      JsonArrayPtr groups(new JsonArray());
      groups->push_back(toJsonStringPtr("BASE"));
      groups->push_back(toJsonStringPtr("ENV"));
      groups->push_back(toJsonStringPtr("SERV"));
      groups->push_back(toJsonStringPtr("SYSTEM"));

      msgHandler.registerApp(
          appData.appName,
          Version(appData.version),
          groups
      );
    } catch(MsgHandlerException &e) {
        std::cerr << e.what() << std::endl;
        return (EXIT_FAILURE);
    }

    while(1) {
        try {
            printCommands();

            int cmd = 0;
            std::string str;

            std::cin >> cmd;

            switch(cmd) {
                case Message::MT_VOID:
                    msgHandler.sendVoid();
                    break;

                case Message::MT_ECHO_REQ:
                    std::cout << "Data?";
                    std::cin >> str;
                    std::cout << std::endl;
                    msgHandler.sendEchoReq(str);
                    break;

                case Message::MT_RESET_CLIENT:
                    std::cout << "AppId?";
                    std::cin >> cmd;
                    std::cout << std::endl;
                    msgHandler.sendResetClient(cmd);
                    break;

                case Message::MT_SERVER_INFO_REQ:
                    msgHandler.sendServerInfoReq();
                    break;

                case Message::MT_CON_CLIENTS_REQ:
                    msgHandler.sendConClientsReq();
                    break;

                case Message::MT_GET_GLOBAL_TIMER:
                    msgHandler.sendGetGlobalTimer();
                    break;

                case Message::MT_SET_GLOBAL_TIMER:
                    //msgHandler.sendSetGlobalTimer();
                    break;

                case Message::MT_GET_ENVIRONMENT:
                    msgHandler.sendGetEnvironment();
                    break;

                case Message::MT_SET_ENVIRONMENT:
                    msgHandler.sendSetEnvironment();
                    break;

                case Message::MT_SET_AUTO_MODE:
                    std::cout << "t|f|n?";
                    std::cin >> str;
                    std::cout << std::endl;
                    if(str == "t") {
                        msgHandler.sendSetAutoMode(true);
                    }
                    if(str == "f") {
                        msgHandler.sendSetAutoMode(false);
                    }
                    if(str == "n") {
                        msgHandler.sendSetAutoMode();
                    }
                    break;

                case Message::MT_GET_COLOR_THEME:
                case Message::MT_SET_COLOR_THEME:
                case Message::MT_EMERGENCY_STOP:
                case Message::MT_EMERGENCY_STOP_CLEARING:
                case Message::MT_GET_HARDWARE_STATE:
                case Message::MT_SET_HARDWARE_STATE:
                case Message::MT_HARDWARE_SHUTDOWN:
                    msgHandler.sendHardwareShutdown();
                    break;

                case Message::MT_HARDWARE_RESET:
                    msgHandler.sendHardwareReset();
                    break;

                case Message::MT_HARDWARE_SWITCH_STANDBY:
                    msgHandler.sendHardwareSwitchStandby();
                    break;

                case 98:
                    break;

                case 99:
                    msgHandler.sendClientClose();
                    return EXIT_SUCCESS;
            }

            while(msg = msgHandler.recieveMsg()) {
                prettyPrint(msg->getData());
            }








/*
            //msgHandler.sendServerSleep();
            //msg = msgHandler.recieveMsg();
            //prettyPrint(msg.getData()->getJsonString());

            //msgHandler.sendHardwareStateRequest();
            //msg = msgHandler.recieveMsg();
            //prettyPrint(msg.getData()->getJsonString());

            //msgHandler.sendServerShutdown();
            //msg = msgHandler.recieveMsg();

*/

        } catch(std::exception &e) {
            std::cerr << e.what() << std::endl;
            return (EXIT_FAILURE);
        }
    }
 }

