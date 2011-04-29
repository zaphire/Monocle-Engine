#!/bin/bash

install_name_tool -id @executable_path/../Frameworks/$1.framework/$1 ./$1.framework/$1
