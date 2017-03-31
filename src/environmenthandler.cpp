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

#include "environmenthandler.h"
#include "helper.h"

void EnvironmentHandler::handle(moba::Message::MessageType cmd) {

    switch(cmd) {
        case moba::Message::MT_GET_GLOBAL_TIMER: {
            this->envHandler.sendGetGlobalTimer();
            break;
        }

        case moba::Message::MT_SET_GLOBAL_TIMER: {
            std::string date = "Sa 10:00";  // FIXME getString("DD hh:mm (e.g. Sa 10:00)");
            unsigned int intervall = 0;
            getData("Seconds (modulo 5)", intervall);
            unsigned int multiplicator = 0;
            getData("Multiplicator (realtime * multiplicator = modeltime)", multiplicator);
            this->envHandler.sendSetGlobalTimer(date, intervall, multiplicator);
            break;
        }

        case moba::Message::MT_GET_ENVIRONMENT:
            this->envHandler.sendGetEnvironment();
            break;

        case moba::Message::MT_SET_ENVIRONMENT:
            this->envHandler.sendSetEnvironment(
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
            this->envHandler.sendGetAmbience();
            break;
        }

        case moba::Message::MT_SET_AMBIENCE: {
            this->envHandler.sendSetAmbience(
                getToggleState("curtainUp"),
                getToggleState("mainLightOn")
            );
            break;
        }

        case moba::Message::MT_GET_AMBIENT_LIGHT: {
            this->envHandler.sendGetAmbientLight();
            break;
        }

        case moba::Message::MT_SET_AMBIENT_LIGHT: {
            std::vector<moba::EnvironmentHandler::AmbientLightData> v;
            moba::EnvironmentHandler::AmbientLightData ald;
            for(int i = 0; i < 3; ++i) {
                getData("red", ald.red);
                getData("blue", ald.blue);
                getData("white", ald.white);
                v.push_back(ald);
            }
            this->envHandler.sendSetAmbientLight(v);
            break;
        }

        case moba::Message::MT_GET_AUTO_MODE: {
            this->envHandler.sendGetAutoMode();
            break;
        }

        case moba::Message::MT_SET_AUTO_MODE: {
            this->envHandler.sendSetAutoMode(getBool(""));
            break;
        }

        case moba::Message::MT_GET_COLOR_THEME: {
            this->envHandler.sendGetColorTheme();
            break;
        }

        case moba::Message::MT_SET_COLOR_THEME: {
            this->envHandler.sendSetColorTheme(
                getString("dimTime [hh:mm]"),
                getString("brightTime [hh:mm]"),
                getThreeState("condition")
            );
            break;
        }
    }
}