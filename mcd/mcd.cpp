/*
 * MCD
 *
 * A simple executable to act as a combination to the Windows commands MD and CD.
 * Issue the command MCD with a path and the system will make that path the current
 * working directory (as would be the case with CD), and if any parts of the path don't 
 * exist, this utility will attempt to create them (as MD would do).
 * As with CD, launching the utility no arguments will display the current working directory
 * of the current drive, or if launched with just a drive specification it will display the
 * current working directory for that drive.
 * Launching the utility with a path will change the working directory to the path provided. 
 * If that path is on a drive different to the current drive, the system will only change 
 * to that drive the /D argument was also specified.
 * Launch with /? to display usage information.
 * 
 * Usage examples:
 *   MCD
 *   MCD D:
 *   MCD \Temp\New\Directory
 *   MDC D:\Temp\New\Directory /D
 *   MDC /?
 *
 * MIT License
 *
 * Copyright( c ) 2020 Motivesoft
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this softwareand associated documentation files( the "Software" ), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and /or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions :
 *
 * The above copyright noticeand this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

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
   std::wcout << "Type " << name << " /? to display this help information." << std::endl;
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
   std::wcout << "" << std::endl;
   std::wcout << "MCD will not modify the case of the path when creating directories, but this does not" << std::endl;
   std::wcout << "necessarily mean that the underlying operating system will behave similarly." << std::endl;
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
