// mcd.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <direct.h>

void printCurrentWorkingDirectory( int drive );
bool isHelp( const wchar_t* arg );
void printHelp( const wchar_t* name );
int driveToIndex( wchar_t drive );

int wmain( int argc, const wchar_t* argv[] )
{
   if ( argc == 1 )
   {
      printCurrentWorkingDirectory( _getdrive() );
   }
   else
   {
      std::wstring path;
      bool showHelp = false;
      for ( int i = 1; i < argc && !showHelp; i++ )
      {
         showHelp = isHelp( argv[ i ] );
         if ( !showHelp )
         {
            if ( path.length() > 0 )
            {
               path = path.append( L" " );
            }
            path.append( argv[ i ] );
         }
      }

      if ( showHelp )
      {
         wchar_t appName[ _MAX_FNAME + 1 ];
         _wsplitpath_s( argv[ 0 ], NULL, 0, NULL, 0, appName, _MAX_FNAME, NULL, 0 );
         _wcsupr_s( appName, _MAX_FNAME );

         printHelp( appName );
      }
      else
      {
         // TODO if path is just a drive letter, print the current working directory for that drive and exit
         wchar_t drive[ _MAX_DRIVE + 1 ];
         wchar_t dir[ _MAX_DIR + 1 ];
         _wsplitpath_s( path.c_str(), drive, _MAX_DRIVE, dir, _MAX_DIR, NULL, 0, NULL, 0 );

         if ( wcslen( dir ) == 0 )
         {
            printCurrentWorkingDirectory( driveToIndex( drive[ 0 ] ) );
         }
         else
         {
            // 
            std::wcout << "\"" << path << "\"" << std::endl;

         }
      }
   }
}

bool isHelp( const wchar_t* arg )
{
   return arg != NULL && wcscmp( arg, L"/?" ) == 0;
}

void printHelp( const wchar_t* name )
{
   std::wcout << "Displays the name of or changes the current directory. If the directory doesn't exist, it is created." << std::endl;
   std::wcout << "" << std::endl;
   std::wcout << name << " [/D] [drive:][path]" << std::endl;
   std::wcout << name << " [..]" << std::endl;
   std::wcout << "" << std::endl;
   std::wcout << "  ..   Specifies that you want to change to the parent directory." << std::endl;
   std::wcout << "" << std::endl;
   std::wcout << "Type " << name << " drive: to display the current directory in the specified drive." << std::endl;
   std::wcout << "Type " << name << " without parameters to display the current drive and directory." << std::endl;
   std::wcout << "" << std::endl;
   std::wcout << "Use the /D switch to change current drive in addition to changing current" << std::endl;
   std::wcout << "directory for a drive." << std::endl;
   std::wcout << "" << std::endl;
   std::wcout << name << " does not treat spaces as delimiters, so it is possible to" << std::endl;
   std::wcout << "create and change into a subdirectory name that contains a space without surrounding" << std::endl;
   std::wcout << "the name with quotes.  For example:" << std::endl;
   std::wcout << "" << std::endl;
   std::wcout << "    " << name << " \\winnt\\profiles\\username\\programs\\start menu" << std::endl;
   std::wcout << "" << std::endl;
   std::wcout << "is the same as:" << std::endl;
   std::wcout << "" << std::endl;
   std::wcout << "    " << name << " \"\\winnt\\profiles\\username\\programs\\start menu\"" << std::endl;
   std::wcout << "" << std::endl;
   std::wcout << "Multiple adjacent spaces will be condensed down to one, which is not precisely" << std::endl;
   std::wcout << "the same behaviour as CD with extensions enabled, but close enough for most uses. For" << std::endl;
   std::wcout << "multiple adjecent spaces, surround the path with quotes." << std::endl;
   /*
The current directory string is converted to use the same case as
the on disk names.  So CD C:\TEMP would actually set the current
directory to C:\Temp if that is the case on disk.
   */
}

/// <summary>
/// Output the current working directory for the provided drive.
/// <para><code>drive</code> is a 1-based drive index, e.g. A: = 1, B: = 2</para>
/// </summary>
/// <param name="drive">The drive letter (1-26)</param>
void printCurrentWorkingDirectory( int drive )
{
   // Check we can change to this drive before deciding that we can do other things
   if ( _chdrive( drive ) == 0 )
   {
      wchar_t* buffer = _wgetcwd( NULL, _MAX_PATH );

      if ( buffer )
      {
         std::wcout << buffer << std::endl;
      }

      free( buffer );
   }
   else
   {
      std::wcout << "The system cannot find the drive specified." << std::endl;
   }
}

/// <summary>
/// Take a drive letter and make it into a drive index.
/// <para>This essentially does "upper(drive)-'A'+1", but slightly more intelligently.</para>
/// <para>For an invalid value, return 0. 0 is testable, and it is also the 'default drive', so calling methods can take their pick what route to take
/// </summary>
/// <param name="drive">the drive letter, a-z, case-insensitive</param>
/// <returns>the drive index (1-26), 0 for unknown</returns>
int driveToIndex( wchar_t drive )
{
   if ( drive >= 'a' && drive <= 'z' )
   {
      return drive - 'a' + 1;
   }
   if ( drive >= 'A' && drive <= 'Z' )
   {
      return drive - 'A' + 1;
   }
   return 0;
}
