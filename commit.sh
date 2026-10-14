#!/bin/bash
#
# Opinionated one-command commits. Picks a category, enforces one format:
#
#     type(scope): one-line message
#
# Multi-line pastes are accepted: the first line becomes the subject, the
# rest becomes the commit body. The changelog only ever uses the subject.
#
# Usage:
#     bash commit.sh [-t type] [-s scope] [-m "message"] [-b "body"] [--push] [--no-push] [--dry-run]
#
# Anything not passed as a flag is prompted for. Works in any repo.
# Examples:
#     bash commit.sh -t fix -s widgets -m "stop flicker on resize" --push
#     bash commit.sh   # fully interactive

set -u

# Proper insert-mode line editing on interactive prompts (arrows, insert,
# Ctrl keys) instead of the kernel's append-only echo, which displays edits
# as overwrite. Bracketed paste is disabled so pasted text cannot smuggle
# escape sequences into messages. Process-local; affects nothing else.
bind 'set enable-bracketed-paste off' 2>/dev/null || true

TYPES="feat fix engine ui opcode build docs chore"

describe() {
    case "$1" in
        feat)    echo "new feature or capability" ;;
        fix)     echo "bug fix" ;;
        engine)  echo "audio engine / backend core" ;;
        ui)      echo "widgets / frontend UI" ;;
        opcode)  echo "Csound opcodes" ;;
        build)   echo "build system / CI" ;;
        docs)    echo "documentation" ;;
        chore)   echo "maintenance and chores" ;;
    esac
}

usage() {
    sed -n '2,18p' "$0"
    echo ""
    echo "Types:"
    local i=1
    for t in $TYPES; do
        printf "  %d) %-8s %s\n" "$i" "$t" "$(describe "$t")"
        i=$((i + 1))
    done
}

TYPE=""; SCOPE=""; MESSAGE=""; BODY=""; BODY_GIVEN=0; PUSH="ask"; DRY_RUN=0

while [ $# -gt 0 ]; do
    case "$1" in
        -t|--type)    TYPE="${2:-}"; shift 2 ;;
        -s|--scope)   SCOPE="${2:-}"; shift 2 ;;
        -m|--message) MESSAGE="${2:-}"; shift 2 ;;
        -b|--body)    BODY="${2:-}"; BODY_GIVEN=1; shift 2 ;;
        -p|--push)    PUSH="yes"; shift ;;
        --no-push)    PUSH="no"; shift ;;
        -n|--dry-run) DRY_RUN=1; shift ;;
        -h|--help)    usage; exit 0 ;;
        *) echo "Unknown option: $1"; usage; exit 1 ;;
    esac
done

git rev-parse --is-inside-work-tree >/dev/null 2>&1 || { echo "Not inside a git repository."; exit 1; }

# Nothing staged? Offer to stage everything (the lazy default).
# Dry runs never touch the index.
if [ "$DRY_RUN" != "1" ] && git diff --cached --quiet; then
    printf "Nothing staged. Stage everything? [Y/n] "
    read -r -e answer
    answer=${answer%$'\r'}
    if [ -z "$answer" ] || [ "$answer" = "Y" ] || [ "$answer" = "y" ]; then
        git add -A
    fi
    git diff --cached --quiet && { echo "Nothing to commit. Aborting."; exit 1; }
fi

# Category (the forcing function).
valid_type() {
    for t in $TYPES; do
        [ "$1" = "$t" ] && return 0
    done
    return 1
}
if ! valid_type "$TYPE"; then
    if [ -n "$TYPE" ]; then
        echo "Unknown type: $TYPE"
    fi
    echo "Pick a category:"
    i=1
    for t in $TYPES; do
        printf "  %d) %-8s %s\n" "$i" "$t" "$(describe "$t")"
        i=$((i + 1))
    done
    printf "Type [1-8 or name]: "
    read -r -e TYPE
    TYPE=${TYPE%$'\r'}
    if [[ $TYPE =~ ^[1-8]$ ]]; then
        i=1
        for t in $TYPES; do
            [ "$i" = "$TYPE" ] && { TYPE=$t; break; }
            i=$((i + 1))
        done
    fi
    valid_type "$TYPE" || { echo "Invalid type. Aborting."; exit 1; }
