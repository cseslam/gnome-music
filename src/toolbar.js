/*
 * Copyright (c) 2013 Next Tuesday GmbH.
 *               Authored by: Seif Lotfy <sfl@nexttuesday.de>
 * Copyright (c) 2013 Seif Lotfy <seif@lotfy.com>.
 *
 * Gnome Music is free software; you can Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * Gnome Music is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with Gnome Music; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

const Lang = imports.lang;

const Gio = imports.gi.Gio;
const GLib = imports.gi.GLib;
const Gtk = imports.gi.Gtk;
const Gd = imports.gi.Gd;
const Pango = imports.gi.Pango;
const Signals = imports.signals;

const Gettext = imports.gettext;
const _ = imports.gettext.gettext;

const Searchbar = imports.searchbar;

const ToolbarState = {
    SINGLE: 0,
    ALBUMS: 1,
    ARTISTS: 2,
    PLAYLISTS: 3,
    SONGS: 4};

const Toolbar = new Lang.Class({
    Name: 'MainToolbar',
    Extends: Gd.HeaderBar,

    _init: function() {
        this.parent();
        this._stack_switcher = new Gd.StackSwitcher ();
        this.set_custom_title (null);
        this._addBackButton();
    },

    set_stack: function(stack) {
        this._stack_switcher.set_stack (stack);
    },

    get_stack: function() {
        return this._stack_switcher.get_stack();
    },

    setState: function (state) {
        if (state == ToolbarState.SINGLE) {
            this.custom_title = null
            this._backButton.show()
        }
        else {
            this.title = ""
            this.custom_title = this._stack_switcher;
            this._backButton.hide()
        }
        this.emit ("state-changed")
    },

    _addBackButton: function() {
        let iconName =
            (this.get_direction() == Gtk.TextDirection.RTL) ?
            'go-next-symbolic' : 'go-previous-symbolic';
        this._backButton = new Gd.HeaderSimpleButton({ symbolic_icon_name: iconName,
                                                     label: _("Back") });
        this._backButton.connect('clicked', Lang.bind(this, this.setState))
        this.pack_start(this._backButton);
    }
});
Signals.addSignalMethods(Toolbar.prototype);
