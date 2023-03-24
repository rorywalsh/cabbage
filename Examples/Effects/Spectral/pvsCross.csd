
/* Attribution-NonCommercial-ShareAlike 4.0 International
Attribution - You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
NonCommercial - You may not use the material for commercial purposes.
ShareAlike - If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode */

; pvsCross.csd
; Written by Iain McCurdy, 2012.

; This effect implements cross-synthesis between the left and right inputs.
; By default, the left input is the 'source' signal and the right signal is the 'destination' signal.
;  This arrangement can be swapped with the swap button.

; LEFT----+
;         |
;      (SOURCE)                 +---LEFT
;         |                     |
;         +-CROSS-SYNTHESIS-----+
;         |                     |
;    (DESTINATION)              +---RIGHT
;         |
; RIGHT---+


; Source    -    level of the source signal sent to the output.
; Dest.        -    level of the destination signal sent to the output.
; Cross        -    level of the cross-synthesis signal sent to the output.
; Level        -    global control of the level of all signal that may be present at the output.

<Cabbage>
form caption("pvsCross") size(370, 90), pluginId("crss"), guiMode("queue")
image            bounds(0, 0, 370, 90), colour("Cream"), outlineColour("silver"), outlineThickness(5)

; filter switches
checkbox bounds( 10, 20, 94, 15), text("Swap Inputs"), channel("swap"), fontColour:0(138, 54, 15), fontColour:1(138, 54, 15), colour("yellow")

label    bounds(15, 41, 60,13), text("FFT Size"), fontColour(138, 54, 15)
combobox bounds(15, 55, 60,20), text("128","256","512","1024","2048","4096","8192","16384"), channel("FFT"), value(4)

line     bounds(115, 10,165, 3), colour("Grey")

label   bounds(160,  5, 75, 12), text("M  I  X  E  R"), fontColour(78, 24,  7), colour("Cream")
rslider bounds(100,18, 75, 65), text("Source"),     channel("src_lev"),   range(0, 1.00, 0),   textColour(138, 54, 15), colour("chocolate"), trackerColour(138, 54, 15)
rslider bounds(160,18, 75, 65), text("Dest."),      channel("dst_lev"),   range(0, 1.00, 0),   textColour(138, 54, 15), colour("chocolate"), trackerColour(138, 54, 15)
rslider bounds(220,18, 75, 65), text("Cross"),      channel("cross_lev"), range(0, 1.00, 1),   textColour(138, 54, 15), colour("chocolate"), trackerColour(138, 54, 15)
rslider bounds(285, 8, 75, 75), text("Level"),      channel("lev"),       range(0, 1.00, 0.5), textColour(138, 54, 15), colour("chocolate"), trackerColour(138, 54, 15)
</Cabbage>
<CsoundSynthesizer>

<CsOptions>
-d -n
</CsOptions>

<CsInstruments>

; sr set by host
ksmps       =    64
nchnls      =    2
0dbfs       =    1    ;MAXIMUM AMPLITUDE

;Iain McCurdy
;http://iainmccurdy.org/csound.html

opcode    pvscross_module,a,aakkki
    a_src,a_dst,kampS,kampD,klev,iFFT    xin
    print iFFT
    iOLap      limit      iFFT/4, 64, 4096                   ; LIMIT OVERLAPS TO PREVENT CRASH IF LESS THAN 64
    f_src      pvsanal    a_src, iFFT, iOLap, iFFT, 1        ; ANALYSE AUDIO INPUT SIGNAL AND OUTPUT AN FSIG
    f_dst      pvsanal    a_dst, iFFT, iOLap, iFFT, 1        ; ANALYSE AUDIO INPUT SIGNAL AND OUTPUT AN FSIG
    f_cross    pvscross   f_src, f_dst, kampS, kampD         ; IMPLEMENT fsig CROSS SYNTHESIS
    aout       pvsynth    f_cross                            ; RESYNTHESIZE THE f-SIGNAL AS AN AUDIO SIGNAL
               xout       aout*klev    
endop



instr    1

    kfeedback     cabbageGetValue    "FB"
    kampS         =                  0
    kampD         =                  1
    kporttime     linseg             0,0.001,0.02
    kmix          cabbageGetValue    "mix"
    klev          cabbageGetValue    "lev"
    ksrc_lev      cabbageGetValue    "src_lev"
    kdst_lev      cabbageGetValue    "dst_lev"
    kcross_lev    cabbageGetValue    "cross_lev"
    klev          cabbageGetValue    "lev"
    klisten       cabbageGetValue    "listen"
    kswap         cabbageGetValue    "swap"
    
    a1            inch               1
    a2            inch               2

    ; FOR TESTING
    ;a1            vco2      0.2, 330
    ;a2            pinker

    if kswap==0 then
     a_src        =                  a1
     a_dst        =                  a2
    else
     a_src        =                  a2
     a_dst        =                  a1
    endif
    
    kFFT          cabbageGetValue    "FFT"
    kFFT          init      4
    if changed:k(kFFT)==1 then
     reinit update
    endif
    update:        
    aoutL         pvscross_module    a_src,a_dst,kampS,kampD,klev,2^(i(kFFT)+6)
    aoutR         pvscross_module    a_src,a_dst,kampS,kampD,klev,2^(i(kFFT)+6)

    aMixL         =                  (aoutL*kcross_lev)+(a_src*ksrc_lev)+(a_dst*kdst_lev)
    aMixR         =                  (aoutR*kcross_lev)+(a_src*ksrc_lev)+(a_dst*kdst_lev)

                  outs               aMixL,aMixR
endin

</CsInstruments>

<CsScore>
i 1 0 z
</CsScore>

</CsoundSynthesizer>