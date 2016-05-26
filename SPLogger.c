#include "SPLogger.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

//TODO didnt find the way to check if the file was written successfully. check


//File open mode
#define SP_LOGGER_OPEN_MODE "w"

// Global variable holding the logger
SPLogger logger = NULL;

struct sp_logger_t {
	FILE* outputChannel; //The logger file
	bool isStdOut; //Indicates if the logger is stdout
	SP_LOGGER_LEVEL level; //Indicates the level
};

SP_LOGGER_MSG spLoggerCreate(const char* filename, SP_LOGGER_LEVEL level) {
	if (logger != NULL) { //Already defined
		return SP_LOGGER_DEFINED;
	}
	logger = (SPLogger) malloc(sizeof(*logger));
	if (logger == NULL) { //Allocation failure
		return SP_LOGGER_OUT_OF_MEMORY;
	}
	logger->level = level; //Set the level of the logger
	if (filename == NULL) { //In case the filename is not set use stdout
		logger->outputChannel = stdout;
		logger->isStdOut = true;
	} else { //Otherwise open the file in write mode
		logger->outputChannel = fopen(filename, SP_LOGGER_OPEN_MODE);
		if (logger->outputChannel == NULL) { //Open failed
			free(logger);
			logger = NULL;
			return SP_LOGGER_CANNOT_OPEN_FILE;
		}
		logger->isStdOut = false;
	}
	return SP_LOGGER_SUCCESS;
}

void spLoggerDestroy() {
	if (!logger) {
		return;
	}
	if (!logger->isStdOut) {//Close file only if not stdout
		fclose(logger->outputChannel);
	}
	free(logger);//free allocation
	logger = NULL;
}


SP_LOGGER_MSG spLoggerPrintError(const char* msg, const char* file,
		const char* function, const int line)
{
	if (logger == NULL)
	{
		return SP_LOGGER_UNDIFINED;										// logger is undefined
	}

	if (msg == NULL || file == NULL || function == NULL || line < 0)	// there is invalid argument
	{
		return SP_LOGGER_INVAlID_ARGUMENT;
	}

	if (logger->level == SP_LOGGER_ERROR_LEVEL ||
		logger->level == SP_LOGGER_WARNING_ERROR_LEVEL ||
		logger->level == SP_LOGGER_INFO_WARNING_ERROR_LEVEL ||
		logger->level == SP_LOGGER_DEBUG_INFO_WARNING_ERROR_LEVEL )	// all options
	{
		if (logger->isStdOut)											// should print to screen
		{
			printf("---ERROR---\n");
			printf("%s%s%s", "- file: ", file, "\n");
			printf("%s%s%s", "- function: ", function, "\n");
			printf("%s%d%s", "- line: ", line, "\n");
			printf("%s%s%s", "- message: ", msg, "\n");
		}
		else
		{
			if(fprintf(logger->outputChannel, "%s", "---ERROR---\n")<0){
				return SP_LOGGER_WRITE_FAIL;
			}
			if(fprintf(logger->outputChannel,"%s%s%s", "- file: ", file, "\n")<0){
				return SP_LOGGER_WRITE_FAIL;
			}
			if(fprintf(logger->outputChannel, "%s%s%s", "- function: ", function, "\n")<0){
				return SP_LOGGER_WRITE_FAIL;
			}
			if(fprintf(logger->outputChannel, "%s%d%s", "- line: ", line, "\n")<0){
				return SP_LOGGER_WRITE_FAIL;
			}
			if(fprintf(logger->outputChannel, "%s%s%s", "- message: ", msg, "\n")<0){
				return SP_LOGGER_WRITE_FAIL;
			}
		}
	}
	return SP_LOGGER_SUCCESS;
}



SP_LOGGER_MSG spLoggerPrintWarning(const char* msg, const char* file,
		const char* function, const int line)
{
	if (logger == NULL)
	{
		return SP_LOGGER_UNDIFINED;										// logger is undefined
	}

	if (msg == NULL || file == NULL || function == NULL || line < 0)	// there is invalid argument
	{
		return SP_LOGGER_INVAlID_ARGUMENT;
	}

	if (logger->level == SP_LOGGER_WARNING_ERROR_LEVEL ||
		logger->level == SP_LOGGER_INFO_WARNING_ERROR_LEVEL ||
		logger->level == SP_LOGGER_DEBUG_INFO_WARNING_ERROR_LEVEL )		// all except error
	{
		if (logger->isStdOut)											// should print to screen
		{
			printf("---WARNING---\n");
			printf("%s%s%s", "- file: ", file, "\n");
			printf("%s%s%s", "- function: ", function, "\n");
			printf("%s%d%s", "- line: ", line, "\n");
			printf("%s%s%s", "- message: ", msg, "\n");
		}
		else
		{
			if(fprintf(logger->outputChannel, "%s", "---WARNING---\n")<0){
				return SP_LOGGER_WRITE_FAIL;
			}
			if(fprintf(logger->outputChannel,"%s%s%s", "- file: ", file, "\n")<0){
				return SP_LOGGER_WRITE_FAIL;
			}
			if(fprintf(logger->outputChannel, "%s%s%s", "- function: ", function, "\n")<0){
				return SP_LOGGER_WRITE_FAIL;
			}
			if(fprintf(logger->outputChannel, "%s%d%s", "- line: ", line, "\n")<0){
				return SP_LOGGER_WRITE_FAIL;
			}
			if(fprintf(logger->outputChannel, "%s%s%s", "- message: ", msg, "\n")<0){
				return SP_LOGGER_WRITE_FAIL;
			}
		}
	}
	return SP_LOGGER_SUCCESS;
}



