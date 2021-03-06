/*
Copyright_License {

  XCSoar Glide Computer - http://www.xcsoar.org/
  Copyright (C) 2000-2014 The XCSoar Project
  A detailed list of copyright holders can be found in the file "AUTHORS".

  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License
  as published by the Free Software Foundation; either version 2
  of the License, or (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
}
*/

#include "Net/Session.hpp"

#include <algorithm>
#include <assert.h>
#include <sys/select.h>

bool
Net::Session::Select(int timeout_ms)
{
  fd_set rfds, wfds, efds;
  FD_ZERO(&rfds);
  FD_ZERO(&wfds);
  FD_ZERO(&efds);

  int max_fd;

  if (!multi.FdSet(&rfds, &wfds, &efds, &max_fd))
    return false;

  bool using_curl_timeout = false;
  long curl_timeout = multi.GetTimeout();
  if (curl_timeout >= 0) {
    if (curl_timeout < 50)
      curl_timeout = 50;

    if (timeout_ms < 0 || curl_timeout < (long)timeout_ms) {
      timeout_ms = curl_timeout;
      using_curl_timeout = true;
    }
  }

  struct timeval timeout, *timeout_p;
  if (timeout_ms >= 0) {
    timeout.tv_sec = timeout_ms / 1000;
    timeout.tv_usec = (timeout_ms % 1000) * 1000;
    timeout_p = &timeout;
  } else
    timeout_p = NULL;

  int ret = select(max_fd + 1, &rfds, &wfds, &efds, timeout_p);
  return ret > 0 || (using_curl_timeout && ret == 0);
}
