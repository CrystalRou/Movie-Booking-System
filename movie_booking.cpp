#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <sstream>
using namespace std;

struct MemberRecord
{
   char email[ 40 ];
   char password[ 24 ];
   char IDNumber[ 12 ];
   char name[ 12 ];
   char phone[ 12 ];
};

struct Movie
{
   int movieCode;
   int prices[ 4 ]; // prices[0]:adult, prices[1]:concession, prices[2]:disability, prices[3]:elderly
   bool dates[ 9 ];
   bool sessionTimes[ 17 ];
   bool occupiedSeats[ 9 ][ 17 ][ 8 ][ 12 ]; // occupiedSeats[i][j] is the occupied Seats for all accounts
};                                           // at j-th session time on i-th date

struct BookingInfo
{
   char email[ 40 ];
   int movieCode;
   int dateCode;
   int sessionTimeCode;
   int numTickets[ 4 ]; // numTickets[0]: the number of adult tickets,
                        // numTickets[1]: the number of concession tickets,
                        // numTickets[2]: the number of disability tickets,
                        // numTickets[3]: the number of elderly tickets
   char seletedSeats[ 24 ][ 4 ]; // seleted seats for the user with the specified email
};

char hours[ 17 ][ 8 ] = { "09:00", "10:00", "11:00", "12:00", "13:00", "14:00", "15:00", "16:00", "17:00",
                          "18:00", "19:00", "20:00", "21:00", "22:00", "23:00", "00:00", "01:00" };

void loadMemberInfo( MemberRecord memberDetails[], int &numMembers );
int inputAnInteger( int begin, int end );
void signIn( MemberRecord memberDetails[], int numMembers, Movie movies[], char movieNames[][ 60 ], int numMovies,
             BookingInfo bookingHistories[], int &numBookings, char availableDates[][ 12 ], int numDates );
bool illegal( char email[], char password[], MemberRecord memberDetails[], int numMembers, int &recordNumber );
void accountInfor( MemberRecord memberDetails[], int numMembers, int recordNumber );
void newMember( MemberRecord memberDetails[], int &numMembers );
bool existingID( char newIDNumber[], MemberRecord memberDetails[], int &numMembers );
bool existingEmail( char newEmail[], MemberRecord memberDetails[], int &numMembers );

void loadBookingHistories( BookingInfo bookingHistories[], int &numBookings );
void loadAvailableDates( char availableDates[][ 12 ], int &numDates );
void loadMovies( Movie movies[], int &numMovies );
void loadMovieNames( char movieNames[][ 60 ], int numMovies );
void buyTickets( BookingInfo bookingHistories[], int &numBookings, Movie movies[], char movieNames[][ 60 ],
               int numMovies, char availableDates[][ 12 ], int numDates, char email[] );
void selectSeats( BookingInfo bookingHistories[], int numBookings, Movie movies[] );
void display( Movie movies[], BookingInfo bookingHistories[], int numbooking);
void displaySessionTimes( Movie movies[], char movieNames[][ 60 ], int numMovies,
                          char availableDates[][ 12 ], int numDates );
void displayBookingHistory( BookingInfo bookingHistories[], int numBookings, Movie movies[],
                            char movieNames[][ 60 ], char availableDates[][ 12 ], char email[] );
void saveMemberInfo( MemberRecord memberDetails[], int numMembers );
void saveMovies( Movie movies[], int numMovies );
void saveBookingHistories( BookingInfo bookingHistories[], int numBookings );

int main()
{
   MemberRecord memberDetails[ 100 ] = {};
   int numMembers = 0;
   loadMemberInfo( memberDetails, numMembers );

   BookingInfo bookingHistories[ 1000 ] = {};
   int numBookings = 0;
   loadBookingHistories( bookingHistories, numBookings );

   Movie movies[ 30 ] = {};
   int numMovies = 0;
   loadMovies( movies, numMovies );

   char movieNames[ 30 ][ 60 ] = {};
   loadMovieNames( movieNames, numMovies );

   char availableDates[ 10 ][ 12 ];
   int numDates = 0;
   loadAvailableDates( availableDates, numDates );

   cout << "Welcome to Vieshow Cinemas Taipei QSquare system\n\n";

   int choice;

   while( true )
   {
      cout << "Enter your choice:" << endl;
      cout << "1. Sign In\n";
      cout << "2. New Member\n";
      cout << "3. End\n? ";


      choice = inputAnInteger( 1, 3 );
      cout << endl;

      switch ( choice )
      {
      case 1:
         signIn( memberDetails, numMembers, movies, movieNames, numMovies, bookingHistories, numBookings,
                 availableDates, numDates );
         break;

      case 2:
         newMember( memberDetails, numMembers );
         break;

      case 3:
         saveMemberInfo( memberDetails, numMembers );
         saveMovies( movies, numMovies );
         saveBookingHistories( bookingHistories, numBookings );
         cout << "Thank you...\n\n";
         system( "pause" );
         return 0;

      default:
         cout << "Input Error!\n\n";
         break;
      }
   }
  
   system( "pause" );
}

