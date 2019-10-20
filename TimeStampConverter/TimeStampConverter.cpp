#include "pch.h"
#include <iostream>
#include <regex>
#include <string>
#include <sstream>
#include <stdio.h>
#include <stdint.h>
#include <Winsock.h>
 
struct ntp_time_t {
    uint32_t   second;
    uint32_t    fraction;
};
 
void convert_ntp_time_into_unix_time(struct ntp_time_t *ntp, timeval *unix)
{
    unix->tv_sec = ntp->second - 0x83AA7E80; // the seconds from Jan 1, 1900 to Jan 1, 1970
    unix->tv_usec = (uint32_t)( (double)ntp->fraction * 1.0e6 / (double)(1LL<<32) );
}
 
void convert_unix_time_into_ntp_time(struct timeval *unix, struct ntp_time_t *ntp)
{
    ntp->second = unix->tv_sec + 0x83AA7E80;
    ntp->fraction = (uint32_t)( (double)(unix->tv_usec+1) * (double)(1LL<<32) * 1.0e-6 );
}
 

static const char* INCORRECT_FORMAT = "Incorrect input format! Please, try again...";

int main()
{
	using namespace std;

    ntp_time_t ntp;
    timeval unix;

	std::string inputline;
	std::regex expected_format("(q|[nu]\\s(0x\\w{8}|\\d+))", std::regex_constants::icase);
	while (true)
	{
		cout << "\nInput format: n(NTP format) or u(UNIX format) with timestamp(HEX). q to quit " << endl;
		cout << "Enter: ";
		std::getline(std::cin, inputline);

		if (!std::regex_match(inputline, expected_format))
		{
			std::cerr << INCORRECT_FORMAT << endl;
		}
		else
		{
			std::stringstream ss(inputline);
			char c;
			uint32_t timestamp;
			ss >> c;

			if (c == 'q' || c == 'Q')
				return 0;

			std::regex hex_format("[nu]\\s(0x\\w{8})", std::regex_constants::icase);
			std::regex dec_format("[nu]\\s(\\d+)", std::regex_constants::icase);
			if (std::regex_match(inputline, hex_format))
			{
				ss >> hex >> timestamp;
			}
			else if (std::regex_match(inputline, dec_format))
			{
				ss >> timestamp;
			}
			else
			{
				std::cerr << INCORRECT_FORMAT << endl;
				continue;
			}
			
			if (c == 'n' || c == 'N')
			{
				ntp.second = timestamp;
				convert_ntp_time_into_unix_time(&ntp, &unix);
				cout << "Entered value in unix format: " << unix.tv_sec << endl;
			}
			else //c == u
			{
				unix.tv_sec = timestamp;
				convert_unix_time_into_ntp_time(&unix, &ntp);
				cout << "Entered value in ntp format: " << ntp.second << endl;
			}
		}
	}

	return 0;
}