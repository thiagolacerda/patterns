#include "memoryreporter.h"

#if defined(_WIN32)
#include <windows.h>
#include <psapi.h>

#elif defined(__unix__) || defined(__unix) || defined(unix) || (defined(__APPLE__) && defined(__MACH__))
#include <unistd.h>
#include <sys/resource.h>

#if defined(__APPLE__) && defined(__MACH__)
#include <mach/mach.h>

#elif (defined(_AIX) || defined(__TOS__AIX__)) || (defined(__sun__) || defined(__sun) || defined(sun) && (defined(__SVR4) || defined(__svr4__)))
#include <fcntl.h>
#include <procfs.h>

#elif defined(__linux__) || defined(__linux) || defined(linux) || defined(__gnu_linux__)
#include <stdio.h>
#endif
#endif

size_t MemoryReporter::getMemoryConsumption()
{
    size_t currentConsumption = 0;
#if defined(_WIN32)
    /* Windows -------------------------------------------------- */
    PROCESS_MEMORY_COUNTERS info;
    GetProcessMemoryInfo( GetCurrentProcess( ), &info, sizeof(info) );
    currentConsumption = (size_t) info.WorkingSetSize;

#elif defined(__APPLE__) && defined(__MACH__)
    /* OSX ------------------------------------------------------ */
    struct mach_task_basic_info info;
    mach_msg_type_number_t infoCount = MACH_TASK_BASIC_INFO_COUNT;
    if (task_info(mach_task_self(), MACH_TASK_BASIC_INFO, (task_info_t)&info, &infoCount) != KERN_SUCCESS)
        currentConsumption = (size_t) 0L; /* Can't access? */
    else
        currentConsumption = (size_t) info.resident_size;

#elif defined(__linux__) || defined(__linux) || defined(linux) || defined(__gnu_linux__)
    /* Linux ---------------------------------------------------- */
    long rss = 0L;
    FILE* fp = NULL;
    if ((fp = fopen("/proc/self/statm", "r")) == NULL )
        currentConsumption = (size_t) 0L; /* Can't open? */
    else if (fscanf(fp, "%*s%ld", &rss) != 1)
        currentConsumption = (size_t) 0L; /* Can't read? */
    else
        currentConsumption = (size_t) rss * (size_t) sysconf(_SC_PAGESIZE);

    fclose(fp);
#else
    /* AIX, BSD, Solaris, and Unknown OS ------------------------ */
    currentConsumption = (size_t) 0L; /* Unsupported. */
#endif
    return currentConsumption;
}