fi

# Scope is optional but encouraged.
if [ -z "$SCOPE" ]; then
    printf "Scope (optional, e.g. widgets, engine, ci) [Enter to skip]: "
    read -r -e SCOPE
    SCOPE=${SCOPE%$'\r'}
fi
SCOPE=$(echo "$SCOPE" | tr -d ' |:()')

# The one line (first pasted line wins; the rest becomes the body).
if [ -z "$MESSAGE" ]; then
    printf "One-line message: "
    read -r -e MESSAGE
    MESSAGE=${MESSAGE%$'\r'}
fi
# A -m value containing newlines splits into subject + body.
case "$MESSAGE" in
    *$'\n'*)
        if [ "$BODY_GIVEN" = "0" ]; then
            BODY=$(printf '%s\n' "$MESSAGE" | tail -n +2)
        fi
        MESSAGE=$(printf '%s\n' "$MESSAGE" | head -n 1)
        ;;
esac
MESSAGE=$(echo "$MESSAGE" | sed 's/\r$//;s/^ *//;s/ *$//')
[ -z "$MESSAGE" ] && { echo "Empty message. Aborting."; exit 1; }
case "$MESSAGE" in
    *"|"*) echo "Subject must not contain '|'. Aborting."; exit 1 ;;
esac
if [ "${#MESSAGE}" -gt 72 ]; then
    echo "Note: message is ${#MESSAGE} chars (over 72) — kept as-is."
fi

# Optional body. In a terminal, offer a paste prompt (pipes keep the old
# flow untouched so scripted answers still land on the right prompts).
# COMMIT_PROMPT_BODY=1 forces the prompt (testing / scripting).
if [ "$BODY_GIVEN" = "0" ] && { [ -t 0 ] || [ "${COMMIT_PROMPT_BODY:-0}" = "1" ]; }; then
    printf "Body (optional paste; a short pause or . alone finishes, Enter to skip): "
    read -r -e firstline
    firstline=${firstline%$'\r'}
    if [ -n "$firstline" ] && [ "$firstline" != "." ]; then
        BODY="$firstline"
        # Pasted lines arrive back-to-back, so an idle timeout ends input
        # instead of hanging forever waiting for '.'. '.' still ends early.
        while IFS= read -r -e -t 5 line; do
            line=${line%$'\r'}
            [ "$line" = "." ] && break
            BODY="$BODY
$line"
        done
    fi
fi
# Normalise the body: strip carriage returns and leading blank lines.
BODY=$(printf '%s\n' "$BODY" | sed 's/\r$//' | sed '/./,$!d')
[ -z "$(echo "$BODY" | tr -d '[:space:]')" ] && BODY=""

if [ -n "$SCOPE" ]; then
    SUBJECT="$TYPE($SCOPE): $MESSAGE"
else
    SUBJECT="$TYPE: $MESSAGE"
fi

echo ""
echo "  $SUBJECT"
if [ -n "$BODY" ]; then
    echo ""
    echo "$BODY"
fi
echo ""
if [ "$DRY_RUN" = "1" ]; then
    echo "(dry run — not committed)"
    exit 0
fi
printf "Commit with this message? [Y/n] "
read -r -e answer
answer=${answer%$'\r'}
if [ -n "$answer" ] && [ "$answer" != "Y" ] && [ "$answer" != "y" ]; then
    echo "Aborted."
    exit 1
fi

if [ -n "$BODY" ]; then
    git commit -m "$SUBJECT" -m "$BODY" || exit 1
else
    git commit -m "$SUBJECT" || exit 1
fi

if [ "$PUSH" = "ask" ]; then
    printf "Push now? [y/N] "
    read -r -e answer
    answer=${answer%$'\r'}
    [ "$answer" = "y" ] || [ "$answer" = "Y" ] && PUSH="yes" || PUSH="no"
fi
[ "$PUSH" = "yes" ] && git push
