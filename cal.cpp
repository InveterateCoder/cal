#undef UNICODE
#undef _UNICODE
#include<Windows.h>
#include<iostream>
using namespace std;

HANDLE hConOut;
SYSTEMTIME st;
CONSOLE_SCREEN_BUFFER_INFO csbi;
COORD coord;
bool sw;
char line[]{ "Su Mo Tu We Th Fr Sa" };
char* months[]{ "January","February","March","April","May",
"June","July","August","September","October","November","December" };

int GetDaysInMonth();
void WriteMonth();

int main(int argc, char** argv)
{
	FILETIME ft;
	SYSTEMTIME lst, locstat;
	char buf[20];
	int err = 0;

	ZeroMemory(&lst, sizeof(SYSTEMTIME));
	hConOut = GetStdHandle(STD_OUTPUT_HANDLE);
	
	switch (argc)
	{
	case 1:
		GetLocalTime(&st);
		wsprintf(buf, "%s %u", months[st.wMonth - 1], st.wYear);
		for (int i = 10 - strlen(buf) / 2 - strlen(buf) % 2; i > 0; i--)
			cout << ' ';
		cout << buf << endl << line << endl;
		GetConsoleScreenBufferInfo(hConOut, &csbi);
		coord = csbi.dwCursorPosition;
		sw = true;
		WriteMonth();
		cout << endl;
		break;
	case 2:
		if (strlen(argv[1]) != 2 || argv[1][0] != '-' || argv[1][1] != 'y')
		{
			err = 1;
			break;
		}
		else
		{
			GetLocalTime(&locstat);
			wsprintf(buf, "%u", locstat.wYear);
			for (int i = 16 - strlen(buf) / 2; i > 0; i--)
				cout << "  ";
			cout << buf << endl;
			GetConsoleScreenBufferInfo(hConOut, &csbi);

			lst.wDay = 1;
			lst.wYear = locstat.wYear;
			lst.wMonth = 1;
			for (int i = 0, c(0); i < 12; i += 3, c++)
			{
				for (int n(0); n < 3; n++)
				{
					coord.Y = csbi.dwCursorPosition.Y + c * 9;
					coord.X = csbi.dwCursorPosition.X + n * 22;
					SetConsoleCursorPosition(hConOut, coord);
					for (int tmp = 10 - strlen(months[i + n])/2 - strlen(months[i+n])%2; tmp > 0; tmp--)
						cout << ' ';
					cout << months[i + n];
					coord.Y++;
					SetConsoleCursorPosition(hConOut, coord);
					cout << line;
					coord.Y++;
					SetConsoleCursorPosition(hConOut, coord);
					if (lst.wMonth == locstat.wMonth)
					{
						GetLocalTime(&st);
						sw = true;
						WriteMonth();
						sw = false;
						lst.wMonth++;
					}
					else
					{
						SystemTimeToFileTime(&lst, &ft);
						FileTimeToSystemTime(&ft, &st);
						WriteMonth();
						lst.wMonth++;
					}
				}
			}
			cout << endl;
		}
	}
	if (err)
	{
		switch (err)
		{
		case 1:
			cout << "cal: invalid option " << argv[1] << endl <<
				"Usage:\tcal\n\tcal -y" << endl;
			break;
		}
	}
	return 0;
}

int GetDaysInMonth()
{
	int year = st.wYear, month = st.wMonth;
	if (month == 4 || month == 6 || month == 9 || month == 11)
		return 30;

	else if (month == 2)
	{
		bool leapyear = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);

		if (leapyear == 0)
			return 28;
		else
			return 29;
	}
	else
		return 31;
}
void WriteMonth()
{
	SetConsoleCursorPosition(hConOut, coord);
	int day = st.wDay, dayOfWeek = st.wDayOfWeek;
	int days = GetDaysInMonth();
	int c = 0;
	while (true)
	{
		if (day > 1)
			day -= 7;
		else
		{
			if (day < 1)
				day--;
			break;
		}
	}
	switch (day)
	{
	case 1:
		for (int i(dayOfWeek); i > 0; i--, c++)
			cout << "   ";
		for (int i(1); i <= days; i++, c++)
		{
			if (c && !(c % 7))
			{
				coord.Y++;
				SetConsoleCursorPosition(hConOut, coord);
			}
			if (i == st.wDay && sw)
			{
				SetConsoleTextAttribute(hConOut, csbi.wAttributes | COMMON_LVB_REVERSE_VIDEO);
				if (i < 10)
					cout << ' ' << i;
				else
					cout << i;
				SetConsoleTextAttribute(hConOut, csbi.wAttributes);
				cout << ' ';
			}
			else
			{
				if (i < 10)
					cout << ' ' << i << ' ';
				else
					cout << i << ' ';
			}
		}
		break;
	default:
		day *= -1;
		for (int i((day + dayOfWeek) % 7); i > 0; i--, c++)
			cout << "   ";
		for (int i(1); i <= days; i++, c++)
		{
			if (c && !(c % 7))
			{
				coord.Y++;
				SetConsoleCursorPosition(hConOut, coord);
			}
			if (i == st.wDay && sw)
			{
				SetConsoleTextAttribute(hConOut, csbi.wAttributes | COMMON_LVB_REVERSE_VIDEO);
				if (i < 10)
					cout << ' ' << i;
				else
					cout << i;
				SetConsoleTextAttribute(hConOut, csbi.wAttributes);
				cout << ' ';
			}
			else
			{
				if (i < 10)
					cout << ' ' << i << ' ';
				else
					cout << i << ' ';
			}
		}
	}
}