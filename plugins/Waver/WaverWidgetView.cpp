/*
 * WaverWidgetView.cpp - controls the UI for Waver
 *
 * Copyright (c) 2025 RoxasKH <asketch36@gmail.com>
 *
 * This file is part of LMMS - https://lmms.io
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program (see COPYING); if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301 USA.
 *
 */

/*
You have to install qt5-declarative qt5-quickcontrols2
 */

#include "WaverWidgetView.h"

#include <QDropEvent>
#include <QPainter>
#include <QPushButton>
#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QQuickWidget>
#include <QQmlEngine>
#include <QQmlContext>
#include <QResource>

#include "Clipboard.h"
#include "DataFile.h"
#include "InstrumentView.h"
#include "PixmapButton.h"
#include "FileDialog.h"
#include "Waver.h"
#include "StringPairDrag.h"
#include "Track.h"
#include "embed.h"

namespace lmms {

namespace gui {

WaverWidgetView::WaverWidgetView(Waver* instrument, QWidget* parent)
    : InstrumentView(instrument, parent)
    , m_waverParent(instrument)
{
    // window settings
    setAcceptDrops(true);
    setAutoFillBackground(true);

    setMaximumSize(QSize(10000, 10000));
    setMinimumSize(QSize(850, 600));

    QVBoxLayout *layout = new QVBoxLayout(this);
    setLayout(layout);
    // Remove default layout margins/spacing
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    // Set up the QQuickWidget
    QQuickWidget *quickWidget = new QQuickWidget(this);
    quickWidget->setClearColor(Qt::transparent); // make background transparent
    quickWidget->setAttribute(Qt::WA_TranslucentBackground); // allow transparency

    // Allow QML to know about the WaverWidgetView type
    qmlRegisterUncreatableType<WaverWidgetView>(
        "WaverWidgetView", 1, 0, "WaverWidgetView",
        "Instrument is provided by C++"
    );

    // Allow QML to know about the Waver type
    qmlRegisterUncreatableType<Waver>(
        "Waver", 1, 0, "Waver",
        "Instrument is provided by C++"
    );
    // Had to add this meta type to be able to expose a pointer variable to QML
    // https://forum.qt.io/topic/113897/qml-c-exposing-pointer-type-attribute-to-qml
    qRegisterMetaType<WaverSampleMap*>("WaverSampleMap*");
    qRegisterMetaType<Waver*>("Waver*");

    // Expose the model to QML
    // Use setInitialProperties() once on QT6 for better performance
    quickWidget->rootContext()->setContextProperty("waverModel", this);

    // Keep track of the height of the plugin view in a waverHeight variable for QML purposes
    m_waverHeightGetter = [quickWidget]() { return quickWidget->height(); };

    quickWidget->setSource(QUrl(QStringLiteral("qrc:/artwork/waver/WaverView.qml")));
    quickWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);
    layout->addWidget(quickWidget);

    setLayout(layout); 

    update();
}

void WaverWidgetView::resizeEvent(QResizeEvent* event)
{
    // Call base impementation
    QWidget::resizeEvent(event);

    // Notify QML that waverHeight changed
    emit waverHeightChanged();
}

void WaverWidgetView::openFiles()
{
    const auto audioFile = FileDialog::openAudioFile();
    if (audioFile.isEmpty()) { return; }
    //m_waverParent->updateFile(audioFile);
    m_waverParent->createSampleMap(audioFile);
}

// all the drag stuff is copied from AudioFileProcessor
void WaverWidgetView::dragEnterEvent(QDragEnterEvent* dee)
{
    // For mimeType() and MimeType enum class
    using namespace Clipboard;

    if (dee->mimeData()->hasFormat(mimeType(MimeType::StringPair)))
    {
        QString txt = dee->mimeData()->data(mimeType(MimeType::StringPair));
        if (txt.section(':', 0, 0) == QString("clip_%1").arg(static_cast<int>(Track::Type::Sample)))
        {
            dee->acceptProposedAction();
        }
        else if (txt.section(':', 0, 0) == "samplefile") { dee->acceptProposedAction(); }
        else { dee->ignore(); }
    }
    else { dee->ignore(); }
}

void WaverWidgetView::dropEvent(QDropEvent* de)
{
    QString type = StringPairDrag::decodeKey(de);
    QString value = StringPairDrag::decodeValue(de);
    if (type == "samplefile")
    {
        // set m_wf wave file
        m_waverParent->updateFile(value);
        return;
    }
    else if (type == QString("clip_%1").arg(static_cast<int>(Track::Type::Sample)))
    {
        DataFile dataFile(value.toUtf8());
        m_waverParent->updateFile(dataFile.content().firstChild().toElement().attribute("src"));
        de->accept();
        return;
    }

    de->ignore();
}

} // namespace gui

} // namespace lmms
