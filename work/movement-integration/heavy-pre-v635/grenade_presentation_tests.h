#pragma once
void grenadePresentationTests(){
    GrenadePresentation t;GrenadePresentation::Sample o{};float a=0;
    check(t.push({100,1,1,5,260,400})&&t.push({140,1,1,5,300,400}),"grenade release clock accepts exact native scene");
    check(t.sample(132,o,a)&&GrenadePresentation::visible(o.held,o.variant,a),"grenade remains held before delayed native fire");
    check(t.sample(134,o,a)&&!GrenadePresentation::visible(o.held,o.variant,a),"grenade prop hides at source high-release instant");
    check(GrenadePresentation::visible(1,7,301.f/400)&&!GrenadePresentation::visible(1,7,302.f/400),"low release uses distinct native instant");
    check(!t.push({150,1,1,5,299,400})&&!t.push({150,1,1,7,310,400}),"same grenade event cannot rewind or switch branch");
    check(t.push({180,2,1,6,0,566})&&t.sample(179,o,a)&&o.variant==5,"future empty recovery waits for delayed body");
    check(t.sample(180,o,a)&&o.variant==6&&!GrenadePresentation::visible(o.held,o.variant,a),"empty recovery cannot re-show held prop");
    check(!t.sample(381,o,a),"stale grenade clock expires");
    check(!GrenadePresentation::valid({0,1,0,5,1,400})&&!GrenadePresentation::valid({0,1,1,5,1,399})&&!GrenadePresentation::valid({0,1,1,5,401,400}),"orphan grenade, wrong duration and expired native time refused");
    t.clear();check(!t.sample(180,o,a),"pause clears grenade clock");
    check(t.push({100,1,1,4,1300,1333})&&!t.push({133,1,1,4,0,1333})&&t.push({133,2,1,4,0,1333}),"cook loop restart requires new source event");
    check(!GrenadePresentation::visible(1,5,NAN)&&!GrenadePresentation::visible(0,0,0),"unknown or absent grenade cannot be displayed");
}
