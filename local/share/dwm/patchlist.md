# dwm patches

## bar-height
it's in the name, allows for change of the bar height

## barpadding
adds padding to dwm's bar, looks pretty

## colorbar
allows for changing the color of certain regions of dwm's bar

## cool-autostart
nifty autostart that works through config.h, external
shell script not needed

## cursorwarp
warps/moves cursor to the active window

## dmenumatchtop
makes dmenu match position of bar

## hide-vacant-tags
hides vacant tags, not much else

## pertag
keeps the layout of specific tags, not one layout for all tags

## push-no-master
fork of "push" patch, allows you to move clients with client list,
but doesn't allow you to move inside the master area

## restartsig
implements "restart" functionality so you don't have to kill your X
session after every recompile

## restore after restart
works with restartsig to remember which tags windows/clients were in

## savefloats
saves where windows were in floating layout

## scratchpads
makes terminal "scratchpads"

## statuspadding
allows you to give the statusbar padding, also looks pretty

## swallow
adapted from Plan9's "rio", clients that are marked with "isterminal" will
swallow any windows opened by any child process (i.e. if you're running xclock,
upon xclock closing, your terminal will be restored in the same position)

## vanitygaps
pretty and adjustable gaps
