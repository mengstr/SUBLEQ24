#!/bin/bash

ls macrotests | grep .sleq | sed 's/.sleq//' | while read line; do 

    echo -n "$line "

    script=$(sed -n '/;# SCRIPT/,/;# END/p' macrotests/$line.sleq | sed 's/^;//')
    expected=$(eval "$script")
    if [ "$expected" = "" ]; then
        echo "not tested"
        continue
    fi

    result=$(   \
        ./sleasm.js macrotests/$line.sleq && \
        ./subleq.js macrotests/$line.v20raw -q | \
        tr -d '\n' | \
        tr -d '\r' \
    )

    if [ "$expected" = "$result" ]; then
        echo OK
    else
        echo FAIL
        echo -e "\tExpected \'$expected\'"
        echo -e "\tBut got  \'$result\'"
    fi

done


