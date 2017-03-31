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
