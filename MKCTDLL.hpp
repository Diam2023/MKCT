#pragma once

#ifndef MKCTDLL

/*
* author: monoliths
* date: 2021 6 19
* version: 1.1
*/


// mkc type header
const std::byte MKC_HEADER[4]{ std::byte{0x20}, std::byte{0x15}, std::byte{0x12}, std::byte{0x27} };

/// <summary>
/// main function: copy head append to end, use MK_HEAD replease head
/// </summary>
/// <param name="file">file path</param>
/// <returns>err type</returns>
extern "C" __declspec(dllexport) errno_t _changeHeaderTo(const wchar_t* file);

/// <summary>
/// main function: get last 4 bytes data and replease to first,
/// resize to file size - 4 
/// </summary>
/// <param name="file">file path</param>
/// <returns>error type of program</returns>
extern "C" __declspec(dllexport) errno_t _changeHeaderBack(const wchar_t* file);

/// <summary>
/// get file head
/// </summary>
/// <param name="file">file path</param>
/// <returns>a hex head</returns>
extern "C" __declspec(dllexport) char* _getFileHeader(const wchar_t* file);

/// <summary>
/// get mkc file origin head
/// </summary>
/// <param name="file"></param>
/// <returns></returns>
extern "C" __declspec(dllexport) char* _getMKFileHeader(const wchar_t* file);

/// <summary>
/// from string or char* to hex string
/// </summary>
/// <param name="bytes">data</param>
/// <param name="bytelength">length</param>
extern "C" __declspec(dllexport) void _bytesToHexString(char* bytes, int bytelength);

#endif // !MKCTDLL
