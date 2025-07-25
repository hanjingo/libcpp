#include <gtest/gtest.h>
#include <libcpp/time/date_time.hpp>

TEST(date_time, date_time)
{
    libcpp::date_time dt1;
    libcpp::date_time dt2(2023, 1, 1);
    libcpp::date_time dt3(2024, 1, 1);
    ASSERT_TRUE(dt1 != dt2);

    dt1 = dt2;
    ASSERT_TRUE(dt1 == dt2);

    ASSERT_TRUE(dt2 < dt3);
    ASSERT_TRUE(dt2 <= dt3);
    ASSERT_TRUE(dt3 > dt2);
    ASSERT_TRUE(dt3 >= dt2);
}

TEST(date_time, now)
{
    std::time_t tm = std::time(nullptr);
    std::tm tm_buf;
#if defined(_WIN32)
    localtime_s(&tm_buf, &tm);
#else
    localtime_r(&tm, &tm_buf);
#endif
    ASSERT_EQ(libcpp::date_time::now(libcpp::timezone::UTC).time(), tm);

    ASSERT_TRUE(libcpp::date_time::now(libcpp::timezone::BEIJING).time() > libcpp::date_time::now(libcpp::timezone::UTC).time());
}

TEST(date_time, today)
{
    std::time_t tm = std::time(nullptr);
    std::tm tm_buf;
#if defined(_WIN32)
    localtime_s(&tm_buf, &tm);
#else
    localtime_r(&tm, &tm_buf);
#endif

    libcpp::date_time today = libcpp::date_time::today();
    ASSERT_EQ(today.year(), tm_buf.tm_year + 1900);
    ASSERT_EQ(today.month(), static_cast<libcpp::moon>(tm_buf.tm_mon + 1));
    ASSERT_EQ(today.day(), tm_buf.tm_mday);
    ASSERT_EQ(today.hour(), 0);
    ASSERT_EQ(today.minute(), 0);
    ASSERT_EQ(today.seconds(), 0);
}

TEST(date_time, format)
{
    libcpp::date_time tm(2023, 1, 1, 0, 0, 0);
    ASSERT_STREQ(
        libcpp::date_time::format(tm, TIME_FMT).c_str(), 
        "2023-01-01 00:00:00"
    );

    ASSERT_STREQ(
        libcpp::date_time::format(tm, "%Y%m%d-%H:%M:%S").c_str(), 
        "20230101-00:00:00"
    );
}

TEST(date_time, parse)
{
    ASSERT_EQ(
        libcpp::date_time::parse("2023-01-01 00:00:00", TIME_FMT).time(), libcpp::date_time(2023, 1, 1, 0, 0, 0).time()
    );

    libcpp::date_time dt;
    libcpp::date_time::parse(dt, std::string("2023-01-01 00:00:00"), std::string(TIME_FMT));
    ASSERT_EQ(dt.time(), libcpp::date_time(2023, 1, 1, 0, 0, 0).time());
}

TEST(date_time, is_null)
{
    ASSERT_FALSE(libcpp::date_time::now().is_null());
    ASSERT_TRUE(libcpp::date_time().is_null());
}

TEST(date_time, is_working_day)
{
    ASSERT_FALSE(libcpp::date_time(2025, 7, 12).is_working_day());
    ASSERT_TRUE(libcpp::date_time(2025, 7, 11).is_working_day());
}

TEST(date_time, is_weekend)
{
    ASSERT_TRUE(libcpp::date_time(2025, 7, 12).is_weekend());
    ASSERT_FALSE(libcpp::date_time(2025, 7, 11).is_weekend());
}

TEST(date_time, string)
{
    ASSERT_STREQ(
        libcpp::date_time(2023, 1, 1, 0, 0, 0).string().c_str(), 
        "2023-01-01 00:00:00"
    );
}