SP_LOGGER_MSG spLoggerPrintInfo(const char* msg)
{
	if (logger == NULL)
	{
		return SP_LOGGER_UNDIFINED;										// logger is undefined
	}

	if (msg == NULL)													// there is invalid argument
	{
		return SP_LOGGER_INVAlID_ARGUMENT;
	}

	if (logger->level == SP_LOGGER_INFO_WARNING_ERROR_LEVEL ||
		logger->level == SP_LOGGER_DEBUG_INFO_WARNING_ERROR_LEVEL )		// info and debug
	{
		if (logger->isStdOut)											// should print to screen
		{
			printf("---INFO---\n");
			printf("%s%s%s", "- message: ", msg, "\n");
		}
		else
		{
			if(fprintf(logger->outputChannel, "%s", "---INFO---\n")<0){
				return SP_LOGGER_WRITE_FAIL;
			}
			if(fprintf(logger->outputChannel, "%s%s%s", "- message: ", msg, "\n")<0){
				return SP_LOGGER_WRITE_FAIL;
			}
		}
	}
	return SP_LOGGER_SUCCESS;
}



SP_LOGGER_MSG spLoggerPrintDebug(const char* msg, const char* file,
		const char* function, const int line)
{
	if (logger == NULL)
	{
		return SP_LOGGER_UNDIFINED;										// logger is undefined
	}

	if (msg == NULL || file == NULL || function == NULL || line < 0)	// there is invalid argument
	{
		return SP_LOGGER_INVAlID_ARGUMENT;
	}

	if ( logger->level == SP_LOGGER_DEBUG_INFO_WARNING_ERROR_LEVEL )		// debug only
	{
		if (logger->isStdOut)											// should print to screen
		{
			printf("---DEBUG---\n");
			printf("%s%s%s", "- file: ", file, "\n");
			printf("%s%s%s", "- function: ", function, "\n");
			printf("%s%d%s", "- line: ", line, "\n");
			printf("%s%s%s", "- message: ", msg, "\n");
		}
		else
		{
			if(fprintf(logger->outputChannel, "%s", "---DEBUG---\n")<0){
				return SP_LOGGER_WRITE_FAIL;
			}
			if(fprintf(logger->outputChannel,"%s%s%s", "- file: ", file, "\n")<0){
				return SP_LOGGER_WRITE_FAIL;
			}
			if(fprintf(logger->outputChannel, "%s%s%s", "- function: ", function, "\n")<0){
				return SP_LOGGER_WRITE_FAIL;
			}
			if(fprintf(logger->outputChannel, "%s%d%s", "- line: ", line, "\n")<0){
				return SP_LOGGER_WRITE_FAIL;
			}
			if(fprintf(logger->outputChannel, "%s%s%s", "- message: ", msg, "\n")<0){
				return SP_LOGGER_WRITE_FAIL;
			}
		}
	}
	return SP_LOGGER_SUCCESS;

}



SP_LOGGER_MSG spLoggerPrintMsg(const char* msg)
{
	if (logger == NULL)
	{
		return SP_LOGGER_UNDIFINED;										// logger is undefined
	}

	if ( msg == NULL )													// there is invalid argument
	{
		return SP_LOGGER_INVAlID_ARGUMENT;
	}

	if (logger->level == SP_LOGGER_ERROR_LEVEL ||						// TODO what should be here??
		logger->level == SP_LOGGER_WARNING_ERROR_LEVEL ||
		logger->level == SP_LOGGER_INFO_WARNING_ERROR_LEVEL ||
		logger->level == SP_LOGGER_DEBUG_INFO_WARNING_ERROR_LEVEL )		// all options
	{
		if (logger->isStdOut)											// should print to screen
		{
			printf("%s%s%s", "- message: ", msg, "\n");
		}
		else
		{
			fprintf(logger->outputChannel, "%s%s%s", "- message: ", msg, "\n");
		}
	}
	return SP_LOGGER_SUCCESS;

}







