#ifndef LMMS_WAVER_WAVEFORM_H
#define LMMS_WAVER_WAVEFORM_H

#pragma once

#include <QQuickItem>

#include "WaverSampleMap.h"

namespace lmms {

namespace gui {

class WaverWaveform : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(WaverSampleMap* map READ map WRITE setMap NOTIFY mapChanged)
    Q_PROPERTY(QColor color READ waveformColor WRITE setWaveformColor NOTIFY waveformColorChanged)


public:
    explicit WaverWaveform(QQuickItem* parent = nullptr);

    WaverSampleMap* map() const;
    void setMap(WaverSampleMap* map);

    QColor waveformColor() const { return m_waveformColor; }
    void setWaveformColor(const QColor& color) {
        if (m_waveformColor != color) {
            m_waveformColor = color;
            emit waveformColorChanged();
            update(); // refresh the node
        }
    }

signals:
    void mapChanged();
    void waveformColorChanged();

protected:
    // Scene graph hook
    QSGNode* updatePaintNode(
        QSGNode* oldNode,
        UpdatePaintNodeData* data) override;

    // Called when width / height changes
    void geometryChanged(
        const QRectF& newGeometry,
        const QRectF& oldGeometry) override;

private:
    WaverSampleMap* m_map = nullptr;
    QColor m_waveformColor = Qt::white; // default
};

} // namespace gui

} // namespace lmms

#endif // LMMS_WAVER_WAVEFORM_H
