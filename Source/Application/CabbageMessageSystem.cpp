/*
  Copyright (C) 2016 Rory Walsh

  Cabbage is free software; you can redistribute it
  and/or modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  Cabbage is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with Csound; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
  02111-1307 USA
*/

#include "CabbageMessageSystem.h"

void CabbageMessageQueue::addOutgoingChannelMessageToQueue(String _chan, double _val, String _type)
{
    outgoingChannelMessages.add(CabbageChannelMessage(_chan, _val, _type));
}

void CabbageMessageQueue::addOutgoingChannelMessageToQueue(String _chan, String _val, String _type)
{
    outgoingChannelMessages.add(CabbageChannelMessage(_chan, _val, _type));
}

void CabbageMessageQueue::addOutgoingTableUpdateMessageToQueue(String fStatement, int tableNumber)
{
    CabbageChannelMessage tableMessage("", 0.f, "updateTable");
    tableMessage.fStatement = fStatement;
    tableMessage.tableNumber = tableNumber;
    outgoingChannelMessages.add(tableMessage);
}

CabbageChannelMessage& CabbageMessageQueue::getOutgoingChannelMessageFromQueue(int index)
{
    return outgoingChannelMessages.getReference(index);
}

int CabbageMessageQueue::getNumberOfOutgoingChannelMessagesInQueue()
{
    return outgoingChannelMessages.size();
}

void CabbageMessageQueue::flushOutgoingChannelMessages()
{
    //const MessageManagerLock mmLock;
    outgoingChannelMessages.clear();
}