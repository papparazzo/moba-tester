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

#include "basehandler.h"
#include "helper.h"

void BaseHandler::handle(moba::Message::MessageType cmd) {
    switch(cmd) {
        case moba::Message::MT_VOID:
            this->baseHandler->sendVoid();
            break;

        case moba::Message::MT_ECHO_REQ: {
            std::string str;
            getData("Data", str);
            this->baseHandler->sendEchoReq(str);
            break;
        }

        case moba::Message::MT_RESET_CLIENT: {
            int id = 0;
            getData("AppId", id);
            this->baseHandler->sendResetClient(id);
            break;
        }

        case moba::Message::MT_SELF_TESTING_CLIENT: {
            int id = 0;
            getData("AppId", id);
            this->baseHandler->sendSelfTestingClient(id);
            break;
        }

        case moba::Message::MT_SERVER_INFO_REQ: {
            this->baseHandler->sendServerInfoReq();
            break;
        }

        case moba::Message::MT_CON_CLIENTS_REQ: {
            this->baseHandler->sendConClientsReq();
            break;
        }
    }
}
