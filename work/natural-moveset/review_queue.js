/* Review playback only; feedback and the original library remain authoritative. */
(()=>{
 const bar=document.createElement('div');bar.className='controls';bar.setAttribute('aria-label','Review sequence');
 bar.innerHTML='<button id="previousClip">Previous clip</button><button id="nextClip">Next clip</button><label><input id="autoNext" type="checkbox"> Play next automatically</label><span id="queuePosition" class="small"></span>';
 $('videoLabel').before(bar);
 function rows(){
  if(mode==='requirements')return(selected?.motions||[]).map(id=>data.motions.find(m=>m.id===id)).filter(m=>m?.videos.length);
  const query=$('search').value.toLowerCase(),family=$('family').value;
  const source=mode==='clips'?data.motions:data.game_recordings.map((r,i)=>({...r,id:'game-'+i,title:r.label}));
  return source.filter(r=>(mode==='game'||r.videos.length)&&(!family||mode==='game'||r.family===family)&&[r.title,r.label,r.id].join(' ').toLowerCase().includes(query));
 }
 function position(q){return q.findIndex(r=>r.id===(motion?.id||selected?.id))}
 function update(){let q=rows(),i=position(q);$('previousClip').disabled=i<=0;$('nextClip').disabled=i<0||i>=q.length-1;$('queuePosition').textContent=q.length?(i<0?'Choose a clip':`${i+1} / ${q.length}`)+(mode==='requirements'?' in this requirement':' in this view'):'No recorded clips in this view';$('video').loop=!$('autoNext').checked;}
 function move(delta,resume=false){let q=rows(),i=position(q),entry=q[i+delta];if(i<0||!entry)return;saveIfDirty();if(mode==='requirements'){$('clipChoice').value=entry.id;selectMotion(entry)}else select(entry);update();if(resume)$('video').play().catch(()=>{});}
 const priorList=renderList;renderList=function(){priorList();update()};
 const priorMotion=selectMotion;selectMotion=function(m){priorMotion(m);update()};
 $('previousClip').onclick=()=>move(-1,!$('video').paused);
 $('nextClip').onclick=()=>move(1,!$('video').paused);
 $('autoNext').onchange=update;
 $('video').addEventListener('ended',()=>{if($('autoNext').checked)move(1,true)});
 update();
})();
