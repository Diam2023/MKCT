#include "pch.h"
#include "MKCTDLL.hpp"

/*
* author: monoliths
* mikuclub.com web registration date 2015 12 27
* hex : { 32, 21, 18, 39 }
* version: 1.1
*/

const char MK_HEADER[4] = { char(32), char(21), char(18), char(39) };
// head size
const int FILE_HEAD_SIZE = 4;
// error type
const size_t READ_COUNT = 1;

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

char* _getFileHeader(const wchar_t* file)
{
    // open file
    _ReturnValue = _wfopen_s(&_FileStream, file, L"rb+");
    if (_ReturnValue == 0) {
        // if stream open
        if (_FileStream != 0) {
            // seek to first
            _fseeki64_nolock(_FileStream, 0, SEEK_SET);

            // read head to _FileHead
            _fread_nolock_s(_FileHead, sizeof(_FileHead), FILE_HEAD_SIZE, READ_COUNT, _FileStream);

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
    _ReturnValue = _wfopen_s(&_FileStream, file, L"rb+");
    if (_ReturnValue == 0) {
        // if stream open
        if (_FileStream != 0) {
            // seek file point to last 4 bytes
            _fseeki64_nolock(_FileStream, -FILE_HEAD_SIZE, SEEK_END);

            // read last 4 bytes to FileHead
            _fread_nolock_s(_FileHead, sizeof(_FileHead), FILE_HEAD_SIZE, READ_COUNT, _FileStream);

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
    _ReturnValue = _wfopen_s(&_FileStream, file, L"rb+");
    if (_ReturnValue == 0) {
        // if stream open
        if (_FileStream != 0) {
            // seek to first
            _fseeki64_nolock(_FileStream, 0, SEEK_SET);

            // read head to _FileHead
            _fread_nolock_s(_FileHead, sizeof(_FileHead), FILE_HEAD_SIZE, READ_COUNT, _FileStream);

            // seek to first
            _fseeki64_nolock(_FileStream, 0, SEEK_SET);

            // use MK_HEAD replease origin file head
            _fwrite_nolock(MK_HEADER, sizeof(MK_HEADER), READ_COUNT, _FileStream);

            // from std::string to 16 hex
            _bytesToHexString(_FileHead, FILE_HEAD_SIZE);

            // new file stream
            FILE* _NewFileStream;

            // reopen file use 'ab+' 
            _ReturnValue = _wfreopen_s(&_NewFileStream, file, L"ab+", _FileStream);
            if (_ReturnValue == 0) {
                // use MK_HEAD replease origin file head
                _fwrite_nolock(_FileHead, sizeof(_FileHead), READ_COUNT, _FileStream);
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
    _ReturnValue = _wfopen_s(&_FileStream, file, L"rb+");

    if (_ReturnValue == 0) {
        // if stream open
        if (_FileStream != 0) {
            // get file direct
            _FileHandle = _fileno(_FileStream);

            // get file size
            _FileSize = _filelengthi64(_FileHandle);

            // seek file point to last 4 bytes
            _fseeki64_nolock(_FileStream, -FILE_HEAD_SIZE, SEEK_END);

            // read last 4 bytes to FileHead
            _fread_nolock_s(_FileHead, sizeof(_FileHead), FILE_HEAD_SIZE, READ_COUNT, _FileStream);

            // from std::string to 16 hex
            _bytesToHexString(_FileHead, FILE_HEAD_SIZE);

            // seek file point to first
            _fseeki64_nolock(_FileStream, 0, SEEK_SET);

            // write _FileHead to file first
            _fwrite_nolock(_FileHead, FILE_HEAD_SIZE, READ_COUNT, _FileStream);

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
