Sidereal Clock Console Application

A C++ console application that displays a live, smooth, flicker-free clock with Local Time (system time), Days since J2000.0, Greenwich Sidereal Time (GST) and Local Sidereal Time (LST) based on user longitude.

This tool is useful for astronomical calculations, tracking sidereal time or understanding time in astronomical contexts.

Features

Smooth live display: Updates every second without flickering.

Julian date conversion: Shows precise fractional days since J2000.0.

Sidereal time calculations: Both GST and LST are computed and displayed.

Cross-platform: Works on Windows, Linux, and macOS. Uses localtime_s on Windows and localtime_r on POSIX systems.

Prerequisites

C++ compiler with C++11 support or higher (for <chrono> and <thread>).

Standard C++ libraries: <iostream>, <ctime>, <chrono>, <thread>, <iomanip>, <cmath>.

Compile the code:

Linux/macOS (g++ example):
