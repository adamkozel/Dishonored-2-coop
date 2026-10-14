# commenttoggle.txt

Original: [work/tool-evaluation-2026-10-12/void-slice/testdata/m17-probes/findings/commenttoggle.txt](<../../../../work/tool-evaluation-2026-10-12/void-slice/testdata/m17-probes/findings/commenttoggle.txt>)

SHA-256: `67BFE246E0A648247C97361EBCED8B84DCDCDCFFACC296CEBEE7605E54E1928C`; 783 bytes.

## Original saved record

````text
# commenttoggle: 62 files
testdata/golden/d2/game1/maps.campaign.dunwall.escape.tower.dunwall_escape_tower_p.entities	kind=commenttoggle	variant=0	span=57..60	diff=+PARSE_UNEXPECTED_TOKEN×1	note=comment-out-assignment
testdata/golden/eof.line-comment-no-newline.decl	kind=commenttoggle	variant=1	span=0..3	diff=+PARSE_EXPECTED_SYMBOL×1	note=uncomment-line
testdata/golden/generated.decls.material.models.environment.archi.karnaca.nature.rock.cliff_bottom_01..material.decl	kind=commenttoggle	variant=1	span=694..697	diff=+VOID_SCAN×2	note=uncomment-line
testdata/golden/maps.campaign.dunwall.escape.tower.dunwall_escape_tower_p.excerpt.entities	kind=commenttoggle	variant=0	span=57..60	diff=+PARSE_UNEXPECTED_TOKEN×1	note=comment-out-assignment
# commenttoggle: 4 findings

````
