#!/bin/sh
echo "run program"

#./esv8_debug -e < ../debug/test6.in > ../debug/test6.out
./esv8_debug -e < ../debug/test6.in &
echo "Take screenshot"
shutter --window="Basic Text" &
#echo "shut the window"
#pkill -SIGINT -f "esv8_debug" &


