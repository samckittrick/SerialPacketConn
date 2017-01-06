/* 
   Library for Packet-Based serial communication.

    Copyright (C) 2017 Scott McKittrick

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

   
   Packet Structure:
   ---------------------------------------------------------
   | Overhead Byte | Data | Checksum Byte | Frame End Byte |
   ---------------------------------------------------------
   

   Created By: Scott McKittrick Dec. 31st 2016
   
 */
