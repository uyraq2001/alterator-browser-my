/***********************************************************************************************************************
**
** Copyright (C) 2022 BaseALT Ltd. <org@basealt.ru>
**
** This program is free software; you can redistribute it and/or
** modify it under the terms of the GNU General Public License
** as published by the Free Software Foundation; either version 2
** of the License, or (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
**
***********************************************************************************************************************/

#ifndef AB_FILE_LOGGER_H
#define AB_FILE_LOGGER_H

#include "../core.h"
#include "logger.h"
#include "loggermessage.h"

#include <fstream>

namespace ab
{
namespace logger
{
class AB_CORE_EXPORT FileLogger : public Logger
{
public:
    explicit FileLogger(const char *filename = "alterator-browser.log");
    ~FileLogger();

private:
    void log(const LoggerMessage &message) override;

    static std::string getHomeDir();
    static bool ensureDir(const char *path);

    std::fstream logFileStream = {};
};
} // namespace logger
} // namespace ab

#endif // AB_FILE_LOGGER_H
