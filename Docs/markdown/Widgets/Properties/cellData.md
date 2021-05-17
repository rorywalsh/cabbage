<a name="cellData"><h3 style="padding-top: 40px; margin-top: 40px;"></h3></a>
**cellData(xVal, yVal, "string")** Sets the contents of a cell. The first two values set the actual cell in terms of coordinates, while the 3rd parameter sets the contents of the cell. This identifier is best called within Csound using an [identChannel](./identChannels.md). In order to pass inverted commas to cell data from Csound you will need to use several preceding backslashes. For example:

```csharp
SScoreEvent sprintf "i \\\"Sine\\\" 0 1 %d", random(0, 100)
SMessage sprintf "cellData(%d, %d, \"%s\") ", iColCnt, iRowCnt, SScoreEvent 
chnset SScoreEvent, "eventseqIdent"
```