TEST(date_time, date)
{
    libcpp::date_time dt(2023, 1, 1, 0, 0, 0);
    ASSERT_EQ(dt.date().tm_year + 1900, 2023);
    ASSERT_EQ(dt.date().tm_mon + 1, 1);
    ASSERT_EQ(dt.date().tm_mday, 1);
    ASSERT_EQ(dt.date().tm_hour, 0);
    ASSERT_EQ(dt.date().tm_min, 0);
    ASSERT_EQ(dt.date().tm_sec, 0);
}

TEST(date_time, time)
{
    std::time_t tm = std::time(nullptr);
    std::tm tm_buf;
#if defined(_WIN32)
    localtime_s(&tm_buf, &tm);
#else
    localtime_r(&tm, &tm_buf);
#endif
    ASSERT_EQ(libcpp::date_time(tm).time(), tm);
}

TEST(date_time, seconds)
{
    ASSERT_EQ(libcpp::date_time(2023, 1, 1, 0, 0, 59).seconds(), 59);
}

TEST(date_time, minute)
{
    ASSERT_EQ(libcpp::date_time(2023, 1, 1, 0, 59, 59).minute(), 59);
}

TEST(date_time, minutes_to)
{
    ASSERT_EQ(
        libcpp::date_time(2023, 1, 1, 0, 59, 59).minutes_to(
            libcpp::date_time(2023, 1, 1, 1, 0, 0)), 
        0
    );

    ASSERT_EQ(
        libcpp::date_time(2023, 1, 1, 0, 59, 0).minutes_to(
            libcpp::date_time(2023, 1, 1, 1, 0, 0)), 
        1
    );
}

TEST(date_time, hour)
{
    ASSERT_EQ(libcpp::date_time(2023, 1, 1, 23, 59, 59).hour(), 23);
}

TEST(date_time, hours_to)
{
    ASSERT_EQ(
        libcpp::date_time(2023, 1, 1, 23, 59, 59).hours_to(
            libcpp::date_time(2023, 1, 2, 0, 0, 0)), 
        0
    );

    ASSERT_EQ(
        libcpp::date_time(2023, 1, 1, 23, 0, 0).hours_to(
            libcpp::date_time(2023, 1, 2, 0, 0, 0)), 
        1
    );
}

TEST(date_time, day)
{
    ASSERT_EQ(libcpp::date_time(2023, 1, 31, 0, 0, 0).day(), 31);
}

TEST(date_time, days_to)
{
    ASSERT_EQ(
        libcpp::date_time(2023, 1, 1, 23, 59, 59).days_to(
            libcpp::date_time(2023, 1, 2, 0, 0, 0)), 
        0
    );

    ASSERT_EQ(
        libcpp::date_time(2023, 1, 1, 0, 0, 0).days_to(
            libcpp::date_time(2023, 1, 2, 0, 0, 0)), 
        1
    );
}

TEST(date_time, day_of_week)
{
    ASSERT_TRUE(libcpp::date_time(2023, 1, 31, 0, 0, 0).day_of_week() == libcpp::week_day::tuesday);
}

TEST(date_time, day_of_week_str)
{
    ASSERT_TRUE(libcpp::date_time(2023, 1, 31, 0, 0, 0).day_of_week_str() == "Tue");
}

TEST(date_time, day_of_month)
{
    ASSERT_EQ(libcpp::date_time(2023, 2, 1, 0, 0, 0).day_of_month(), 1);
}

TEST(date_time, day_of_year)
{
    ASSERT_EQ(libcpp::date_time(2023, 2, 1, 0, 0, 0).day_of_year(), 32);
}

TEST(date_time, month)
{
    ASSERT_EQ(libcpp::date_time(2023, 1, 31, 0, 0, 0).month(), static_cast<libcpp::moon>(1));
}

TEST(date_time, month_str)
{
    ASSERT_TRUE(libcpp::date_time(2023, 1, 31, 0, 0, 0).month_str() == "January");
}

TEST(date_time, year)
{
    ASSERT_EQ(libcpp::date_time(2023, 1, 31, 0, 0, 0).year(), 2023);
}

TEST(date_time, start_of_day)
{
    ASSERT_EQ(
        libcpp::date_time(2023, 1, 1, 12, 1, 1).start_of_day().time(), 
        libcpp::date_time(2023, 1, 1, 0, 0, 0).time()
    );
}

