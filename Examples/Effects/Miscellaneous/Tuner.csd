
/* Attribution-NonCommercial-ShareAlike 4.0 International
Attribution - You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
NonCommercial - You may not use the material for commercial purposes.
ShareAlike - If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode */

; Tuner.csd
; Written by Iain McCurdy, 2017.

<Cabbage>
form caption("Tuner"), size(295,100), pluginId("Tunr")
image            bounds(0,0,295,100), colour(50,50,80), outlineThickness(4), outlineColour("silver")
image bounds(10,10,290,85), colour(0,0,0,0), plant("Indicators") {
checkbox bounds(  0, 16,  9,  9), colour(255,  5,  5), channel("Err-50"), shape("ellipse"), value(0), active(0)
checkbox bounds( 14, 14, 13, 13), colour(255, 55, 55), channel("Err-40"), shape("ellipse"), value(0), active(0)
checkbox bounds( 32, 12, 17, 17), colour(255,105,105), channel("Err-30"), shape("ellipse"), value(0), active(0)
checkbox bounds( 54,  9, 22, 22), colour(255,155,155), channel("Err-20"), shape("ellipse"), value(0), active(0)
checkbox bounds( 82,  5, 30, 30), colour(255,205,205), channel("Err-10"), shape("ellipse"), value(0), active(0)
checkbox bounds(117,  0, 40, 40), colour(255,255,255), channel("Err0"),   shape("ellipse"), value(0), active(0)
checkbox bounds(162,  5, 30, 30), colour(255,205,205), channel("Err10"),  shape("ellipse"), value(0), active(0)
checkbox bounds(197,  9, 22, 22), colour(255,155,155), channel("Err20"),  shape("ellipse"), value(0), active(0)
checkbox bounds(224, 12, 17, 17), colour(205,105,105), channel("Err30"),  shape("ellipse"), value(0), active(0)
checkbox bounds(246, 14, 13, 13), colour(255, 55, 55), channel("Err40"),  shape("ellipse"), value(0), active(0)
checkbox bounds(264, 16,  9,  9), colour(255,  5,  5), channel("Err50"),  shape("ellipse"), value(0), active(0)
image    bounds( 91, 48,120, 34), colour(0,0,0), outlineThickness(2), outlineColour("silver")
label    bounds(107, 50, 60, 30), text("---"), identChannel("NoteNameID"), fontColour("white")
label    bounds(157, 58, 60, 20), text("---"), identChannel("ErrorID"), fontColour("yellow")
checkbox bounds( 15, 58, 60, 14), text("mute"), colour:0( 55,255, 55), colour:1( 55,255, 55), channel("mute"), value(1), fontColour("white")
}
</Cabbage>

<CsoundSynthesizer>

<CsOptions>   
-n -dm0
</CsOptions>

<CsInstruments>

; sr set by host
ksmps = 16
nchnls = 2
0dbfs = 1

giTimeDelay        =    0.3        ; delay between printings
giThreshHold    =    -30        ; decibel threshold beneath which printings will be skipped
giFreqThresh    =    20        ; freq. threshold

opcode NNToStr,S,i
 iNote    xin
 iNote    =    round(iNote)
 iOct    =    int(iNote/12) - 1
 iDeg    =    iNote % 12
 if iDeg==0 then
  Sdeg    =    "C"
 elseif iDeg==1 then
  Sdeg    =    "C#"
 elseif iDeg==2 then
  Sdeg    =    "D"
 elseif iDeg==3 then
  Sdeg    =    "D#"
 elseif iDeg==4 then
  Sdeg    =    "E"
 elseif iDeg==5 then
  Sdeg    =    "F"
 elseif iDeg==6 then
  Sdeg    =    "F#"
 elseif iDeg==7 then
  Sdeg    =    "G"
 elseif iDeg==8 then
  Sdeg    =    "G#"
 elseif iDeg==9 then
  Sdeg    =    "A"
 elseif iDeg==10 then
  Sdeg    =    "A#"
 elseif iDeg==11 then
  Sdeg    =    "B"
 endif
 Sname    sprintf    "%s%d",Sdeg,iOct
         xout    Sname
