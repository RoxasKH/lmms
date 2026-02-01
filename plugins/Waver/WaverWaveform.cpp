#include "WaverWaveform.h"

#include <cmath>       // for std::abs
#include <algorithm>   // for std::max
#include <QQuickItem>
#include <QSGNode>
#include <QRectF>
#include <QSGGeometryNode>
#include <QSGFlatColorMaterial>
#include <QSGGeometry>

namespace lmms {
namespace gui {

// Cubic Hermite interpolation
inline float cubicInterp(float p0, float p1, float p2, float p3, float t)
{
    float a = -0.5f*p0 + 1.5f*p1 - 1.5f*p2 + 0.5f*p3;
    float b = p0 - 2.5f*p1 + 2.0f*p2 - 0.5f*p3;
    float c = -0.5f*p0 + 0.5f*p2;
    float d = p1;
    return ((a*t + b)*t + c)*t + d;
}

WaverWaveform::WaverWaveform(QQuickItem* parent)
    : QQuickItem(parent)
{
    setFlag(ItemHasContents, true);
}

QSGNode* WaverWaveform::updatePaintNode(QSGNode* oldNode, UpdatePaintNodeData*)
{
    // --- Root node ---
    QSGNode* rootNode = oldNode;
    if (!rootNode) rootNode = new QSGNode();
    rootNode->removeAllChildNodes();

    if (!m_map || width() <= 0 || height() <= 0) return rootNode;

    const Sample& sample = m_map->sample();
    if (sample.sampleSize() <= 0) return rootNode;

    const auto* frames = sample.data();
    int frameCount = static_cast<int>(sample.sampleSize());
    int startFrame = sample.startFrame();
    int endFrame = sample.endFrame();
    if (endFrame <= startFrame) endFrame = frameCount;

    int pointCount = qMax(1, int(width()));
    const float eps = 0.5f; // small offset to prevent baseline glitches

    // ----------------------
    // 1. Peak waveform (mirrored) with cubic interpolation
    // ----------------------
    auto* peakNode = new QSGGeometryNode();
    auto* peakGeometry = new QSGGeometry(
        QSGGeometry::defaultAttributes_Point2D(),
        pointCount * 2
    );
    peakGeometry->setDrawingMode(QSGGeometry::DrawLineStrip);
    peakNode->setGeometry(peakGeometry);
    peakNode->setFlag(QSGNode::OwnsGeometry);

    auto* peakMaterial = new QSGFlatColorMaterial();
    peakMaterial->setColor(m_waveformColor); // waveform color
    peakMaterial->setFlag(QSGMaterial::Blending);
    peakNode->setMaterial(peakMaterial);
    peakNode->setFlag(QSGNode::OwnsMaterial);

    auto* peakV = peakGeometry->vertexDataAsPoint2D();

    // Precompute mono amplitude for all frames
    std::vector<float> monoValues(frameCount);
    for (int i = 0; i < frameCount; ++i) {
        const auto& f = frames[i];
        monoValues[i] = std::max(std::abs(f.left()), std::abs(f.right())) * sample.amplification();
    }

    // Draw waveform with cubic interpolation for smooth curves
    for (int x = 0; x < pointCount; ++x) {
        float t = x / float(pointCount - 1);
        float fIndex = startFrame + t * (endFrame - startFrame - 1);
        int idx1 = int(fIndex);
        float frac = fIndex - idx1;

        // Clamp indices for cubic interpolation (needs 4 points)
        int idx0 = std::clamp(idx1 - 1, 0, frameCount - 1);
        int idx2 = std::clamp(idx1 + 1, 0, frameCount - 1);
        int idx3 = std::clamp(idx1 + 2, 0, frameCount - 1);

        float mono = cubicInterp(
            monoValues[idx0],
            monoValues[idx1],
            monoValues[idx2],
            monoValues[idx3],
            frac
        );

        float yTop = height() * (0.5f - 0.5f * mono);
        float yBottom = height() * (0.5f + 0.5f * mono) + eps;

        peakV[2*x + 0].set(float(x), yTop);
        peakV[2*x + 1].set(float(x), yBottom);
    }

    rootNode->appendChildNode(peakNode);

    // ----------------------
    // 2. RMS waveform (Hann-weighted, scaled down)
    // ----------------------
    auto* rmsNode = new QSGGeometryNode();
    auto* rmsGeometry = new QSGGeometry(
        QSGGeometry::defaultAttributes_Point2D(),
        pointCount * 2
    );
    rmsGeometry->setDrawingMode(QSGGeometry::DrawTriangleStrip);
    rmsNode->setGeometry(rmsGeometry);
    rmsNode->setFlag(QSGNode::OwnsGeometry);

    auto* rmsMaterial = new QSGFlatColorMaterial();
    QColor rmsColor = m_waveformColor.darker(125); // RMS 25% darker than waveform
    rmsColor.setAlpha(180); // semi-transparent
    rmsMaterial->setColor(rmsColor);
    rmsMaterial->setFlag(QSGMaterial::Blending);
    rmsNode->setMaterial(rmsMaterial);
    rmsNode->setFlag(QSGNode::OwnsMaterial);

    auto* rmsV = rmsGeometry->vertexDataAsPoint2D();

    const int rmsWindow = 128;   // number of samples in Hann window
    const float rmsScale = 0.5f; // scale down RMS for visualization
    const float pi = 3.14159265359f;

    for (int x = 0; x < pointCount; ++x) {
        float t = x / float(pointCount - 1);
        int centerFrame = startFrame + int(t * (endFrame - startFrame - 1));
        centerFrame = std::clamp(centerFrame, 0, frameCount - 1);

        // Hann-weighted RMS computation:
        // Compute a small window of samples around this pixel
        int start = std::max(0, centerFrame - rmsWindow/2);
        int end   = std::min(frameCount, centerFrame + rmsWindow/2);
        int N = end - start; if (N <= 0) N = 1;

        float sumWeightedSquares = 0.0f;
        float sumWeights = 0.0f;

        for (int i = 0; i < N; ++i) {
            int idx = start + i;
            const auto& f = frames[idx];

            // Hann window weight: w = 0.5 * (1 - cos(2 * pi * i / (N-1)))
            float w = 0.5f * (1.0f - std::cos(2.0f * pi * i / (N - 1)));

            float sampleSquared = (f.left()*f.left() + f.right()*f.right()) / 2.0f;

            sumWeightedSquares += sampleSquared * w;
            sumWeights += w;
        }

        // RMS amplitude for this pixel
        float rms = std::sqrt(sumWeightedSquares / sumWeights) * sample.amplification();

        // Scale down RMS to stay inside waveform
        rms *= rmsScale;

        float yTop = height() * (0.5f - 0.5f * rms);
        float yBottom = height() * (0.5f + 0.5f * rms) + eps;

        rmsV[2*x + 0].set(float(x), yTop);
        rmsV[2*x + 1].set(float(x), yBottom);
    }

    rootNode->appendChildNode(rmsNode);

    return rootNode;
}

WaverSampleMap* WaverWaveform::map() const
{
    return m_map;
}

void WaverWaveform::setMap(WaverSampleMap* map)
{
    if (m_map == map)
        return;

    m_map = map;
    emit mapChanged();
    update(); // schedules updatePaintNode()
}

void WaverWaveform::geometryChanged(const QRectF& newGeometry, const QRectF& oldGeometry)
{
    QQuickItem::geometryChanged(newGeometry, oldGeometry);
    update();
}

} // namespace gui
} // namespace lmms
