# constants
NANORC="~/.nanorc"
BASHRC="~/.bashrc"
ALIASES="~/.bash_aliases"
BLINKLEDBASH="~/Lab3/blinkLed.sh"
BLINKLEDPY="~/Lab3/blinkLed.py"


# aliases for quickly editing and sourcing configuration files
alias editnani="nano $NANORC"
alias nani=". $NANORC"

alias editrc="nano $BASHRC"
alias rc=". $BASHRC"

alias editalius="nano $ALIASES"
alias alius=". $ALIASES"

# quicker navigation
alias ..="cd .."

# script aliases
alias blinkBash="bash $BLINKLEDBASH"
alias blinkPy="python $BLINKLEDPY"
