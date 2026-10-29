# 118. Stage 8 step 3: bad connections, crashes and rejoining

Card [#236](https://github.com/adamkozel/Dishonored-2-coop/issues/236), from [the stage 8 brief](115-stage8-plan-brief.md). This step proves that the session survives a bad network and recovers from a leave, a crash and a rejoin. It also adds the Steam liveness rules from coop-anything ([chapter 114](114-coop-anything-playbook.md)). A read-only helper audit of the session code (the transport, liveness, leave, version, lobby, chunking, rejoin, quickload and crash handling) set the list of changes.

## What changed (candidate control-v2010)

- **Lab peer port.** With `D2COOP_LAB_PEER_PORT` (`Start-D2Lab.ps1 -PeerPort`), the lab client connects to `127.0.0.1:<port>`, so coop-anything's chaos relay (vendored, MIT) can sit between the two lab games. The host always listens on 31426, and the relay listens on 31427.
- **Control channel** (`src/control_channel.h`, packet `D2CT`, reliable):
  - **Hello and keep-alive, every second.** It carries the control protocol (1), the game build, a hash of the DLL file and a loading flag. A peer with another protocol, build or DLL is refused with a readable reason (`version_mismatch protocol 1/99 dll …`), where before it got a silent stream of rejected packets.
  - **Leave notice.** It is sent before a deliberate close (F8, Leave co-op), and the close lingers so the notice arrives. The partner can then tell a leave (`partner_left`) from a crash.
  - **Application drop.** With no control packet for 20 s, the peer is dropped (`keepalive_timeout`). The limit is 120 s while the peer says it is loading, or before its first hello. This is the backstop for a game that hangs while its Steam client keeps the connection up.
  - The status goes to `<status>.control.json`. A close, a keep-alive drop and a mismatch all end the connection through one path, `endCoopConnection`.
- **Transport timeout.** It is set explicitly to Steam's default of 10 s. A first try at 20 s (coop-anything's figure) made the host retire a crashed partner's avatar too late (take `peer-loss-dunwall-vm-v2009a`, failed). A 5 s blackout survives 10 s.
- **Online sessions:**
  - A host whose partner reconnects replaces a connection that has been silent for 5 s, as the lab already did. Before, the new connection was refused until the old one timed out.
  - A repeated join request or invite for the lobby we are already in keeps the live session. Before, it tore the session down.
  - **Host lobby recovery.** A host that is hosting with no partner, while Steam reports 0 members in its lobby, creates a new lobby quietly after 10 s (`lobbyRecreated` in the online status). In take `leave-dunwall-vm-v2008c` the host had dropped out of its own lobby, so the partner's rejoin failed with `lobby_enter_failed`. It happened in one of four takes, but nobody could have joined that host again.

## Results (confirmed runtime)

### Network trouble on the lab leg

The client was restarted through the relay, both games were brought to the Dunwall fixture, and then six profiles ran for 40 s each, followed by a 5 s two-way blackout. The relay's own statistics prove the impairment happened. The difference detector ([chapter 117](117-stage8-difference-detector.md)) ran after every phase.

| Take | Build | Profiles: fresh samples, world | Hell profile | 5 s blackout | Version mismatch |
|---|---|---|---|---|---|
| `network-trouble-v2006a` | relay-v2006 | **failed:** `campaign` from the main menu did not bring both games into gameplay; the relay itself carried traffic | – | – | – |
| `network-trouble-v2006b` | relay-v2006 (no control channel) | 40/40 fresh in all six; equal (554 values) | 532 client-to-host and 2735 host-to-client datagrams dropped, 69 duplicated; still fresh | connection kept; fresh 281 ms after it ended; equal | – |
| `network-trouble-v2007a` | control-v2007 (20 s transport timeout) | 40/40 in all six; equal (553–554) | 2210 host-to-client dropped; control live, no false drop | connection kept; fresh 267 ms after; equal | the host refused the client announcing protocol 99, `version_mismatch protocol 1/99`, 16 s after the restart |
| `network-trouble-v2010a` | **control-v2010 (final)** | **40/40 in all six; equal (553–554)** | 2628 host-to-client and 510 client-to-host dropped, 66 duplicated; control live | **connection kept; fresh 259 ms after; equal** | **refused after 16 s: `version_mismatch protocol 1/99`** |

Scope: the fixture is idle, so nothing in the world changes during a phase. The worlds stay equal because no shared change is lost or duplicated, not because a busy world was repaired. "Requests run exactly once" can only be measured once partner actions become requests (step 4); the coop-anything chaos rule is applied there.

### Leave, crash and rejoin on two Steam accounts

The host runs in the lab and the partner on the VM (the second account), using the Dunwall route of the earlier loss takes. The harness is `work/natural-moveset/Route-MissionVM-v2008.ps1`. It adds both games' control status at the leave, the kill and the relaunch, and the difference detector across the two machines at the end.

| Take | Case | Result |
|---|---|---|
| `leave-dunwall-vm-v2008a` | leave and rejoin | failed on the harness: its lobby check used a variable set only for a mid-mission join. The leave itself worked (`partner_left`, host still hosting) |
| `leave-dunwall-vm-v2008b` | leave and rejoin | failed on the harness: the rejoin placement check expected a join-warp, which is not due for a partner who never left the map (13 m, no warp); rejoined in 4.0 s |
| `leave-dunwall-vm-v2008c` | leave and rejoin | failed: the host had dropped out of its own Steam lobby (0 members), so the rejoin failed with `lobby_enter_failed`. That led to the lobby recovery above |
| `leave-dunwall-vm-v2008d` | leave and rejoin | **passed.** The host read `partner_left` and kept hosting; the partner rejoined in 4.0 s; both control channels live; gameflow equal; the detector across the machines: **538 values, equal** |
| `peer-loss-dunwall-vm-v2009a` | the partner's game killed, then relaunched | failed: the 20 s transport timeout kept the crashed partner's avatar in the host's world past the 15 s check |
| `peer-loss-dunwall-vm-v2010a` | the same, control-v2010 | **passed.** The host saw the loss in 1.8 s, with no stale ownership, still hosting, and no false leave (control `live`, not `partner_left`); the relaunched partner rejoined 23.2 s after its 173 s load; control live; gameflow equal; detector **538 values, equal** |
| `host-loss-dunwall-vm-v2010a` | the host's game killed | **passed.** The partner saw the loss in 1.7 s and stayed in gameplay on the map; all six host-driven NPC replicas got their AI back (pause refs 1 to 0, none left paused) |

## Not covered here

- **Quickload mid-session** is measured and fixed in area 4.4, where the fix also lives ([#240](https://github.com/adamkozel/Dishonored-2-coop/issues/240)). Today the partner does not reload when the host loads.
- **A version mismatch on screen:** the refusal is readable in the status and console. A native on-screen message comes with step 5's polish.
- **Chunking:** the largest message is 10.9 KB (the guard body packet), far below the limits. Large unreliable messages lose the whole message when one fragment is lost; that was measured under the relay profiles without a freshness effect.
