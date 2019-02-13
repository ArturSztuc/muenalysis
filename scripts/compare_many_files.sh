#!/bin/bash

# Run comparisons on multiple files separately
# FLAG: WE ASUME ALL FILENAMES BETWEEN FOLDERS ARE THE SAME!!

# input and output files
# FILES{1,2} must be in "/home/user/output/output_file*" format
# OFILDER must be in "/home/user/output/FOLDER" format
EXE=$MUENALYSIS/bin/compareTTree
FILES1=$1
FILES2=$2
OFOLDER=$3

if [ -z $MUENALYSIS ]; then
  echo "muenalysis package not set up, you need to run . setup.sh to use this script!!"
  # Using return because exit closes the shell too, and we don't want that...
  return 0
fi

if [ ! -f $MUENALYSIS/bin/compareTTree ]; then
  echo "muenalysis package set but the executible not compiled!!!"
  # Using return because exit closes the shell too, and we don't want that...
  return 0
fi

FILENAMES=`ls $FILES1`

# Watch out, we're going to generate A LOT of folders/files!
for filename in $FILENAMES
do
  if [ ${filename: -5} == ".root" ]
  then
    # Make output folders
    OUTFOLDER=$OFOLDER/`basename $filename .root`
    mkdir $OUTFOLDER

    # Run the exec
    $MUENALYSIS/bin/compareTTree $FILES1/$filename $FILES2/$filename $OUTFOLDER/`basename $filename .root`_comparison.root &> $OUTFOLDER/`basename $filename .root`_log.log 
  fi
done
