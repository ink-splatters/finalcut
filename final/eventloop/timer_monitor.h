/***********************************************************************
* timer_monitor.h - Time monitoring object                             *
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

/*  Inheritance diagram
 *  ═══════════════════
 *
 *      ▕▔▔▔▔▔▔▔▔▔▏
 *      ▕ Monitor ▏
 *      ▕▁▁▁▁▁▁▁▁▁▏
 *           ▲
 *           │
 *    ▕▔▔▔▔▔▔▔▔▔▔▔▔▔▔▏
 *    ▕ TimerMonitor ▏
 *    ▕▁▁▁▁▁▁▁▁▁▁▁▁▁▁▏
 */

#ifndef TIMER_MONITOR_H
#define TIMER_MONITOR_H

#include <time.h>

#include <array>
#include <chrono>

#include "ftypes.h"
#include "monitor.h"

namespace finalcut
{

constexpr auto getTimerClass() -> char*
{
  #if defined(__APPLE__) && defined(__MACH__)
    return C_STR("kqueue-timer");
  #elif defined(__OpenBSD__)
    return C_STR("kqueue-timer");
  #else
    return C_STR("posix-timer");
  #endif
}


class TimerMonitor final : public Monitor
{
  public:
    TimerMonitor() = delete;

    // Disable copy constructor
    TimerMonitor (const TimerMonitor&) = delete;

    // Disable move constructor
    TimerMonitor (TimerMonitor&&) noexcept = delete;

    // Constructor
    explicit TimerMonitor (EventLoop*);

    // Destructor
    ~TimerMonitor() noexcept override;

    // Disable copy assignment operator (=)
    auto operator = (const TimerMonitor&) -> TimerMonitor& = delete;

    // Disable move assignment operator (=)
    auto operator = (TimerMonitor&&) noexcept -> TimerMonitor& = delete;

    // Methods
    void init (handler_t, void*);
    void setInterval ( std::chrono::nanoseconds
                     , std::chrono::nanoseconds );
    void trigger(short) override;

  private:
    // Data members
    timer_t timer_id{};
    std::array<int, 2> alarm_pipe_fd{-1, -1};
};

}  // namespace finalcut

#endif  // TIMER_MONITOR_H
