#include <iostream>
#include <cassert>

#include <cannon/log/registry.hpp>

using namespace cannon::log;

/*!
 * You are given the following information, but you may prefer to do some research for yourself.
 *
 *   1 Jan 1900 was a Monday.
 *
 *   Thirty days has September, April, June, and November. All the rest have
 *   thirty-one, saving February alone, which has twenty-eight, rain or shine.
 *   And on leap years, twenty-nine. 
 *
 *   A leap year occurs on any year evenly divisible by 4, but not on a century
 *   unless it is divisible by 400. 
 *
 * How many Sundays fell on the first of the month during the twentieth century
 * (1 Jan 1901 to 31 Dec 2000)?
 */

struct DayOfWeek {

  DayOfWeek() : day_num(0) {}

  DayOfWeek& operator++() {
    day_num += 1;
    if (day_num > 6)
      day_num = 0;

    return *this;
  }

  DayOfWeek operator++(int) {
    DayOfWeek ret_day = *this;

    day_num += 1;
    if (day_num > 6)
      day_num = 0;

    return ret_day;
  }

  unsigned int day_num;
};

std::ostream& operator<<(std::ostream& os, const DayOfWeek& d) {
  switch (d.day_num) {
    case 0:
      os << "Monday";
      break;
    case 1:
      os << "Tuesday";
      break;
    case 2:
      os << "Wednesday";
      break;
    case 3:
      os << "Thursday";
      break;
    case 4:
      os << "Friday";
      break;
    case 5:
      os << "Saturday";
      break;
    case 6:
      os << "Sunday";
      break;
    default:
      throw std::runtime_error("Should not get here");
  }

  return os;
}


struct Month {

  Month() : month_num(0) {}

  Month& operator++() {
    month_num += 1;
    if (month_num > 11)
      month_num = 0;

    return *this;
  }

  Month operator++(int) {
    Month ret_month = *this;

    month_num += 1;
    if (month_num > 11)
      month_num = 0;

    return ret_month;
  }

  /*!
   * \brief Get the number of days in this month.
   *
   * \param year The year to get number of days for. Only relevant for
   * February. 
   *
   * \returns The number of days in the month.
   */
  unsigned int get_num_days(unsigned int year) {

    switch (month_num) {
      case 0:
        return 31;
      case 1:
        if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0)
          return 29;
        else
          return 28;
      case 2:
        return 31;
      case 3:
        return 30;
      case 4:
        return 31;
      case 5:
        return 30;
      case 6:
        return 31;
      case 7:
        return 31;
      case 8:
        return 30;
      case 9:
        return 31;
      case 10:
        return 30;
      case 11:
        return 31;
      default:
        throw std::runtime_error("Should not get here");
    }
  }

  unsigned int month_num;
};

std::ostream& operator<<(std::ostream& os, const Month& m) {
  switch (m.month_num) {
    case 0:
      os << "January";
      break;
    case 1:
      os << "February";
      break;
    case 2:
      os << "March";
      break;
    case 3:
      os << "April";
      break;
    case 4:
      os << "May";
      break;
    case 5:
      os << "June";
      break;
    case 6:
      os << "July";
      break;
    case 7:
      os << "August";
      break;
    case 8:
      os << "September";
      break;
    case 9:
      os << "October";
      break;
    case 10:
      os << "November";
      break;
    case 11:
      os << "December";
      break;
  }

  return os;
}

struct Date {
  Date() : day(1), year(1900) {}

  Date& operator++() {
    ++day_of_the_week;

    if (day + 1 > month.get_num_days(year)) {
      ++month;
      if (month.month_num == 0)
        ++year;

      day = 1; 
    } else {
      ++day;
    }

    return *this;
  }

  Date operator++(int) {
    Date ret_day = *this;

    ++day_of_the_week;

    if (day + 1 > month.get_num_days(year)) {
      ++month;
      if (month.month_num == 0)
        ++year;

      day = 1; 
    } else {
      ++day;
    }

    return ret_day;
  }

  unsigned int day; //!< Day of the month
  Month month; //!< Month
  unsigned int year; //!< Year
  DayOfWeek day_of_the_week; //!< Day of the week
};

std::ostream& operator<<(std::ostream& os, const Date& d) {
  os << d.day << " " << d.month << ", " << d.year << " (" << d.day_of_the_week << ")";
  return os;
}

unsigned int get_num_first_sundays() {
  Date date;
  unsigned num_first_sundays = 0;

  while (date.year < 2001) {
    if (date.day_of_the_week.day_num == 6 && date.day == 1 && date.year >= 1901)
      num_first_sundays += 1;

    ++date;
    log_info(date);
  }

  return num_first_sundays;
}

int main(int argc, char** argv) {
  std::cout << get_num_first_sundays() << std::endl;
}
