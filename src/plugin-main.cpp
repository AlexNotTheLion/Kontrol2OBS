#include <QtWidgets/QAction>
#include <QtWidgets/QMainWindow>

#include <obs-module.h>
#if __has_include(<obs-frontend-api.h>)
#include <obs-frontend-api.h>
#else
#include <obs-frontend-api/obs-frontend-api.h>
#endif
#include <obs-data.h>
#include <string>
#include <map>
#include <iostream>
#include <utility>

#include "plugin-main.h"

#include "forms/settings-dialog.h"
#include "events.h"

OBS_DECLARE_MODULE()
OBS_MODULE_USE_DEFAULT_LOCALE(PLUGIN_NAME, "en-US")

bool obs_module_load(void)
{

	//setup
	blog(LOG_DEBUG, "Setup UI");

	obs_frontend_push_ui_translation(obs_module_get_string);
	QMainWindow *mainWindow = (QMainWindow *)obs_frontend_get_main_window();
	plugin_window = new SettingsDialog(mainWindow);
	obs_frontend_pop_ui_translation();

	const char *menuActionText =
		obs_module_text("OBS-kontrol Settings");
	QAction *menuAction =
		(QAction *)obs_frontend_add_tools_menu_qaction(menuActionText);
	QObject::connect(menuAction, &QAction::triggered, [] {
		// The settings dialog belongs to the main window. Should be ok
		// to pass the pointer to this QAction belonging to the main window
		plugin_window->ToggleShowHide();
	});

	blog(LOG_DEBUG, "OBSKONTROL: Setup Complete");

	blog(LOG_INFO, "plugin loaded successfully (version %s)", PLUGIN_VERSION);

	return true;
}

void obs_module_unload()
{
	blog(LOG_INFO, "plugin unloaded");
}
