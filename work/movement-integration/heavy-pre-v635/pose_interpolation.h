#pragma once
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <cstring>
// Keep the sender's sampling intervals when Steam delivers packets together.
// Arrival timestamps compressed a ~40ms sprint step into ~10ms in the exact
// runtime trace sprint-root-trace-v1.json. The clock mapping changes at no more
// than 1ms per second, so jitter cannot become an artificial movement speed.
struct SourcePoseTimeline {
    std::uint64_t session=0,epoch=0,map=0,lastSource=0;double offset=0,lastArrival=0,lastMapped=0;
    void clear(){*this={};}
    bool mapTime(std::uint64_t us,double arrival,std::uint64_t s,std::uint64_t e,std::uint64_t m,double& mapped){
        if(!us||us>9007199254740991ull||!std::isfinite(arrival)||!s||!e||!m)return false;
        const double source=double(us)*.001;
        const bool replaced=session!=s||epoch!=e||map!=m;
        if(!replaced&&(us<=lastSource||arrival<lastArrival))return false;
        double candidateOffset=arrival-source;
        if(!replaced){
            // A source clock discontinuity is not permission to replay history.
            if(arrival-lastArrival<=250.&&double(us-lastSource)*.001>1000.)return false;
            // A receive gap is not a new clock incarnation. Its first packet
            // can be queued and old; rebasing to its arrival makes subsequent
            // fresh root/action packets falsely appear >80ms in the future.
            const double limit=(std::min)(5.,(arrival-lastArrival)*.001);
            candidateOffset=offset+(std::max)(-limit,(std::min)(limit,arrival-source-offset));
        }
        mapped=source+candidateOffset;
        if(!replaced&&(mapped<=lastMapped||mapped>arrival+80.||arrival-mapped>250.))return false;
        // Rejected observations cannot mutate the clock used by other streams.
        session=s;epoch=e;map=m;offset=candidateOffset;
        lastSource=us;lastArrival=arrival;lastMapped=mapped;return true;
    }
};
// Quarantine an initial burst before publishing a clock to root/action/world
// streams. A queued first sample can carry >150ms of startup delay. Its arrival
// is not a trustworthy offset. Established clocks retain the strict mapper
// above; this calibration never runs merely because an established stream gaps.
struct CalibratedSourcePoseTimeline : SourcePoseTimeline {
    struct Calibration {
        std::uint64_t session=0,epoch=0,map=0,firstSource=0,lastSource=0;
        double firstArrival=0,lastArrival=0,minimumOffset=0,nearStart=0;
        unsigned samples=0,nearSamples=0;
    } calibration{};
    void clear(){SourcePoseTimeline::clear();calibration={};}
    bool mapTime(std::uint64_t us,double arrival,std::uint64_t s,std::uint64_t e,std::uint64_t m,double& mapped){
        if(!us||us>9007199254740991ull||!std::isfinite(arrival)||!s||!e||!m)return false;
        if(session==s&&epoch==e&&map==m)return SourcePoseTimeline::mapTime(us,arrival,s,e,m,mapped);
        // Old published identity cannot authorize side streams during rejoin.
        SourcePoseTimeline::clear();auto& c=calibration;const double observed=arrival-double(us)*.001;
        const bool replaced=c.session!=s||c.epoch!=e||c.map!=m;
        if(!replaced&&(us<=c.lastSource||arrival<c.lastArrival))return false;
        if(replaced||arrival-c.lastArrival>250.||double(us-c.lastSource)*.001>1000.){
            c={s,e,m,us,us,arrival,arrival,observed,arrival,1,1};return false;
        }
        if(observed<c.minimumOffset-20.||observed>c.minimumOffset+20.){c.nearSamples=0;c.nearStart=arrival;}
        c.minimumOffset=(std::min)(c.minimumOffset,observed);
        if(observed<=c.minimumOffset+20.){if(!c.nearSamples)c.nearStart=arrival;++c.nearSamples;}
        c.lastSource=us;c.lastArrival=arrival;++c.samples;
        if(c.samples<8||arrival-c.firstArrival<250.||double(us-c.firstSource)*.001<250.||
           c.nearSamples<3||arrival-c.nearStart<100.)return false;
        mapped=double(us)*.001+c.minimumOffset;
        if(mapped>arrival+80.||arrival-mapped>250.)return false;
        session=s;epoch=e;map=m;offset=c.minimumOffset;lastSource=us;lastArrival=arrival;lastMapped=mapped;
        return true;
    }
};
// Presentation-only delayed sampling; never extrapolates gameplay or movement.
// Timestamps belong to the receiver's monotonic high-resolution clock.
struct PresentationPose {
    double time=0;
    std::uint64_t session=0,epoch=0,map=0;
    std::uint32_t sequence=0;
    float position[3]{},orientation[9]{};
    // Optional native player-body channel, sampled on the same presentation
    // timeline as its root. This does not change the network packet format.
    bool bodyValid=false,motionValid=false,teleport=false;
    // Local presentation evidence for a vertical interpolation segment. This
    // does not change the replicated motion state or authorize gameplay.
    bool airTrajectory=false;
    unsigned stance=0,motion=0;
    float body[20][12]{};
};
inline void presentationQuaternion(const float* m,float* q){
    // Matrices store XYZ columns, not rows.
    const float trace=m[0]+m[4]+m[8];
    if(trace>0){const float s=std::sqrt(trace+1.f)*2;q[3]=s*.25f;q[0]=(m[5]-m[7])/s;q[1]=(m[6]-m[2])/s;q[2]=(m[1]-m[3])/s;}
    else{int i=m[4]>m[0]?1:0;if(m[8]>m[i*3+i])i=2;const int j=(i+1)%3,k=(i+2)%3;
        const float s=std::sqrt((std::max)(0.f,1.f+m[i*3+i]-m[j*3+j]-m[k*3+k]))*2;
        q[i]=s*.25f;q[j]=(m[j*3+i]+m[i*3+j])/s;q[k]=(m[k*3+i]+m[i*3+k])/s;q[3]=(m[j*3+k]-m[k*3+j])/s;
    }
}
inline void presentationRotation(const float* a,const float* b,float t,float* out){
    float qa[4]{},qb[4]{},q[4]{},dot=0,n=0;presentationQuaternion(a,qa);presentationQuaternion(b,qb);
    for(int i=0;i<4;++i)dot+=qa[i]*qb[i];
    for(int i=0;i<4;++i){q[i]=qa[i]*(1-t)+(dot<0?-qb[i]:qb[i])*t;n+=q[i]*q[i];}
    n=1.f/std::sqrt(n);for(float& f:q)f*=n;
    const auto x=q[0],y=q[1],z=q[2],w=q[3];
    const float m[]={1-2*(y*y+z*z),2*(x*y+z*w),2*(x*z-y*w),2*(x*y-z*w),1-2*(x*x+z*z),2*(y*z+x*w),2*(x*z+y*w),2*(y*z-x*w),1-2*(x*x+y*y)};
    std::memcpy(out,m,sizeof(m));
}
struct PoseInterpolation {
    static constexpr double delayMs=80.;
    PresentationPose samples[16]{};
    unsigned count=0;
    std::uint64_t resets=0;
    mutable bool renderClockReady=false;
    mutable double renderTime=0,lastRenderNow=0;
    void clear(){count=0;renderClockReady=false;}
    double targetTime(double now)const{
        const double desired=now-delayMs;
        if(!renderClockReady||now<lastRenderNow||now-lastRenderNow>250.){
            renderTime=desired;renderClockReady=true;
        }else{
            // If a packet arrived after the buffer ran dry, recover at at most
            // 5% faster clock speed. Jumping straight to the newly available
            // sample produces a visible position spike despite source timestamps.
            renderTime=(std::min)(desired,renderTime+(now-lastRenderNow)*1.05);
        }
        renderTime=(std::min)(renderTime,samples[count-1].time);lastRenderNow=now;return renderTime;
    }
    bool push(const PresentationPose& p){
        if(!std::isfinite(p.time)||!p.session||!p.epoch||!p.map||!p.sequence)return false;
        if(p.motionValid&&(p.stance>1||p.motion>3))return false;
        for(float f:p.position)if(!std::isfinite(f))return false;
        for(float f:p.orientation)if(!std::isfinite(f))return false;
        for(int i=0;i<3;++i)for(int j=i;j<3;++j){float dot=0;for(int k=0;k<3;++k)dot+=p.orientation[i*3+k]*p.orientation[j*3+k];if(std::fabs(dot-(i==j?1.f:0.f))>.02f)return false;}
        const auto* m=p.orientation;const float det=m[0]*(m[4]*m[8]-m[5]*m[7])-m[3]*(m[1]*m[8]-m[2]*m[7])+m[6]*(m[1]*m[5]-m[2]*m[4]);
        if(det<.98f||det>1.02f)return false;
        if(count){const auto& last=samples[count-1];
            if(p.session==last.session&&p.epoch==last.epoch&&p.map==last.map){
                if(static_cast<std::int32_t>(p.sequence-last.sequence)<=0||p.time<last.time)return false;
                float distance=0;for(int i=0;i<3;++i){const float d=p.position[i]-last.position[i];distance+=d*d;}
                // Stop interpolation across teleports or a long packet outage.
                if(distance>9.f||p.time-last.time>250.){clear();++resets;}
                else if(p.time-last.time<.5){samples[count-1]=p;return true;}
            }else{clear();++resets;}
        }
        if(count==16){for(unsigned i=1;i<count;++i)samples[i-1]=samples[i];--count;}
        samples[count++]=p;return true;
    }
    bool airTrajectoryAt(unsigned hi)const{
        if(!hi||hi>=count)return false;
        const auto& a=samples[hi-1];const auto& b=samples[hi];
        if(!a.motionValid||!b.motionValid)return false;
        if(a.motion>=2||b.motion>=2)return true;
        // Native v168: contact is reported before the capsule finishes its
        // final downward settle. Bound this to 100 ms and 25 cm after the FIRST
        // confirmed Fall -> Ground sample, with coherent descending ground
        // samples throughout. Ordinary ground/stairs cannot grant this flag.
        if(a.motion||b.motion||b.position[2]>a.position[2])return false;
        unsigned first=hi-1;
        while(first&&samples[first-1].motionValid&&samples[first-1].motion==0){
            if(samples[first].position[2]>samples[first-1].position[2])return false;
            --first;
        }
        if(!first)return false;
        const auto& fall=samples[first-1];const auto& contact=samples[first];
        return fall.motionValid&&fall.motion==3&&contact.time-fall.time<=100.&&
            b.time-contact.time<=100.&&contact.position[2]<=fall.position[2]&&
            contact.position[2]-b.position[2]<=.25f;
    }
    bool sample(double now,PresentationPose& out)const{
        if(!count||!std::isfinite(now))return false;
        const double target=targetTime(now);
        if(target<=samples[0].time){out=samples[0];out.airTrajectory=false;return true;}
        for(unsigned i=1;i<count;++i)if(target<=samples[i].time){
            const auto& a=samples[i-1];const auto& b=samples[i];const float t=static_cast<float>((target-a.time)/(b.time-a.time));out=b;out.time=target;
            // Discrete native motion belongs to its source sample, independently
            // of optional first-person body matrices. Do not announce ground
            // contact early merely because root position interpolates toward it.
            if(t<1.f){out.motionValid=a.motionValid;out.motion=a.motion;out.stance=a.stance;}
            out.airTrajectory=airTrajectoryAt(i);
            for(int j=0;j<3;++j)out.position[j]=a.position[j]+(b.position[j]-a.position[j])*t;
            presentationRotation(a.orientation,b.orientation,t,out.orientation);
            return true;
        }
        out=samples[count-1];out.airTrajectory=airTrajectoryAt(count-1);return true; // Hold rather than predict on loss.
    }
    bool sampleBody(double now,PresentationPose& out)const{
        out.bodyValid=false;if(!count||!std::isfinite(now))return false;
        int first=-1,last=-1;
        for(unsigned i=0;i<count;++i)if(samples[i].bodyValid){if(first<0)first=i;last=i;}
        // Incoherent source reads omit only this optional channel. Bridge a
        // short hole with coherent samples; never invent data or hold stale
        // body animation indefinitely while unrelated root packets arrive.
        if(last<0||now-samples[last].time>=200.)return false;
        const double target=targetTime(now);int lo=first,hi=first;
        for(int i=first;i<=last;++i)if(samples[i].bodyValid){hi=i;if(samples[i].time>=target)break;lo=i;}
        const auto& a=samples[lo];const auto& b=samples[hi];
        const float t=hi==lo?0.f:static_cast<float>((std::max)(0.,(std::min)(1.,(target-a.time)/(b.time-a.time))));
        out.stance=b.stance;out.motion=b.motion;out.bodyValid=true;
        for(unsigned j=0;j<20;++j){presentationRotation(a.body[j],b.body[j],t,out.body[j]);
            for(unsigned k=9;k<12;++k)out.body[j][k]=a.body[j][k]+(b.body[j][k]-a.body[j][k])*t;}
        return true;
    }
};
