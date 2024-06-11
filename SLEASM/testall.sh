#!/bin/bash

ls macrotests | grep .sleq | sed 's/.sleq//' | while read line; do 

    expected=$(cat macrotests/$line.sleq | grep EXPECT | sed 's/; EXPECT //' | tr -d '\n'  | tr -d '\r')
    if [ "$expected" = "" ]; then
        continue
    fi

    echo -n "$line "
    result=$(./sleasm.js macrotests/$line.sleq -0 && ./subleq.js macrotests/$line.v20raw -q |  tr -d '\n'  | tr -d '\r')

    if [ "$expected" = "$result" ]; then
        echo OK
    else
        echo FAIL
        echo -e "\tExpected \'$expected\'"
        echo -e "\tBut got  \'$result\'"
    fi

done


