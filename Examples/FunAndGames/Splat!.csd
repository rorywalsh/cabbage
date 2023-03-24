
/* Attribution-NonCommercial-ShareAlike 4.0 International
Attribution - You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
NonCommercial - You may not use the material for commercial purposes.
ShareAlike - If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode */

; Splat.csd
; Written by Iain McCurdy, 2014.
; 
; Left click somewhere within the swarm to splat the flies. 
; Left click again somewhere within the field of splatted flies to unsplat them and bring them back to life.
; 
; Population    -    the number of flies in the swarm
; Liveliness    -    the speed with which flies will move about within the swarm
; Excursion    -    how far flies will move away from the centre of the swarm, therefore this also defines the area covered by the swarm.
; Fly Size    -    doesn't affect the physical size of flies but instead affects the central frequency of the buzzing tone they produce.
; Swarm Speed    -    the speed with which the swarm moves around the panel. The swarm can exit the panel completely.
; 
; If the swarm area (excursion) is very small it can become difficult to splat (and unsplat) it. 
; If this becomes a problem, simply raise the value for excursion, even while the swarm is splatted.

<Cabbage>
form caption("Splat!"), size(1100,530), colour(230,230,230), pluginId("Splt"), guiRefresh(10)
image bounds(0,   0, 0, 0), colour(white), shape(ellipse), widgetArray("fly",500)
nslider  bounds(  5,495,60,25), channel("Population"),    range(1,100, 30,1,1),       textColour(black), fontColour(black), text("Population"), colour(white)
nslider  bounds( 70,495,60,25), channel("Liveliness"),    range(0.1,10.00, 4,1,0.1),  textColour(black), fontColour(black), text("Liveliness"), colour(white)
nslider  bounds(135,495,60,25), channel("Excursion"),     range(2,300, 50,1,1),       textColour(black), fontColour(black), text("Excursion"),  colour(white)
nslider  bounds(200,495,60,25), channel("FlySize"),       range(2,12,8,1,0.001),     textColour(black), fontColour(black), text("Fly Size"),   colour(white)
nslider  bounds(265,495,80,25), channel("SwarmSpeed"),    range(0.1,10, 0.3,1,0.001), textColour(black), fontColour(black), text("Swarm Speed"),colour(white)
label    bounds(  0, 0,  0, 0), text("Click swarm to splat. Click again to unsplat."), align(centre), fontColour(white), identChannel("instructions1")        ; Instructions
label    bounds(350,519,100, 10), text("Iain McCurdy 2014"), align(left), fontColour(0,0,0,150)
</Cabbage>

<CsoundSynthesizer>

<CsOptions>
-n -dm0
</CsOptions>

<CsInstruments>

; sr set by host
ksmps = 32
nchnls = 2
0dbfs = 1
seed    0

giPanelWidth  =     1100
giPanelHeight =     530
gisine        ftgen 0, 0, 131072, 10, 1
giMaxFlies    =     500

; UDO that provide different portamento times whether a value is rising or falling
opcode    SwitchPort, k, kkk
 kin,kUpPort,kDnPort    xin
 kold        init   0
 kporttime   =      (kin<kold?kDnPort:kUpPort)
 kout        portk  kin, kporttime
 kold        =      kout
             xout   kout
endop



; UDO for an i-rate version of the 'scale' opcode
opcode    scale_i,i,iii
 ival,imax,imin    xin
 ival     =       (ival * (imax-imin)) + imin
          xout    ival
endop



