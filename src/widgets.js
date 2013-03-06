/*
 * Copyright (c) 2013 Gnome.
 *
 * Gnome Music is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by the
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
 * Author: Eslam Mostafa <cseslam@gmail.com>
 *
 */

const Lang = imports.lang;
const Gtk = imports.gi.Gtk;
const Gio = imports.gi.Gio;

const Scale = new Lang.Class({
    Name: "Scale",
    Extends: Gtk.HScale,
    
    _init: function(){
        this.parent();
        this.set_digits(0);
        this.set_value_pos(Gtk.PositionType.RIGHT);
        this.set_range(0.0,4.0);
        this.set_value(0);
    },
    
    setRange: function(maxValue){
        this.set_range(0, maxValue);
    },
    
});

const ToolbarButton = new Lang.Class({
    Name: "ToolbarButton",
    Extends: Gtk.Button,
    
    _init: function(label, iconName, relief){
        this.parent();
        if(label != null){
            this.set_label(label);
        } else {
            this.set_image(new Gtk.Image.new_from_icon_name(iconName, Gtk.IconSize.BUTTON));            
        }
        this.set_size_request(34, 34);        
        this.set_can_focus(false);
        if ( relief === false ) {
            this.set_relief(Gtk.ReliefStyle.NONE);
        }
    },
});

const SymbolicToolButton = new Lang.Class({
    Name: "SymbolicToolButton",
    Extends: Gtk.Button,
    
    _init: function(iconName, setRelief){
        this.parent();
        //let icon = Gio.ThemedIcon.new_with_default_fallbacks(iconName);
        //let image = new Gtk.Image();
        //image.set_from_gicon(icon, Gtk.IconSize.MENU);
        //image.show();
        //this.add(image);
        this.set_image(new Gtk.Image.new_from_icon_name(iconName, Gtk.IconSize.BUTTON));
        this.set_size_request(34, 34);
        this.set_can_focus(false);
        if ( setRelief === false ) {
            this.set_relief(Gtk.ReliefStyle.NONE);
        }
     },
});


const SymbolicToggleButton = new Lang.Class({
    Name: "SymbolicToggleButton",
    Extends: Gtk.ToggleButton,
    
    _init: function(iconName){
        this.parent();
        
        let icon = Gio.ThemedIcon.new_with_default_fallbacks(iconName);
        let image = new Gtk.Image();
        image.set_from_gicon(icon, Gtk.IconSize.MENU);
        image.show();
        this.add(image);
        this.set_size_request(34, 34);
    },
});



