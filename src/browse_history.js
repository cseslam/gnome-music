/*/*
 * Copyright (c) 2013 Eslam Mostafa.
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
 * Author: Eslam Mostafa <cseslam@gmail.com>
 *
 */

const Lang = imports.lang;

const BrowseHistory = new Lang.Class({
    Name: "BrowseHistory",
    
    _init: function(){
        this.history = new Array();
        this.history_types = {};
    },
    
    pushItem: function(id, type){
        this.history.push(id);
        this.history_types.id = type;
        
        this.changed();
    },
    
    get_last_item_id: function (){
        if(history.length >= 2) {
            let last = history.length - 2;
            return history[last];
        }
        
        return "";
    },
    
    get_last_item_type: function (){
        id = this.get_last_item_id();
        return this.history_types[id];
    },
    
    delete_last_item: function() {
        // We want to go back one position in the history, so we only need to
        // delete the actual item (history.size - 1).
        let last = this.history.length - 1;
        let id = get_last_item_id();
        this.history.splice(last, 1);
        delete this.history_types.id;
        
        this.changed();
    },
    
    get_length: function() {
        return this.history.length;
    },
    
    clear: function() {
        this.history.length = 0;
        this.history_types.length = 0;
        
        this.changed();
    },
});
