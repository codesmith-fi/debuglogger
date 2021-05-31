/**
 * Sample usage of the different loggers provided by this project
 *
 * Author: Erno Pakarinen (codesmith.fi@gmail.com)
 * Date: 31st of May 2021
 */
#include <iostream>

#include "simple/DebugLogger.h"

int main(int /* argc */, char** /* argv */)
{
	auto the_answer = 42;

	LOG_INFO() << "I am a logger, logging stuff";
	LOG_INFO() << "The answer to life, universe and everything is " << the_answer;
	LOG_ERROR_NT() << "Something fatal happened\n";

	return 0;
}