void loadMemberInfo(MemberRecord memberDetails[], int &numMembers)
{
	ifstream inMemberFile("Member Info.dat", ios::in | ios::binary);

	if (!inMemberFile)
	{
		cerr << "File could not be opened." << endl;
	}

	numMembers = 0;
	while (!inMemberFile.eof())
	{
		inMemberFile.read(reinterpret_cast<char *>(&memberDetails[numMembers]), sizeof(MemberRecord));
		numMembers++;
	}
	inMemberFile.close();

}
int inputAnInteger(int begin, int end)
{
	string numberstr;
	int number;
	cin >> numberstr;
	int choicemovie = 0;
	stringstream sn;
	sn.str("");
	sn.clear();

	if ((sn << numberstr && sn >> number) || number >= begin || number <= end)
		return number;
	else
		cout << "Inpur Error" << endl;
		return 0;
}
void signIn(MemberRecord memberDetails[], int numMembers, Movie movies[], char movieNames[][60], int numMovies, BookingInfo bookingHistories[], int &numBookings, char availableDates[][12], int numDates)
{
	int record = 0;
	cout << "Enter email address : ";
	char address[40];
	cin >> address;
	cout << "Enter password : ";
	char password[24];
	cin >> password;
	cout << endl;

	for (int i = 0; address[i] != '\0'; i++)
		bookingHistories[numBookings].email[i] = address[i];
	
	if (illegal(address, password, memberDetails, numMembers, record))
	{
		while (true)
		{
			cout << "Enter your choice:" << endl;
			cout << "1. Account Information\n";
			cout << "2. Buy Tickets\n";
			cout << "3. My Bookings\n";
			cout << "4. End\n? ";

			int choice = inputAnInteger(1, 4);

			switch (choice)
			{
			case 1:
				accountInfor(memberDetails, numMembers, record);
				break;

			case 2:
				displaySessionTimes(movies, movieNames, numMovies, availableDates, numDates);
				buyTickets(bookingHistories, numBookings, movies, movieNames, numMovies, availableDates, numDates, address);
				display(movies, bookingHistories, numBookings);
				selectSeats(bookingHistories, numBookings, movies);
				break;

			case 3:
				displayBookingHistory(bookingHistories, numBookings, movies, movieNames, availableDates, address);
				break;

			case 4:
				cout << "Thank you..." << endl;
				return;

			default:
				cout << "Input Error!\n\n";
				break;
			}
		}
	}
	else
		cout << "Sorry, unrecognized email or password." << endl;
}
bool illegal(char email[], char password[], MemberRecord memberDetails[], int numMembers, int &recordNumber)
{
	for (recordNumber = 0; recordNumber < numMembers; recordNumber++)
	{
		if (strcmp(email, memberDetails[recordNumber].email) == 0)
			if (strcmp(password, memberDetails[recordNumber].password) == 0)
				return true;
			else
				continue;
	}
	return false;
}
void accountInfor(MemberRecord memberDetails[], int numMembers, int recordNumber)
{
	cout << "1.Name: " << memberDetails[recordNumber].name << endl;
	cout << "2.Email address : " << memberDetails[recordNumber].email << endl;
	cout << "3.Phone number : " << memberDetails[recordNumber].phone << endl;
	cout << "4.ID number: " << memberDetails[recordNumber].IDNumber << endl;
	cout << "5.Password : " << memberDetails[recordNumber].password << endl;

	int modifynum;
	cout << "\nWhich one do you want to modify (0 �V not modify)? ";
	cin >> modifynum;
	cout << endl;

	if (modifynum == 0)
		return;
	else if (modifynum == 1)
	{
		cout << "Enter correct data: ";
		cin >> memberDetails[recordNumber].name;

	}
	else if (modifynum == 2)
	{
		cout << "Enter correct data: ";
		cin >> memberDetails[recordNumber].email;
		if (existingEmail(memberDetails[recordNumber].email, memberDetails, numMembers))
		{
			cout << "An account already exists with the Email!";
			return;
		}
	}
	else if (modifynum == 3)
	{
		cout << "Enter correct data: ";
		cin >> memberDetails[recordNumber].phone;
	}
	else if (modifynum == 4)
	{
		cout << "Enter correct data: ";
		cin >> memberDetails[recordNumber].IDNumber;
		if (existingID(memberDetails[recordNumber].IDNumber, memberDetails, numMembers))
		{
			cout << "An account already exists with the ID number!";
			return;
		}
	}
	else if (modifynum == 5)
	{
		cout << "Enter correct data: ";
		cin >> memberDetails[recordNumber].password;
	}
	else
	{
		cout << "Input Error! Please try again: " << endl;
	}
}
void newMember(MemberRecord memberDetails[], int &numMembers)
{
	ofstream outMemberFile("Member Info.dat", ios::app | ios::binary);
	MemberRecord test = { " ", " ", " ", " ", " " };

	cout << "Enter your ID number: ";
	cin >> test.IDNumber;
	if (existingID(test.IDNumber, memberDetails, numMembers))
	{
		cout << "An account already exists with the ID number!";
		return;
	}

	cout << "Enter your name: ";
	cin >> test.name;

	cout << "Enter an email address : ";
	cin >> test.email;

	if (existingEmail(test.email, memberDetails, numMembers))
	{
		cout << "An account already exists with the email!" << endl;
		return;
	}

	cout << "Enter a password : ";
	cin >> test.password;

	cout << "Enter your phone number : ";
	cin >> test.phone;

	if (test.IDNumber, test.name, test.email, test.password, test.phone != '\0')
	{
		memberDetails[numMembers] = test;
		cout << "\nSuccessful!\n" << endl;
		numMembers++;
	}
	else
	{
		cout << "Input Error!" << endl;
	}
	outMemberFile.close();
}
bool existingID(char newIDNumber[], MemberRecord memberDetails[], int &numMembers)
{
	for (int i = 0; i < numMembers; i++)
	{
		if (strcmp(newIDNumber, memberDetails[i].IDNumber) == 0)
			return true;
	}
	return false;
}
bool existingEmail(char newEmail[], MemberRecord memberDetails[], int &numMembers)
{
	for (int i = 0; i < numMembers; i++)
	{
		if (strcmp(newEmail, memberDetails[i].email) == 0)
			return true;
	}
	return false;
}

