#include<stdio.h>
#include <time.h>
#include <cstdlib>
#define numrows 100
#define numruns 1000
#define MAX(A,B) ((A>B)?A:B)
struct row {
	int iat;
	int st;
	int arrtime;
	int servstart;
	int servend;
	int waittime;
	int queuelen;
	int idle;
};

int main() {
	struct row customerstable[numrows];

	// probablites of service time
	float number1 = 0.10 * 100;
	float number2 = 0.20 * 100 + number1;
	float number3 = 0.30 * 100 + number2;
	float number4 = 0.25 * 100 + number3;
	float number5 = 0.10 * 100 + number4;
	float number6 = 0.05 * 100 + number5;

	srand(time(0));

	//average waiting time;
	float averge_waiting_runs = 0;
	float av_wait = 0;

	//max wait time
	int max_waittime_in_rows = 0;
	int max_waittime_in_runs = 0;

	//Percentage of customers who wait in the queue
	float wait_customers_in_rows = 0;
	float wait_customers_in_runs = 0;
	float percentage_ofcustomers_wait = 0;

	//Percentage of customers who wait more than 10 minutes in the queue
	float waittime_customers_more10_rows = 0;
	float waittime_customers_more10_runs = 0;
	float percentage_ofcustomers_waitmore10 = 0;

	// percentage of time the employee is busy serving a customer
	float employee_busy_runs=0;
	float serverutilization;

	// Percentage of time the system is overcrowded
	float customers_time_more8_rows = 0;
	float customers_time_more8_runs = 0;
	float percentage_of_time_customers_more8 = 0;
	
	int sum_idle = 0;

	// first row and current run
	for (int runs = 0;runs < numruns;runs++) {
		
		av_wait = 0;
		max_waittime_in_rows = 0;
		wait_customers_in_rows=0;
		waittime_customers_more10_rows=0;
	    customers_time_more8_rows = 0;

		customerstable[0].iat = rand() % 8 + 1;
		int prop1 = rand() % 100 + 1;
		if (prop1 <= number1) {
			customerstable[0].st = 1;
		}
		else if (number1 < prop1 && prop1 <= number2) {
			customerstable[0].st = 2;
		}
		else if (number2 < prop1 && prop1 <= number3) {
			customerstable[0].st = 3;
		}
		else if (number3 < prop1 && prop1 <= number4) {
			customerstable[0].st = 4;
		}
		else if (number4 < prop1 && prop1 <= number5) {
			customerstable[0].st = 5;
		}
		else if (number5 < prop1 && prop1 <= number6) {
			customerstable[0].st = 6;
		}
		
		customerstable[0].arrtime = customerstable[0].iat;
		customerstable[0].servstart = customerstable[0].iat;
		customerstable[0].servend = customerstable[0].servstart + customerstable[0].st;
		customerstable[0].waittime = customerstable[0].queuelen = 0;
		customerstable[0].idle = customerstable[0].iat;
		sum_idle = customerstable[0].idle;

		// rest of rows
		for (int rows = 1;rows < numrows;rows++) {
			customerstable[rows].iat = rand() % 8 + 1;
			int prop2 = rand() % 100 + 1;
			if (prop2 <= number1) {
				customerstable[rows].st = 1;
			}
			else if (number1 < prop2 && prop2 <= number2) {
				customerstable[rows].st = 2;
			}
			else if (number2 < prop2 && prop2 <= number3) {
				customerstable[rows].st = 3;
			}
			else if (number3 < prop2 && prop2 <= number4) {
				customerstable[rows].st = 4;
			}
			else if (number4 < prop2 && prop2 <= number5) {
				customerstable[rows].st = 5;
			}
			else if (number5 < prop2 && prop2 <= number6) {
				customerstable[rows].st = 6;
			}
			customerstable[rows].arrtime = customerstable[rows - 1].arrtime + customerstable[rows].iat;
			customerstable[rows].servstart = MAX(customerstable[rows].arrtime, customerstable[rows - 1].servend);
			customerstable[rows].servend = customerstable[rows].servstart + customerstable[rows].st;
			customerstable[rows].waittime = customerstable[rows].servstart - customerstable[rows].arrtime;
			int queucout = 0;
			int i = rows;
			while (customerstable[i].servstart > customerstable[rows].arrtime) {
				i--;
				queucout++;
			}
			customerstable[rows].queuelen = queucout;
			// check of max wait time in rows
			if (max_waittime_in_rows < customerstable[rows].waittime) {
				max_waittime_in_rows = customerstable[rows].waittime;
			}
			
			if (customerstable[rows].arrtime > customerstable[rows - 1].servend) {
				customerstable[rows].idle = customerstable[rows].arrtime - customerstable[rows - 1].servend;
			}
			else {
				customerstable[rows].idle = 0;
			}
			sum_idle += customerstable[rows].idle;
			// calculate average wait time in rows
			av_wait = av_wait + customerstable[rows].waittime;

			// check of  customers who need to wait in a queue and count them
			if (customerstable[rows].queuelen >= 1) {
				wait_customers_in_rows++;
			}
			// check of  customers who need to wait more than 10 minites in a queue and count them

			if ((customerstable[rows].queuelen >= 1) && ((customerstable[rows-1].servend - customerstable[rows].arrtime)> 10))  {
				waittime_customers_more10_rows++;
			}
			//  check time the system is overcrowded and count time 
			if (customerstable[rows].queuelen >= 8) {
				customers_time_more8_rows++;
			}
		}
		// check of max wait time in runs
		if (max_waittime_in_runs < max_waittime_in_rows) {
			max_waittime_in_runs = max_waittime_in_rows;
		}
		// calculate statistics for current run
		averge_waiting_runs = averge_waiting_runs + (av_wait / numrows);
		wait_customers_in_runs = wait_customers_in_runs+(wait_customers_in_rows / numrows);
		waittime_customers_more10_runs = waittime_customers_more10_runs+(waittime_customers_more10_rows / numrows);
		customers_time_more8_runs = customers_time_more8_runs + (customers_time_more8_rows / numrows);
		employee_busy_runs = employee_busy_runs + ((customerstable[numrows - 1].servend - customerstable[numrows - 1].idle) / customerstable[numrows - 1].servend);
		
	}
		
	    // calculate  average and percentage for each point for all runs
	    averge_waiting_runs = averge_waiting_runs / numruns;
		percentage_ofcustomers_wait = (wait_customers_in_runs / numruns)*100;
		percentage_ofcustomers_waitmore10 = (waittime_customers_more10_runs / numruns)*100;
		percentage_of_time_customers_more8 = (customers_time_more8_runs / numruns) * 100;
		serverutilization = (employee_busy_runs/numruns) * 100;

		//print output statistics
		printf("the Average waiting time for a customer = %.2f\n", averge_waiting_runs);
		printf("the maximum waiting time for a customer in a queue = %d\n", max_waittime_in_runs);
		printf("the percentage of customers need to wait = %.2f\n", percentage_ofcustomers_wait);
		printf("the percentage of customers need to wait more than 10 minutes in a queue = %.2f\n", percentage_ofcustomers_waitmore10);
		printf("Server utilization = %.2f\n",serverutilization);
		printf("the Percentage of time the system is overcrowded = %.2f\n", percentage_of_time_customers_more8);
		
}