#pragma once
// 1213m: how long every partner-presentation timeline (movement semantics and
// each action timeline) holds its last source sample across a network gap,
// and how far an action's clock runs on past its last sample.
// The lab keeps its strict rules: a 200 ms sample hold, a timeline cleared on
// a 250 ms gap and 100 ms of action extrapolation. Under the product
// controller all three match the 1 s presentation lease (presentationFreshMs,
// 1203d): on mission maps the source state stream gaps by 250-375 ms several
// times per 10 s (take 1235d), and each gap snapped the partner to its native
// pose, or froze an action, until the stream resumed. Native actions run on
// real time, so the held action keeps moving and the next sample continues it.
struct PresentationHold{double sampleMs=200,gapMs=250,extrapolateMs=100;};
inline PresentationHold& presentationHold(){static PresentationHold hold;return hold;}
inline void setPresentationHold(bool product){presentationHold()=product?PresentationHold{1000,1000,1000}:PresentationHold{};}
