#include <stdio.h>
#include <ctype.h>
#include <string.h> 

enum tag_read_state {READ_UNSTARTED, READ_STARTED, READ_DONE};
enum trkpt_state {TRKPT_ENDED, TRKPT_STARTED};
enum lat_state {LAT_UNSTARTED, LAT_STARTED, LAT_UNPRINTED};
enum lon_state {LON_UNSTARTED, LON_STARTED, LON_UNPRINTED};
enum lat_arg_state {LAT_ARG_UNDONE, LAT_ARG_READING, LAT_ARG_DONE};
enum lon_arg_state {LON_ARG_UNDONE, LON_ARG_READING, LON_ARG_DONE};
enum ele_state {ELE_UNSTARTED, ELE_STARTED};
enum ele_pos {ELE_OUT, ELE_IN};
enum time_state {TIME_UNSTARTED, TIME_STARTED};
enum time_pos {TIME_OUT, TIME_IN};
enum quote_state {QUOTE_ENDED, QUOTE_STARTED};

int strcomp(char str1[] , char str2[])
{
	int identical = 1;
	int maxlength = 0;

	if (strlen(str1) < strlen(str2)) maxlength = strlen(str1);
	else (maxlength = strlen(str2));

	for (int i = 0; i < maxlength; i++)
	{
		if (toupper(str1[i]) != toupper(str2[i])) identical = 0;
	}

	return identical;
}

