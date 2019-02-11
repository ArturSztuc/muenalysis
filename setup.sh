#!/bin/bash

if [ -z $MUENALYSIS ]; then
  export MUENALYSIS=$(pwd)
fi

export LD_LIBRARY_PATH=${MUENALYSIS}/lib:${LD_LIBRARY_PATH}
