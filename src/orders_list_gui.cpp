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
#include "order_base.h"
#include "vehicle_base.h"

#include "widgets/orders_list_widget.h"

static const NWidgetPart _nested_orders_list_widgets[] = {
	NWidget(NWID_HORIZONTAL),
		NWidget(WWT_CLOSEBOX, COLOUR_GREY),
		NWidget(WWT_CAPTION, COLOUR_GREY), SetDataTip(STR_ABOUT_OPENTTD, STR_TOOLTIP_WINDOW_TITLE_DRAG_THIS),
	EndContainer(),
	NWidget(NWID_HORIZONTAL),
		NWidget(WWT_MATRIX, COLOUR_GREY, WID_OL_LIST), SetMinimalSize(248, 0), SetFill(1, 0), SetResize(1, 1), SetMatrixDataTip(1, 0, STR_NULL), SetScrollbar(WID_OL_SCROLLBAR),
		NWidget(NWID_VSCROLLBAR, COLOUR_GREY, WID_OL_SCROLLBAR),
	EndContainer(),
	NWidget(NWID_HORIZONTAL),
		NWidget(WWT_PANEL, COLOUR_GREY), SetMinimalSize(0, 12), SetResize(1, 0), SetFill(1, 1), EndContainer(),
		NWidget(WWT_RESIZEBOX, COLOUR_GREY),
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

/**
 * Get the height of a vehicle in the vehicle list GUIs.
 * @param type    the vehicle type to look at
 * @param divisor the resulting height must be dividable by this
 * @return the height
 */
uint GetOrdersListHeight(uint divisor)
{
	/* Name + vehicle + profit */
	uint base = ScaleGUITrad(14) + 2 * FONT_HEIGHT_SMALL;
//	/* Drawing of the 4 small orders + profit*/
//	if (type >= VEH_SHIP) base = max(base, 5U * FONT_HEIGHT_SMALL);

	if (divisor == 1) return base;

	/* Make sure the height is dividable by divisor */
	uint rem = base % divisor;
	return base + (rem == 0 ? 0 : divisor - rem);
}

struct OrdersListWindow : public Window {
	Scrollbar *vscroll;
	SmallVector<OrderList const *, 32> data;

	OrdersListWindow() : Window(&_orders_list_desc)
	{
		this->CreateNestedTree();
		this->vscroll = this->GetScrollbar(WID_OL_SCROLLBAR);

		OrderList * ol;
		FOR_ALL_ORDER_LISTS(ol)
		{
			std::fprintf(stderr, "orderlist #%lu\n", orderlist_index);
			std::fprintf(stderr, "NumOrders = %u\n", ol->GetNumOrders());
			std::fprintf(stderr, "number of vehicle = %u\n", ol->GetNumVehicles());
			for(Vehicle * i = ol->GetFirstSharedVehicle(); i != NULL; i = i->NextShared())
			{
				std::fprintf(stderr, "   Vehicule: %p\n", i);
			}
			*(data.Append()) = ol;
		}

		vscroll->SetCount(data.Length());

		this->FinishInitNested(this->window_number);

	}

	virtual void SetStringParameters(int widget) const
	{
		if (widget == WID_OL_WEBSITE) SetDParamStr(0, "Website: http://www.openttd.org");
	}

	virtual void UpdateWidgetSize(int widget, Dimension *size, const Dimension &padding, Dimension *fill, Dimension *resize)
	{
		switch (widget) {
			case WID_OL_LIST:
				resize->height = GetOrdersListHeight(1);
				size->height = 4 * resize->height;
				break;
		}
	}

	virtual void DrawWidget(const Rect &r, int widget) const
	{
		//std::fprintf(stderr, "wid = %d, left = %d, top = %d, right = %d, bottom = %d\n", widget, r.left, r.top, r.right, r.bottom);
		switch (widget) {
			case WID_OL_LIST:
				this->DrawOrdersListItems(this->resize.step_height, r);
				break;
		}
	}

	virtual void OnResize()
	{
		this->vscroll->SetCapacityFromWidget(this, WID_OL_LIST);
	}

	virtual void OnClick(Point pt, int widget, int click_count)
	{
		switch (widget) {
			case WID_OL_LIST: { // Matrix to show vehicles
				uint id_v = this->vscroll->GetScrolledRowFromWidget(pt.y, this, WID_OL_LIST);
				if (id_v >= this->data.Length()) return; // click out of list bound

				const OrderList *ol = this->data[id_v];
				ShowOrdersWindow(ol->GetFirstSharedVehicle());
				break;
			}
		}
	}

	/**
	 * Draw all the vehicle list items.
	 * @param selected_vehicle The vehicle that is to be highlighted.
	 * @param line_height      Height of a single item line.
	 * @param r                Rectangle with edge positions of the matrix widget.
	 */
	void DrawOrdersListItems(int line_height, const Rect &r) const
	{
		int left = r.left + WD_MATRIX_LEFT;
		int right = r.right - WD_MATRIX_RIGHT;
		int width = right - left;
		bool rtl = _current_text_dir == TD_RTL;

//		int text_offset = std::max<int>(GetSpriteSize(SPR_PROFIT_LOT).width, GetDigitWidth() * this->unitnumber_digits) + WD_FRAMERECT_RIGHT;
//		int text_left  = left  + (rtl ?           0 : text_offset);
//		int text_right = right - (rtl ? text_offset :           0);
//
//		bool show_orderlist = this->vli.vtype >= VEH_SHIP;
//		int orderlist_left  = left  + (rtl ? 0 : max(ScaleGUITrad(100) + text_offset, width / 2));
//		int orderlist_right = right - (rtl ? max(ScaleGUITrad(100) + text_offset, width / 2) : 0);
//
//		int image_left  = (rtl && show_orderlist) ? orderlist_right : text_left;
//		int image_right = (!rtl && show_orderlist) ? orderlist_left : text_right;
//
//		int vehicle_button_x = rtl ? right - GetSpriteSize(SPR_PROFIT_LOT).width : left;

		int y = r.top;
		uint max = std::min<int>(this->vscroll->GetPosition() + this->vscroll->GetCapacity(), this->data.Length());
		for (uint i = this->vscroll->GetPosition(); i < max; ++i) {
			const OrderList *ol = this->data[i];
			StringID str;
			int text_offset = std::max<int>(GetSpriteSize(SPR_PROFIT_LOT).width, GetDigitWidth() * 3) + WD_FRAMERECT_RIGHT;
			int text_left  = left  + (rtl ?           0 : text_offset);
			int text_right = right - (rtl ? text_offset :           0);

			char buf[256];
			std::snprintf(buf, 256, "Orders list #%d\n", i);
			DrawString(text_left, text_right, y, buf);

			std::snprintf(buf, 256, "%d Vehicle(s)\n", ol->GetNumVehicles());
			DrawString(text_left, text_right, y + line_height - 2*FONT_HEIGHT_SMALL - WD_FRAMERECT_BOTTOM - 1, buf);

			std::snprintf(buf, 256, "%d Orders(s)\n", ol->GetNumOrders());
			DrawString(text_left, text_right, y + line_height - FONT_HEIGHT_SMALL - WD_FRAMERECT_BOTTOM - 1, buf);

			y += line_height;
		}
	}
};

void ShowOrdersListWindow()
{
	DeleteWindowByClass(WC_ORDERS_LIST);
	new OrdersListWindow();
}
