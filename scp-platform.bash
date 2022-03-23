#! /bin/bash

# Indentity file path:
IDENTITY_FILE=$HOME/kcuhp-gpu-key-pair-paris.pem

function scp-platform {
	# $1: -r or -f if directory or file

	if [ $# -ne 3 ]
	then
		echo 'Usage Error! scp [-r or -f] [file_name or folder_name] remote_path'
		return 1
	fi

	if [ "$1" = "-f" ]
	then
		scp -i $IDENTITY_FILE $2 ubuntu@ec2-15-188-103-75.eu-west-3.compute.amazonaws.com:$3
	elif [ "$1" = "-r" ]
	then
		scp -i $IDENTITY_FILE -r $2 ubuntu@ec2-15-188-103-75.eu-west-3.compute.amazonaws.com:$3
	else
		echo 'Bad arguments! Options: -r or -f'
		return 1
	fi

	return 0
}
