; MouseSense.csd
; Written by Iain McCurdy, 2014

<Cabbage>
form caption("Mouse Sense") size(785, 400), pluginID("SMo1"), guirefresh(16)

numberbox bounds( 55, 25, 90, 35), text("MOUSE X"),             fontcolour("white"), textbox(1),                channel("X"),           range(0, 800, 0,1,1)
numberbox bounds(160, 25, 90, 35), text("MOUSE Y"),             fontcolour("white"), textbox(1),                channel("Y"),           range(0, 400, 0,1,1)
numberbox bounds(  5, 70, 90, 35), text("MOUSE LEFT"),          fontcolour("white"), textbox(1),                channel("LEFT"),        range(0, 1,   0,1,1)
numberbox bounds(105, 70, 90, 35), text("MOUSE MIDDLE"),        fontcolour("white"), textbox(1),                channel("MIDDLE"),      range(0, 1,   0,1,1)
numberbox bounds(205, 70, 90, 35), text("MOUSE RIGHT"),         fontcolour("white"), textbox(1),                channel("RIGHT"),       range(0, 1,   0,1,1)


textbox bounds(300, 10,475,350), file("MouseSenseInfo.txt")

</Cabbage>

<CsoundSynthesizer>

<CsOptions>
-d -n
</CsOptions>

<CsInstruments>

sr              =       44100   ;SAMPLE RATE
ksmps           =       32      ;NUMBER OF AUDIO SAMPLES IN EACH CONTROL CYCLE
nchnls          =       2       ;NUMBER OF CHANNELS (2=STEREO)
0dbfs           =       1

instr   1
 kMOUSE_X               chnget  "MOUSE_X"
 kMOUSE_Y               chnget  "MOUSE_Y"
 kMOUSE_DOWN_LEFT       chnget  "MOUSE_DOWN_LEFT"
 kMOUSE_DOWN_MIDDLE     chnget  "MOUSE_DOWN_MIDDLE"
 kMOUSE_DOWN_RIGHT      chnget  "MOUSE_DOWN_RIGHT"

 kmetro	metro	20
 
 if kmetro==1 then
  chnset        kMOUSE_X, "X"
  chnset        kMOUSE_Y, "Y"
  chnset        kMOUSE_DOWN_LEFT, "LEFT"
  chnset        kMOUSE_DOWN_MIDDLE, "MIDDLE"
  chnset        kMOUSE_DOWN_RIGHT, "RIGHT"
 endif

endin
                
</CsInstruments>

<CsScore>
i 1 0 [3600*24*7]
</CsScore>

</CsoundSynthesizer>