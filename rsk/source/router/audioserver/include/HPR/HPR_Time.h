#ifndef __HPR_TIME_H__
#define __HPR_TIME_H__

#include "HPR_Config.h"
#include "HPR_Types.h"

#if defined(OS_WINDOWS)
#if !defined OS_WINCE
#include <sys/timeb.h>
#endif
    typedef SYSTEMTIME HPR_OS_EXP_TIME_T;
    typedef FILETIME HPR_OS_TIME_T;
#elif defined(OS_POSIX)
#else
    #error OS Not Implement Yet.
#endif

typedef HPR_INT64 HPR_TIME_T; //(usec)

typedef struct HPR_TIME_EXP_S {
    HPR_INT32 tm_usec;
    HPR_INT32 tm_sec;
    HPR_INT32 tm_min;
    HPR_INT32 tm_hour;
    HPR_INT32 tm_mday;
    HPR_INT32 tm_mon;
    HPR_INT32 tm_year;
    HPR_INT32 tm_wday;
    HPR_INT32 tm_yday;
    HPR_INT32 tm_isdst;
    HPR_INT32 tm_gmtoff;
} HPR_TIME_EXP_T;

HPR_DECLARE HPR_TIME_T CALLBACK HPR_TimeNow();
HPR_DECLARE HPR_UINT32 CALLBACK HPR_GetTimeTick();
HPR_DECLARE HPR_TIME_T CALLBACK HPR_TimeFromAnsiTime(HPR_TIME_T tAnsiTime);
HPR_DECLARE HPR_TIME_T CALLBACK HPR_AnsiTimeFromTime(HPR_TIME_T iTime);
HPR_DECLARE HPR_INT32 CALLBACK HPR_TimeFromExpTime(HPR_TIME_EXP_T* pExpTime, HPR_TIME_T* pTime);
HPR_DECLARE HPR_INT32 CALLBACK HPR_TimeFromExpTimeGMT(HPR_TIME_EXP_T* pExpTime, HPR_TIME_T* pTime);
HPR_DECLARE HPR_INT32 CALLBACK HPR_ExpTimeFromTime(HPR_TIME_T nTime, HPR_INT32 iOff, HPR_TIME_EXP_T* pExpTime);
HPR_DECLARE HPR_INT32 CALLBACK HPR_ExpTimeFromTimeGMT(HPR_TIME_T iTime, HPR_TIME_EXP_T* pExpTime);
HPR_DECLARE HPR_INT32 CALLBACK HPR_ExpTimeFromTimeLocal(HPR_TIME_T iTime, HPR_TIME_EXP_T* pExpTime);
HPR_DECLARE HPR_INT32 CALLBACK HPR_CTime(char* pDateStr, HPR_TIME_T nTime);
HPR_DECLARE HPR_INT32 CALLBACK HPR_StrFmtTime(char* pDst, HPR_INT32* iDstLen, HPR_INT32 iMaxDstSize, const char* pFmt, HPR_TIME_EXP_T* pTimeExp);

#include <time.h>

#define HPR_US_PER_SEC (1000000)
#define HPR_MS_PER_SEC (1000)
#define HPR_US_PER_MS (1000)
#define HPR_NS_PER_US (1000)
#define HPR_NS_PER_MS (1000000)
#define HPR_NS_PER_SEC (1000000000)

namespace hpr {

    class hpr_time_tick {
    public:
        hpr_time_tick();
        hpr_time_tick(hpr_int64 t);
        hpr_time_tick(const hpr_time_tick&);
        virtual ~hpr_time_tick();

        static hpr_int64 timeval2ms(struct timeval* tv);
        static struct timeval* ms2timeval(hpr_int64 t, struct timeval* tv);
        static hpr_int64 timeval2us(struct timeval* tv);
        static struct timeval* us2timeval(hpr_int64 t, struct timeval* tv);
        static hpr_int64 us2ms(hpr_int64 t);
        static hpr_int64 us2s(hpr_int64 t);
        static hpr_int64 ms2s(hpr_int64 t);

        hpr_time_tick& operator=(const hpr_time_tick&);
        hpr_time_tick& operator+=(const hpr_time_tick&);
        hpr_time_tick& operator-=(const hpr_time_tick&);
        hpr_int64 now();
        hpr_int64 get_time();
        hpr_int64 update_time();

    private:
        hpr_int64 m_nTime;
    };

    class hpr_time {
    public:
        hpr_time();
        hpr_time(hpr_int64);
        hpr_time(const hpr_time&);
        virtual ~hpr_time();

        hpr_time& operator=(const hpr_time&);
        hpr_time& operator+=(const hpr_time&);
        hpr_time& operator-=(const hpr_time&);

        hpr_int64 now();
        hpr_int64 get_time();
        hpr_int64 update_time();

        hpr_int32 get_year();
        hpr_int32 get_mon();
        hpr_int32 get_day();
        hpr_int32 get_hour();
        hpr_int32 get_min();
        hpr_int32 get_sec();
        hpr_int32 get_week_day();
        hpr_int32 get_mon_day();
        hpr_int32 get_year_day();

        static struct tm* time2tm(hpr_int64 t, struct tm*);

    private:
        time_t m_tTime;
        struct tm m_tmTime;
    };

    bool operator==(const hpr_time&, const hpr_time&);
    bool operator!=(const hpr_time&, const hpr_time&);
    bool operator>(const hpr_time&, const hpr_time&);
    bool operator<(const hpr_time&, const hpr_time&);

    bool operator==(const hpr_time_tick&, const hpr_time_tick&);
    bool operator>(const hpr_time_tick&, const hpr_time_tick&);
    bool operator<(const hpr_time_tick&, const hpr_time_tick&);

}

#endif // __HPR_TIME_H__
