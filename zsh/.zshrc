# If you come from bash you might have to change your $PATH.
# export PATH=$HOME/bin:/usr/local/bin:$PATH

SELF_DIR="$HOME/workspace/dotfiles/zsh"




export GO_PATH=~/go
export PATH=$PATH:/$GO_PATH/bin



export NVM_DIR="$([ -z "${XDG_CONFIG_HOME-}" ] && printf %s "${HOME}/.nvm" || printf %s "${XDG_CONFIG_HOME}/nvm")"
[ -s "$NVM_DIR/nvm.sh" ] && \. "$NVM_DIR/nvm.sh" # This loads nvm


export PATH="$PATH:$HOME/bin"


# Path to your oh-my-zsh installation.
export ZSH="$HOME/.oh-my-zsh"

ZSH_THEME="robbyrussell"

plugins=(git zsh-autosuggestions zsh-syntax-highlighting fast-syntax-highlighting zsh-autocomplete)

source $ZSH/oh-my-zsh.sh

# Load Zinit
ZINIT_HOME="${XDG_DATA_HOME:-${HOME}/.local/share}/zinit/zinit.git"
if [[ ! -f $ZINIT_HOME/zinit.zsh ]]; then
    print -P "%F{33}▓▒░ %F{220}Installing Zinit...%f"
    command mkdir -p "$(dirname $ZINIT_HOME)"
    command git clone https://github.com/zdharma-continuum/zinit "$ZINIT_HOME" && \
        print -P "%F{33}▓▒░ %F{34}Installation successful.%f" || \
        print -P "%F{160}▓▒░ The clone has failed.%f"
fi
source "${ZINIT_HOME}/zinit.zsh"


if [[ -f "${SELF_DIR}/zsh_functions.inc" ]]; then
  zinit ice wait lucid
  zinit load "${SELF_DIR}/zsh_functions.inc"
fi

# Load custom aliases
if [[ -f "${SELF_DIR}/zsh_aliases.inc" ]]; then
    zinit ice wait lucid
    zinit load "${SELF_DIR}/zsh_aliases.inc"
else
    echo >&2 "WARNING: can't load shell aliases"
fi


source "${SELF_DIR}/zsh_functions.inc"
source "${SELF_DIR}/zsh_aliases.inc"






# chrome debugging
alias chrome="open -a /Applications/Google\ Chrome.app --args --remote-debugging-port=9222"


alias k="kubectl"

complete -o default -F __start_kubectl k







