
/* Attribution-NonCommercial-ShareAlike 4.0 International
Attribution - You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
NonCommercial - You may not use the material for commercial purposes.
ShareAlike - If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode */

; FunctionGenerator.csd
; Written by Iain McCurdy, 2015
    
; GENERATOR section generates a function using one of a variety of methods. 
; (The range of values output by 'GENERATOR' are always within the range zero to 1.)

; TRANSFORM warps the function output by GENERATOR in one of a variety of ways.
; (Can also be bypassed.)

; RESCALE reScales the function to lie between the given limits.

; GENERATOR
; sine        -    sine wave LFO
; Triangle    -    triangle wave LFO
; Square    -    square wave LFO
; Pulse        -    pulse wave LFO.
;            'Width' controls the percentage of the pulse which is on/high.
;            N.B. the pulse wave can also be inverted by swapping using the reScale values
; Saw Up    -    upward sawtooth wave LFO
; Saw Dn    -    downward sawtooth wave LFO
; Randomi    -    interpolating random function generator
; Randomh    -    sample and hold random function generator. 'Dereg.' deregulates the rate at which new random values are generated.
; Rspline    -    random spline function generator
; Gauss.    -    gaussian noise. Probably to be used with the 'sample and hold' 'TRANSFORM' mechanism
; Exp.        -    exponential noise. Probably to be used with the 'sample and hold' 'TRANSFORM' mechanism
; Rand.Loop    -    loop of discrete random values.
;            'Number' sets the number of values in the loop
;            'Reset' triggers a new set of values 

; TRANSFORM
; Bypass    -    bypasses TRANSFORM completely 
; Port        -    adds portamento (a kind of lowpass filter). This will work most noticably with stepped function such as Randomh.
; Lineto    -    creates a straight line across the defined duration between stepped values.
;            This transformation needs a stepped GENERATOR, such as randomh, in order to function.
; Samp.Hold    -    applies a sample and hold function to the generated function, retriggered at the defined rate.
; Power Skew    -    skews the function using a 'power of' mathematical procedure.
;            'Power' values greater than '1' will skew the function increasingly to the middle of the range.
;            Values less than '1' will skew the function increasingly to the edges of the range.
;            With a 'Power' value of 1 no skewing will occur.
; Limit Skew    -    Skews the function in favour of the one or the other of the limits.
;            Values less than '1' will skew the function in favour of the lower limit.
;            Values greater than '1' will skew the function in favour of the upper limit.
; Gauss.Noise    -    Adds a defined amount of gaussian noise to the function.
; Dual Port.    -    Dual portamento in which a different portamento time can be defined for rising or falling values
; Quantise    -    Quantise the values of the function. 
;            Bear in mind that the function will normally lie within the range zero to '1'.
;            A value of zero disables this function.
        
<Cabbage>
form caption("MIDI Function Generator"), size(560,115), pluginId("FnGn")

image    bounds(  5,  5,150,105), colour("black"), shape("rounded"), outlineThickness("4"), outlineColour("white")
label    bounds( 10,  9,140, 11), text("G E N E R A T O R"), fontColour("WHITE")
combobox bounds( 30, 30,100, 20), channel("generator"), text("Sine","Triangle","Square","Pulse","Saw Up","Saw Down","Randomi","Randomh","Rspline","Gauss.Noise","Exp.Noise","Rand.Loop"), value(1)
rslider  bounds( 10, 55, 50, 50), channel("frq"), text("Freq."), range(0.01,100,1,0.375,0.01), identChannel("frqID")
rslider  bounds( 60, 55, 50, 50), channel("wid"), text("Width"), range(1,99,50,1,1), identChannel("widID"), visible(0)
rslider  bounds( 60, 55, 50, 50), channel("DeReg"), text("Dereg."), range(0,4,0,1,0.001), identChannel("DeRegID"), visible(0)
rslider  bounds( 10, 55, 50, 50), channel("frq1"), text("Freq.1"), range(0.01,100,1,0.5,0.01), identChannel("frq1ID"), visible(0)
rslider  bounds( 60, 55, 50, 50), channel("frq2"), text("Freq.2"), range(0.01,100,1,0.5,0.01), identChannel("frq2ID"), visible(0)
rslider  bounds( 10, 55, 50, 50), channel("lambda"), text("Lambda"), range(0.01,1,0.125,0.5,0.001), identChannel("lambdaID"), visible(0)
combobox bounds( 56, 60, 45, 20), channel("number"), text("2","3","4","5","6","7","8","9","10","11","12","13","14","15","16"), value(7), identChannel("numberID"), visible(0)
label    bounds( 56, 80, 45, 12), text("Number"), identChannel("numberID2"), visible(0)
button   bounds(108, 60, 42, 23), text("Reset","Reset"), channel("reset"), latched(0), identChannel("resetID"), visible(0)