void loadBookingHistories(BookingInfo bookingHistories[], int &numBookings)
{
	ifstream inbookingFile("Booking Histories.dat", ios::in | ios::binary);
	numBookings = 0;
	while (inbookingFile)
	{
		inbookingFile.seekg(numBookings * sizeof(BookingInfo));
		inbookingFile.read(reinterpret_cast<char*>(&bookingHistories[numBookings]), sizeof(BookingInfo));
		numBookings++;
	}

	inbookingFile.close();
}
void loadAvailableDates(char availableDates[][12], int &numDates)
{
	ifstream indateFile("Available Dates.txt", ios::in);
	if (!indateFile)
	{
		cerr << "File could not be opened." << endl;
	}
	cout << endl;

	string Dates;
	numDates = 0;
	while (!indateFile.eof())
	{		
		getline(indateFile, Dates, '\n');
		std::copy(Dates.begin(), Dates.end(), availableDates[numDates]);
		availableDates[numDates][Dates.length()] = '\0';		
		numDates++;	
	}	
	indateFile.close();
}
void loadMovies(Movie movies[], int &numMovies)
{
	ifstream inmovieFile("Movies.dat", ios::in | ios::binary);

	numMovies = 0;
	while (inmovieFile)
	{
		inmovieFile.seekg(numMovies * sizeof(Movie));
		inmovieFile.read(reinterpret_cast<char*>(&movies[numMovies]), sizeof(Movie));
		numMovies++;
	}
	inmovieFile.close();
}
void loadMovieNames(char movieNames[][60], int numMovies)
{
	ifstream inmovienameFile("Movie Names.txt", ios::in);
	if (!inmovienameFile)
	{
		cerr << "File could not be opened." << endl;
	}
	numMovies = 0;
	string Movies;
	while (!inmovienameFile.eof())
	{			
		getline(inmovienameFile, Movies, '\n');
		std::copy(Movies.begin(), Movies.end(), movieNames[numMovies]);
		movieNames[numMovies][Movies.length()] = '\0';
		numMovies++;		
	}
	inmovienameFile.close();
}
void buyTickets(BookingInfo bookingHistories[], int &numBookings, Movie movies[], char movieNames[][60], int numMovies, char availableDates[][12], int numDates, char email[])
{
	string choicemoviestr;
	int choicemovie = 0;
	stringstream sn;
	sn.str("");
	sn.clear();

	cout << "Enter movie code (0 - 11): ";
	cin >> choicemoviestr;

	while (!(sn << choicemoviestr && sn >> choicemovie) || choicemovie < 0 || choicemovie > 11)
	{
		cout << "Input Error" << endl;
		cout << "Enter movie code (0 - 11): ";
		cin >> choicemoviestr;
		sn.str("");
		sn.clear();
	}
	bookingHistories[numBookings].movieCode = choicemovie;

	string choicedatestr;
	stringstream ss;
	ss.str("");
	ss.clear();
	int choicedate;
	cout << "Enter date code (0 - 8): ";
	cin >> choicedatestr;
	while (!(ss << choicedatestr && ss >> choicedate )|| choicedate < 0 || choicedate > 8 || (movies[choicemovie].dates[choicedate] == 0))
	{
		cout << "Input Error" << endl;
		cout << "Enter date code (0 - 8): ";
		cin >> choicedatestr;
		ss.str("");
		ss.clear();
	}
	bookingHistories[numBookings].dateCode = choicedate;

	string chiocetimestr;
	stringstream se;
	se.str("");
	se.clear();
	int choicetime;
	cout << "Enter session time code(0 - 16) : ";
	cin >> chiocetimestr;
	while ( !(se << chiocetimestr && se >> choicetime) || choicetime < 0 || choicetime > 16 || movies[choicemovie].sessionTimes[choicetime] == 0)
	{
		cout << "Input Error" << endl;
		cout << "Enter session time code(0 - 16) : ";
		cin >> chiocetimestr;
		se.str("");
		se.clear();
	}
	bookingHistories[numBookings].sessionTimeCode = choicetime;
	cout << endl;

	cout << "Movie: " << movieNames[choicemovie] << endl;
	cout << "Date: " << availableDates[choicedate] << endl;
	cout << "Show Time: " << hours[choicetime] << endl;
	cout << "Price:  Adult-" << movies[choicemovie].prices[0] << ", Concession-" << movies[choicemovie].prices[1] << ", Disability-" << movies[choicemovie].prices[2] << ", Elderly-" << movies[choicemovie].prices[3] << endl;
	cout << endl;

	cout << "Enter the number of adult tickets (0 - 6): " << endl;
	cin >> bookingHistories[numBookings].numTickets[0];
	while (bookingHistories[numBookings].numTickets[0] < 0 || bookingHistories[numBookings].numTickets[0] > 6)
	{
		cout << "Enter the number of adult tickets (0 - 6): " << endl;
		cin >> bookingHistories[numBookings].numTickets[0];
	}
	cout << "Enter the number of concession tickets (0 - 6): " << endl;
	cin >> bookingHistories[numBookings].numTickets[1];
	while (bookingHistories[numBookings].numTickets[1] < 0 || bookingHistories[numBookings].numTickets[1] > 6)
	{
		cout << "Enter the number of concession tickets (0 - 6): " << endl;
		cin >> bookingHistories[numBookings].numTickets[1];
	}
	cout << "Enter the number of disability tickets (0 - 6): " << endl;
	cin >> bookingHistories[numBookings].numTickets[2];
	while (bookingHistories[numBookings].numTickets[2] < 0 || bookingHistories[numBookings].numTickets[2] > 6)
	{
		cout << "Enter the number of disability tickets (0 - 6): " << endl;
		cin >> bookingHistories[numBookings].numTickets[2];
	}
	cout << "Enter the number of elderly tickets (0 - 6): " << endl;
	cin >> bookingHistories[numBookings].numTickets[3];
	cout << endl;
	while (bookingHistories[numBookings].numTickets[3] < 0 || bookingHistories[numBookings].numTickets[3] > 6)
	{
		cout << "Enter the number of elderly tickets (0 - 6): " << endl;
		cin >> bookingHistories[numBookings].numTickets[3];
		cout << endl;
	}
	cout << endl;
	while ((bookingHistories[numBookings].numTickets[0] + bookingHistories[numBookings].numTickets[1] + bookingHistories[numBookings].numTickets[2] + bookingHistories[numBookings].numTickets[3]) == 0)
	{
		cout << "Enter the number of adult tickets (0 - 6): " << endl;
		cin >> bookingHistories[numBookings].numTickets[0];
		while (bookingHistories[numBookings].numTickets[0] < 0 || bookingHistories[numBookings].numTickets[0] > 6)
		{
			cout << "Enter the number of adult tickets (0 - 6): " << endl;
			cin >> bookingHistories[numBookings].numTickets[0];
		}
		cout << "Enter the number of concession tickets (0 - 6): " << endl;
		cin >> bookingHistories[numBookings].numTickets[1];
		while (bookingHistories[numBookings].numTickets[1] < 0 || bookingHistories[numBookings].numTickets[1] > 6)
		{
			cout << "Enter the number of concession tickets (0 - 6): " << endl;
			cin >> bookingHistories[numBookings].numTickets[1];
		}
		cout << "Enter the number of disability tickets (0 - 6): " << endl;
		cin >> bookingHistories[numBookings].numTickets[2];
		while (bookingHistories[numBookings].numTickets[2] < 0 || bookingHistories[numBookings].numTickets[2] > 6)
		{
			cout << "Enter the number of disability tickets (0 - 6): " << endl;
			cin >> bookingHistories[numBookings].numTickets[2];
		}
		cout << "Enter the number of elderly tickets (0 - 6): " << endl;
		cin >> bookingHistories[numBookings].numTickets[3];
		cout << endl;
		while (bookingHistories[numBookings].numTickets[3] < 0 || bookingHistories[numBookings].numTickets[3] > 6)
		{
			cout << "Enter the number of elderly tickets (0 - 6): " << endl;
			cin >> bookingHistories[numBookings].numTickets[3];
			cout << endl;
		}
		cout << endl;
	}
}
	
