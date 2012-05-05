/*
Copyright_License {

  XCSoar Glide Computer - http://www.xcsoar.org/
  Copyright (C) 2000-2012 The XCSoar Project
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

#ifndef XCSOAR_LOGGER_FRECORD_HPP
#define XCSOAR_LOGGER_FRECORD_HPP

#include "GPSClock.hpp"
#include "NMEA/Info.hpp"

struct BrokenTime;

class LoggerFRecord
{
  GPSClock clock;
  bool update_needed;

  bool satellite_ids_available;
  int satellite_ids[GPSState::MAXSATELLITES];

public:
  LoggerFRecord()
    :clock(fixed_270 /* 4.5 minutes */) {}

  /**
   * Returns true if the IGCWriter is supposed to write a new F record to
   * the IGC file or false if no update is needed.
   */
  bool Update(const GPSState &gps, fixed time, bool nav_warning);

  void Reset();
};

#endif