; UDO for moving fly and creating its buzzing sound
opcode    Fly, aa, kkkkkkkip
 kSplat, kExcursion, kLiveliness, koct, kFlySize, kOffX, kOffY, iMaxFlies, iCount    xin

 if kSplat==0 then                                                         ; if flies are living...
  kDisp       rspline     -kExcursion, kExcursion, kLiveliness, kLiveliness * 2  ; variables for fly movement within the swarm
  kX          rspline     -kExcursion, kExcursion, kLiveliness, kLiveliness * 2  
  kY          rspline     -kExcursion, kExcursion, kLiveliness, kLiveliness * 2  
 ;kRotation    rspline     -360, 360, kLiveliness * 0.1, kLiveliness * 2 * 0.1  
 ;kX = cos(kRotation)*kDisp
 ;kY = sin(kRotation)*kDisp
 
  kDist       rspline     0.5, 1.5, 0.02, 0.03                                   ; z-plane distance of the fly. Really just a bit of size modulation.
 endif

 kamp         =           ((kY*0.5)/kExcursion) + 0.5                     ; amplitude of the buzzing sound varies according to the distance of the fly along the y-axis from the front of the swarm
 kamp         scale       limit:k(kamp,0,1), 1, 0.1                                    ; amplitude value scaled so that flies never audibly disappear entirely
 asig         vco2        0.01*kamp,cpsoct(koct)*semitone((kX+kY)*0.02),6 ; the buzzing sound: a pulse wave
 
 kpan         =           ((kX*0.5)/kExcursion) + 0.5                     ; buzzing sounds are panned according to their left/right position within the swarm
 aL,aR        pan2        asig,kpan                                       ; panned

 kFlySizeL    scale       (kFlySize-1)/11, 3, 0.5        ; this layer's fly size

 kRefreshTrig metro       ksmps,iCount/giMaxFlies                ; frame rate with which flies will be graphically moved. A different trigger (each with a different phase offset) for each fly so that all chnsets don't happen at the same time and cripple performance.
 if kRefreshTrig==1 &&    kSplat==0 then                    ; if a refresh trigger has occured...
     Smess    sprintfk    "bounds(%d,%d,%d,%d), colour(0,0,0,200)",kOffX+kX,kOffY+kY,kDist*kFlySizeL,kDist*kFlySizeL    ; generate message for fly widget
     Sident   sprintfk    "fly_ident%d", iCount                            ; generate ident string appropriate to this fly
              chnset      Smess, Sident                        ; update the fly
 endif

 aMixL        =           0
 aMixR        =           0

 if iCount<iMaxFlies then
  aMixL,aMixR Fly     kSplat,kExcursion,kLiveliness,koct,kFlySize,kOffX,kOffY,iMaxFlies,iCount+1
 endif
aL,aMixL,aR,aMixR init 0  
              xout       aL+aMixL, aR+aMixR

endop



instr    1
 kMOUSE_X          chnget   "MOUSE_X"              ; track mouse activity
 kMOUSE_Y          chnget   "MOUSE_Y"
 kMOUSE_DOWN_LEFT  chnget   "MOUSE_DOWN_LEFT"
 kMOUSE_DOWN_RIGHT chnget   "MOUSE_DOWN_RIGHT"

 kSplat            init     0                      ; splat status: 0 = alive, 1 = splatted
 
 aMixL,aMixR       init     0                      ; audio mix of all buzzing sounds (initialised)
 kLiveliness       chnget   "Liveliness"           ; speed with which flies move within the swarm 
 kExcursion        chnget   "Excursion"            ; range (in pixels) that individual flies might go from the the centre of the swarm
 kFlySize          chnget   "FlySize"              ; fly size (affects pitch of buzzing only)
 koct              scale    1-((kFlySize-1)/11), 12, 7  ; scale buzzing pitch (oct format)
 kporttime         linseg   0,0.001,0.05           ; portamento time rises from zero to a held value
 koct              portk    koct,kporttime         ; smooth chages to buzzing frequency
 kSwarmSpeed       chnget   "SwarmSpeed"           ; 

 if kSplat==0 then                    ; if flies are alive...
  gkPopulation     chnget   "Population"           ; read changes to population. This widget is only read while flies are alive so that splatted flies don't appear or disappear while splatted.
 endif
 
 if changed(gkPopulation)==1 then                  ; if a change to the number of flies is made by the user. (Changes made while flies are dead will only register once the flies are unsplatted.)
                   event    "i",10,0,0             ; call instrument that hides all active flies. This is done so to ensure that deactivated flies are no longer visible. Flies that should be active flies will be made visible immediately after in this instrument.
 endif
 
 if trigger(kSplat,0.5,0)==1 then                  ; if a successful splatting has been made...
                   event    "i",20,0,0             ; call the instrument that splats all active flies
 endif

 if kSplat==0 then                    ; if flies are living...
  kOffX            rspline  0,giPanelWidth,kSwarmSpeed,kSwarmSpeed*2        ; generate variables that move the entire swarm. i.e. the swarm should not move while all flies are splatted.
  kOffY            rspline  0,giPanelHeight,kSwarmSpeed,kSwarmSpeed*2
 endif
 
 ; if a left click is made within the boundaries of the swarm it will be alternately splatted or unsplatted as appropriate
 if trigger(kMOUSE_DOWN_LEFT,0.5,0)==1 && kMOUSE_X>=(kOffX-kExcursion) && kMOUSE_X<=(kOffX+kExcursion)  && kMOUSE_Y>=(kOffY-kExcursion) && kMOUSE_Y<=(kOffY+kExcursion) then
  kSplat           =        (kSplat==1?0:1)
 endif

 ; Call UDO for moving fly and creating buzzing sound
 if changed:k(gkPopulation)==1 then
                   reinit   ChangedPopulation
 endif
 ChangedPopulation:
 aMixL,aMixR       Fly      kSplat,kExcursion,kLiveliness,koct,kFlySize,kOffX,kOffY,i(gkPopulation)
 rireturn

 kenv              linseg   0,4,1    ; initial rise in amplitude
 aMixL             *=       SwitchPort((1-kSplat),0.5,0.001)*kenv    ; turn audio on an off according to splat status. When splatted audio is interrupted suddenly, when unsplatted audio rises more slowly.
 aMixR             *=       SwitchPort((1-kSplat),0.5,0.001)*kenv    ;
                   outs     aMixL,aMixR                              ; send audio to outputs
                   clear    aMixL,aMixR                              ; clear audio mix variables
