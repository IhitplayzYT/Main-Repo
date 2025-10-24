#!/bin/bash
# Folder with wallpapers
WALLDIR="$HOME/ViableImages/wallpapers/"

# File to keep track of the current wallpaper index
INDEXFILE="$HOME/.cache/hyprpaper_wall_index"

# Read current index or start from 0
INDEX=$(cat "$INDEXFILE" 2>/dev/null || echo 0)

# Get list of wallpapers
WALLPAPERS=("$WALLDIR"/*.{png,jpg,jpeg,webp})
NUM=${#WALLPAPERS[@]}

# Set next index
NEXT=$(( (INDEX + 1) % NUM ))

# Set wallpaper using hyprctl hyprpaper
hyprctl hyprpaper preload "${WALLPAPERS[$NEXT]}"
hyprctl hyprpaper wallpaper "eDP-1,${WALLPAPERS[$NEXT]}"

# Save index
echo "$NEXT" > "$INDEXFILE"