void selectSeats(BookingInfo bookingHistories[], int numBookings, Movie movies[])
{
	cout << "  A B C D E F G H I J K L" << endl;
	for (int i = 0; i < 8; i++)
	{
		cout << i << " ";
		for (int j = 0; j < 12; j++)
			cout << movies[bookingHistories[numBookings].movieCode].occupiedSeats[bookingHistories[numBookings].dateCode][bookingHistories[numBookings].sessionTimeCode][i][j] << " ";
		cout << endl;
	}
	cout << endl << "Select " << bookingHistories[numBookings].numTickets[0] + bookingHistories[numBookings].numTickets[1] + bookingHistories[numBookings].numTickets[2] + bookingHistories[numBookings].numTickets[3] << " seats (e.g. 0A):" << endl;;
	for (int n = 0; n < bookingHistories[numBookings].numTickets[0] + bookingHistories[numBookings].numTickets[1] + bookingHistories[numBookings].numTickets[2] + bookingHistories[numBookings].numTickets[3]; n++)
	{		
		cout << "?";
		cin >> bookingHistories[numBookings].seletedSeats[n][0] >> bookingHistories[numBookings].seletedSeats[n][1];
		cout << endl;
		if (48 <= bookingHistories[numBookings].seletedSeats[n][0] <= 55 && 65 <= bookingHistories[numBookings].seletedSeats[n][1] <= 76)
		{
			if (movies[bookingHistories[numBookings].movieCode].occupiedSeats[bookingHistories[numBookings].dateCode][bookingHistories[numBookings].sessionTimeCode][(bookingHistories[numBookings].seletedSeats[n][0]) - 48][(bookingHistories[numBookings].seletedSeats[n][1]) - 65] == 0)
			{
				movies[bookingHistories[numBookings].movieCode].occupiedSeats[bookingHistories[numBookings].dateCode][bookingHistories[numBookings].sessionTimeCode][(bookingHistories[numBookings].seletedSeats[n][0]) - 48][(bookingHistories[numBookings].seletedSeats[n][1]) - 65] = 1;
				break;
			}
			else
			{
				cout << "\nThis seat bas been occupied. Please select another seat.";
				continue;
			}
		}
		/*while (bookingHistories[numBookings].seletedSeats[n][0] > 55 || bookingHistories[numBookings].seletedSeats[n][0] < 48 || 65 > bookingHistories[numBookings].seletedSeats[n][1] || bookingHistories[numBookings].seletedSeats[n][1] < 76)
		{
			cout << "\nThis seat is not exist. Please select another seat.";
			break;
		}*/
		else /*(bookingHistories[numBookings].seletedSeats[n][0] > 55 || bookingHistories[numBookings].seletedSeats[n][0] < 48 || 65 > bookingHistories[numBookings].seletedSeats[n][1] || bookingHistories[numBookings].seletedSeats[n][1] < 76 )*/
		{
			cout << "\nThis seat is not exist. Please select another seat.";
			cout << endl << "Select " << bookingHistories[numBookings].numTickets[0] + bookingHistories[numBookings].numTickets[1] + bookingHistories[numBookings].numTickets[2] + bookingHistories[numBookings].numTickets[3] << " seats (e.g. 0A):" << endl;;
			for (int n = 0; n < bookingHistories[numBookings].numTickets[0] + bookingHistories[numBookings].numTickets[1] + bookingHistories[numBookings].numTickets[2] + bookingHistories[numBookings].numTickets[3]; n++)
			{
				cout << "?";
				cin >> bookingHistories[numBookings].seletedSeats[n][0] >> bookingHistories[numBookings].seletedSeats[n][1];
				cout << endl;
				if (48 <= bookingHistories[numBookings].seletedSeats[n][0] <= 55 && 65 <= bookingHistories[numBookings].seletedSeats[n][1] <= 76)
				{
					if (movies[bookingHistories[numBookings].movieCode].occupiedSeats[bookingHistories[numBookings].dateCode][bookingHistories[numBookings].sessionTimeCode][(bookingHistories[numBookings].seletedSeats[n][0]) - 48][(bookingHistories[numBookings].seletedSeats[n][1]) - 65] == 0)
					{
						movies[bookingHistories[numBookings].movieCode].occupiedSeats[bookingHistories[numBookings].dateCode][bookingHistories[numBookings].sessionTimeCode][(bookingHistories[numBookings].seletedSeats[n][0]) - 48][(bookingHistories[numBookings].seletedSeats[n][1]) - 65] = 1;
						break;
					}
				}
				else
				{
					cout << "\nThis seat bas been occupied. Please select another seat.";
					continue;
				}
			}
		}
	}
		cout << "\nSuccessful!\n" << endl;
}
void display(Movie movies[], BookingInfo bookingHistories[], int numbooking)
{	
	cout << setw(10) << "           No. of Tickets" << setw(10) << "Price" << setw(15) << "Subtotal" << endl;

	if (bookingHistories[numbooking].numTickets[0] != 0)
		cout << setw(10) << "adult" << setw(10) << bookingHistories[numbooking].numTickets[0] << setw(15) << movies[bookingHistories[numbooking].movieCode].prices[0] << setw(15) << bookingHistories[numbooking].numTickets[0] * movies[bookingHistories[numbooking].movieCode].prices[0] << endl;

	if (bookingHistories[numbooking].numTickets[1] != 0)
		cout << setw(10) << "concession" << setw(10) << bookingHistories[numbooking].numTickets[1] << setw(15) << movies[bookingHistories[numbooking].movieCode].prices[1] << setw(15) << bookingHistories[numbooking].numTickets[1] * movies[bookingHistories[numbooking].movieCode].prices[1] << endl;

	if (bookingHistories[numbooking].numTickets[2] != 0)
		cout << setw(10) << "disability" << setw(10) << bookingHistories[numbooking].numTickets[2] << setw(15) << movies[bookingHistories[numbooking].movieCode].prices[2] << setw(15) << bookingHistories[numbooking].numTickets[2] * movies[bookingHistories[numbooking].movieCode].prices[2] << endl;

	if (bookingHistories[numbooking].numTickets[3] != 0)
		cout << setw(10) << "elderly" << setw(10) << bookingHistories[numbooking].numTickets[3] << setw(15) << movies[bookingHistories[numbooking].movieCode].prices[3] << setw(15) << bookingHistories[numbooking].numTickets[3] * movies[bookingHistories[numbooking].movieCode].prices[3] << endl;
	
	cout << endl;
	cout << "Total Amount For Tickets: " << bookingHistories[numbooking].numTickets[0] * movies[bookingHistories[numbooking].movieCode].prices[0] + bookingHistories[numbooking].numTickets[1] * movies[bookingHistories[numbooking].movieCode].prices[1] + bookingHistories[numbooking].numTickets[2] * movies[bookingHistories[numbooking].movieCode].prices[2] + bookingHistories[numbooking].numTickets[3] * movies[bookingHistories[numbooking].movieCode].prices[3] << endl;
	cout << endl;
}
void displaySessionTimes(Movie movies[], char movieNames[][60], int numMovies, char availableDates[][12], int numDates)
{	
	int n = 0;
	for (int i = 0; i < 12; i++)
	{		
		cout << i << ". Movie: " << movieNames[i] << endl;
		cout << "Date: ";
		for (int k = 0; k < 9; k++)
		{
			if (movies[n].dates[k] == 1)
			{
				cout << k << ". " << availableDates[k] << ", ";
			}
		}
		cout << endl;
		cout << "Session Time: ";
		for (int m = 0; m < 17; m++)
		{
			if (movies[n].sessionTimes[m] == 1)
			{
				cout << m << ". " << hours[m] << ", ";
			}
		}
		n++;
		cout << endl;
		cout << endl;
	}
	cout << endl;

}
	