int main()
{

  enum tag_read_state curr_tag_read_state = READ_UNSTARTED;
  enum trkpt_state curr_trkpt_state = TRKPT_ENDED;
  enum lat_state curr_lat_state = LAT_UNSTARTED;
  enum lon_state curr_lon_state = LON_UNSTARTED;
  enum lat_arg_state curr_lat_arg_state = LAT_ARG_UNDONE;
  enum lon_arg_state curr_lon_arg_state = LON_ARG_UNDONE;
  enum ele_state curr_ele_state = ELE_UNSTARTED;
  enum ele_pos curr_ele_pos = ELE_OUT;
  enum time_state curr_time_state = TIME_UNSTARTED;
  enum time_pos curr_time_pos = TIME_OUT;
  enum quote_state curr_quote_state = QUOTE_ENDED;

  int ch;
  char curr_tag[99999];
  int curr_tag_index = 0;
  char curr_lat_arg[99999];
  int curr_lat_arg_i = 0;
  char curr_lon_arg[99999];
  int curr_lon_arg_i = 0;
  char curr_lat[99999];
  int curr_lat_i = 0;
  char curr_lon[99999];
  int curr_lon_i = 0;
  char curr_ele[99999];
  int curr_ele_index = 0;
  char curr_time[99999];
  int curr_time_index = 0;
  char curr_quote_symbol;

  //read through characters one by one, if EOF is read then immediately quit
  while ((ch = getchar()) != EOF)
  {
    //check if there is any quote and update quote state accordingly
  	if ((ch == '"' || ch == '\'') && curr_quote_state != QUOTE_STARTED)
  	{
  		curr_quote_state = QUOTE_STARTED;
  		curr_quote_symbol = ch;
  	} else if ((ch == '"' || ch == '\'') && curr_quote_state == QUOTE_STARTED && ch == curr_quote_symbol)
  	{
  		curr_quote_state = QUOTE_ENDED;
  		curr_quote_symbol = '\0';
  	}

	//read latest start-tag or end-tag
  	if (ch == '<')
  	{
  		curr_tag_read_state = READ_STARTED;
  	}
  	if ((ch != '<') && (curr_tag_read_state == READ_STARTED) && (ch != '>') && (!isspace(ch)))
  	{
  		curr_tag[curr_tag_index] = ch;
  		curr_tag_index++;
  	}
  	else if ((ch != '<') && curr_tag_read_state == READ_STARTED && (isspace(ch) || ch == '>'))
  	{
  		curr_tag[curr_tag_index] = '\0';
  		curr_tag_index++;
  		curr_tag_read_state = READ_UNSTARTED;
  	}

  	//when tag has been committed to curr_tag, update states of trkpt/ele/time tags accordingly
  	if ((isspace(ch) || ch == '>') && curr_tag[0] != '\0' && curr_tag_read_state == READ_UNSTARTED && curr_quote_state == QUOTE_ENDED)
  	{
  		if (strcomp(curr_tag,"TRKPT")) curr_trkpt_state = TRKPT_STARTED;
  		else if	(strcomp(curr_tag,"/TRKPT")) curr_trkpt_state = TRKPT_ENDED;
  		else if (strcomp(curr_tag,"ELE")) curr_ele_state = ELE_STARTED;
  		else if (strcomp(curr_tag,"/ELE")) curr_ele_state = ELE_UNSTARTED;
  		else if (strcomp(curr_tag,"TIME")) curr_time_state = TIME_STARTED;
  		else if (strcomp(curr_tag,"/TIME")) curr_time_state = TIME_UNSTARTED;

  		curr_tag[0] = '\0';
  		curr_tag_index = 0;
  		curr_tag_read_state = READ_UNSTARTED;
  	}

  	//read & print content of latitude attribute inside trkpt tag
	if (!isspace(ch) && ch != '=' && curr_quote_state == QUOTE_ENDED && curr_trkpt_state == TRKPT_STARTED && curr_lat_state == LAT_UNSTARTED && curr_ele_state == ELE_UNSTARTED && curr_time_state == TIME_UNSTARTED && (curr_lat_arg_state == LAT_ARG_UNDONE || curr_lat_arg_state == LAT_ARG_READING))
	{	
		curr_lat_arg[curr_lat_arg_i] = ch;
		curr_lat_arg_i++;
		curr_lat_arg_state = LAT_ARG_READING;
	}
	else if ((isspace(ch) || ch == '=') && curr_quote_state == QUOTE_ENDED && curr_trkpt_state == TRKPT_STARTED && curr_lat_state == LAT_UNSTARTED && curr_ele_state == ELE_UNSTARTED && curr_time_state == TIME_UNSTARTED && curr_lat_arg_state == LAT_ARG_READING)
	{
		curr_lat_arg[curr_lat_arg_i] = '\0';
		curr_lat_arg_i++;
		curr_lat_arg_state = LAT_ARG_UNDONE;
		if (strcomp(curr_lat_arg, "lat")) curr_lat_state = LAT_STARTED;
		else 
			{
				curr_lat_arg[0] = '\0';
				curr_lat_arg_i = 0;
				curr_lat_arg_state = LAT_ARG_UNDONE;
			}
	}
	else if ((ch == '"' || ch == '\'') && curr_trkpt_state == TRKPT_STARTED && curr_lat_state == LAT_STARTED)
	{
		curr_lat[curr_lat_i] = ch;
		curr_lat_i++;
	}
	else if (curr_quote_state == QUOTE_STARTED && curr_trkpt_state == TRKPT_STARTED && curr_lat_state == LAT_STARTED)
	{
		curr_lat[curr_lat_i] = ch;
		curr_lat_i++;
	}
	else if (isspace(ch) && curr_quote_state == QUOTE_ENDED && curr_trkpt_state == TRKPT_STARTED && curr_lat_state == LAT_STARTED && curr_lat[0] != '\0')
	{
		curr_lat[curr_lat_i] = '\0';
		curr_lat_i++;
		curr_lat_state = LAT_UNPRINTED;
	}
	else if (ch == '>' && curr_quote_state == QUOTE_ENDED && curr_trkpt_state == TRKPT_STARTED && curr_lat_state == LAT_UNPRINTED)
	{
		for (int i = 1; i < strlen(curr_lat) - 1; i++)
		{
			printf("%c", curr_lat[i]);
		}
		printf(",");
		curr_lat[0] = '\0';
		curr_lat_i = 0;
		curr_lat_state = LAT_UNSTARTED;
		curr_lat_arg[0] = '\0';
		curr_lat_arg_i = 0;
		curr_lat_arg_state = LAT_ARG_UNDONE;
	}

	//read & print content of longitude attribute inside trkpt tag
	if (!isspace(ch) && ch != '=' && curr_quote_state == QUOTE_ENDED && curr_trkpt_state == TRKPT_STARTED && curr_lon_state == LON_UNSTARTED && curr_ele_state == ELE_UNSTARTED && curr_time_state == TIME_UNSTARTED && (curr_lon_arg_state == LON_ARG_UNDONE || curr_lon_arg_state == LON_ARG_READING))
	{
		curr_lon_arg[curr_lon_arg_i] = ch;
		curr_lon_arg_i++;
		curr_lon_arg_state = LON_ARG_READING;
	}
	else if ((isspace(ch) || ch == '=') && curr_quote_state == QUOTE_ENDED && curr_trkpt_state == TRKPT_STARTED && curr_lon_state == LON_UNSTARTED && curr_ele_state == ELE_UNSTARTED && curr_time_state == TIME_UNSTARTED && curr_lon_arg_state == LON_ARG_READING)
	{
		curr_lon_arg[curr_lon_arg_i] = '\0';
		curr_lon_arg_i++;
		curr_lon_arg_state = LON_ARG_UNDONE;
		if (strcomp(curr_lon_arg, "lon")) curr_lon_state = LON_STARTED;
		else 
			{
				curr_lon_arg[0] = '\0';
				curr_lon_arg_i = 0;
				curr_lon_arg_state = LON_ARG_UNDONE;
			}
	}
	else if ((ch == '"' || ch == '\'') && curr_trkpt_state == TRKPT_STARTED && curr_lon_state == LON_STARTED)
	{
		curr_lon[curr_lon_i] = ch;
		curr_lon_i++;
	}
	else if (curr_quote_state == QUOTE_STARTED && curr_trkpt_state == TRKPT_STARTED && curr_lon_state == LON_STARTED)
	{
		curr_lon[curr_lon_i] = ch;
		curr_lon_i++;
	}
	else if ((isspace(ch) || ch == '>') && curr_quote_state == QUOTE_ENDED && curr_trkpt_state == TRKPT_STARTED && curr_lon_state == LON_STARTED && curr_lon[0] != '\0')
	{
		curr_lon[curr_lon_i] = '\0';
		curr_lon_i++;
		curr_lon_state = LON_UNPRINTED;
	}
	
	if (ch == '>' && curr_quote_state == QUOTE_ENDED && curr_trkpt_state == TRKPT_STARTED && curr_lon_state == LON_UNPRINTED)
	{
		for (int i = 1; i < strlen(curr_lon) - 1; i++)
		{
			printf("%c", curr_lon[i]);
		}
		printf(",");
		curr_lon[0] = '\0';
		curr_lon_i = 0;
		curr_lon_state = LON_UNSTARTED;
		curr_lon_arg[0] = '\0';
		curr_lon_arg_i = 0;
		curr_lon_arg_state = LON_ARG_UNDONE;
	}

  	//read & print content of elevation tag iff inside trkpt tag
  	if (ch == '>' && curr_quote_state == QUOTE_ENDED && curr_trkpt_state == TRKPT_STARTED && curr_ele_state == ELE_STARTED)
  	{ 
  		curr_ele_pos = ELE_IN;
  	}
  	else if (ch != '<' && curr_ele_pos == ELE_IN)
  	{
  		curr_ele[curr_ele_index] = ch;
  		curr_ele_index++;
  	}
  	else if (ch == '<' && curr_ele_pos == ELE_IN)
  	{
  		curr_ele_pos = ELE_OUT;
  		curr_ele[curr_ele_index] = '\0';
  		curr_ele_index++;
  	}
  	else if ((isspace(ch) || ch == '>') && curr_trkpt_state == TRKPT_STARTED && curr_ele_state == ELE_UNSTARTED && curr_ele[0] != '\0')
  	{
  		printf("%s,", curr_ele);
  		curr_ele_state = ELE_UNSTARTED;
  		curr_ele[0] = '\0';
  		curr_ele_index = 0;
  	}

  	//read & print content of time tag iff inside trkpt tag
  	if (ch == '>' && curr_quote_state == QUOTE_ENDED && curr_trkpt_state == TRKPT_STARTED && curr_time_state == TIME_STARTED)
  	{
  		curr_time_pos = TIME_IN;
  	}
  	else if (ch != '<' && curr_time_pos == TIME_IN)
  	{
  		curr_time[curr_time_index] = ch;
  		curr_time_index++;
  	}
  	else if (ch == '<' && curr_time_pos == TIME_IN)
  	{
  		curr_time_pos = ELE_OUT;
  		curr_time[curr_time_index] = '\0';
  		curr_time_index++;
  	}
  	else if ((isspace(ch) || ch == '>') && curr_trkpt_state == TRKPT_STARTED && curr_time_state == TIME_UNSTARTED && curr_time[0] != '\0')
  	{
  		for (int i = 0; i < strlen(curr_time); i++)
  		{
  			if (curr_time[i] == ',') printf("&comma;");
  				else printf("%c", curr_time[i]);
  		}
  		printf("\n");
  		curr_time_state = TIME_UNSTARTED;
  		curr_time[0] = '\0';
  		curr_time_index = 0;
  	}
 	  }
  return 0;
}