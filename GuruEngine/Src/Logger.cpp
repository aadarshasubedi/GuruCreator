/*
Author:		Dan Strycharske
Date:
File:
Brief:
*/

#include "Logger.h"


DLogGuru::DLogGuru( const char* filename) : mFileName(filename)
{
	mFileStream.open( mFileName);
	mFileStream.clear();
}


DLogGuru::~DLogGuru()
{
	mFileStream.close();
}


//Function to log a message to the file, adding a new line after it.
bool	DLogGuru::LogSingleMessage( const char* message, bool newline, bool box )
{
	if( !box )
	{
		mFileStream << message;
		if( newline )
		{
			mFileStream << std::endl;
		}
		mFileStream.flush();
	}
	else
	{
		fprintf( stderr, message );
		fprintf( stderr, "\n" );
		//MessageBoxA( NULL, message, "Dude System Message", MB_OK );
	}
	return true;
}


bool	DLogGuru::LogMultipleMessage( int fileLog, const char* message, ... )
{
	va_list args;
	va_start(args,message);
	char szBuf[1024];
	vsprintf_s(szBuf,message,args);
	switch( fileLog )
	{
		case 0:
		{
			return LogSingleMessage(szBuf);
		}
		case 1:
		{
			//MessageBoxA( NULL, szBuf, "Logger_Error", MB_OK );
			return true;
		}
		default:
		{
			break;
		}
	}

	return true;
}


//Function to go down a line in the file (new line).
bool	DLogGuru::GoDownLines( int numLines )
{
	while( numLines-- )
	{
		mFileStream << std::endl;
	}
	return true;
}


//Force close the file.
bool	DLogGuru::CloseFile()
{
	mFileStream.close();
	return !mFileStream.is_open();
}


//Force clears the entire file.
void	DLogGuru::Clear()
{
	mFileStream.close();
	mFileStream.open( mFileName );
}


//This will overwrite the entire file with the given message.
void	DLogGuru::OverWriteMessage( const char* message, bool newline )
{
	Clear();
	LogSingleMessage( message, newline );
}


void DLogGuru::print(const char* message)
{
	printf("%s", message);
}
