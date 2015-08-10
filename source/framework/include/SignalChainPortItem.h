#ifndef SIGNALCHAINPORTITEM_H
#define SIGNALCHAINPORTITEM_H

#include "FrameworkApi.h"
#include "SignalChainItem.h"

class SignalChainConnectionItem;
class SignalChainAudioUnitItem;
class InputPort;
class OutputPort;

/**
 * @brief Absttract graphics item representing a signal port.
 *
 * Signal port item is drawn as a circle. The color is defined by
 * the data type passed via thisport.
 */
class QMUSIC_FRAMEWORK_API SignalChainPortItem : public SignalChainItem
{
public:

    SignalChainPortItem(Type type, const QString &labelText, QGraphicsItem *pParent = nullptr);
    ~SignalChainPortItem();

    /**
     * @brief Tells whether this port is an output port.
     * @return true if the port is an output.
     */
    virtual bool isOutput() const = 0;

    bool isInput() const { return !isOutput(); }

    virtual QVariant::Type dataType() const = 0;

    /**
     * @brief Returns pointer to audio unit item this port belongs to.
     * @note Audio unit item is a parent of this port graphics item.
     * @return
     */
    SignalChainAudioUnitItem* audioUnitItem() const;

    void addConnection(SignalChainConnectionItem *pConnection);
    void removeConnection(SignalChainConnectionItem *pConnection);
    QList<SignalChainConnectionItem*> connections() const { return m_connections; }
    bool hasConnections() const { return !m_connections.isEmpty(); }

    void setLabel(const QString &text);

protected:

    void paint(QPainter *pPainter, const QStyleOptionGraphicsItem *pOption, QWidget *pWidget) override;
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);

private:

    QGraphicsSimpleTextItem *m_pLabelItem;

    /// List of connection to this port.
    QList<SignalChainConnectionItem*> m_connections;
};

class QMUSIC_FRAMEWORK_API SignalChainInputPortItem : public SignalChainPortItem
{
public:

    SignalChainInputPortItem(QGraphicsItem *pParent = nullptr);
    SignalChainInputPortItem(InputPort *pInput, QGraphicsItem *pParent = nullptr);
    bool isOutput() const override { return false; }
    QVariant::Type dataType() const override;
    InputPort* inputPort() const { return m_pInputPort; }

private:
    InputPort *m_pInputPort;
};

class QMUSIC_FRAMEWORK_API SignalChainOutputPortItem : public SignalChainPortItem
{
public:

    SignalChainOutputPortItem(QGraphicsItem *pParent = nullptr);
    SignalChainOutputPortItem(OutputPort *pOutput, QGraphicsItem *pParent = nullptr);
    bool isOutput() const override { return true; }
    QVariant::Type dataType() const override;
    OutputPort* outputPort() const { return m_pOutputPort; }

private:
    OutputPort *m_pOutputPort;
};

#endif // SIGNALCHAINPORTITEM_H

