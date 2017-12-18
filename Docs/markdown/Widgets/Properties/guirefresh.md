<a name="guirefresh"></a>
**guirefresh(val)** Sets the rate at which Cabbage will update its GUI widgets when controlled by Csound. The value passed represents the number of k-rate cycles to be skipped before the next update. The larger this is the slower the GUI updates will take place, but the less CPU intensive the instrument will be. val should be an integer greater than 1 and is set to ksmps x 2 by default. 

>For best performance one should set guirefresh to be a factor of ksmps.