# Historical note: sites/d2-animation-review/README.md

Original: [sites/d2-animation-review/README.md](<../../../../sites/d2-animation-review/README.md>)

SHA-256: `9312378649AAA1968DA6535FFB90AAD3430A58FA2C9607AF57149C9D65082CE8`; 1132 bytes.

````markdown
# Dishonored 2 animation review

ChatGPT Site containing a published snapshot of the development review library.
The site includes rendered MP4 previews and catalogue metadata, not game binaries,
native model files, saves, runtime tools or editable animation assets.

The source library remains independent in `work/natural-moveset` in the parent workspace.
Run `python -B build_snapshot.py` explicitly to prepare a fresh snapshot before republishing.
Updates are not automatic. Review notes use browser-local storage and can be exported as JSON.
Preserve the current hosting audience on updates. It was owner-private on first
publication and was found public when the user requested the next update.

On Windows, prepend Git's `bin` and `usr/bin` to the helper process PATH and set
`TAR_OPTIONS=--force-local` when running the Sites source workflow. The GNU archive
tool otherwise interprets the drive-letter colon as a remote-host separator.

Source clips, in-game pose demonstrations and gameplay-bound tests retain their original
labels. The catalogue does not claim that animation candidates are complete or visually accepted.

````
