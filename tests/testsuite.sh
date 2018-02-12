#!/bin/bash

# The testsuite compare the output of my bin and the acu's bin
# I redirect the outputs on 2 files and compare those files with diff
FILE=file
MY_BIN=libmalloc.so
COMMAND=command
NB_TEST=1

echo =====  Testing the following commands =====
echo 

cp ${MY_BIN} tests
cd tests
touch ${MY_FILE} ${MODELE_FILE}

while read line
  do
    LD_PRELOAD=./${MY_BIN} $line > ${FILE}
    
    if [ $? -eq 0 ] #if not an error
      then
        echo 
        echo -e "TEST ${NB_TEST} [\033[32m OK \033[0m] $line" 
        echo "-------------------------------"
    else 
        echo
        echo -e "TEST ${NB_TEST} [\033[31m FAILED \033[0m] With command $line "
        echo "-------------------------------"
    fi

  let NB_TEST++
  done < ${COMMAND}

echo
rm ${FILE}
cd ..
