<a name="celldata"></a>**celldata(xVal, yVal, "string")** Sets the contents of a cell. The first two values set the actual cell in terms of coordinates, while the 3rd parameter sets the contents of the cell. This identifier is best called within Csound using an [identchannel](./identchannels.md). In order to pass inverted commas to cell data from Csound you will need to use several preceding backslashes. For example:

```csharp
SScoreEvent sprintf "i \\\"Sine\\\" 0 1 %d", random(0, 100)
SMessage sprintf "celldata(%d, %d, \"%s\") ", iColCnt, iRowCnt, SScoreEvent 
chnset SScoreEvent, "eventseqIdent"
```