#ifndef CELLFUNCTIONCOMMUNICATORIMPL_H
#define CELLFUNCTIONCOMMUNICATORIMPL_H

#include "model/features/cellfunction.h"

#include <QVector3D>

class TestCellFunctionCommunicator;

class CellFunctionCommunicatorImpl : public CellFunction
{
public:
    CellFunctionCommunicatorImpl (SimulationContext* context);
    CellFunctionCommunicatorImpl (QByteArray data, SimulationContext* context);

    void serializePrimitives (QDataStream& stream) const override;
    void deserializePrimitives (QDataStream& stream) override;

    Enums::CellFunction::Type getType () const { return Enums::CellFunction::COMMUNICATOR; }
	QByteArray getInternalData () const override;

	struct MessageData {
		quint8 channel = 0;
		quint8 message = 0;
		quint8 angle = 0;
		quint8 distance = 0;
	};
    bool& getNewMessageReceivedRef();
    MessageData& getReceivedMessageRef();

protected:
    ProcessingResult processImpl (Token* token, Cell* cell, Cell* previousCell) override;

private:

    bool _newMessageReceived = false;
    MessageData _receivedMessage;
	SimulationParameters* _parameters = nullptr;

    Enums::CommunicatorIn::Type readCommandFromToken (Token* token) const;
    void setListeningChannel (Token* token);

    void sendMessageToNearbyCommunicatorsAndUpdateToken (Token* token, Cell* cell, Cell* previousCell) const;
    int sendMessageToNearbyCommunicatorsAndReturnNumber (const MessageData& messageDataToSend, Cell* senderCell, Cell* senderPreviousCell) const;
    QList< Cell* > findNearbyCommunicator (Cell* cell) const;
    bool sendMessageToCommunicatorAndReturnSuccess (const MessageData& messageDataToSend, Cell* senderCell, Cell* senderPreviousCell, Cell* receiverCell) const;
    QVector3D calcDisplacementOfObjectFromSender (const MessageData& messageDataToSend, Cell* senderCell, Cell* senderPreviousCell) const;

    void receiveMessage (Token* token,Cell* receiverCell, Cell* receiverPreviousCell);
    void calcReceivedMessageAngle (Cell* receiverCell, Cell* receiverPreviousCell);
};

#endif // CELLFUNCTIONCOMMUNICATORIMPL_H