image    bounds(160,  5,130,105), colour("black"), shape("rounded"), outlineThickness("4"), outlineColour("white"), plant("transform") {
label    bounds(  5,  9,120, 11), text("T R A N S F O R M"), fontColour("WHITE")
combobox bounds( 15, 30,100, 20), channel("transform"), text("Bypass","Port","Lineto","Samp.Hold","Power Skew","Limit Skew","Gauss.Noise","Dual Port.","Quantise"), value(1)
rslider  bounds( 40, 55, 50, 50), channel("time"), text("Time"), range(0.002,1,0.1,0.5,0.001), identChannel("timeID"), visible(0)
rslider  bounds( 15, 55, 50, 50), channel("timeUp"), text("Time Up"), range(0.002,1,0.003,0.5,0.001), identChannel("timeUpID"), visible(0)
rslider  bounds( 65, 55, 50, 50), channel("timeDn"), text("Time Dn."), range(0.002,1,0.1,0.5,0.001), identChannel("timeDnID"), visible(0)
rslider  bounds( 40, 55, 50, 50), channel("rate"), text("Rate"), range(0.1,50,3,0.5,0.001), identChannel("rateID"), visible(0)
rslider  bounds( 40, 55, 50, 50), channel("power"), text("Power"), range(0.01,20,3,0.5,0.01), identChannel("powerID"), visible(0)
rslider  bounds( 40, 55, 50, 50), channel("amount"), text("Amount"), range(0,1,0.1,0.5,0.01), identChannel("amountID"), visible(0)
rslider  bounds( 40, 55, 50, 50), channel("LimSkew"), text("Amount"), range(0.1,20,1,0.5,0.01), identChannel("LimSkewID"), visible(0)
rslider  bounds( 40, 55, 50, 50), channel("QuantVal"), text("Value"), range(0,1,0.1,1,0.01), identChannel("QuantValID"), visible(0)
}

image    bounds(295,  5,130,105), colour("black"), shape("rounded"), outlineThickness("4"), outlineColour("white"), plant("scale") {
label    bounds(  5,  9,120, 11), text("S C A L E"), fontColour("WHITE")
rslider  bounds( 10, 25, 55, 55), channel("ScaleMin"), text("Min"), range(0,127,0,  1,1)
rslider  bounds( 65, 25, 55, 55), channel("ScaleMax"), text("Max"), range(0,127,127,1,1)
checkbox bounds( 20, 83, 70, 12), channel("ScaleInt"), text("Integers")

}

image    bounds(430,  5,130,105), colour("black"), shape("rounded"), outlineThickness("4"), outlineColour("white"), plant("output") {
label    bounds(  5,  9,120, 11), text("O U T P U T"), fontColour("WHITE")
label    bounds( 25, 22, 80, 12), text("Channel"), fontColour("WHITE")
combobox bounds( 25, 35, 80, 20), channel("channel"), text("1","2","3","4","5","6","7","8","9","10","11","12","13","14","15","16"), value(1)
label    bounds( 25, 57, 80, 12), text("Controller"), fontColour("WHITE")
combobox bounds( 25, 70, 80, 20), channel("controller"), text("1","2","3","4","5","6","7","8","9","10","11","12","13","14","15","16","17","18","19","20","21","22","23","24","25","26","27","28","29","30","31","32","33","34","35","36","37","38","39","40","41","42","43","44","45","46","47","48","49","50","51","52","53","54","55","56","57","58","59","60","61","62","63","64","65","66","67","68","69","70","71","72","73","74","75","76","77","78","79","80","81","82","83","84","85","86","87","88","89","90","91","92","93","94","95","96","97","98","99","100","101","102","103","104","105","106","107","108","109","110","111","112","113","114","115","116","117","118","119","120","121","122","123","124","125","126","127"), value(1)
}
</Cabbage>
                    
<CsoundSynthesizer>

<CsOptions>   
-dm0 -n -+rtmidi=NULL -Q0
</CsOptions>

<CsInstruments>

; sr set by host
ksmps     =    32
nchnls     =    2
0dbfs    =    1

ginoise    ftgen    0,0,1024,21,1,1

opcode    lineto2,k,kk
 kinput,ktime    xin
 ktrig    changed    kinput,ktime                ; reset trigger
 if ktrig==1 then                    ; if new note has been received or if portamento time has been changed...
  reinit RESTART
 endif
 RESTART:                        ; restart 'linseg' envelope
 if i(ktime)==0 then                    ; 'linseg' fails if duration is zero...
  koutput    =    i(kinput)            ; ...in which case output simply equals input
 else
  koutput    linseg    i(koutput),i(ktime),i(kinput)    ; linseg envelope from old value to new value
 endif
 rireturn
         xout    koutput
