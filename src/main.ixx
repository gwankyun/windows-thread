module;

#include <boost/scope/defer.hpp>
#include <catch2/../catch2/catch_session.hpp>
#include <catch2/catch_test_macros.hpp>
#include <fmt/core.h>
#include <spdlog/spdlog.h>
#include <windows.h>

export module main;

import std;
import thread;
import mutex;
import event;

TEST_CASE("thread mutex", "[thread]")
{
    lite::mutex m;
    std::string str;

    auto f = [&]
    {
        Sleep(300);
        {
            lite::lock_guard<lite::mutex> guard(m);
            str += "456";
        }
    };
    lite::thread t(f);

    {
        lite::lock_guard<lite::mutex> guard(m);
        str += "123";
    }

    if (t.joinable())
    {
        t.join();
    }
    REQUIRE(str == "123456");
}

TEST_CASE("jthread mutex", "[thread]")
{
    lite::mutex m;
    std::string str;

    auto f = [&]
    {
        Sleep(300);
        {
            lite::lock_guard<lite::mutex> guard(m);
            str += "456";
        }
    };

    {
        lite::jthread t(f);
        {
            lite::lock_guard<lite::mutex> guard(m);
            str += "123";
        }
    }

    REQUIRE(str == "123456");
}

TEST_CASE("event", "[thread]")
{
    lite::event e;
    std::string str;

    auto f3 = [&]
    {
        Sleep(1000);
        str += "123";
        e.set();
    };

    lite::jthread t3(f3);

    auto f4 = [&]
    {
        bool flag = true;
        while (flag)
        {
            auto status = e.wait(50);
            switch (status)
            {
            case lite::wait_status::timeout:
                SPDLOG_INFO("timeout");
                break;
            case lite::wait_status::ready:
                SPDLOG_INFO("ready");
                str += "456";
                flag = false;
                break;
            default:
                flag = false;
                break;
            }
            Sleep(50);
        }
    };

    {
        lite::jthread t4(f4);
    }
    REQUIRE(str == "123456");
}

export int main(int _argc, char* _argv[])
{
    spdlog::set_pattern("[%C-%m-%d %T.%e] [%^%L%$] [t:%6t] [%-8!!:%4#] %v");

    auto result = Catch::Session().run(_argc, _argv);
    return result;
}
