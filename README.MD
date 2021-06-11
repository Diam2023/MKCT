# MKCTDLL
author: monoliths
this project can change file head to miku web registration date

## Change header to 20 15 12 27
mikuclub.com web registration date: December 27, 2019

hex data: { 32, 21, 18, 39 }
use chars: {' ', '', '', '\''}

## This project contains three functions
- void _bytesToHexString(char* bytes, int bytelength): change byte data to hex

- FileErrEnum _changeHeaderTo(const string& file): copy head append to end, use MK_HEAD replease head

- FileErrEnum _changeHeaderBack(const string& file): get last 4 bytes data and replease to first, resize to file size - 4 

MKCT project will be used for MKCompress
