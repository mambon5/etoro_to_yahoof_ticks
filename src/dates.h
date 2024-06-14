
// C++ program two find number of  
// days between two given dates 
#include <iostream> 
#include <chrono>


using namespace std; 

// A date has day 'd', month 'm' and year 'y' 
struct Date { 
    int d, m, y; 
}; 
  
// To store number of days in  
// all months from January to Dec. 
const int monthDays[12] 
    = { 31, 28, 31, 30, 31, 30,  
       31, 31, 30, 31, 30, 31 }; 
  
// This function counts number of  
// leap years before the given date 
int countLeapYears(Date d) 
{ 
    int years = d.y; 
  
    // Check if the current year needs to be 
    //  considered for the count of leap years 
    // or not 
    if (d.m <= 2) 
        years--; 
  
    // An year is a leap year if it  
    // is a multiple of 4, 
    // multiple of 400 and not a  
     // multiple of 100. 
    int leapy = years / 4  - years / 100 + years / 400; 
    return leapy;
} 

long int comptaDies(Date date) {
    // initialize count using years and day 
    long int n1 = date.y * 365 + date.d; 
  
    // Add days for months in given date 
    for (int i = 0; i < (date.m - 1); i++) 
        n1 += monthDays[i]; 
  
    // Since every leap year is of 366 days, 
    // Add a day for every leap year 
    n1 += countLeapYears(date); 
    return n1;
}
  
// This function returns number of  
// days between two given dates 
int calcularDiesEntreDates(Date dt1, Date dt2) 
{ 
    // COUNT TOTAL NUMBER OF DAYS 
    // BEFORE FIRST DATE 'dt1' 
  
    long int n1 = comptaDies(dt1);
  
    // SIMILARLY, COUNT TOTAL NUMBER OF 
    // DAYS BEFORE 'dt2' 
  
    long int n2 = comptaDies(dt2);
  
    // return difference between two counts 
    return (n2 - n1); 
}

int DiesEntreDates(const string & date1, const string & date2) // dates in format "yyyy-mm-dd" 
{ 
    Date dt1 = { stoi(date1.substr(8,2)), 
    stoi(date1.substr(5,2)), stoi(date1.substr(0,4)) }; 
    Date dt2 = { stoi(date2.substr(8,2)), 
    stoi(date2.substr(5,2)), stoi(date2.substr(0,4)) }; 

    return calcularDiesEntreDates(dt1, dt2); 
}


string getCurrentDate() {
    // Obtener la fecha y hora actual
    time_t now = time(0);
    struct tm *currentTime = localtime(&now);

    // Formatear la fecha en el formato deseado
    char buffer[11];  // Suficiente espacio para "YYYY-MM-DD" y el carácter nulo
    strftime(buffer, sizeof(buffer), "%Y-%m-%d", currentTime);
    return buffer;
}

