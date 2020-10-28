#pragma once
#include <iostream>
#include <vector>

static float total_waiting_time = 0, total_idle_time = 0, wait_count = 0;

struct Simulation
{
    int customer_no;
    // int rd_ar;
    float intr_arr_time;
    float arrival;
    // float rd_se;
    float service_begin;
    float service_time;
    float time_ser_end;
    float waiting_time;
    float idle_time;
    float spend;
};

std::vector<Simulation> s;
static int n = 0;

void arrival_service_end(std::vector<float> &arrival, std::vector<float> &departure)
{
    n = arrival.size();
    s.resize(n);
    for (int i = 0; i < n; i++)
    {
        s[i].customer_no = i + 1;
        s[i].arrival = arrival[i];
        s[i].time_ser_end = departure[i];
    }
    s[0].service_time = s[0].time_ser_end;
    s[0].intr_arr_time = 0;
    s[0].waiting_time = 0;
    s[0].idle_time = 0;
}

void inter_arrival()
{
    for (int i = 1; i < n; i++)
    {
        s[i].intr_arr_time = s[i].arrival - s[i - 1].arrival;
    }
}

void schedule_service_time()
{
    for (int i = 1; i < n; i++)
    {
        if (s[i].arrival > s[i - 1].time_ser_end)
        {
            s[i].service_begin = s[i].arrival;
        }
        else
        {
            s[i].service_begin = s[i - 1].time_ser_end;
        }
    }
}

void service_time(float &total_service_time)
{
    for (int i = 0; i < n; i++)
    {
        s[i].service_time = s[i].time_ser_end - s[i].service_begin;
        total_service_time += s[i].service_time;
    }
}

void waiting_and_idle_time()
{
    for (int i = 1; i < n; i++)
    {
        if (s[i].arrival > s[i - 1].time_ser_end)
        {
            s[i].idle_time = s[i].arrival - s[i - 1].time_ser_end;
            total_idle_time += s[i].idle_time;
            s[i].waiting_time = 0;
        }
        else
        {
            s[i].waiting_time = s[i - 1].time_ser_end - s[i].arrival;
            total_waiting_time += s[i].waiting_time;
            s[i].idle_time = 0;
        }
        if (s[i].waiting_time != 0)
        {
            wait_count = wait_count + 1;
        }
    }
}

float schedule_spend_time(std::vector<Simulation> &s, int n)
{
    float total_spend_time = 0;
    for (int i = 0; i < n; i++)
    {
        s[i].spend = s[i].service_time + s[i].waiting_time;
        total_spend_time += s[i].spend;
    }
    return total_spend_time;
}

void table()
{
    int i, j;
    float total_service_time = 0;
    float avg_waiting_time, probability_wait, probability_of_idle_server, avg_service_time, avg_time_btn_arrival,
        avg_waiting_time_those_who_wait, Average_time_customer_spends_in_the_system;

    // arrival_service_end(s, total_service_time);
    inter_arrival();
    // schedule_arrival_time(s, n);
    schedule_service_time();
    service_time(total_service_time);
    waiting_and_idle_time();

    float total_spend_time = schedule_spend_time(s, n);

    std::cout << "\n   -------------------------------------------------------------------------------------------------------------------------------------------------";
    std::cout << "\n  | Cust.|\t Inter_arrival |\t Arrival |\t Service |\tT.Service|\t   Service|\t Waiting \t| Server \t| Spend in |\n";
    std::cout << "  |  No. |\t     Time      |\t  Time   |\t  Time   |\t Begin   |\t  End(at) |\t in Queue\t| Idle(T)\t|  System  |";
    std::cout << "\n   -------------------------------------------------------------------------------------------------------------------------------------------------" << std::endl;
    for (i = 0; i < n; i++)
    {

        printf("     %d        %f              %f           %f       %f          %f            %f      %f       %f\n", s[i].customer_no, s[i].intr_arr_time, s[i].arrival, s[i].service_time, s[i].service_begin,
               s[i].time_ser_end, s[i].waiting_time, s[i].idle_time, s[i].spend);
        if (i != n - 1)
        {
            std::cout << "   ---------------------------------------------------------------------------------------------------------------------------------------------------\n";
        }
        else
        {
            std::cout << "   --------------------------------------------------------====----------------------------------------------------------====---------====--------====--\n";
        }
    }

    std::cout << "\t\t\t\t\t\t\t    " << total_service_time << " \t\t\t\t\t            " << total_waiting_time << "     " << total_idle_time << "    " << total_spend_time << std::endl;
    avg_waiting_time = (float)total_waiting_time / (float)n;
    probability_wait = (float)wait_count / (float)n;
    probability_of_idle_server = (float)total_idle_time / (float)s[n - 1].time_ser_end;
    avg_service_time = (float)total_service_time / (float)n;
    avg_time_btn_arrival = (float)s[n - 1].arrival / (float)(n - 1);
    avg_waiting_time_those_who_wait = (float)total_waiting_time / (float)wait_count;
    Average_time_customer_spends_in_the_system = (float)total_spend_time / (float)n;

    std::cout << "\n Average waiting time = " << std::setprecision(2) << avg_waiting_time << std::endl;
    std::cout << " Probability(wait) = " << std::setprecision(2) << probability_wait << std::endl;
    std::cout << " Probability of idle server = " << std::setprecision(2) << probability_of_idle_server << std::endl;
    std::cout << " Average service time = " << std::setprecision(2) << avg_service_time << std::endl;
    std::cout << " Average time between arrival= " << std::setprecision(2) << avg_time_btn_arrival << std::endl;
    std::cout << " Average waiting time of those who wait= " << std::setprecision(2) << avg_waiting_time_those_who_wait << std::endl;
    std::cout << " Average time customer spends in the system= " << std::setprecision(2) << Average_time_customer_spends_in_the_system << std::endl;
    std::cout << "\n\n"
              << std::endl;
}