TEST(date_time, end_of_day)
{
    ASSERT_EQ(
        libcpp::date_time(2023, 1, 1, 12, 1, 1).end_of_day().time(), 
        libcpp::date_time(2023, 1, 1, 23, 59, 59).time()
    );
}

TEST(date_time, start_of_week)
{
    ASSERT_EQ(
        libcpp::date_time(2023, 1, 31, 12, 1, 1).start_of_week().time(), 
        libcpp::date_time(2023, 1, 30, 0, 0, 0).time()
    );
}

TEST(date_time, end_of_week)
{
    ASSERT_EQ(
        libcpp::date_time(2023, 1, 31, 12, 0, 0).end_of_week().time(), 
        libcpp::date_time(2023, 2, 5, 23, 59, 59).time()
    );
}

TEST(date_time, start_of_month)
{
    ASSERT_EQ(
        libcpp::date_time(2023, 1, 31, 12, 1, 1).start_of_month().time(), 
        libcpp::date_time(2023, 1, 1, 0, 0, 0).time()
    );
}

TEST(date_time, end_of_month)
{
    ASSERT_EQ(
        libcpp::date_time(2023, 1, 30, 12, 1, 1).end_of_month().time(), 
        libcpp::date_time(2023, 1, 31, 23, 59, 59).time()
    );
}

TEST(date_time, start_of_quarter)
{
    ASSERT_EQ(
        libcpp::date_time(2023, 1, 30, 12, 1, 1).start_of_quarter().time(), 
        libcpp::date_time(2023, 1, 1, 0, 0, 0).time()
    );
}

TEST(date_time, end_of_quarter)
{
    ASSERT_EQ(
        libcpp::date_time(2023, 1, 30, 12, 1, 1).end_of_quarter().time(), 
        libcpp::date_time(2023, 3, 31, 23, 59, 59).time()
    );
}

TEST(date_time, start_of_half_year)
{
    ASSERT_EQ(
        libcpp::date_time(2023, 7, 1, 12, 1, 1).start_of_half_year().time(), 
        libcpp::date_time(2023, 7, 1, 0, 0, 0).time()
    );
}

TEST(date_time, end_of_half_year)
{
    ASSERT_EQ(
        libcpp::date_time(2023, 7, 1, 12, 1, 1).end_of_half_year().time(), 
        libcpp::date_time(2023, 12, 31, 23, 59, 59).time()
    );
}

TEST(date_time, start_of_year)
{
    ASSERT_EQ(
        libcpp::date_time(2023, 1, 31, 12, 1, 1).start_of_year().time(), 
        libcpp::date_time(2023, 1, 1, 0, 0, 0).time()
    );
}

TEST(date_time, end_of_year)
{
    ASSERT_EQ(
        libcpp::date_time(2023, 1, 31, 12, 1, 1).end_of_year().time(), 
        libcpp::date_time(2023, 12, 31, 23, 59, 59).time()
    );
}

TEST(date_time, next_day)
{
    ASSERT_EQ(
        libcpp::date_time(2023, 1, 31, 12, 1, 1).next_day().time(), 
        libcpp::date_time(2023, 2, 1, 0, 0, 0).time()
    );

    ASSERT_EQ(
        libcpp::date_time(2023, 1, 31, 12, 1, 1).next_day(3).time(), 
        libcpp::date_time(2023, 2, 3, 0, 0, 0).time()
    );
}

TEST(date_time, pre_day)
{
    ASSERT_EQ(
        libcpp::date_time(2023, 2, 1, 12, 1, 1).pre_day().time(), 
        libcpp::date_time(2023, 1, 31, 0, 0, 0).time()
    );

    ASSERT_EQ(
        libcpp::date_time(2023, 2, 1, 12, 1, 1).pre_day(5).time(), 
        libcpp::date_time(2023, 1, 27, 0, 0, 0).time()
    );
}

