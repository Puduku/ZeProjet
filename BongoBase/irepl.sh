#!/bin/bash

# --- Configuration et Aide ---
usage() {
    echo "Usage: $0 [-r] <rechercher> <remplacer>"
    echo "  -r : Active le mode Expressions Régulières (par défaut: mode littéral)"
    exit 1
}

# Initialisation
USE_REGEX=false

# --- Gestion des options (Compatible tout Bash) ---
while getopts "r" opt; do
    case $opt in
        r) USE_REGEX=true ;;
        *) usage ;;
    esac
done
shift $((OPTIND-1))

if [ "$#" -ne 2 ]; then
    usage
fi

SEARCH=$1
REPLACE=$2

# Délimiteur invisible (Ctrl-A) pour éviter les conflits avec / ou #
DELIM=$(printf '\001')

# --- Préparation de la commande Vim ---
if [ "$USE_REGEX" = false ]; then
    VIM_PATTERN="\\V${SEARCH}"
    GREP_OPTS="-lF"
else
    VIM_PATTERN="${SEARCH}"
    GREP_OPTS="-l"
fi

# --- Recherche des fichiers (Version compatible Bash 3.2 / macOS) ---
# On construit la liste des fichiers en protégeant les espaces avec %q
FILES_LIST=""
while IFS= read -r line; do
    # On ajoute chaque fichier à la liste avec un échappement propre
    FILES_LIST+="$(printf " %q" "$line")"
done < <(find . -type f -name "*.[ch]" -exec grep $GREP_OPTS "$SEARCH" {} +)

# Vérification si la liste est vide
if [ -z "$FILES_LIST" ]; then
    echo "Aucune occurrence trouvée pour : $SEARCH"
    exit 0
fi

# --- Lancement de Vim ---
# 'set nomore' évite les blocages "Appuyez sur une touche"
# 'eval' est nécessaire pour interpréter les noms de fichiers échappés par %q
eval "vim -c \"set nomore | argdo %s${DELIM}${VIM_PATTERN}${DELIM}${REPLACE}${DELIM}cg | update\" -- $FILES_LIST"
