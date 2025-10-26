#include <iostream>
#include <ctime>
#include <chrono>
#include <thread>
#include <iomanip>
#include <cmath>

using namespace std;

double julianDate(int year, int month, int day, int hour, int minute, int second);
double daysSinceJ2000(int year, int month, int day, int hour, int minute, int second);
void hoursToHMS(double hours, int& h, int& m, double& s);
double julianToGST(double JD);
double gstToLst(double gstHours, double longitudeDeg);
void displayClockWithSidereal(double longitudeDeg);

int main() {
    double longitudeDeg;
    cout << "Enter your longitude in degrees (east +, west -): ";
    cin >> longitudeDeg;

    displayClockWithSidereal(longitudeDeg);
    return 0;
}

double julianDate(int year, int month, int day, int hour, int minute, int second) {
    if (month <= 2) {
        year--;
        month += 12;
    }
    int A = year / 100;
    int B = 2 - A + (A / 4);
    double dayFraction = hour / 24.0 + minute / 1440.0 + second / 86400.0;
    double JD = int(365.25 * (year + 4716))
        + int(30.6001 * (month + 1))
        + day + dayFraction + B - 1524.5;
    return JD;
}

double daysSinceJ2000(int year, int month, int day, int hour, int minute, int second) {
    const double JD_J2000 = 2451545.0;
    double JD_now = julianDate(year, month, day, hour, minute, second);
    return JD_now - JD_J2000;
}

void hoursToHMS(double hours, int& h, int& m, double& s) {
    h = int(hours);
    double frac = (hours - h) * 60;
    m = int(frac);
    s = (frac - m) * 60;
}

double julianToGST(double JD) {
    double D = JD - 2451545.0;
    double gstDeg = 280.46061837 + 360.98564736629 * D;
    gstDeg = fmod(gstDeg, 360.0);
    if (gstDeg < 0) gstDeg += 360.0;
    return gstDeg / 15.0; // convert degrees to hours
}

double gstToLst(double gstHours, double longitudeDeg) {
    double lst = gstHours + longitudeDeg / 15.0;
    while (lst < 0) lst += 24;
    while (lst >= 24) lst -= 24;
    return lst;
}

void displayClockWithSidereal(double longitudeDeg) {
    while (true) {
        time_t now = time(nullptr);
        tm currentTime;

#ifdef _WIN32
        localtime_s(&currentTime, &now); // safe version for Windows
#else
        localtime_r(&now, &currentTime); // POSIX-safe version
#endif

        // Normal clock
        char timeBuffer[80];
        strftime(timeBuffer, sizeof(timeBuffer), "%Y-%m-%d %H:%M:%S", &currentTime);

        // Julian Date
        double JD = julianDate(
            currentTime.tm_year + 1900,
            currentTime.tm_mon + 1,
            currentTime.tm_mday,
            currentTime.tm_hour,
            currentTime.tm_min,
            currentTime.tm_sec
        );

        // Days since J2000
        double jdDays = JD - 2451545.0;

        // GST
        double gstHours = julianToGST(JD);
        int gstH, gstM; double gstS;
        hoursToHMS(gstHours, gstH, gstM, gstS);

        // LST
        double lstHours = gstToLst(gstHours, longitudeDeg);
        int lstH, lstM; double lstS;
        hoursToHMS(lstHours, lstH, lstM, lstS);

        // Display everything smoothly
        cout << "\r" << timeBuffer
            << " | Days since J2000: " << fixed << setprecision(6) << jdDays
            << " | GST: " << gstH << ":" << gstM << ":" << fixed << setprecision(2) << gstS
            << " | LST: " << lstH << ":" << lstM << ":" << fixed << setprecision(2) << lstS
            << flush;

        this_thread::sleep_for(chrono::seconds(1));
    }
}