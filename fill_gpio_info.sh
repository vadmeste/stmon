#!/bin/bash

which dialog

if [ $? -ne 0 ] ; then
    echo "You have to install dialog program before continuing."
    exit -1
fi

if [ "$1" == "" ] ; then
    echo "You should provide at least one parameter"
    exit -1
fi

exit_fn() {
    should_exit=1
}

trap exit_fn SIGINT

cp $1 $1.new

cat $1 |
    while read ln ; do
        # echo "$ln"
        col1=$(echo $ln | awk '{ print $1 }')
        col2=$(echo $ln | awk '{ print $2 }')
        # echo "|$col1|$col2|"
        if [ "$col1" != "" -a "$col2" == "" ] ; then
            dialog --inputbox "Enter the name of the following parameter : $col1" 10 30 2>resp.lock
            res="`cat resp.lock`"
            
            sed -i "s%^.*$col1.*$%$col1 $res%" $1.new
        fi
    done
