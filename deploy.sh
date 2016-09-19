#! /bin/sh

SCRIPTPATH="$(cd $(dirname "$0") && pwd)"

SYNC_DIR="~/dev/berryllio/tests/"

for test in "$SCRIPTPATH"/bin/*.out
do
    scp "$test" pi@raspberrypi:"$SYNC_DIR"
done

