#! /bin/sh

SCRIPTPATH="$(cd $(dirname "$0") && pwd)"

SYNC_DIR="~/dev/berryllio/tests/"

for test in "$SCRIPTPATH"/bin/*.out
do
    scp "$test" pi@raspberrypi:"$SYNC_DIR"
done

xfce4-terminal --title "Raspberry pi terminal" -x bash -c "ssh -t pi@raspberrypi 'cd $SYNC_DIR; bash'"
