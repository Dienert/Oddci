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

#ifndef ODDCIEXCEPTION_H_
#define ODDCIEXCEPTION_H_

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

class OddciException : public std::runtime_error {
    std::string msg;
public:
    OddciException(const std::string &arg, const char *file, int line) :
        std::runtime_error(arg) {
        std::ostringstream o;
        o << file << ":" << line << ": " << arg;
        msg = o.str();
    }
    virtual ~OddciException() throw() {}
    const char *what() const throw() {
        return msg.c_str();
    }
};


#endif /* ODDCIEXCEPTION_H_ */
