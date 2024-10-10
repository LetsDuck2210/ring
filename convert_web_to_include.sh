#!/usr/bin/env bash

echo "$0"
root_dir=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )

cd "$root_dir/web"
mkdir -p "$root_dir/src/www"
for file in *; do
  echo "$file > $root_dir/src/www/$file.h"
  xxd -i $file | sed "s/\(unsigned char .*\[\]\) =/\1 PROGMEM =/" > "$root_dir/src/www/$file.h"
done