void displayBookingHistory(BookingInfo bookingHistories[], int numBookings, Movie movies[], char movieNames[][60], char availableDates[][12], char email[])
{
	int counter = 0;
	for (int i = 0; i < numBookings; i++)
	{
		counter = 0;
		if (strcmp(bookingHistories[i].email, email) == 0)
		{
			counter = i;
			break;
		}
		else
			continue;
	}

	if (bookingHistories[counter].dateCode == 0 && bookingHistories[counter].movieCode == 0 && bookingHistories[counter].sessionTimeCode == 0)
	{
		cout << "\nNo booking!\n" << endl;
		return;
	}

	else
	{
		cout << "\nBooking History :\n" << endl;
		cout << "Movie: " << movieNames[bookingHistories[counter].movieCode] << endl;
		cout << "Date: " << availableDates[bookingHistories[counter].dateCode] << endl;
		cout << "Show Time: " << hours[bookingHistories[counter].sessionTimeCode] << endl;
		cout << "Seats: ";

		for (int i = 0; i < bookingHistories[counter].numTickets[0] + bookingHistories[counter].numTickets[1] + bookingHistories[counter].numTickets[2] + bookingHistories[counter].numTickets[3]; i++)
		{
			cout << bookingHistories[counter].seletedSeats[i] << " ";
		}
	}	
	cout << endl;
}

