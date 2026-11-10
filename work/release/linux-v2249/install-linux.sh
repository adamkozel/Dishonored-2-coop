#!/bin/sh
# Dishonored 2 co-op for Linux (Steam with Proton). Installs into your Steam Dishonored 2 folder, or the folder given
# as the first argument. Start Dishonored 2 once from Steam before this, so Proton has made the game's prefix.
cd "$(dirname "$0")" || exit 1
command -v python3 >/dev/null 2>&1 || { echo "ERROR: python3 is needed." >&2; exit 1; }
exec python3 d2coop_linux.py install "$@"
