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
 *   MCD E:
 *   MCD \Temp\New\Directory
 *   MDC /D E:\Temp\New\Directory
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

#include <filesystem>

bool isChangeDrive( const wchar_t* arg );
bool isHelp( const wchar_t* arg );
void printHelp( const wchar_t* name );

int driveToIndex( const std::filesystem::path& p );
bool driveExists( const std::filesystem::path& p );
bool isUNC( const std::filesystem::path& p );
void printCurrentWorkingDirectory( const std::filesystem::path& p );

int wmain( int argc, const wchar_t* argv[] )
{
   if ( argc == 1 )
   {
      printCurrentWorkingDirectory( std::filesystem::current_path() );
   }
   else
   {
      std::wstring path;
      bool showHelp = false;
      bool changeDrive = false;
      for ( int i = 1; i < argc && !showHelp; i++ )
      {
         // Support /?
         if ( isHelp( argv[ i ] ) )
         {
            showHelp = true;
            break;
         }

         // Support /D - only works if placed before any path elements
         if ( isChangeDrive( argv[ i ] ) && path.length() == 0 )
         {
            changeDrive = true;
            continue;
         }

         // Build up the path by treating multiple arguments as part of the same path and using spaces
         // although with spaces it would typically be better if the user surrounded them with quotes
         // This is more or less what CD also does althoug (somehow) it also seems to understand a double space
         // and we simply can't
         if ( path.length() > 0 )
         {
            path = path.append( L" " );
         }
         path.append( argv[ i ] );
      }

      if ( showHelp )
      {
         wchar_t appName[ _MAX_FNAME + 1 ];
         _wsplitpath_s( argv[ 0 ], nullptr, 0, nullptr, 0, appName, _MAX_FNAME, nullptr, 0 );
         _wcsupr_s( appName, _MAX_FNAME );

         printHelp( appName );
      }
      else if ( changeDrive && path.length() == 0 )
      {
         // Change drive with no path - error
         std::wcout << "The filename, directory name, or volume label syntax is incorrect." << std::endl;
      }
      else
      {
         std::filesystem::path p(path);
         std::wcout << "Extension:     " << p.has_extension() << " " << (p.has_extension() ? p.extension().wstring() : L"") << std::endl;
         std::wcout << "Filename:      " << p.has_filename() << " " << ( p.has_filename() ? p.filename().wstring() : L"" ) << std::endl;
         std::wcout << "Parent path:   " << p.has_parent_path() << " " << ( p.has_parent_path() ? p.parent_path().wstring() : L"" ) << std::endl;
         std::wcout << "Relative path: " << p.has_relative_path() << " " << ( p.has_relative_path() ? p.relative_path().wstring() : L"" ) << std::endl;
         std::wcout << "Root directory:" << p.has_root_directory() << " " << ( p.has_root_directory() ? p.root_directory().wstring() : L"" ) << std::endl;
         std::wcout << "Root name:     " << p.has_root_name() << " " << ( p.has_root_name() ? p.root_name().wstring() : L"" ) << std::endl;
         std::wcout << "Root path:     " << p.has_root_path() << " " << ( p.has_root_path() ? p.root_path().wstring() : L"" ) << std::endl;
         std::wcout << "Stem:          " << p.has_stem() << " " << ( p.has_stem() ? p.stem().wstring() : L"" ) << std::endl;
         std::wcout << "Is absolute:   " << p.is_absolute() << std::endl;
         std::wcout << "Is relative:   " << p.is_relative() << std::endl;
         std::wcout << "Formats:       " << p.generic_format << " " << p.native_format << std::endl;
         //std::wcout << "Root exists:   " << ( p.has_root_name() ? std::filesystem::exists( p.root_name() ) : true ) << std::endl;
         //std::wcout << "Exists:        " << std::filesystem::exists( p ) << std::endl;
         //std::wcout << "Canonical:     " << std::filesystem::canonical( p ) << std::endl;
         //std::wcout << "Status:        " << std::filesystem::_Is_drive_prefix_with_slash_slash_question(p.c_str()) << std::endl;
         // NB: root name but no root directory means drive letter only

         // Drive and no path?
         if ( p.has_root_name() && !p.has_relative_path() )
         {
            if ( !changeDrive )
            {
               // Display working directory for the specified drive
               printCurrentWorkingDirectory( p );
            }
         }
         else if( p.has_root_name() && !driveExists( p ) )
         {
            std::wcout << "The system cannot find the drive specified." << std::endl;
         }
         else if ( isUNC( p ) )
         {
            std::wcout << "'" << path.c_str() << "'" << std::endl;
            std::wcout << "CMD does not support UNC paths as current directories." << std::endl;
         }
         else
         {
            // Make directory path (if it doesn't exist) and then change to it
            struct _stat buffer;
            if ( _wstat( path.c_str(), &buffer ) )
            {
               //::CreateDirectory( path.c_str() );

               if ( p.has_filename() )
               {
                  std::wcout << "P:" << path << std::endl;
                  path = path.append( L"\\" );
                  std::wcout << "P:" << path << std::endl;
                  p = std::filesystem::path( path );
               }
               int result = _wmkdir( path.c_str() );
               if ( result == ENOENT )
               {
                  std::wcout << "Failed to create path " << path.c_str() << std::endl;
               }
               else if ( result == EEXIST )
               {
                  std::wcout << "Failed to create path. Already exists? " << path.c_str() << std::endl;
               }
               else if ( result == 0 )
               {
                  // Success
               }
               else
               {
                  std::wcout << "The device is not ready." << std::endl;
               }
            }
         }

         // Change to the directory
         //std::filesystem::current_path( p );

         if ( changeDrive )
         {
            // Change drive
            if ( driveExists( p ) )
            {
               _chdrive( driveToIndex( p ) );
            }
         }
      }
   }
}

