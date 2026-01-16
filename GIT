#!/bin/bash
TOP_DIR="$(pwd)"
find "$TOP_DIR" -type f -name ".git*" -not -path "$TOP_DIR/.git*" | while read -r file; do
    rm -f "$file"
done
find "$TOP_DIR" -type d -name ".git*" -not -path "$TOP_DIR/.git*" | while read -r dir; do
    rm -rf "$dir"
done

git add .
git commit -m "${1}"
git push origin main
