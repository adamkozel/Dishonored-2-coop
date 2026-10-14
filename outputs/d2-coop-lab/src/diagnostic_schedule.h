#pragma once
#include <array>
#include <cstdint>
// Round-robin file diagnostics stay on the native thread. A delayed cycle
// never catches up by writing the entire catalogue in one frame.
template<unsigned N> struct DiagnosticSchedule {
    std::array<std::uint64_t,N> last{};unsigned cursor=0;
    bool next(std::uint64_t now,unsigned& index){
        if(last[cursor]&&(now<last[cursor]||now-last[cursor]<250))return false;
        index=cursor;last[cursor]=now;cursor=(cursor+1)%N;return true;
    }
};
