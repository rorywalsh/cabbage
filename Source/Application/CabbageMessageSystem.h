/*
  Copyright (C) 2016 Rory Walsh

  Cabbage is free software; you can redistribute it
  and/or modify it under the terms of the GNU General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  Cabbage is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Lesser General Public License for more details.

  You should have received a copy of the GNU General Public
  License along with Csound; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
  02111-1307 USA
*/

#ifndef CABBMESS_H
#define CABBMESS_H

#include "../Utilities/CabbageUtilities.h"

#ifndef Cabbage_No_Csound
#include "csound.hpp"
#endif

using namespace std;


//simple channel message classe
class CabbageChannelMessage : public CabbageUtilities
{
public:
    String channelName;
#ifndef Cabbage_No_Csound
    MYFLT value;
#else
    float value;
#endif
    String type;
    String fStatement;
    int tableNumber;
    String stringVal;

    CabbageChannelMessage(String chan, double val, String _type)
    {
        channelName = chan;
        value = val;
        type = _type;
    }

    CabbageChannelMessage(String chan, String val, String _type)
    {
        channelName = chan;
        stringVal = val;
        type = _type;
    }

    ~CabbageChannelMessage()
    {}

};

//message queue class
class CabbageMessageQueue : public CabbageUtilities
{
    Array<CabbageChannelMessage, CriticalSection> outgoingChannelMessages;

public:
    CabbageMessageQueue() {}
    ~CabbageMessageQueue() {}

    void addOutgoingChannelMessageToQueue(String _chan, double _val, String _type="");
    void addOutgoingChannelMessageToQueue(String _chan, String _val, String _type="");
    void addOutgoingTableUpdateMessageToQueue(String fStatement, int tableNumber);
    CabbageChannelMessage &getOutgoingChannelMessageFromQueue(int index);
    int getNumberOfOutgoingChannelMessagesInQueue();
    void flushOutgoingChannelMessages();

};



#endif
