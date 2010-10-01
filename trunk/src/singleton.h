/***************************************************************************
 *   Copyright (C) 2010 by Pavel Andreev                                   *
 *   Mail: apavelm on gmail point com (apavelm@gmail.com)                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, you can read it                      *
 *   here <http://www.gnu.org/licenses/>, or write to the                  *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef SINGLETON_H
#define SINGLETON_H

template<typename T>
class Singleton {
public:
	Singleton() { }
	virtual ~Singleton() { }

	static T* getInstance() {
                if (instance)
                    return instance;
                else
                    return 0;
	}

	static void newInstance() {
		if(instance)
			delete instance;

		instance = new T();
	}

	static void deleteInstance() {
		if(instance)
			delete instance;
                instance = 0;
	}
protected:
        static T* volatile instance;
private:
	Singleton(const Singleton&);
	Singleton& operator=(const Singleton&);

};

template<class T> T* volatile Singleton<T>::instance = 0;

#endif // SINGLETON_H
