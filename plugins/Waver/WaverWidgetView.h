/*
 * WaverWidgetView.h - declaration of class WaverWidgetView
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

#ifndef LMMS_GUI_WAVER_VIEW_H
#define LMMS_GUI_WAVER_VIEW_H

#include <QQuickWidget>

#include "InstrumentView.h"

class QPushButton;

namespace lmms {

class Waver;
class WaverSampleMap;

namespace gui {

class WaverWidgetView : public InstrumentView
{
    Q_OBJECT
    Q_PROPERTY(Waver* instrument READ instrument CONSTANT)
    // Keep track of height changes, needed in QML combobox
    Q_PROPERTY(int waverHeight READ waverHeight NOTIFY waverHeightChanged)

public slots:
    Q_INVOKABLE void openFiles();

public:
    WaverWidgetView(Waver* instrument, QWidget* parent);

    Waver* instrument() { return m_waverParent; }

    int waverHeight() const { return m_waverHeightGetter ? m_waverHeightGetter() : 0; }

signals:
    void waverHeightChanged();

protected:
    void dragEnterEvent(QDragEnterEvent* dee) override;
    void dropEvent(QDropEvent* de) override;
    void resizeEvent(QResizeEvent* event) override;

private:
    bool isResizable() const override { return true; }

    Waver* m_waverParent;

    std::function<int()> m_waverHeightGetter;
};

} // namespace gui

} // namespace lmms

#endif // LMMS_GUI_WAVER_VIEW_H
