#!/bin/bash

source "`ueberzug library`"
COVER="/tmp/album_cover.png"

function add_cover {
  ImageLayer::add [identifier]="img" [x]="2" [y]="1" [path]="$COVER"
}

ImageLayer 0< <(
if [ ! -f "$COVER" ]; then
  cp "/home/beau/.config/ncmpcpp/default_cover.png" "$COVER"
fi
#rerender image when changed
while inotifywait -q -q -e close_write "$COVER"; do
  add_cover
done
)
