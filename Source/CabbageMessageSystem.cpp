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