TEST(date_time, next_weekday)
{
    ASSERT_EQ(
        libcpp::date_time(2025, 7, 12, 0, 0, 0).next_weekday(libcpp::week_day::monday).time(), 
        libcpp::date_time(2025, 7, 14, 0, 0, 0).time()
    );

    ASSERT_EQ(
        libcpp::date_time(2025, 7, 12, 0, 0, 0).next_weekday(libcpp::week_day::friday).time(), 
        libcpp::date_time(2025, 7, 18, 0, 0, 0).time()
    );
}

TEST(date_time, pre_weekday)
{
    ASSERT_EQ(
        libcpp::date_time(2025, 7, 12, 0, 0, 0).pre_weekday(libcpp::week_day::monday).time(), 
        libcpp::date_time(2025, 7, 7, 0, 0, 0).time()
    );

    ASSERT_EQ(
        libcpp::date_time(2025, 7, 12, 0, 0, 0).pre_weekday(libcpp::week_day::friday).time(), 
        libcpp::date_time(2025, 7, 11, 0, 0, 0).time()
    );
}

TEST(date_time, next_working_day)
{
    ASSERT_EQ(
        libcpp::date_time(2025, 7, 11).next_working_day().time(), 
        libcpp::date_time(2025, 7, 14, 0, 0, 0).time()
    );
    
    ASSERT_EQ(
        libcpp::date_time(2025, 7, 11).next_working_day(5).time(), 
        libcpp::date_time(2025, 7, 18, 0, 0, 0).time()
    );

    ASSERT_EQ(
        libcpp::date_time(2025, 7, 11).next_working_day(6).time(), 
        libcpp::date_time(2025, 7, 21, 0, 0, 0).time()
    );
}

TEST(date_time, pre_working_day)
{
    ASSERT_EQ(
        libcpp::date_time(2025, 7, 11).pre_working_day().time(),
        libcpp::date_time(2025, 7, 10, 0, 0, 0).time()
    );
    
    ASSERT_EQ(
        libcpp::date_time(2025, 7, 11).pre_working_day(5).time(),
        libcpp::date_time(2025, 7, 4, 0, 0, 0).time()
    );

    ASSERT_EQ(
        libcpp::date_time(2025, 7, 11).pre_working_day(10).time(),
        libcpp::date_time(2025, 6, 27, 0, 0, 0).time()
    );
}

TEST(date_time, next_month)
{
    ASSERT_EQ(
        libcpp::date_time(2023, 2, 1, 12, 1, 1).next_month().time(), 
        libcpp::date_time(2023, 3, 1, 0, 0, 0).time()
    );

    ASSERT_EQ(
        libcpp::date_time(2023, 2, 1, 12, 1, 1).next_month(3).time(), 
        libcpp::date_time(2023, 5, 1, 0, 0, 0).time()
    );
}

TEST(date_time, pre_month)
{
    ASSERT_EQ(
        libcpp::date_time(2023, 2, 1, 12, 1, 1).pre_month().time(), 
        libcpp::date_time(2023, 1, 1, 0, 0, 0).time()
    );

    ASSERT_EQ(
        libcpp::date_time(2023, 2, 1, 12, 1, 1).pre_month(3).time(), 
        libcpp::date_time(2022, 11, 1, 0, 0, 0).time()
    );
}

TEST(date_time, next_quarter)
{
    ASSERT_EQ(
        libcpp::date_time(2023, 2, 1, 12, 1, 1).next_quarter().time(), 
        libcpp::date_time(2023, 4, 1, 0, 0, 0).time()
    );

    ASSERT_EQ(
        libcpp::date_time(2023, 2, 1, 12, 1, 1).next_quarter(2).time(), 
        libcpp::date_time(2023, 7, 1, 0, 0, 0).time()
    );

    ASSERT_EQ(
        libcpp::date_time(2023, 2, 15).next_quarter().time(),
        libcpp::date_time(2023, 4, 1, 0, 0, 0).time()
    );
    
    ASSERT_EQ(
        libcpp::date_time(2023, 2, 15).next_quarter(2).time(),
        libcpp::date_time(2023, 7, 1, 0, 0, 0).time()
    );
    
    ASSERT_EQ(
        libcpp::date_time(2023, 11, 15).next_quarter().time(),
        libcpp::date_time(2024, 1, 1, 0, 0, 0).time()
    );
}

