#!/bin/bash

header_path="inc/gpio_aflist.h"
ioinfo_path="gpio_info"

ind1="  "
ind2="$ind1$ind1"
ind3="$ind2$ind1"

echo "" > $header_path
echof() {
    echo "$@" >> $header_path
}

echof "#ifndef __GPIO_AF_LIST_H"
echof "#define __GPIO_AF_LIST_H"
echof ""
echof "# Data found in "
echof "#    http://www.st.com/st-web-ui/static/active/en/resource/technical/document/datasheet/DM00071990.pdf"
echof ""

echof "char* gpio_aflist[][16][16] = {"

for x in "A" "B" "C" "D" "E" "F" "G" "H" "I" "J" "K" ; do
    echof "$ind1{"
    for((y=0;y<16;y++)) ; do
        echof "$ind2 {"
        for((z=0;z<16;z++)) ; do
           if [ $z -eq 0 ] ; then echof -ne "$ind3" ; fi
           if [ $z -eq 8 ] ; then echof -ne "\n$ind3" ; fi
           echof -n "\""
           echof -n "$x$y-f$z"
           echof -n "\""
           if [ $z -ne 15 ] ; then echof -n ", " ; fi
        done
        echof -ne "\n$ind2 }"
        if [ $y -ne 15 ] ; then echof ", " ; fi
    done
    echof -ne "\n$ind1}"
    if [ "x" != "K" ] ; then echof ", " ; fi
done 

echof "};"

echof ""
echof "#endif"

cat $ioinfo_path | 
while read line; do
    orig="`echo "$line" | awk '{ print $1 }'`"
    new="`echo "$line" | awk '{ print $2 }'`"
    sed -i "s#$orig#$new#g" $header_path
done
