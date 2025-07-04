/*
 * This file is part of the TrinityCore Project. See AUTHORS file for Copyright information
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "Appender.h"
#include "Common.h"
#include "LogMessage.h"

Appender::Appender(uint8 _id, std::string _name, LogLevel _level /* = LOG_LEVEL_DISABLED */, AppenderFlags _flags /* = APPENDER_FLAGS_NONE */):
id(_id), name(std::move(_name)), level(_level), flags(_flags) { }

Appender::~Appender() = default;

uint8 Appender::getId() const
{
    return id;
}

std::string const& Appender::getName() const
{
    return name;
}

LogLevel Appender::getLogLevel() const
{
    return level;
}

AppenderFlags Appender::getFlags() const
{
    return flags;
}

void Appender::setLogLevel(LogLevel _level)
{
    level = _level;
}

void Appender::write(LogMessage* message)
{
    if (!level || level > message->level)
        return;

    if (flags & (APPENDER_FLAGS_PREFIX_TIMESTAMP | APPENDER_FLAGS_PREFIX_LOGLEVEL | APPENDER_FLAGS_PREFIX_LOGFILTERTYPE))
    {
        message->prefix.reserve(100);
        message->prefix.clear();

        if (flags & APPENDER_FLAGS_PREFIX_TIMESTAMP)
        {
            message->prefix.append(message->getTimeStr());
            message->prefix.append(1, ' ');
        }

        if (flags & APPENDER_FLAGS_PREFIX_LOGLEVEL)
        {
            std::string_view levelStr = getLogLevelString(message->level);
            message->prefix.append(levelStr);
            if (levelStr.length() < 5)
                message->prefix.append(5 - levelStr.length(), ' ');
        }

        if (flags & APPENDER_FLAGS_PREFIX_LOGFILTERTYPE)
        {
            message->prefix.append(1, '[');
            message->prefix.append(message->type);
            message->prefix.append("] ", 2);
        }
    }

    _write(message);
}

std::string_view Appender::getLogLevelString(LogLevel level)
{
    switch (level)
    {
        case LOG_LEVEL_FATAL:
            return "FATAL"sv;
        case LOG_LEVEL_ERROR:
            return "ERROR"sv;
        case LOG_LEVEL_WARN:
            return "WARN"sv;
        case LOG_LEVEL_INFO:
            return "INFO"sv;
        case LOG_LEVEL_DEBUG:
            return "DEBUG"sv;
        case LOG_LEVEL_TRACE:
            return "TRACE"sv;
        default:
            return "DISABLED"sv;
    }
}
