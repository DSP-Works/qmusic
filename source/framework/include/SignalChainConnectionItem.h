#ifndef SIGNALCHAINCONNECTIONITEM_H
#define SIGNALCHAINCONNECTIONITEM_H

#include "FrameworkApi.h"
#include "SignalChainItem.h"

class QGraphicsPathItem;
class SignalChainPortItem;
class SignalChainInputPortItem;
class SignalChainOutputPortItem;

class QMUSIC_FRAMEWORK_API SignalChainConnectionItem : public SignalChainItem
{
public:

    const static QString UID;

    SignalChainConnectionItem(QGraphicsItem *pParent = nullptr);
    ~SignalChainConnectionItem();

    SignalChainInputPortItem* inputPortItem() const { return m_pTargetPortItem; }
    SignalChainOutputPortItem* outputPortItem() const { return m_pSourcePortItem; }

    void setSourcePort(SignalChainOutputPortItem *pPort);
    void setTargetPort(SignalChainInputPortItem *pPort);
    void setPort(SignalChainPortItem *pPort);
    void removePort(SignalChainPortItem *pPortItem);

    void setSourcePoint(const QPointF &p) { m_sourcePoint = p; }
    void setTargetPoint(const QPointF &p) { m_targetPoint = p; }
    void updatePathFromPorts();
    void updatePath();

    bool isConnected() const;

    // ISerializable interface
    QString uid() const override final { return UID; }
    void serialize(QVariantMap &data, SerializationContext *pContext) const;
    void deserialize(const QVariantMap &data, SerializationContext *pContext);
    static ISerializable* create() { return new SignalChainConnectionItem(); }

protected:

    void paint(QPainter *pPainter, const QStyleOptionGraphicsItem *pOption, QWidget *pWidget) override;

private:

    SignalChainOutputPortItem *m_pSourcePortItem;
    SignalChainInputPortItem *m_pTargetPortItem;
    QPointF m_sourcePoint;
    QPointF m_targetPoint;
};

#endif // SIGNALCHAINCONNECTIONITEM_H

