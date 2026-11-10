#!/bin/sh
# Dishonored 2 co-op for Linux: restores the original game files from the install's backup.
# Usage: ./uninstall-linux.sh [<Dishonored2 folder>] [--keep-backup]
cd "$(dirname "$0")" || exit 1
command -v python3 >/dev/null 2>&1 || { echo "ERROR: python3 is needed." >&2; exit 1; }
exec python3 d2coop_linux.py uninstall "$@"