endop

alwayson    1
instr        1

 /*
 kNote        rspline            36,72,0.05,0.08
 aL        vco2            0.4,cpsmidinn(kNote),4,0.5
 aR        =                aL
 ;        outs            aL,aR
 */
 
 aL,aR        ins

 /*
 icps        =            261.7
 aImp        mpulse        1,0
 aBuff        delayr        1/icps
 aL            deltapi        1/icps
             delayw        aImp+(aL*0.9999)
 aR            =            aL
              outs        aL,aR
 */
 
 /*
 kthresh    =            0.003
 fSig        pvsanal        aL,2048,512,2048,1
 kfr,kamp    pvspitch    fSig,kthresh
 */
 
 ;aL poscil 0.1, 440
 
 kfr, kamp     ptrack         aL, 512
 knum        =            (octcps(kfr)-3) * 12    ; convert oct format to MIDI note number format
 kround        =            round(knum)                ; round note numbers to the nearest integer
 kdiff        =            knum     - kround         ; difference between precise note number (including fraction) and rounded integer will be value (in semitones) by which to scale input to achieve chromatic pitch correction
 
 kfr        mediank        kfr, 111, 111
 kfr        portk       kfr, 0.1
 
 kPrintTrig    =    kamp > giThreshHold ? 1 : 0
 if metro:k(1/giTimeDelay)==1 then
  reinit PRINT_INFO
 endif
 PRINT_INFO:
 Snote    NNToStr        i(kround)
 ;Sinfo    sprintf        "Detected: %s, %d, %dHz",Snote,i(kdiff)*100,i(kfr)
 ;puts        Sinfo,kPrintTrig
 rireturn
 
 ;iUniTri    ftgen    0,0,4096,-7,-1,2048,1,2048,-1
 ;kdiff        poscil    55,.1,iUniTri
 kdiffInt    =    round(kdiff*10)
