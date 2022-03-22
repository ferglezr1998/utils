#! /bin/sh

gcc -c -Wall -Wshadow csv_parser.c

if [ $? -ne 0 ]
then

    echo '[ERR] Compilation Errors!'
    exit 1

fi

gcc -o csv_parser csv_parser.o

if [ $? -ne 0 ]
then

    echo '[ERR] Linking Errors!'
    exit 1

fi

echo '[INFO] Success'
exit 0
