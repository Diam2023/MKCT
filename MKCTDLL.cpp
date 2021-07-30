#include "pch.h"
#include "MKCTDLL.hpp"

/*
* author: monoliths
* mikuclub.com web registration date 2015 12 27
* hex : { 32, 21, 18, 39 }
* version: 1.2
*/

// null fileStream
constexpr FILE* NULL_FILESTREAM = 0;

// head size
constexpr int FILE_HEAD_SIZE = 4;

// error type
constexpr size_t DEFAULT_COUNT = 1;

// default offset
constexpr int DEFAULT_OFFSET = 0;

// default error type
constexpr errno_t DEFAULT_ERROR_TYPE = 0;

// addition size
constexpr int LAST_DATA_COUNT_SIZE = 4;

// mkc type header
constexpr std::byte MKC_HEADER[4]{ std::byte{0x20}, std::byte{0x15}, std::byte{0x12}, std::byte{0x27} };

// binary read and write mode
const wchar_t* BINARY_READANDWRITE_MODE = L"rb+";

// binary append mode
const wchar_t* BINARY_APPEND_MODE = L"ab+";

constexpr auto MK_HEADER = [&] {
  char* data = (char*)malloc(sizeof(char) * 4);
  if (data != nullptr) {
    for (int i = 0; i < 4; i++)
    {
      data[i] = std::to_integer<char>(MKC_HEADER[i]);
    }
  }
  return data;
};

// file stream
FILE* _FileStream;

// open err
errno_t _ReturnValue;

// file head
char _FileHead[4];

// _FileHandle from fopen
int _FileHandle;

// file size
__int64 _FileSize;

// err type
errno_t _Result = 0;

void _bytesToHexString(char* bytes, int bytelength)
{
    std::string hexString("0123456789ABCDEF");
    for (int i = 0; i < bytelength; i++) {
        int b;
        b = 0x0f & (bytes[i] >> 4);
        char s1 = hexString.at(b);
        b = 0x0f & bytes[i];
        char s2 = hexString.at(b);
    }
    // consult to https://blog.csdn.net/weixin_30861797/article/details/97326458
}

errno_t _pushLastBytes(const wchar_t* file, char* bytes)
{
    // open file
    _ReturnValue = _wfopen_s(&_FileStream, file, BINARY_APPEND_MODE);
    if (_ReturnValue == DEFAULT_ERROR_TYPE) {

        // if stream open
        if (_FileStream != NULL_FILESTREAM) {

            // seek to last
            _fseeki64_nolock(_FileStream, DEFAULT_OFFSET, SEEK_END);

            // get size for string
            std::string count = std::to_string(sizeof(bytes));

            // get size for bytes
            const char* content_bytes = count.c_str();

            // write origin data
            _fwrite_nolock(bytes, sizeof(content_bytes), DEFAULT_COUNT, _FileStream);

            // write data size data
            _fwrite_nolock(content_bytes, LAST_DATA_COUNT_SIZE, DEFAULT_COUNT, _FileStream);

            // close all stream
            _fcloseall();
        }
    }
    return _ReturnValue;
}

char* _popLatsBytes(const wchar_t* file)
{
    // open file
    _ReturnValue = _wfopen_s(&_FileStream, file, BINARY_READANDWRITE_MODE);
    if (_ReturnValue == DEFAULT_ERROR_TYPE) {
        // if stream open
        if (_FileStream != NULL_FILESTREAM) {

            // get file direct
            _FileHandle = _fileno(_FileStream);

            // get file size
            _FileSize = _filelengthi64(_FileHandle);

            // seek to befor origin last data
            _fseeki64_nolock(_FileStream, -LAST_DATA_COUNT_SIZE, SEEK_END);

            // initialize cont_bytes
            char content_bytes[5];
            // add '\0' to last
            content_bytes[4] = '\0';

            // read last 4 bytes
            _fread_nolock_s(content_bytes, LAST_DATA_COUNT_SIZE, LAST_DATA_COUNT_SIZE, DEFAULT_COUNT, _FileStream);

            // cast char* to int
            __int64 content_size = _atoi64(content_bytes);

            // get last_all_size
            __int64 last_all_size = static_cast<__int64>(LAST_DATA_COUNT_SIZE) + content_size;

            // seek to before last_all_size position
            _fseeki64_nolock(_FileStream, -last_all_size, SEEK_END);

            // initialize origin_data
            char* origin_data;
            origin_data = (char*)malloc(content_size * sizeof(char));

            if (origin_data != nullptr) {
                // read data from file
                _fread_nolock_s(origin_data, content_size, content_size, DEFAULT_COUNT, _FileStream);
            }
            // file resize
            _ReturnValue = _chsize_s(_FileHandle, _FileSize - last_all_size);

            // flush to file
            _fflush_nolock(_FileStream);

            // cloase file stream
            _fcloseall();
            return origin_data;
        }
    }
    // casr errno_t to char*
    auto result = std::to_string(_ReturnValue);
    auto result_value = result.c_str();
    return const_cast<char*>(result_value);
}