endop

opcode TriggerToGatek,k,kki
 ktrig,kdur,imax xin
 kdlytrig  vdelayk    ktrig,kdur,imax
 kgate       samphold    ktrig,ktrig + kdlytrig
           xout         kgate
endop

opcode    SwitchPortk, k, kkk
    kin,kupport,kdnport    xin
    kold            init    0
    kporttime        =    (kin<kold?kdnport:kupport)
    kout            portk    kin, kporttime
    kold            =    kout
                xout    kout
endop








instr    1
 ksmooth    linseg    0,0.001,0.05
 
 ; GENERATOR
 kgenerator    chnget    "generator"

 ; show/hide
 if changed(kgenerator)==1 then        ; initially hide all
      chnset    "visible(0)","frqID"
      chnset    "visible(0)","DeRegID"
      chnset    "visible(0)","widID"
      chnset    "visible(0)","frq1ID"
      chnset    "visible(0)","frq2ID"
      chnset    "visible(0)","lambdaID"
      chnset    "visible(0)","numberID"
      chnset    "visible(0)","numberID2"
      chnset    "visible(0)","resetID"
  if kgenerator==1 then            ; sine
      chnset    "visible(1)","frqID"
  elseif kgenerator==2 then        ; triangle
      chnset    "visible(1)","frqID"
  elseif kgenerator==3 then        ; square
      chnset    "visible(1)","frqID"
  elseif kgenerator==4 then        ; pulse
      chnset    "visible(1)","frqID"
      chnset    "visible(1)","widID"
  elseif kgenerator==5 then        ; saw up
      chnset    "visible(1)","frqID"
  elseif kgenerator==6 then        ; saw down
      chnset    "visible(1)","frqID"
  elseif kgenerator==7 then        ; randomi
      chnset    "visible(1)","frqID"
      chnset    "visible(1)","DeRegID"
  elseif kgenerator==8 then        ; randomh
      chnset    "visible(1)","frqID"
      chnset    "visible(1)","DeRegID"
  elseif kgenerator==9 then        ; rspline
      chnset    "visible(1)","frq1ID"
      chnset    "visible(1)","frq2ID"
  elseif kgenerator==11 then        ; exponential
      chnset    "visible(1)","lambdaID"  
  elseif kgenerator==12 then        ; random loop
      chnset    "visible(1)","frqID"
      chnset    "visible(1)","numberID"
      chnset    "visible(1)","numberID2"
      chnset    "visible(1)","resetID"
  endif
 endif 

 kfrq    chnget    "frq"
 kwid    chnget    "wid"
 kfrq1    chnget    "frq1"
 kfrq2    chnget    "frq2"
 klambda    chnget    "lambda"
 
 if kgenerator==1 then
 kfn    lfo    0.5,kfrq,0    ; sine
 kfn    =    kfn + 0.5

 elseif kgenerator==2 then
  kfn    lfo    0.5,kfrq,1    ; tri
  kfn    =    kfn + 0.5

 elseif kgenerator==3 then
  kfn    lfo    1,kfrq,3    ; sq

 elseif kgenerator==4 then
  kpls    metro    kfrq        ; pulse
  kfn    TriggerToGatek    kpls,(kwid*0.01)/kfrq,1/0.01
 
 elseif kgenerator==5 then
  kfn    lfo    1,kfrq,4    ; saw up

 elseif kgenerator==6 then
  kfn    lfo    1,kfrq,5    ; saw down

 elseif kgenerator==7 then
  kDeReg    chnget    "DeReg"
  if kDeReg>0 then
   krate    trandom    metro(kfrq),-kDeReg,kDeReg
   krate    =    kfrq * octave(krate)
  else
   krate    =    kfrq
  endif
  kfn    randomi    0,1,kfrq,1    ; randomi

 elseif kgenerator==8 then
  kDeReg    chnget    "DeReg"
  if kDeReg>0 then
   krate    trandom    metro(kfrq),-kDeReg,kDeReg
   krate    =    kfrq * octave(krate)
  else
   krate    =    kfrq
  endif
  kfn    randomh    0,1,krate,1    ; randomh

 elseif kgenerator==9 then
  kfn    rspline    0,1,kfrq1,kfrq2    ; rspline

 elseif kgenerator==10 then
  kfn    gauss    0.5        ; gauss
  kfn    limit    kfn + 0.5, 0, 1

 elseif kgenerator==11 then
  kfn    exprand    klambda        ; exp
  kfn    limit    kfn, 0, 1

 elseif kgenerator==12 then    ; random loop
  kphs    phasor    kfrq
  kreset    chnget    "reset"
  if changed(kfn)==1 then
   kRandLoopTrig    trigger    kreset,0.5,0
  endif
  kOS    trandom    kRandLoopTrig,0,1024
  knumber    chnget    "number"
  kfn    table    int(kOS) + (kphs*(knumber+1)), ginoise

 endif


 













 ; TRANSFORM
 ktransform    chnget    "transform"

 ; show/hide
 if changed(ktransform)==1 then        ; initially hide all
      chnset    "visible(0)","timeID"
      chnset    "visible(0)","timeUpID"
      chnset    "visible(0)","timeDnID"
      chnset    "visible(0)","rateID"
      chnset    "visible(0)","powerID"
      chnset    "visible(0)","LimSkewID"
      chnset    "visible(0)","amountID"
      chnset    "visible(0)","QuantValID"
  if ktransform==2 then                ; portk
      chnset    "visible(1)","timeID"
  elseif ktransform==3 then            ; lineto
      chnset    "visible(1)","timeID"
  elseif ktransform==4 then            ; sample and hold
      chnset    "visible(1)","rateID"
  elseif ktransform==5 then            ; power skew
      chnset    "visible(1)","powerID"
  elseif ktransform==6 then            ; limit skew
      chnset    "visible(1)","LimSkewID"
  elseif ktransform==7 then            ; add gaussian noise
      chnset    "visible(1)","amountID"
  elseif ktransform==8 then            ; dual portamento
      chnset    "visible(1)","timeUpID"
      chnset    "visible(1)","timeDnID"
  elseif ktransform==9 then            ; quantise
      chnset    "visible(1)","QuantValID"
  endif
 endif

 kporttime    linseg    0,0.001,1
 ktime        chnget    "time"
 krate        chnget    "rate"
 kporttime    =    kporttime * ktime
 
 if ktransform==2 then            ; port
  kfn    portk    kfn,kporttime
 
 elseif ktransform==3 then        ; lineto
  kfn    lineto    kfn,kporttime
 
 elseif ktransform==4 then        ; sample and hold
  ktrig    metro    krate
  kfn    samphold    kfn,ktrig
 
 elseif ktransform==5 then        ; power skew
  kpower    chnget    "power"
  kpower    portk    kpower, ksmooth
  kval    pow    abs((kfn*2)-1),kpower
  kfn    =    kfn<=0.5 ? (1-kval)*0.5 : (1+kval)*0.5

 elseif ktransform==6 then        ; limit skew
  kLimSkew    chnget    "LimSkew"
  kLimSkew    portk    kLimSkew, ksmooth
  kfn    pow    kfn,kLimSkew

 elseif ktransform==7 then        ; add gaussian noise
  kamount    chnget    "amount"
  knse    gauss    kamount    
  kfn    mirror    kfn + knse, 0, 1

 elseif ktransform==8 then        ; dual portamento
  ktimeUp    chnget    "timeUp"
  ktimeDn    chnget    "timeDn"
  kfn    SwitchPortk    kfn, ktimeUp, ktimeDn

 elseif ktransform==9 then        ; quantise
  kQuantVal    chnget    "QuantVal"
  if kQuantVal>0 then
   kfn        =    round((kfn/kQuantVal)-(kQuantVal*0.5))*kQuantVal
  endif
 endif
 
 
; SCALE
 gkScaleMin     chnget    "ScaleMin"
 gkScaleMax     chnget    "ScaleMax"
 gkScaleInt        chnget    "ScaleInt"
 kfn            scale    kfn,gkScaleMax,gkScaleMin
 kfn            =        gkScaleInt==1?int(kfn):kfn
 
; OUTPUT 
 kchannel    chnget    "channel"
 kcontroller    chnget    "controller"
         outkc     kchannel,kcontroller, kfn, 0, 1

; printk 1, kfn
 
 gkfn    =    kfn            ; OUTPUT FUNCTION. GLOBAL VARIABLE FOR USE IN instr 3

endin








instr    3                ; AN AUDIO REPRESENTATION OF THE FUNCTION
 asig    poscil    0.1,cpsmidinn((gkfn*12)+60)
     outs    asig,asig
endin


</CsInstruments>

<CsScore>
i 1 0 3600    ; CREATE, TRANSFORM AND RESCALE A FUNCTION
;i 3 0 3600    ; PLAY AN AUDIO REPRESENTATION OF THE FUNCTION
</CsScore>

</CsoundSynthesizer>