void saveMemberInfo(MemberRecord memberDetails[], int numMembers)
{
	ofstream outMemberInfoFile("Member Info.dat", ios::app | ios::binary);

	if (!outMemberInfoFile)
	{
		cout << "File could not be opened !" << endl;
		system("pause");
		exit(1);
	}

	for (int i = 0; i < numMembers; i++)
	{
		outMemberInfoFile.seekp(i * sizeof(MemberRecord));
		outMemberInfoFile.write(reinterpret_cast<const char *>(&memberDetails[i]), sizeof(MemberRecord));
	}

	outMemberInfoFile.close();
}
void saveMovies(Movie movies[], int numMovies)
{
	ofstream saveMoviesFile("Movies.dat", ios::out | ios::binary);

	if (!saveMoviesFile)
	{
		cout << "Movies file could not be saved!" << endl;
		system("pause");
		exit(1);
	}

	for (int i = 0; i < numMovies; i++)
	{
		saveMoviesFile.seekp(i * sizeof(Movie));
		saveMoviesFile.write(reinterpret_cast<const char*>(&movies[i]), sizeof(Movie));
	}

	saveMoviesFile.close();
}
void saveBookingHistories(BookingInfo bookingHistories[], int numBookings)
{
	ofstream outBookingHistoriesFile("Booking Histories.dat", ios::app | ios::binary);

	if (!outBookingHistoriesFile)
	{
		cout << "Booking histories file could not be saved!" << endl;
		system("pause");
		exit(1);
	}

	for (int i = 0; i < numBookings; i++)
	{
		outBookingHistoriesFile.seekp(i * sizeof(BookingInfo));
		outBookingHistoriesFile.write(reinterpret_cast<const char*>(&bookingHistories[numBookings]), sizeof(BookingInfo));
    }

	outBookingHistoriesFile.close();
}