char* _getFileHeader(const wchar_t* file)
{
    // open file
    _ReturnValue = _wfopen_s(&_FileStream, file, BINARY_READANDWRITE_MODE);
    if (_ReturnValue == DEFAULT_ERROR_TYPE) {
        // if stream open
        if (_FileStream != NULL_FILESTREAM) {
            // seek to first
            _fseeki64_nolock(_FileStream, DEFAULT_OFFSET, SEEK_SET);

            // read head to _FileHead
            _fread_nolock_s(_FileHead, sizeof(_FileHead), FILE_HEAD_SIZE, DEFAULT_COUNT, _FileStream);

            // from std::string to 16 hex
            _bytesToHexString(_FileHead, FILE_HEAD_SIZE);
        }
        _fcloseall();
    }
    return _FileHead;
}

char* _getMKFileHeader(const wchar_t* file)
{
    // open file
    _ReturnValue = _wfopen_s(&_FileStream, file, BINARY_READANDWRITE_MODE);
    if (_ReturnValue == DEFAULT_ERROR_TYPE) {
        // if stream open
        if (_FileStream != NULL_FILESTREAM) {
            // seek file point to last 4 bytes
            _fseeki64_nolock(_FileStream, -FILE_HEAD_SIZE, SEEK_END);

            // read last 4 bytes to FileHead
            _fread_nolock_s(_FileHead, sizeof(_FileHead), FILE_HEAD_SIZE, DEFAULT_COUNT, _FileStream);

            // from std::string to 16 hex
            _bytesToHexString(_FileHead, FILE_HEAD_SIZE);
        }
        _fcloseall();
    }
    return _FileHead;
}

errno_t _changeHeaderTo(const wchar_t* file)
{
    // open file
    _ReturnValue = _wfopen_s(&_FileStream, file, BINARY_READANDWRITE_MODE);
    if (_ReturnValue == DEFAULT_ERROR_TYPE) {
        // if stream open
        if (_FileStream != NULL_FILESTREAM) {
            // seek to first
            _fseeki64_nolock(_FileStream, DEFAULT_OFFSET, SEEK_SET);

            // read head to _FileHead
            _fread_nolock_s(_FileHead, sizeof(_FileHead), FILE_HEAD_SIZE, DEFAULT_COUNT, _FileStream);

            // seek to first
            _fseeki64_nolock(_FileStream, DEFAULT_OFFSET, SEEK_SET);

            // use MK_HEAD replease origin file head
            _fwrite_nolock(MK_HEADER(), sizeof(MK_HEADER()), DEFAULT_COUNT, _FileStream);

            // from std::string to 16 hex
            _bytesToHexString(_FileHead, FILE_HEAD_SIZE);

            // new file stream
            FILE* _NewFileStream;

            // reopen file use 'ab+' 
            _ReturnValue = _wfreopen_s(&_NewFileStream, file, BINARY_APPEND_MODE, _FileStream);
            if (_ReturnValue == DEFAULT_ERROR_TYPE) {
                // use MK_HEAD replease origin file head
                _fwrite_nolock(_FileHead, sizeof(_FileHead), DEFAULT_COUNT, _FileStream);
            }
            else {
                _Result = _ReturnValue;
            }
        }
        else {
            _Result = _ReturnValue;
        }
        // close file
        _fcloseall();
    }
    else {
        return _Result = _ReturnValue;
    }
    return _Result;
}

errno_t _changeHeaderBack(const wchar_t* file)
{
    // open file
    _ReturnValue = _wfopen_s(&_FileStream, file, BINARY_READANDWRITE_MODE);

    if (_ReturnValue == DEFAULT_ERROR_TYPE) {
        // if stream open
        if (_FileStream != NULL_FILESTREAM) {
            // get file direct
            _FileHandle = _fileno(_FileStream);

            // get file size
            _FileSize = _filelengthi64(_FileHandle);

            // seek file point to last 4 bytes
            _fseeki64_nolock(_FileStream, -FILE_HEAD_SIZE, SEEK_END);

            // read last 4 bytes to FileHead
            _fread_nolock_s(_FileHead, sizeof(_FileHead), FILE_HEAD_SIZE, DEFAULT_COUNT, _FileStream);

            // from std::string to 16 hex
            _bytesToHexString(_FileHead, FILE_HEAD_SIZE);

            // seek file point to first
            _fseeki64_nolock(_FileStream, DEFAULT_OFFSET, SEEK_SET);

            // write _FileHead to file first
            _fwrite_nolock(_FileHead, FILE_HEAD_SIZE, DEFAULT_COUNT, _FileStream);

            // file resize
            _ReturnValue = _chsize_s(_FileHandle, _FileSize - FILE_HEAD_SIZE);

            // flush to file
            _fflush_nolock(_FileStream);
        }
        else {
            _Result = _ReturnValue;
        }
        // close file
        _fcloseall();
    }
    else {
        _Result = _ReturnValue;
    }
    // return error type
    return _Result;
}
