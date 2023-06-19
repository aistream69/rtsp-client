/**********
This library is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the
Free Software Foundation; either version 2.1 of the License, or (at your
option) any later version. (See <http://www.gnu.org/copyleft/lesser.html>.)

This library is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
more details.

You should have received a copy of the GNU Lesser General Public License
along with this library; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
**********/
/* Copyright (C) 2011 Zack Xue <zackxue@163.com> */


#include "playerEnv.h"
#include "MediaSink.hh"
#include "BasicUsageEnvironment.hh"
#include "GroupsockHelper.hh"

playerBasicUsageEnvironment::playerBasicUsageEnvironment(TaskScheduler& taskScheduler)
: BasicUsageEnvironment(taskScheduler) {

}

playerBasicUsageEnvironment::~playerBasicUsageEnvironment() {
}

playerBasicUsageEnvironment*
playerBasicUsageEnvironment::createNew(TaskScheduler& taskScheduler) {
    return new playerBasicUsageEnvironment(taskScheduler);
}

UsageEnvironment& playerBasicUsageEnvironment::operator<<(char const* str) {
	printf("%s", str);
	return *this;
}

UsageEnvironment& playerBasicUsageEnvironment::operator<<(int i) {
	printf("%d", i);
	return *this;
}

UsageEnvironment& playerBasicUsageEnvironment::operator<<(unsigned u) {
	printf("%u", u);
	return *this;
}

UsageEnvironment& playerBasicUsageEnvironment::operator<<(double d) {
	printf("%f", d);
	return *this;
}

UsageEnvironment& playerBasicUsageEnvironment::operator<<(void* p) {
	printf("%p", p);
	return *this;
}


