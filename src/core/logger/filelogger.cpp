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

#include "filelogger.h"

#include <iostream>
#include <pwd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

namespace ab
{
namespace logger
{
FileLogger::FileLogger(const char *filename)
{
    // TODO(mchernigin): timestamp on creation/editing log file is UTC and not local timezone
    std::string homeDir = getHomeDir();
    std::string logDir  = homeDir + "/.local/share/alterator-browser/";
    ensureDir(logDir.c_str());

    std::string logFile = logDir + filename;
    this->logFileStream.open(logFile, std::fstream::out | std::fstream::app);
}

FileLogger::~FileLogger()
{
    this->logFileStream.close();
}

void FileLogger::log(const LoggerMessage &message)
{
    const char *prefix = this->logLevelMap.at(message.msgType);
    this->logFileStream << message.getTimeFormatted("%H:%M:%S") << " | " << prefix << ": " << message.message << " ("
                        << message.filePath << ":" << message.line << ")" << std::endl;
}

std::string FileLogger::getHomeDir()
{
    const char *HOME = getenv("HOME");
    if (HOME != NULL)
    {
        return HOME;
    }

    long bufsize = sysconf(_SC_GETPW_R_SIZE_MAX);
    if (bufsize == -1)
    {
        bufsize = 1 << 14;
    }

    char *resultBuf = new char[bufsize];
    passwd pwd;
    passwd *result;
    getpwuid_r(getuid(), &pwd, resultBuf, bufsize, &result);

    std::string homeDir;
    if (result != NULL)
    {
        homeDir = result->pw_dir;
    }
    else
    {
        homeDir = "/root";
        std::cerr << "FileLogger Error: cannot determine home directory, defaulting to " << homeDir << std::endl;
    }

    delete[] resultBuf;

    return homeDir;
}

bool FileLogger::ensureDir(const char *path)
{
    struct stat sb;
    if (stat(path, &sb) != 0 && mkdir(path, 0750) != 0)
    {
        std::cerr << "FileLogger Error: Cannot create log directory (" << path << ")" << std::endl;
        return false;
    }

    return true;
}
} // namespace logger
} // namespace ab
