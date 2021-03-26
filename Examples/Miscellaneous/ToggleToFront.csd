<Cabbage>
form size(350, 240), caption("Toggle to Front"), pluginId("TToF")
label bounds(10, 10, 200, 100) text("1"), colour(255, 0, 0), identChannel("img0Ident")
label bounds(10, 10, 200, 100) text("2"), colour(255, 255, 0), identChannel("img1Ident")
label bounds(10, 10, 200, 100) text("3"), colour(255, 0, 255), identChannel("img2Ident")
label bounds(10, 10, 200, 100) text("4"), colour(0, 255, 0), identChannel("img3Ident")
button bounds(224, 14, 80, 40), channel("button1") text("Toggle to front")
</Cabbage>
<CsoundSynthesizer>
<CsOptions>
-n -d
</CsOptions>
<CsInstruments>
;sr is set by the host
ksmps = 64
nchnls = 2
0dbfs=1

instr 1
    kToggleIndex init 0
    kButton chnget "button1"

    if changed(kButton) == 1 then
        if kToggleIndex == 3 then
            chnset "toFront()", "img3Ident"
        elseif kToggleIndex == 2 then
            chnset "toFront()", "img2Ident"
        elseif kToggleIndex == 1 then
            chnset "toFront()", "img1Ident"
        elseif kToggleIndex == 0 then
            chnset "toFront()", "img0Ident"
        endif
        kToggleIndex = kToggleIndex<3 ? kToggleIndex+1 : 0
    endif
endin

</CsInstruments>
<CsScore>
i1 0 z
</CsScore>
