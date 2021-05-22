<a name="ampRange"><h3 style="padding-top: 40px; margin-top: 40px;"></h3></a>
_____________________________
**ampRange(min, max, tableNumber, quantise)** Sets the amplitude range(Y-axis) for a particular table. Min and Max are the minimum and maximum values. Quantise will set the resolution of the Y axis for editing. For example, if quantise is set to 1, all points added to the table will be quantised to integer values. If quantise matches the dynamic range of the table, the table will be drawn as a grid of on/off switches. If only one ampRange() identifier is used, a table number of -1 can be set so that each table displayed will share the same amp range.

>If the table is not active this identifier can usually be left out. If it is active, and therefore editable, it is important to use this identifier. Failure to do so may cause unexpected result.   
