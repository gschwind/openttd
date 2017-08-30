/* $Id$ */

/*
 * This file is part of OpenTTD.
 * OpenTTD is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 2.
 * OpenTTD is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details. You should have received a copy of the GNU General Public License along with OpenTTD. If not, see <http://www.gnu.org/licenses/>.
 */

/** @file orders_list_gui.cpp GUIs for a number of misc windows. */

#include "stdafx.h"
#include "debug.h"
#include "landscape.h"
#include "error.h"
#include "gui.h"
#include "command_func.h"
#include "company_func.h"
#include "town.h"
#include "string_func.h"
#include "company_base.h"
#include "texteff.hpp"
#include "strings_func.h"
#include "window_func.h"
#include "querystring_gui.h"
#include "core/geometry_func.hpp"
#include "newgrf_debug.h"
#include "zoom_func.h"

#include "widgets/orders_list_widget.h"

static const NWidgetPart _nested_orders_list_widgets[] = {
	NWidget(NWID_HORIZONTAL),
		NWidget(WWT_CLOSEBOX, COLOUR_GREY),
		NWidget(WWT_CAPTION, COLOUR_GREY), SetDataTip(STR_ABOUT_OPENTTD, STR_TOOLTIP_WINDOW_TITLE_DRAG_THIS),
	EndContainer(),
	NWidget(WWT_PANEL, COLOUR_GREY), SetPIP(4, 2, 4),
		NWidget(WWT_LABEL, COLOUR_GREY), SetDataTip(STR_ABOUT_ORIGINAL_COPYRIGHT, STR_NULL),
		NWidget(WWT_LABEL, COLOUR_GREY), SetDataTip(STR_ABOUT_VERSION, STR_NULL),
		NWidget(WWT_FRAME, COLOUR_GREY), SetPadding(0, 5, 1, 5),
			NWidget(WWT_EMPTY, INVALID_COLOUR, WID_OL_SCROLLING_TEXT),
		EndContainer(),
		NWidget(WWT_LABEL, COLOUR_GREY, WID_OL_WEBSITE), SetDataTip(STR_BLACK_RAW_STRING, STR_NULL),
		NWidget(WWT_LABEL, COLOUR_GREY), SetDataTip(STR_ABOUT_COPYRIGHT_OPENTTD, STR_NULL),
	EndContainer(),
};

static WindowDesc _orders_list_desc(
	WDP_CENTER, NULL, 0, 0,
	WC_ORDERS_LIST, WC_NONE,
	0,
	_nested_orders_list_widgets, lengthof(_nested_orders_list_widgets)
);

static const char * const _credits[] = {
	"Original design by Chris Sawyer",
	"Original graphics by Simon Foster",
	"",
	"The OpenTTD team (in alphabetical order):",
	"  Albert Hofkamp (Alberth) - GUI expert (since 0.7)",
	"  Matthijs Kooijman (blathijs) - Pathfinder-guru, Debian port (since 0.3)",
	"  Ulf Hermann (fonsinchen) - Cargo Distribution (since 1.3)",
	"  Christoph Elsenhans (frosch) - General coding (since 0.6)",
	"  Lo\xC3\xAF""c Guilloux (glx) - General / Windows Expert (since 0.4.5)",
	"  Michael Lutz (michi_cc) - Path based signals (since 0.7)",
	"  Owen Rudge (orudge) - Forum host, OS/2 port (since 0.1)",
	"  Peter Nelson (peter1138) - Spiritual descendant from NewGRF gods (since 0.4.5)",
	"  Ingo von Borstel (planetmaker) - General, Support (since 1.1)",
	"  Remko Bijker (Rubidium) - Lead coder and way more (since 0.4.5)",
	"  Jos\xC3\xA9 Soler (Terkhen) - General coding (since 1.0)",
	"  Leif Linse (Zuu) - AI/Game Script (since 1.2)",
	"",
	"Inactive Developers:",
	"  Jean-Fran\xC3\xA7ois Claeys (Belugas) - GUI, NewGRF and more (0.4.5 - 1.0)",
	"  Bjarni Corfitzen (Bjarni) - MacOSX port, coder and vehicles (0.3 - 0.7)",
	"  Victor Fischer (Celestar) - Programming everywhere you need him to (0.3 - 0.6)",
	"  Jaroslav Mazanec (KUDr) - YAPG (Yet Another Pathfinder God) ;) (0.4.5 - 0.6)",
	"  Jonathan Coome (Maedhros) - High priest of the NewGRF Temple (0.5 - 0.6)",
	"  Attila B\xC3\xA1n (MiHaMiX) - Developer WebTranslator 1 and 2 (0.3 - 0.5)",
	"  Zden\xC4\x9Bk Sojka (SmatZ) - Bug finder and fixer (0.6 - 1.3)",
	"  Christoph Mallon (Tron) - Programmer, code correctness police (0.3 - 0.5)",
	"  Patric Stout (TrueBrain) - NoAI, NoGo, Network (0.3 - 1.2), sys op (active)",
	"  Thijs Marinussen (Yexo) - AI Framework, General (0.6 - 1.3)",
	"",
	"Retired Developers:",
	"  Tam\xC3\xA1s Farag\xC3\xB3 (Darkvater) - Ex-Lead coder (0.3 - 0.5)",
	"  Dominik Scherer (dominik81) - Lead programmer, GUI expert (0.3 - 0.3)",
	"  Emil Djupfeld (egladil) - MacOSX (0.4.5 - 0.6)",
	"  Simon Sasburg (HackyKid) - Many bugfixes (0.4 - 0.4.5)",
	"  Ludvig Strigeus (ludde) - Original author of OpenTTD, main coder (0.1 - 0.3)",
	"  Cian Duffy (MYOB) - BeOS port / manual writing (0.1 - 0.3)",
	"  Petr Baudi\xC5\xA1 (pasky) - Many patches, NewGRF support (0.3 - 0.3)",
	"  Benedikt Br\xC3\xBCggemeier (skidd13) - Bug fixer and code reworker (0.6 - 0.7)",
	"  Serge Paquet (vurlix) - 2nd contributor after ludde (0.1 - 0.3)",
	"",
	"Special thanks go out to:",
	"  Josef Drexler - For his great work on TTDPatch",
	"  Marcin Grzegorczyk - Track foundations and for describing TTD internals",
	"  Stefan Mei\xC3\x9Fner (sign_de) - For his work on the console",
	"  Mike Ragsdale - OpenTTD installer",
	"  Christian Rosentreter (tokai) - MorphOS / AmigaOS port",
	"  Richard Kempton (richK) - additional airports, initial TGP implementation",
	"",
	"  Alberto Demichelis - Squirrel scripting language \xC2\xA9 2003-2008",
	"  L. Peter Deutsch - MD5 implementation \xC2\xA9 1999, 2000, 2002",
	"  Michael Blunck - Pre-signals and semaphores \xC2\xA9 2003",
	"  George - Canal/Lock graphics \xC2\xA9 2003-2004",
	"  Andrew Parkhouse (andythenorth) - River graphics",
	"  David Dallaston (Pikka) - Tram tracks",
	"  All Translators - Who made OpenTTD a truly international game",
	"  Bug Reporters - Without whom OpenTTD would still be full of bugs!",
	"",
	"",
	"And last but not least:",
	"  Chris Sawyer - For an amazing game!"
};