endin

instr    10    ; hide all flies
 iCount            =        1
 while iCount<=giMaxFlies do
 Sid               sprintf  "fly_ident%d",iCount
                   chnset   "bounds(-100,-100,0,0), colour(0,0,0,0)",Sid
 iCount            +=       1
 od
endin


instr    20    ; graphically splat flies
                   event_i  "i",21,0,0.1    ; call instrument that produces the sound effect
 iCount    =    1
 while iCount<=giMaxFlies do
 isize             random   4,20                                                      ; splat size randomised
 ialpha            random   80,180                                                    ; alpha (transparency) randomised
 ired              random   30,150                                                    ; add in a random amount of red to give it a bit of gore
 Smess             sprintf  "size(%d,%d), colour(%d,0,0,%d)",isize,isize,ired,ialpha  ; create message for graphical splat
 Sid               sprintf  "fly_ident%d",iCount
                   chnset   Smess,Sid                                                 ; send message to the fly widget
 iCount            +=       1
 od
endin

instr    21    ; splat sound effect 
 idur              scale_i    rnd(1)^2,0.15,0.08           ; random duration
 p3                =          idur                         ; assign to p3
 imin              random     8,10                         ; minimum frequency for splat filter (in oct)
 anoise            dust2      10, 100 * gkPopulation       ; some crackly noise, the density of which is dependent upon the fly population
 kcf               random     cpsoct(imin),cpsoct(imin+3)  ; cutoff frequency is a random function moving to a new value every k-cycle
 anoise            moogladder anoise,kcf,0.7               ; filter the crackly noise using moogladder to give it a bit of squelch
 anoise            buthp      anoise,200                   ; highpass filter to remove some of the lower frequencies
 aenv              expon      1,p3,0.1                     ; amplitude envelope which will give the splat sound a percussive shape
 anoise            *=         aenv                         ; apply envelope
                   outch      1,anoise                     ; send this signal to the left channel
 
 anoise            dust2      10,1000                      ; repeat for the left channel. Doing both channels completely seperately create a nice stereo effect
 kcf               random     cpsoct(imin),cpsoct(imin+3)
 anoise            moogladder anoise,kcf,0.7
 anoise            buthp      anoise,200
 aenv              expon      1,p3,0.1
 anoise            *=         aenv
                   outch      2,anoise
endin




instr    1001    ; Print and then hide instructions
 kMOUSE_DOWN_LEFT  chnget     "MOUSE_DOWN_LEFT"        ; sense left click
 kEscapeTrig       trigger    kMOUSE_DOWN_LEFT,0.5,0   ; trigger of left click is pressed
 krel              release                             ; release flag - normally 0, 1 at note release
 kalpha            transeg    0,0.2,4,255,p3-0.4,0,255,0.2,-4,-1    ; colour alpha channel: fades in, holds, fades out

 if metro(ksmps)==1||krel==1 then                      ; peg updates or interrupt if mouse has been clicked
  Smess1           sprintfk   "bounds(%d,%d,400,17), fontColour(0,0,0,%d)", (giPanelWidth*0.5) - 200, (giPanelHeight*0.5)-10, kalpha*(1-krel)
  chnset           Smess1,    "instructions1"
 endif

 ktime             times                               ; note elapsed time in seconds
 if trigger(ktime,p3-0.2,0)==1||kEscapeTrig==1 then    ; if time is up mouse has been clicked...
                   event      "i",1,0,3600*24*7        ; start swarm
  turnoff
 endif
endin

</CsInstruments>  

<CsScore>
i 1001 0 3        ; Instructions fade up then down
f 0 z
</CsScore>

</CsoundSynthesizer>
