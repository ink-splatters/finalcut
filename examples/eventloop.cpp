/***********************************************************************
* eventloop.cpp - Example of using the event loop                      *
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

#include <fcntl.h>
#include <termios.h>
#include <unistd.h>

#include <cstdio>
#include <iostream>

#include <final/final.h>

finalcut::EventLoop Loop{};

struct termios OriginalTermIoSettings{};

//----------------------------------------------------------------------
void onExit()
{
  // Restore terminal control
  tcsetattr (STDIN_FILENO, TCSAFLUSH, &OriginalTermIoSettings);
  std::cout << "Bye!" << std::endl;
}


//----------------------------------------------------------------------
//                               main part
//----------------------------------------------------------------------

auto main() -> int
{
  finalcut::TimerMonitor timer1{&Loop};
  finalcut::TimerMonitor timer2{&Loop};
  finalcut::SignalMonitor sig_int_monitor{&Loop};
  finalcut::SignalMonitor sig_abrt_monitor{&Loop};
  finalcut::IoMonitor stdin_monitor{&Loop};

  // Save terminal setting and set terminal to raw mode
  // (no echo, no line buffering).
  tcgetattr (STDIN_FILENO, &OriginalTermIoSettings);
  atexit (onExit);
  struct termios new_term_io_settings{OriginalTermIoSettings};
  new_term_io_settings.c_lflag &= ~(ECHO | ICANON);
  tcsetattr (STDIN_FILENO, TCSAFLUSH, &new_term_io_settings);

  // Set file descriptor of stdin to non-blocking mode
  int stdin_flags{fcntl(STDIN_FILENO, F_GETFL, 0)};
  (void)fcntl(STDIN_FILENO, F_SETFL, stdin_flags | O_NONBLOCK);

  // Configure monitors
  timer1.init ( [] (const finalcut::Monitor*, short)
                {
                   std::cout << "Tick" << std::endl;
                }
                , nullptr );

  timer2.init ( [] (const finalcut::Monitor*, short)
                {
                  std::cout << "Tock" << std::endl;
                }
                , nullptr );

  timer1.setInterval ( std::chrono::nanoseconds{ 500'000'000 }
                     , std::chrono::nanoseconds{ 1'000'000'000 } );

  timer2.setInterval ( std::chrono::nanoseconds{ 1'000'000'000 }
                     , std::chrono::nanoseconds{ 1'000'000'000 } );

  sig_int_monitor.init ( SIGINT
                       , [] (const finalcut::Monitor*, short)
                         {
                           std::cout << "Signal SIGINT received."
                                     << std::endl;
                           Loop.leave();
                         }
                         , nullptr );

  sig_abrt_monitor.init ( SIGABRT
                        , [] (const finalcut::Monitor*, short)
                        {
                          std::cout << "Signal SIGABRT received."
                                    << std::endl;
                          Loop.leave();
                        }
                        , nullptr );

  stdin_monitor.init ( STDIN_FILENO
                     , POLLIN
                     , [] (const finalcut::Monitor* monitor, short)
                       {
                         uint8_t Char{0};
                         const ssize_t bytes = ::read( monitor->getFd(), &Char, 1);

                         if ( bytes > 0 )
                           std::cout << "typed in: '" << Char << "'"
                                     << std::endl;
                       },
                       nullptr );

  // Start monitors
  timer1.resume();
  timer2.resume();
  sig_int_monitor.resume();
  sig_abrt_monitor.resume();
  stdin_monitor.resume();

  // Monitoring
  return Loop.run();
}
