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

#include "layouthandler.h"
#include "helper.h"

void LayoutHandler::handle(moba::Message::MessageType cmd) {
    switch(cmd) {
        case moba::Message::MT_GET_LAYOUTS_REQ:
            this->tloHandler.sendGetLayoutsRequest();
            break;

        case moba::Message::MT_DEL_LAYOUT: {
            int id = 0;
            getData("LayoutId", id);
            this->tloHandler.sendDeleteLayout(id);
            break;
        }

        case moba::Message::MT_CREATE_LAYOUT_REQ: {
            std::string name;
            std::string desc;
            getData("name", name);
            getData("desc", desc);
            this->tloHandler.sendCreateLayout(name, desc);
            break;
        }

        case moba::Message::MT_UPDATE_LAYOUT: {
            int id = 0;
            std::string name;
            std::string desc;
            getData("LayoutId", id);
            getData("name", name);
            getData("desc", desc);
            this->tloHandler.sendUpdateLayout(id, name, desc);
            break;
        }

        case moba::Message::MT_UNLOCK_LAYOUT: {
            int id = 0;
            getData("LayoutId", id);
            this->tloHandler.sendUnlockLayout(id);
            break;
        }

        case moba::Message::MT_GET_LAYOUT_REQ: {
            int id = 0;
            getData("LayoutId", id);
            this->tloHandler.sendGetLayoutRequest(id);
            break;
        }
    }
}
