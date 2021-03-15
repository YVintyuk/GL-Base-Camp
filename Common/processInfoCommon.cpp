#include "processInfoCommon.h"

/**
 * Print process info with streams
 * @param os
 * @return
 */
std::ostream &operator<<(std::ostream &os, const processInfo_t& p) {
    os << "Process\n\tpid: " << p.pid
       << "\n\tuser: " << p.user
       << "\n\texe: " << p.exeName
       << "\n\tcommand line: " << p.commandLine << "\n";
    return os;
}
