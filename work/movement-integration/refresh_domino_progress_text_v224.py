from pathlib import Path
R=Path(__file__).resolve().parents[2];p=R/'work/movement-integration/refresh_grenade_handoff_v58.py'
s=p.read_text(encoding='utf-8-sig')
a=s.index("m['LatestContinuation']['in_progress']='Candidate203")
b=s.index('\n',a)
s=s[:a]+"m['LatestContinuation']['in_progress']='Candidate211 partner exclusion passesv212; ordinary native linking passesv214 and paired normal-guard body/mark regression passesv218 with12 checks,66 full-weight samples andzero clock rejects. Strict native connection reader passesv221:477/477 valid samples,227 linked. Candidate222 adds Domino world2/1000-byte cosmetic beam presentation and passes identity/protocol/load checks; consult final paired take receipts for runtime status. Source-world coordinates still require NPC authority/alignment; shared gameplay, complete movement and playable co-op remain incomplete.'"+s[b:]
old='Latest scoped results: [Chapter29]'
a=s.index(old);b=s.index('\n\n',a)
s=s[:a]+'''Latest scoped results: [Chapter29](29-airborne-root-continuity.md) records armed/holstered Mesmerize air continuity. [Chapter30](30-native-domino-investigation.md) records native Domino phases, partner exclusion, target marks and directed connection/beam ownership. Candidate211 passesv212 partner exclusion andv218 paired normal-enemy body/mark regression (12 checks,66 full-weight root samples,zero clock rejections). Thev221 source beam reader passes477/477 snapshots,including227 linked. Candidate222 carries native observed beams in Domino world2 and passes synthetic/load checks; individual paired final receipts establish runtime status. Earlier failed camera/floor/retirement assertions and their fixes remain preserved. Native NPC positions and shared gameplay outcomes remain unsynchronized; complete movement and playable co-op are unfinished.'''+s[b:]
p.write_text(s,encoding='utf-8')
