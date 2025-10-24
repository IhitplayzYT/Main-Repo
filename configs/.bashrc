# If not running interactively, don't do anything
[[ $- != *i* ]] && return
alias l='script -qfc "lsd --color=always $*" /dev/null | lolcat -a -s 100'
alias tp='cd "$(dirname $(fzf))" '
alias liah='script -qfc "lsd -liah --color=always $*" /dev/null | lolcat -a -s 100'
alias s0="shutdown 0 "
alias vi="nvim "
alias vim="nvim "
alias purge="sudo pacman -R "
alias ls="lsd "
alias wtr="curl wttr.in "
alias cat='lolcat '
alias less='less -R ' 
alias tree='tree -C | lolca -a -s 100'
alias pwd="pwd | lolcat"
alias install="sudo pacman -Sy "
alias src="source "
alias cbonsai="cbonsai -li -t 0.2 -M 10 -L 50 "
alias sbash="source /home/IhitplayzYT/.bashrc "
alias typetest="toipe -w commonly-misspelled "
alias audio_visualiser="cava "
alias music_player="kew "
alias ascii_text="figlet -f smslant "
alias time="date +\"%r\" | lolcat -a -s 100"
alias cool_decrypt="nms "
alias stars="astroterm -s 100 -c -C -l 2 -t 100.0 --fps=64 -u "
alias aquarium="asciiquarium "
alias cmatrix="cmatrix -r -a -u 8 "
alias rain="source myenv/bin/activate ; terminal-rain --lightning-color magenta ; deactivate "
alias zen="flatpak run app.zen_browser.zen "
eval $(thefuck --alias)
eval $(thefuck --alias fk)


RED='\[\033[0;31m\]'
GREEN='\[\033[0;32m\]'
YELLOW='\[\033[0;33m\]'
BLUE='\[\033[0;34m\]'
PURPLE='\[\033[0;35m\]'
CYAN='\[\033[0;36m\]'
WHITE='\[\033[0;37m\]'
BOLD_RED='\[\033[1;31m\]'
BOLD_GREEN='\[\033[1;32m\]'
BOLD_YELLOW='\[\033[1;33m\]'
BOLD_BLUE='\[\033[1;34m\]'
BOLD_PURPLE='\[\033[1;35m\]'
BOLD_CYAN='\[\033[1;36m\]'
NC='\[\033[0m\]'


# Random Neofetch 
TERMINAL_IMAGE_DIR="$HOME/terminal_images"
LAST_IMAGE_FILE="$HOME/.last_neofetch_image"
if [[ -f "$LAST_IMAGE_FILE" ]]; then
    LAST_IMAGE=$(cat "$LAST_IMAGE_FILE")
else
    LAST_IMAGE=""
fi
while :; do
    RANDOM_IMAGE=$(find "$TERMINAL_IMAGE_DIR" -type f | shuf -n1)
    [[ "$RANDOM_IMAGE" != "$LAST_IMAGE" ]] && break
done
echo "$RANDOM_IMAGE" > "$LAST_IMAGE_FILE"
export IMAGE_SOURCE="$RANDOM_IMAGE"


rfkill unblock all
clear
if [[ -z "$TMUX" ]]; then
    neofetch
fi

PS1='\[\033[34m\] $(date +"%r") \n\[\033[36m\]\w \[\033[35m\] \[\033[0m\]'

figlet -f smslant IhitplayzYT| awk '{ printf "%*s\n", (100+length($0))/2, $0 }'  | lolcat -a -s 400 
export LANG=en_US.UTF-8
export LC_ALL=en_US.UTF-8

export PATH=$PATH:/home/IhitplayzYT/.spicetify

export FZF_DEFAULT_COMMAND="fd --hidden --strip-cwd-prefix --exclude .git "
export FZF_CTRL_T_COMMAND="$FZF_DEFAULT_COMMAND"
export FZF_DEFAULT_OPTS="--height 50% --layout=default --border --color=hl:#2dd4bf"
export FZF_TMUX_OPTS=" -p90%,70% "
export FZF_CTRL_T_OPTS="--preview 'bat --color=always -n --line-range :500 {}'"
export PATH=/path/to/instantclient:$PATH
export LD_LIBRARY_PATH=/usr/lib/oracle/23/client64/lib
export PATH=$PATH:/usr/lib/oracle/23/client64/bin

