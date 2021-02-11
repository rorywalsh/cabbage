<Cabbage>
#define SLIDER_APPEARANCE trackercolour("DarkSlateGrey"), textcolour("black") 

form caption("PruebaSide") size(440,530), pluginid("FSR3") style("legacy"), sidechain(2)
image            bounds(  0,  0,440,130), outlinethickness(6), outlinecolour("white"), colour("grey")
rslider bounds( 80, 10, 70, 70), channel("att"), text("Attack"),  range(0,1,0.01,0.5), $SLIDER_APPEARANCE
rslider bounds(150, 10, 70, 70), channel("rel"), text("Release"), range(0,1,0.05,0.5), $SLIDER_APPEARANCE
rslider bounds(220, 10, 70, 70), channel("ratio"), text("Ratio"), range(1,300,10000,0.5), $SLIDER_APPEARANCE
rslider bounds(290, 10, 70, 70), channel("look"), text("Lookahead"), range(0,1,0.01,0.5), $SLIDER_APPEARANCE
rslider bounds(360, 10, 70, 70), channel("gain"), text("Gain"), range(-36,36,0), $SLIDER_APPEARANCE
hrange   bounds( 10, 80,420, 30), channel("LowKnee","HighKnee"), range(0, 120, 48:60), $SLIDER_APPEARANCE
label    bounds( 10,108,420, 13), text("Soft Knee"), fontcolour("black")
csoundoutput bounds(10, 130, 390, 300)

</Cabbage>

<CsoundSynthesizer>

<CsOptions>
-d -n
</CsOptions>

<CsInstruments>

;sr is set by the host
nchnls = 2 ;main outputs
nchnls_i = 4 ;main inputs(chan 1, chan 2) and sidechain(chan 3, chan4)
ksmps = 32
0dbfs = 1



instr 1
    print nchnls 
	print nchnls_i
    a1 inch 1               ;main in left
    a2 inch 2               ;main in right or sidechain input if logic is running in dual mono
    a3, a4 init 0           ;declare variables for sidechain inputs

    if nchnls_i == 3 then
        a3 inch 3           ;sidechain left
    endif
    if nchnls_i == 4 then
        a4 inch 4           ;sidechain right	
    endif

						; read in live audio
    kthresh = -1000000000				; read in widgets
    kLowKnee	chnget		"LowKnee"
    kHighKnee	chnget		"HighKnee"
    katt		chnget		"att"
    krel		chnget		"rel"
    kratio 	chnget		"ratio"
    kgain	 	chnget		"gain"
    klook 	= 0.00  						; look-ahead time (this will have to be cast as an i-time variable)
    klook		init		0.00							
    if changed(klook)==1 then								; if slider is moved...
    reinit REINIT										; ... reinit
    endif
    REINIT:

    if nchnls_i == 2 then
        aC_L 	compress a1, a2, kthresh, kLowKnee, kHighKnee, kratio, katt, krel, i(klook)	; compress both channels
        aC_R 	compress a1, a2, kthresh, kLowKnee, kHighKnee, kratio, katt, krel, i(klook)
    elseif nchnls_i == 4 then
        aC_L 	compress a1, a3, kthresh, kLowKnee, kHighKnee, kratio, katt, krel, i(klook)	; compress both channels
        aC_R 	compress a2, a4, kthresh, kLowKnee, kHighKnee, kratio, katt, krel, i(klook)
    endif
    
    aC_L	*=	ampdb(kgain)								; apply make up gain
    aC_R	*=	ampdb(kgain)

	if nchnls == 1 then
        outs	aC_L
	else
		outs aC_L, aC_R
	endif

    endin

</CsInstruments>

<CsScore>
i 1 0 [3600*24*7]									; play instr 1 for 1 week
</CsScore>

</CsoundSynthesizer>
