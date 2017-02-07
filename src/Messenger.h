#ifndef _MESSENGER_H_
#define _MESSENGER_H_


class Date
{
    Date(char day, char month, char year):
        day(day), month(month), year(year)
    { }

    char getDay() const { return day; }
    char getMonth() const { return month; }
    char getYear() const { return year; }
private:
    char day;
    char month;
    char year;
};

class Contact
{
private:
    Date m_date;
};



#endif
