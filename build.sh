#! /bin/sh

gcc -c -Wall -Wshadow csv_parser.c

if [ $? -eq 0 ]
then

	gcc -o csv_parser csv_parser.o

fi
