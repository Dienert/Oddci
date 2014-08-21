//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

package oddci.neds;

//
// Generates messages with a configurable interarrival time.
//
simple Provider
{
    parameters:
        //        volatile double sendInterval @unit(s) = default(exponential(10s));
        @display("i=device/server;is=l");
        string providerAddress = default("192.168.0.1");
        string controller1Address = default("192.168.0.2");
        string controller2Address = default("192.168.0.2");
        
}
