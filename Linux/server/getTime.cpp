#include "getTime.h"

using namespace std;

// Get current date/time, format is YYYY-MM-DD HH:mm:ss
const string currentDateTime() {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d %X", &tstruct);
    
    return buf;
}
