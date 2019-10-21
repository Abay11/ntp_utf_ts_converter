#include "pch.h"
#include <iostream>
#include <regex>
#include <string>
#include <sstream>
#include <ctime>

void convert_ntp_time_into_unix_time(uint32_t& ntp, uint32_t& unix)
{
	unix = ntp - 0x83AA7E80; // the seconds from Jan 1, 1900 to Jan 1, 1970
}

void convert_unix_time_into_ntp_time(uint32_t& unix, uint32_t& ntp)
{
	ntp = unix + 0x83AA7E80;
}

static const char* INCORRECT_FORMAT = "Incorrect input format! Please, try again...";

int main()
{
	uint32_t ntp_;
	uint32_t unix_;

	std::string inputline;
	std::regex expected_format("(q|[nu]\\s(0x\\w{8}|\\d+))", std::regex_constants::icase);
	while (true)
	{
		std::cout << "\nInput format: n(NTP format) or u(UNIX format) with timestamp(HEX). q to quit " << std::endl;
		std::cout << "Enter: ";
		std::getline(std::cin, inputline);

		if (!std::regex_match(inputline, expected_format))
		{
			std::cerr << INCORRECT_FORMAT << std::endl;
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
				ss >> std::hex >> timestamp;
			}
			else if (std::regex_match(inputline, dec_format))
			{
				ss >> timestamp;
			}
			else
			{
				std::cerr << INCORRECT_FORMAT << std::endl;
				continue;
			}

			time_t tmp;
			char str_time[26];
			if (c == 'n' || c == 'N')
			{
				ntp_ = timestamp;
				convert_ntp_time_into_unix_time(ntp_, unix_);
				std::cout << "Entered value in unix format: " << unix_ << std::endl;
			}
			else //c == u
			{
				unix_ = timestamp;
				convert_unix_time_into_ntp_time(unix_, ntp_);
				std::cout << "Entered value in ntp format: " << ntp_ << std::endl;
			}

			tmp = unix_;
			ctime_s(str_time, 26, &tmp);
			std::cout << "As UTC date representation: " << str_time << std::endl;
		}
	}

	return 0;
}