TEST(date_time, pre_quarter)
{
    ASSERT_EQ(
        libcpp::date_time(2023, 2, 1, 12, 1, 1).pre_quarter().time(), 
        libcpp::date_time(2022, 10, 1, 0, 0, 0).time()
    );

    ASSERT_EQ(
        libcpp::date_time(2023, 2, 1, 12, 1, 1).pre_quarter(2).time(), 
        libcpp::date_time(2022, 7, 1, 0, 0, 0).time()
    );

    ASSERT_EQ(
        libcpp::date_time(2023, 5, 15).pre_quarter().time(),
        libcpp::date_time(2023, 1, 1, 0, 0, 0).time()
    );
    
    ASSERT_EQ(
        libcpp::date_time(2023, 2, 15).pre_quarter().time(),
        libcpp::date_time(2022, 10, 1, 0, 0, 0).time()
    );
}

TEST(date_time, next_half_year)
{
    ASSERT_EQ(
        libcpp::date_time(2023, 2, 1, 12, 1, 1).next_half_year().time(), 
        libcpp::date_time(2023, 7, 1, 0, 0, 0).time()
    );

    ASSERT_EQ(
        libcpp::date_time(2023, 2, 1, 12, 1, 1).next_half_year(3).time(), 
        libcpp::date_time(2024, 7, 1, 0, 0, 0).time()
    );

    ASSERT_EQ(
        libcpp::date_time(2023, 3, 15).next_half_year().time(),
        libcpp::date_time(2023, 7, 1, 0, 0, 0).time()
    );
    
    ASSERT_EQ(
        libcpp::date_time(2023, 9, 15).next_half_year().time(),
        libcpp::date_time(2024, 1, 1, 0, 0, 0).time()
    );
}

TEST(date_time, pre_half_year)
{
    ASSERT_EQ(
        libcpp::date_time(2023, 9, 15).pre_half_year().time(),
        libcpp::date_time(2023, 1, 1, 0, 0, 0).time()
    );
    
    ASSERT_EQ(
        libcpp::date_time(2023, 3, 15).pre_half_year().time(),
        libcpp::date_time(2022, 7, 1, 0, 0, 0).time()
    );
}

TEST(date_time, next_year)
{
    ASSERT_EQ(
        libcpp::date_time(2023, 2, 1, 12, 1, 1).next_year().time(), 
        libcpp::date_time(2024, 1, 1, 0, 0, 0).time()
    );

    ASSERT_EQ(
        libcpp::date_time(2023, 2, 1, 12, 1, 1).next_year(2).time(), 
        libcpp::date_time(2025, 1, 1, 0, 0, 0).time()
    );

    ASSERT_EQ(
        libcpp::date_time(2023, 6, 15, 12, 30, 45).next_year().time(),
        libcpp::date_time(2024, 1, 1, 0, 0, 0).time()
    );
    
    ASSERT_EQ(
        libcpp::date_time(2023, 6, 15).next_year(3).time(),
        libcpp::date_time(2026, 1, 1, 0, 0, 0).time()
    );
}

TEST(date_time, pre_year)
{
    ASSERT_EQ(
        libcpp::date_time(2023, 2, 1, 12, 1, 1).pre_year().time(), 
        libcpp::date_time(2022, 1, 1, 0, 0, 0).time()
    );

    ASSERT_EQ(
        libcpp::date_time(2023, 2, 1, 12, 1, 1).pre_year(2).time(), 
        libcpp::date_time(2021, 1, 1, 0, 0, 0).time()
    );

    ASSERT_EQ(
        libcpp::date_time(2023, 6, 15, 12, 30, 45).pre_year().time(),
        libcpp::date_time(2022, 1, 1, 0, 0, 0).time()
    );
    
    ASSERT_EQ(
        libcpp::date_time(2023, 6, 15).pre_year(2).time(),
        libcpp::date_time(2021, 1, 1, 0, 0, 0).time()
    );
}