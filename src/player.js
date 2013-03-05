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

const Gtk = imports.gi.Gtk;
const Lang = imports.lang;


const Player = new Lang.Class({
	Name: "Player",
	
	_init: function(){		
		this.playlist = new Array();
	},
	
	getCurrentSongId: function(){
	},
	
	getPreviousSongId: function(){
	},
	
	getNextSongId: function(){
	},
	
	play: function(){
	},
	
	pause: function() {
	},
	
	stop: function(){
	},
	
	getPlaylist: function(){
		return this.playlist;
	},
	
	addToPlaylist: function(order){
	},
	
	appendToPlaylist: function(){
	},
	
	removeFromPlaylistById: function(){
	},
	
});