struct OrdersListWindow : public Window {
	int text_position;                       ///< The top of the scrolling text
	byte counter;                            ///< Used to scroll the text every 5 ticks
	int line_height;                         ///< The height of a single line
	static const int num_visible_lines = 19; ///< The number of lines visible simultaneously

	OrdersListWindow() : Window(&_orders_list_desc)
	{
		this->InitNested(0);

		this->counter = 5;
		this->text_position = this->GetWidget<NWidgetBase>(WID_OL_SCROLLING_TEXT)->pos_y + this->GetWidget<NWidgetBase>(WID_OL_SCROLLING_TEXT)->current_y;
	}

	virtual void SetStringParameters(int widget) const
	{
		if (widget == WID_OL_WEBSITE) SetDParamStr(0, "Website: http://www.openttd.org");
	}

	virtual void UpdateWidgetSize(int widget, Dimension *size, const Dimension &padding, Dimension *fill, Dimension *resize)
	{
		if (widget != WID_OL_SCROLLING_TEXT) return;

		this->line_height = FONT_HEIGHT_NORMAL;

		Dimension d;
		d.height = this->line_height * num_visible_lines;

		d.width = 0;
		for (uint i = 0; i < lengthof(_credits); i++) {
			d.width = max(d.width, GetStringBoundingBox(_credits[i]).width);
		}
		*size = maxdim(*size, d);
	}

	virtual void DrawWidget(const Rect &r, int widget) const
	{
		if (widget != WID_OL_SCROLLING_TEXT) return;

		int y = this->text_position;

		/* Show all scrolling _credits */
		for (uint i = 0; i < lengthof(_credits); i++) {
			if (y >= r.top + 7 && y < r.bottom - this->line_height) {
				DrawString(r.left, r.right, y, _credits[i], TC_BLACK, SA_LEFT | SA_FORCE);
			}
			y += this->line_height;
		}
	}

	virtual void OnTick()
	{
		if (--this->counter == 0) {
			this->counter = 5;
			this->text_position--;
			/* If the last text has scrolled start a new from the start */
			if (this->text_position < (int)(this->GetWidget<NWidgetBase>(WID_OL_SCROLLING_TEXT)->pos_y - lengthof(_credits) * this->line_height)) {
				this->text_position = this->GetWidget<NWidgetBase>(WID_OL_SCROLLING_TEXT)->pos_y + this->GetWidget<NWidgetBase>(WID_OL_SCROLLING_TEXT)->current_y;
			}
			this->SetDirty();
		}
	}
};

void ShowOrdersListWindow()
{
	DeleteWindowByClass(WC_ORDERS_LIST);
	new OrdersListWindow();
}
