/*
obs-websocket
Copyright (C) 2016-2019	Stéphane Lepin <stephane.lepin@gmail.com>

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program. If not, see <https://www.gnu.org/licenses/>
*/

#pragma once

#include <QtWidgets/QDialog>

#include "ui_settings-dialog.h"

class PluginWindow : public QDialog
{
	Q_OBJECT

public:
	explicit PluginWindow(QWidget *parent = 0);
	~PluginWindow();
	void showEvent(QShowEvent* event);
	void ToggleShowHide();
	void PreparePasswordEntry();

private Q_SLOTS:
	void AuthCheckboxChanged();
	void FormAccepted();

private:
	Ui::PluginWindow *ui;
};
