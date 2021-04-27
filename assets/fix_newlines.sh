# Script to fix all json files that don't have a newline at the end
for FILE in $(find . -type f -name "*.json")
do
  LAST_CHAR=$(tail -c 1 $FILE)
  if [[ ! -z "$LAST_CHAR" ]];
  then
    echo "Fixing: $FILE";
    echo "" >> $FILE
  fi
done
