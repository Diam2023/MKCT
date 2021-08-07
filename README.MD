# MKCTDLL
Author: monoliths <br>
This project can change file head to miku web registration date

## Environment
- C++ version
  - C++17 standard
- JAVA version
  - Openjdk >= 1.8

## Change header to 20 15 12 27
Mikuclub.com web registration date: December 27, 2015 <br>
Hex data: { 32, 21, 18, 39 }

## Main function
  - char* _getMKFileHeader(const wchar_t* file)
  - char* _getFileHeader(const wchar_t* file)
  - errno_t _pushLastBytes(const wchar_t* file, const char* bytes)
  - char* _popLatsBytes(const wchar_t* file)
  - void _bytesToHexString(char* bytes, int bytelength): change byte data to hex
  - errno_t _changeHeaderTo(const wchar_t* file): copy head append to end, use MK_HEAD replease head
  - errno_t _changeHeaderBack(const wchar_t* file): get last 4 bytes data and replease to first, resize to file size - 4 

Return last error value.<br>
Value: https://docs.microsoft.com/en-us/cpp/c-runtime-library/errno-constants?view=msvc-160

## Version
- 1.1
  - added char* _getMKFileHeader(const wchar_t* file) function.
  - added char* _getFileHeader(const wchar_t* file).
  - resolve string to char* codec error.
  - change argvs type char* to wchar_t*, use Unicode file name.
- 1.2
  - added java version of MKCT
  - added new two function
  - use constexpr to initialize some const data
  - solve header cover 4 to 8 bytes error
