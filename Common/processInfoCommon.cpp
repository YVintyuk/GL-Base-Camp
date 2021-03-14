#include "processInfoCommon.h"

/**
 * Print process info with streams
 * @param os
 * @return
 */
std::ostream &operator<<(std::ostream &os, const processInfo_t& p) {
    os << "\tProcess\n\tpid: " << p.pid
       << "\n\tuser: " << p.user
       << "\n\tcmd: " << p.exeName
       << "\n\targv: " << p.commandLine << "\n";
    return os;
}