bool isChangeDrive( const wchar_t* arg )
{
   return arg != nullptr && (wcscmp( arg, L"/D" ) == 0 || wcscmp( arg, L"/d" ) == 0);
}

bool isHelp( const wchar_t* arg )
{
   return arg != nullptr && wcscmp( arg, L"/?" ) == 0;
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
void printCurrentWorkingDirectory( const std::filesystem::path& p )
{
   std::wcout << std::filesystem::canonical( p ).wstring() << std::endl;
}
void printCurrentWorkingDirectory( int drive )
{
   // Check we can change to this drive before deciding that we can do other things
   if ( _chdrive( drive ) == 0 )
   {
      wchar_t* buffer = _wgetcwd( nullptr, _MAX_PATH );

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
int driveToIndex( const std::filesystem::path& p )
{
   if ( p.has_root_name() )
   {
      const wchar_t drive = p.root_name().c_str()[ 0 ];
      if ( drive >= 'a' && drive <= 'z' )
      {
         // Letter-based drive index is 1-based
         return drive - 'a' + 1;
      }
      if ( drive >= 'A' && drive <= 'Z' )
      {
         // Letter-based drive index is 1-based
         return drive - 'A' + 1;
      }
   }
   // Drive index of zero means 'default drive'
   return 0;
}

bool driveExists( const std::filesystem::path& p )
{
   int driveIndex;
   if ( !p.has_root_name() )
   {
      // No drive specified, use the current drive.
      // Yes, we could just return a default value if no drive specified, but it is possible to have a
      // current drive that no longer valid so let's let the system decide
      driveIndex = _getdrive();
   }
   else
   {
      driveIndex = driveToIndex( p );
   }

   // Make it zero-based, not 1-based
   driveIndex--;

   // See if the drive has its bit set in the drive mask
   unsigned long driveMask = _getdrives();
   return ( driveMask >> driveIndex ) & 1;
}

/// <summary>
/// Returns true if the specified path is a UNC path
/// </summary>
/// <param name="p">the path</param>
/// <returns>true if the path is a UNC path - i.e. starts with \\</returns>
bool isUNC( const std::filesystem::path& p )
{
   return p.has_root_name() && wcsncmp( p.root_name().c_str(), L"\\\\", 2 ) == 0;
}
