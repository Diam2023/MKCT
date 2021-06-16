#include "pch.h"
#include "MKCTDLL.h"

/*
*
* mikuclub.com web registration date 2015 12 27
* hex : { 32, 21, 18, 39 }
*
*/
// const char MK_HEADER[4] = {' ', '', '', '\''};
const char MK_HEADER[4] = { char(32), char(21), char(18), char(39) };
// head size
const int FILE_HEAD_SIZE = 4;
// error type
const size_t READ_COUNT = 1;

// file stream
FILE* _FileStream;
// open err
errno_t _OpenError;
// file head
char _FileHead[4];
// _FileHandle from fopen
int _FileHandle;
// file size
__int64 _FileSize;
// err type
FileErrEnum _Result = FileErrEnum::NO_ERR;

void _bytesToHexString(char* bytes, int bytelength)
{
    string str("");
    string str2("0123456789ABCDEF");
    for (int i = 0; i < bytelength; i++) {
        int b;
        b = 0x0f & (bytes[i] >> 4);
        char s1 = str2.at(b);
        str.append(1, str2.at(b));
        b = 0x0f & bytes[i];
        str.append(1, str2.at(b));
        char s2 = str2.at(b);
    }
    // consult to https://blog.csdn.net/weixin_30861797/article/details/97326458
}

char* _getFileHeader(const string& file)
{
    // open file
    _OpenError = fopen_s(&_FileStream, file.c_str(), "rb+");
    if (_OpenError == 0) {
        // if stream open
        if (_FileStream != 0) {
            // seek to first
            _fseeki64_nolock(_FileStream, 0, SEEK_SET);

            // read head to _FileHead
            _fread_nolock_s(_FileHead, sizeof(_FileHead), FILE_HEAD_SIZE, READ_COUNT, _FileStream);

            // from string to 16 hex
            _bytesToHexString(_FileHead, FILE_HEAD_SIZE);
        }
        if (_FileStream != 0) {
            // close file
            if (_fclose_nolock(_FileStream) != 0) {
                _Result = FileErrEnum::CLOSE_ERR;
            }
        }
    }
    return _FileHead;
}

char* _getMKFileHeader(const string& file)
{
    // open file
    _OpenError = fopen_s(&_FileStream, file.c_str(), "rb+");
    if (_OpenError == 0) {
        // if stream open
        if (_FileStream != 0) {
            // seek file point to last 4 bytes
            _fseeki64_nolock(_FileStream, -FILE_HEAD_SIZE, SEEK_END);

            // read last 4 bytes to FileHead
            _fread_nolock_s(_FileHead, sizeof(_FileHead), FILE_HEAD_SIZE, READ_COUNT, _FileStream);

            // from string to 16 hex
            _bytesToHexString(_FileHead, FILE_HEAD_SIZE);
        }
        if (_FileStream != 0) {
            // close file
            if (_fclose_nolock(_FileStream) != 0) {
                _Result = FileErrEnum::CLOSE_ERR;
            }
        }
    }
    return _FileHead;
}

FileErrEnum _changeHeaderTo(const string& file)
{
    // open file
    _OpenError = fopen_s(&_FileStream, file.c_str(), "rb+");
    if (_OpenError == 0) {
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

            // from string to 16 hex
            _bytesToHexString(_FileHead, FILE_HEAD_SIZE);

            // new file stream
            FILE* _NewFileStream;

            // reopen file use 'ab+' 
            _OpenError = freopen_s(&_NewFileStream, file.c_str(), "ab+", _FileStream);
            if (_OpenError == 0) {
                // use MK_HEAD replease origin file head
                _fwrite_nolock(_FileHead, sizeof(_FileHead), READ_COUNT, _FileStream);
            }
            else {
                _Result = FileErrEnum::OPEN_ERR;
            }
        }
        else {
            _Result = FileErrEnum::OPEN_ERR;
        }
        // close file
        _fcloseall();
    }

    return _Result;
}

FileErrEnum _changeHeaderBack(const string& file)
{
    // open file
    _OpenError = fopen_s(&_FileStream, file.c_str(), "rb+");

    if (_OpenError == 0) {
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

            // from string to 16 hex
            _bytesToHexString(_FileHead, FILE_HEAD_SIZE);

            // seek file point to first
            _fseeki64_nolock(_FileStream, 0, SEEK_SET);

            // write _FileHead to file first
            _fwrite_nolock(_FileHead, FILE_HEAD_SIZE, READ_COUNT, _FileStream);

            // file resize
            _chsize_s(_FileHandle, _FileSize - FILE_HEAD_SIZE);

            // flush to file
            _fflush_nolock(_FileStream);
        }
        else {
            _Result = FileErrEnum::OPEN_ERR;
        }
        if (_FileStream != 0) {
            // close file
            if (_fclose_nolock(_FileStream) != 0) {
                _Result = FileErrEnum::CLOSE_ERR;
            }
        }
        else {
            _Result = FileErrEnum::OPEN_ERR;
        }
    }
    // return error type
    return _Result;
}
