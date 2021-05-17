<a name="keypressBaseOctave"><h3 style="padding-top: 40px; margin-top: 40px;"></h3></a>

**keypressBaseOctave(val)** This sets the base note that is played when you press a key on the computer keyboard. The value must be between 0 and 10. The default is 3 that means the C note of the third MIDI octave (C1 key on the widget keyboard, if you don't change the default 'middleC' value).

<!--UPDATE WIDGET_IN_CSOUND
    SIdent sprintf "keypressBaseOctave(%d) ", rnd(10)
    SIdentifier strcat SIdentifier, SIdent
-->