#!/bin/bash
g++ -c code.cpp
g++ code.o -o app -lsfml-graphics -lsfml-window -lsfml-system
./app
