/*
Author:		Dan Strycharske
Date:
File:
Brief:
*/

#ifndef LOGGER_H_INCLUDED
#define LOGGER_H_INCLUDED


//The interface for a logger class.
#include <stdarg.h>  //For using the ... syntax.
#include <iostream>  //cout, cin, all that good stuff
#include <fstream>   //output to files.
#include "DSingleton.h"  //Singleton for now, could have multiple logging systems but for now one is enough.


class DLogGuru : public DSingleton<DLogGuru>// : public NxUserOutputStream
{

private:
	std::ofstream	mFileStream;

	const char*		mFileName;

public:
	DLogGuru( const char* filename = "LogFile.txt" );
	virtual ~DLogGuru();

	void print(const char* message);

	//Function to log a message to the file, adding a new line after it.
	bool	LogSingleMessage( const char* message, bool newline = true, bool box = false );
	bool	LogMultipleMessage( int fileLog, const char* message, ... );

	//Function to go down a line in the file (new line).
	bool	GoDownLines( int numLines = 1 );

	//Force close the file.
	bool	CloseFile();

	//Force clears the entire file.
	void	Clear();

	//This will overwrite the entire file with the given message.
	void	OverWriteMessage( const char* message, bool newline = true );
};


#endif // LOGGER_H_INCLUDED
