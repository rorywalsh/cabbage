#!/bin/bash  

bin_path_default='/usr/bin'
cabbage_rack_path_default='/usr/bin/CabbageRack'
icon_path_default='/usr/share/icons/hicolor/512x512/apps'
doc_path_default='/usr/share/doc/cabbage'
theme_path_default='/usr/share/cabbage'
desktop_path_default='/usr/share/applications'

cabbage_rack_path=$cabbage_rack_path_default
bin_path=$bin_path_default
icon_path=$icon_path_default
doc_path=$doc_path_default
theme_path=$theme_path_default
desktop_path=$desktop_path_default

build_path="../../CabbageInstall"

while getopts ":hb:i:d:t:" opt; do
  case $opt in
    h)
      echo "Usage:"
      echo " `basename "$0"` [options]"
      echo
      echo "Options:"
      echo " -b <bin_path>      where to install binary files"
      echo "                      default: $bin_path_default"
      echo " -i <icon_path>     where to install desktop icons"
      echo "                      default: $icon_path_default"
      echo " -d <doc_path>      where to install documentation"
      echo "                      default: $doc_path_default"
      echo " -t <desktop_path>  where to install desktop entries"
      echo "                      default: $desktop_path_default"
      exit 0
      ;;
    b)
      bin_path="$OPTARG"
      ;;
    i)
      icon_path="$OPTARG"
      ;;
    d)
      doc_path="$OPTARG"
      ;;
    t)
      desktop_path="$OPTARG"
      ;;
    \?)
      echo "Invalid option: -$OPTARG"
      exit 1
      ;;
  esac
done

install -d "$icon_path"
for file in "$build_path"/images/*; do
  install -m644 "$file" "$icon_path"
done

install -d "$bin_path"
for file in "$build_path"/bin/*; do
  install -m755 "$file" "$bin_path"
done

install -d "$cabbage_rack_path"
for file in "$build_path"/CabbageRack/*; do
  install -m755 "$file" "$cabbage_rack_path"
done

install -d "$desktop_path"
for file in "$build_path"/desktop/*; do
  install -m644 "$file" "$desktop_path"
done

install -d "$doc_path"
cp -r "${build_path}/Examples" "$doc_path"
chmod -R 755 "$doc_path"

install -d "$theme_path"
cp -r "${build_path}/Themes" "$theme_path"
chmod -R 755 "$theme_path"