#!/bin/bash

if ! [ -x "$(command -v gcc)" ]; then
  echo 'Error: gcc is required, but not installed.'
  echo 'Try execute command: sudo apt-get -y install build-essential'
  exit 1
fi

if ! [ -x "$(command -v cmake)" ]; then
  echo 'Error: cmake is required, but not installed.'
  echo 'Try execute command: sudo apt-get -y install cmake'
  exit 1
fi

# create code src directory
if ! [ -d "/usr/src/finder" ]; then
  if ! mkdir "/usr/src/finder"; then
    echo "Try use sudo"
    exit 1
  fi
fi
# shellcheck disable=SC2028
echo "$(cat main.c)" > /usr/src/finder/main.c


BIN_NAME="finder_bin"
# shellcheck disable=SC2016
echo "$(cat CMakeLists.txt | sed 's/better_name/'$BIN_NAME'/' )" > /usr/src/finder/CMakeLists.txt


# create code lib directory
if ! [ -d "/usr/lib/finder" ]; then
  if ! mkdir "/usr/lib/finder"; then
      exit 1
  fi
fi
# CMake create build cache
if ! sudo cmake -S /usr/src/finder/ -B /usr/lib/finder/; then
  echo "Error: cmake did not build"
  echo "Try use sudo"
  exit 1
fi
# CMake build binary program
sudo cmake --build /usr/lib/finder/ > /dev/null; # remove /dev/null -> output result of build
sudo chmod +x /usr/lib/finder/$BIN_NAME


# create script finder file
# shellcheck disable=SC2016
echo "$(cat finder.sh)" > /usr/bin/finder_bin


# set permission to execute finder
sudo chmod a+x /usr/bin/finder_bin

# need to start sudo and not sudo


if ! grep -q 'alias finder="/usr/bin/finder_bin"' ~/.bashrc ; then
 echo 'alias finder="/usr/bin/finder_bin"' >> ~/.bashrc && source ~/.bashrc
fi

if ! [ -f "~/.bashrc" ]; then
  RED='\033[0;31m'
  NC='\033[0m' # No Color
  printf "${RED}NO HOME DIR FOUND${NC} insert: alias finder='/usr/bin/finder_bin'\n"
fi

if ! grep -q 'alias finder="/usr/bin/finder_bin"' ~/.bashrc; then
  printf "add alias finder='/usr/bin/finder_bin'\ninto ~/.bashrc file\n"
else
  GRN='\033[0;32m'
  NC='\033[0m' # No Color
  printf "SETUP ${GRN}FINISHED${NC}\nSTART A NEW CONSOLE\n"
fi
# do tests
