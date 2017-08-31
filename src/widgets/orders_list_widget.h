/* $Id$ */

/*
 * This file is part of OpenTTD.
 * OpenTTD is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 2.
 * OpenTTD is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details. You should have received a copy of the GNU General Public License along with OpenTTD. If not, see <http://www.gnu.org/licenses/>.
 */

/** @file orders_list_widget.h Types related to the order list widgets. */

#ifndef WIDGETS_ORDERS_LIST_WIDGET_H
#define WIDGETS_ORDERS_LIST_WIDGET_H

/** Widgets of the #OrdersListWindow class. */
enum OrdersListWidgets {
	WID_OL_SCROLLING_TEXT, ///< The actually scrolling text.
	WID_OL_WEBSITE,        ///< URL of OpenTTD website.
	WID_OL_LIST,           ///< List of Orders list
	WID_OL_SCROLLBAR,      ///< Scrollbar Orders list
};

#endif /* WIDGETS_ORDERS_LIST_WIDGET_H */
