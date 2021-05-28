
/* Attribution-NonCommercial-ShareAlike 4.0 International
Attribution - You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
NonCommercial - You may not use the material for commercial purposes.
ShareAlike - If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode */

; MouseSense.csd
; Written by Iain McCurdy, 2014

<Cabbage>
form caption("Mouse Sense") size(785, 400), pluginId("SMo1"), guiRefresh(16)

nslider bounds( 55, 25, 90, 35), text("MOUSE X"),             fontColour("white"), textBox(1),                channel("X"),           range(0, 800, 0,1,1)
nslider bounds(160, 25, 90, 35), text("MOUSE Y"),             fontColour("white"), textBox(1),                channel("Y"),           range(0, 400, 0,1,1)
nslider bounds(  5, 70, 90, 35), text("MOUSE LEFT"),          fontColour("white"), textBox(1),                channel("LEFT"),        range(0, 1,   0,1,1)
nslider bounds(105, 70, 90, 35), text("MOUSE MIDDLE"),        fontColour("white"), textBox(1),                channel("MIDDLE"),      range(0, 1,   0,1,1)
nslider bounds(205, 70, 90, 35), text("MOUSE RIGHT"),         fontColour("white"), textBox(1),                channel("RIGHT"),       range(0, 1,   0,1,1)


textBox bounds(300, 10,475,350), file("MouseSenseInfo.txt")

</Cabbage>

<CsoundSynthesizer>

<CsOptions>
-d -n
</CsOptions>

<CsInstruments>

; sr set by host
ksmps           =       32      ;NUMBER OF AUDIO SAMPLES IN EACH CONTROL CYCLE
nchnls          =       2       ;NUMBER OF CHANNELS (2=STEREO)
0dbfs           =       1

instr   1
 kMOUSE_X               chnget  "MOUSE_X"
 kMOUSE_Y               chnget  "MOUSE_Y"
 kMOUSE_DOWN_LEFT       chnget  "MOUSE_DOWN_LEFT"
 kMOUSE_DOWN_MIDDLE     chnget  "MOUSE_DOWN_MIDDLE"
 kMOUSE_DOWN_RIGHT      chnget  "MOUSE_DOWN_RIGHT"

 kmetro    metro    20

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