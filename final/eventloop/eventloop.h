/***********************************************************************
* eventloop.h - Implements the event loop                              *
*                                                                      *
* This file is part of the FINAL CUT widget toolkit                    *
*                                                                      *
* Copyright 2023 Andreas Noe                                           *
*                                                                      *
* FINAL CUT is free software; you can redistribute it and/or modify    *
* it under the terms of the GNU Lesser General Public License as       *
* published by the Free Software Foundation; either version 3 of       *
* the License, or (at your option) any later version.                  *
*                                                                      *
* FINAL CUT is distributed in the hope that it will be useful, but     *
* WITHOUT ANY WARRANTY; without even the implied warranty of           *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        *
* GNU Lesser General Public License for more details.                  *
*                                                                      *
* You should have received a copy of the GNU Lesser General Public     *
* License along with this program.  If not, see                        *
* <http://www.gnu.org/licenses/>.                                      *
***********************************************************************/

/*  Standalone class
 *  ════════════════
 *
 * ▕▔▔▔▔▔▔▔▔▔▔▔▏
 * ▕ EventLoop ▏
 * ▕▁▁▁▁▁▁▁▁▁▁▁▏
 */

#ifndef EVENTLOOP_H
#define EVENTLOOP_H

#include <list>
#include <poll.h>
#include <array>

#include "monitor.h"

namespace finalcut
{

class EventLoop
{
  public:
    // Constructor
    EventLoop() = default;

    // Methods
    auto run() -> int;
    void leave();

  private:
    // Constants
    static constexpr nfds_t MAX_MONITORS{50};
    static constexpr int WAIT_INDEFINITELY{-1};

    // Methods
    void addMonitor (Monitor*);
    void removeMonitor (Monitor*);

    // Data members
    bool running{false};
    bool monitors_changed{false};
    std::list<Monitor*>  monitors{};
    std::array<struct pollfd, MAX_MONITORS> fds{};
    std::array<Monitor*, MAX_MONITORS>      lookup_table{};

    // Friend classes
    friend class Monitor;
};

}  // namespace finalcut

#endif  // EVENTLOOP_H