; printk2    kdiff
 if changed(kdiffInt)==1 then
  if kdiffInt==-5 then
   chnset    k(1), "Err-50"
   chnset    k(0), "Err-40"
   chnset    k(0), "Err-30"
   chnset    k(0), "Err-20"
   chnset    k(0), "Err-10"
   chnset    k(0), "Err0"
   chnset    k(0), "Err10"
   chnset    k(0), "Err20"
   chnset    k(0), "Err30"
   chnset    k(0), "Err40"
   chnset    k(0), "Err50"
  elseif kdiffInt==-4 then
   chnset    k(0), "Err-50"
   chnset    k(1), "Err-40"
   chnset    k(0), "Err-30"
   chnset    k(0), "Err-20"
   chnset    k(0), "Err-10"
   chnset    k(0), "Err0"
   chnset    k(0), "Err10"
   chnset    k(0), "Err20"
   chnset    k(0), "Err30"
   chnset    k(0), "Err40"
   chnset    k(0), "Err50"
  elseif kdiffInt==-3 then
   chnset    k(0), "Err-50"
   chnset    k(0), "Err-40"
   chnset    k(1), "Err-30"
   chnset    k(0), "Err-20"
   chnset    k(0), "Err-10"
   chnset    k(0), "Err0"
   chnset    k(0), "Err10"
   chnset    k(0), "Err20"
   chnset    k(0), "Err30"
   chnset    k(0), "Err40"
   chnset    k(0), "Err50"
  elseif kdiffInt==-2 then
   chnset    k(0), "Err-50"
   chnset    k(0), "Err-40"
   chnset    k(0), "Err-30"
   chnset    k(1), "Err-20"
   chnset    k(0), "Err-10"
   chnset    k(0), "Err0"
   chnset    k(0), "Err10"
   chnset    k(0), "Err20"
   chnset    k(0), "Err30"
   chnset    k(0), "Err40"
   chnset    k(0), "Err50"
  elseif kdiffInt==-1 then
   chnset    k(0), "Err-50"
   chnset    k(0), "Err-40"
   chnset    k(0), "Err-30"
   chnset    k(0), "Err-20"
   chnset    k(1), "Err-10"
   chnset    k(0), "Err0"
   chnset    k(0), "Err10"
   chnset    k(0), "Err20"
   chnset    k(0), "Err30"
   chnset    k(0), "Err40"
   chnset    k(0), "Err50"
  elseif kdiffInt==0 then
   chnset    k(0), "Err-50"
   chnset    k(0), "Err-40"
   chnset    k(0), "Err-30"
   chnset    k(0), "Err-20"
   chnset    k(0), "Err-10"
   chnset    k(1), "Err0"
   chnset    k(0), "Err10"
   chnset    k(0), "Err20"
   chnset    k(0), "Err30"
   chnset    k(0), "Err40"
   chnset    k(0), "Err50"
  elseif kdiffInt==1 then
   chnset    k(0), "Err-50"
   chnset    k(0), "Err-40"
   chnset    k(0), "Err-30"
   chnset    k(0), "Err-20"
   chnset    k(0), "Err-10"
   chnset    k(0), "Err0"
   chnset    k(1), "Err10"
   chnset    k(0), "Err20"
   chnset    k(0), "Err30"
   chnset    k(0), "Err40"
   chnset    k(0), "Err50"
  elseif kdiffInt==2 then
   chnset    k(0), "Err-50"
   chnset    k(0), "Err-40"
   chnset    k(0), "Err-30"
   chnset    k(0), "Err-20"
   chnset    k(0), "Err-10"
   chnset    k(0), "Err0"
   chnset    k(0), "Err10"
   chnset    k(1), "Err20"
   chnset    k(0), "Err30"
   chnset    k(0), "Err40"
   chnset    k(0), "Err50"
  elseif kdiffInt==3 then
   chnset    k(0), "Err-50"
   chnset    k(0), "Err-40"
   chnset    k(0), "Err-30"
   chnset    k(0), "Err-20"
   chnset    k(0), "Err-10"
   chnset    k(0), "Err0"
   chnset    k(0), "Err10"
   chnset    k(0), "Err20"
   chnset    k(1), "Err30"
   chnset    k(0), "Err40"
   chnset    k(0), "Err50"
  elseif kdiffInt==4 then
   chnset    k(0), "Err-50"
   chnset    k(0), "Err-40"
   chnset    k(0), "Err-30"
   chnset    k(0), "Err-20"
   chnset    k(0), "Err-10"
   chnset    k(0), "Err0"
   chnset    k(0), "Err10"
   chnset    k(0), "Err20"
   chnset    k(0), "Err30"
   chnset    k(1), "Err40"
   chnset    k(0), "Err50"
  elseif kdiffInt==5 then
   chnset    k(0), "Err-50"
   chnset    k(0), "Err-40"
   chnset    k(0), "Err-30"
   chnset    k(0), "Err-20"
   chnset    k(0), "Err-10"
   chnset    k(0), "Err0"
   chnset    k(0), "Err10"
   chnset    k(0), "Err20"
   chnset    k(0), "Err30"
   chnset    k(0), "Err40"
   chnset    k(1), "Err50"
  endif
 endif

 if metro(16)==1 then
  Sstr    sprintfk    "text(%d)",kdiff*100
  chnset        Sstr,"ErrorID"
 endif
   
 if changed(Snote)==1 then
  Sstr    sprintfk    "text(%s)",Snote
  chnset        Sstr,"NoteNameID"
 endif 
 
 if kfr<giFreqThresh || kamp<giThreshHold then
  chnset    k(0), "Err-50"
  chnset    k(0), "Err-40"
  chnset    k(0), "Err-30"
  chnset    k(0), "Err-20"
  chnset    k(0), "Err-10"
  chnset    k(0), "Err0"
  chnset    k(0), "Err10"
  chnset    k(0), "Err20"
  chnset    k(0), "Err30"
  chnset    k(0), "Err40"
  chnset    k(0), "Err50"
  chnset    "---","NoteNameID"
  chnset    "---","ErrorID"
 endif
 
 kmute    =    1-chnget:k("mute")
         outs    aL*kmute, aR*kmute
endin

</CsInstruments>

<CsScore>
i 1 0 3600
</CsScore>

</CsoundSynthesizer>