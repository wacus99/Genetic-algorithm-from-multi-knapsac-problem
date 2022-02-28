#!/bin/bash

FLAGS = -Wall -Wpedantic -Werror -Wextra

main:
	g++ main.cpp src/* -I inc

clean:
	rm -rf a.out
