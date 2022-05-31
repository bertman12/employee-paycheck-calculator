//Program:employee-paycheck-calculator.cpp
//Author:Eddy Daniel Estrada
//Due Date:May 13, 2015
//Description: This program will show you an employee's gross pay, net pay, and taxes based on the hours you worked.
//  edit

#include <iostream> //cin, cout, fixed, endl, left, right
#include <iomanip> //setprecision, setw
#include <string> 
#include <fstream> //ofstream, ifstream
#include <cstdlib> //exit

using namespace std;

#define LIMIT 10    //max number of employees that can be processed
#define TAX	0.3

void input_employee_data(string &first_name, string &last_name, double &hours_worked, double &hourly_rate, double &deductions, ifstream &empdata);
double gather_deductions();
string join_name(string first_name, string last_name);
void split_hours(double hours_worked, double &regular_hours, double &overtime_hours);
double calculate_gross_pay(double regular_hours, double overtime_hours, double hourly_rate);
double calculate_tax(double gross_pay, double tax_rate);
double calculate_net_pay(double gross_pay, double tax, double deductions);
void output_payroll_data(string formatted_name, double regular_hours, double overtime_hours, double hourly_rate, double gross_pay, double tax, double deductions, double net_pay, ofstream &results, int employees);
void min_max_gross(double gross_pay, int employees, double &min_gross, double &max_gross);
void employee_summary(string employee_names[], int employees, double employee_gross_pay[], double summed_gross_pay, double min_gross, double max_gross, ofstream &results);
void sort_employees(double employee_gross_pay[], string employee_names[], int employees);

int main()
	
{
    string first_name,                               //first name input by user
        last_name,                                   //last name input by user
        full_name,                                   //first and last name formatted into last, first
        prompt,										 //Prompts the user to repeat the program
        employee_names[LIMIT];                       //Keeps track of employee names that were processed

    double hours,                   //the hours worked input by user
        hourly_rate,                //the dollars the worker gets paid per hour
        overtime_hours,             //the number of hours worked that you earn overtime pay on
        regular_hours,              //the number of hours worked that you earn regular pay on
        gross_pay,                  //the amount of money you make before taxes
        net_pay,                    //the amount of money you make after deductions
        tax,                        //the amount of money deducted from your gross pay because of taxes
        deductions,                 //amount of money deducted from paycheck
        summed_gross_pay=0,         //keeps track of the sum of gross pay of all employees processed
        employee_gross_pay[LIMIT],  //keeps track of a specific employee's gross_pay
        min_gross=0,                //Tracks the lowest grosspay evaluated
        max_gross=0;                //Tracks the highest grosspay evaluated

    int employees = 0;              //counts the number of employees the program has completed

	ofstream results;           //this is where the results of an employee summary are output

    ifstream empdata;           //gets employee data for processing


    results.open("E:\\CISP301\\LAB 20\\results.txt");
        if (!results.is_open()){
            cout << "FILE NOT OPEN. NOW TERMINATING!";
            exit(EXIT_FAILURE);
        }
    empdata.open("E:\\CISP301\\LAB 20\\empdata.txt");
        if (!empdata.is_open()){
            cout << "FILE NOT OPEN. NOW TERMINATING!";
            exit(EXIT_FAILURE);
        }

    while (!empdata.eof() && employees < LIMIT){
        input_employee_data(first_name, last_name, hours, hourly_rate, deductions, empdata);

		if (!empdata.eof()){
            
			full_name = join_name(first_name, last_name);
			employee_names[employees] = full_name;
			split_hours(hours, regular_hours, overtime_hours);
			gross_pay = calculate_gross_pay(regular_hours, overtime_hours, hourly_rate);
			min_max_gross(gross_pay, employees, min_gross, max_gross);
			employee_gross_pay[employees] = gross_pay;
			tax = calculate_tax(gross_pay, TAX);
			net_pay = calculate_net_pay(gross_pay, tax, deductions);
			output_payroll_data(full_name, regular_hours, overtime_hours, hourly_rate, gross_pay, tax, deductions, net_pay, results, employees);
			summed_gross_pay = summed_gross_pay + gross_pay;
			employees = employees + 1;
		}
    
    }

    sort_employees(employee_gross_pay, employee_names, employees);
  

    if (employees != 0)
        employee_summary(employee_names, employees , employee_gross_pay, summed_gross_pay, min_gross, max_gross, results);
   
    results.close();
    empdata.close();
}


//---------------------------------------------------------------------------------------------------------
//sorts employee gross pay
//---------------------------------------------------------------------------------------------------------
void sort_employees(double employee_gross_pay[], string employee_names[], int employees)
{
	bool swap=true;			//check if you have swapped
	int pass,				//number of passes
		i;					//counting variable
	double temp_gross_pay;	//temporarily holds employee gross pay
	string temp_name;		//temporarily holds employee name

/*bubble sort v3*/
	for (pass = 1; pass < employees && swap; pass ++){
        swap = false;
        for (i=0; i<employees-pass ; i++){

            if (employee_gross_pay[i] < employee_gross_pay[i+1]){
                swap = true;
                temp_gross_pay=employee_gross_pay[i];
                employee_gross_pay[i]=employee_gross_pay[i+1];
                employee_gross_pay[i+1] = temp_gross_pay;
                    

                temp_name=employee_names[i];
                employee_names[i]=employee_names[i+1];
                employee_names[i+1]=temp_name;
            }
                
        }
    }
}
 
		/*continue the rest from the handout online and also archive all online files for later use*/





//---------------------------------------------------------------------------------------------------------
//Displays the employee summary
//---------------------------------------------------------------------------------------------------------
void employee_summary(string employee_names[], int employees, double employee_gross_pay[], double summed_gross_pay, double min_gross, double max_gross, ofstream &results)
{
    int i;
    results << endl;
    results << "==============================" << endl;
    results << "   Employee Payroll Summary   " << endl;
    results << "==============================" << endl;
    results << endl;
    results << "                      Gross   " << endl;
    results << "Name                  Pay     " << endl;
    results << "====================  ========" << endl;
    
    for (i = 0; i < employees; i++)
        results << left << setw(22) << employee_names[i] << right << setw(8) << employee_gross_pay[i] << endl;

    results << endl;
    results << "Total gross pay  : $" << summed_gross_pay << endl;
    results << "Average gross pay: $" << (summed_gross_pay / employees) << endl;
    results << "Minumum gross pay: $" << min_gross << endl;
    results << "Maximum gross pay: $" << max_gross << endl;
}



//---------------------------------------------------------------------------------------------------------
//Gathers necessary employee information
//---------------------------------------------------------------------------------------------------------
void input_employee_data(string &first_name, string &last_name, double &hours, double &hourly_rate, double &deductions, ifstream &empdata)
{
        empdata >> first_name;
        empdata >> last_name;
        empdata >> hours;
        empdata >> hourly_rate;
        empdata >> deductions;
}



//---------------------------------------------------------------------------------------------------------
//Combines the first and last name into the proper format
//---------------------------------------------------------------------------------------------------------
string join_name(string first_name, string last_name)
{
    return last_name + ", " + first_name;
}


//---------------------------------------------------------------------------------------------------------
//Splits how many hours worked are overtime and regular hours
//---------------------------------------------------------------------------------------------------------
void split_hours(double hours, double &regular_hours, double &overtime_hours)
{
    if (hours > 40)
    {

        overtime_hours = hours - 40;
        regular_hours = 40;
    }
    else
    {

        overtime_hours = 0;
        regular_hours = hours;
    }
}


//---------------------------------------------------------------------------------------------------------
//Calculates the gross pay
//---------------------------------------------------------------------------------------------------------
double calculate_gross_pay(double regular_hours, double overtime_hours, double hourly_rate)
{
    return (regular_hours*hourly_rate) + (overtime_hours*(1.5*hourly_rate));
}

//---------------------------------------------------------------------------------------------------------
//Calculates the taxes
//---------------------------------------------------------------------------------------------------------
double calculate_tax(double gross_pay, double tax_rate)
{
    return gross_pay*tax_rate;
}

//---------------------------------------------------------------------------------------------------------
//Calculates the net pay
//---------------------------------------------------------------------------------------------------------
double calculate_net_pay(double gross_pay, double tax, double deductions)
{
    return gross_pay - tax - deductions;
}

//---------------------------------------------------------------------------------------------------------
//outputs the payroll information
//---------------------------------------------------------------------------------------------------------
void output_payroll_data(string formatted_name, double regular_hours, double overtime_hours, double hourly_rate, double gross_pay, double tax, double deductions, double net_pay, ofstream &results, int employees)
{
    if (employees == 0){
        results << "                      Reg.   Ovt.   Hourly  Gross                    Net    " << endl;
        results << "Name                  Hours  Hours  Rate    Pay      Taxes   Deduct  Pay    " << endl;
        results << "====================  =====  =====  ======  =======  ======  ======  =======" << endl;
   }
 
    results << fixed << setprecision(2);
    results << left << setw(20) << formatted_name;
    results << right << setw(7) << regular_hours << setw(7) << overtime_hours << setw(8) << hourly_rate; //hours
    results << setw(9) << gross_pay << setw(8) << tax << setw(8) << deductions << setw(9) << net_pay; // money
    results << endl;
}


//---------------------------------------------------------------------------------------------------------
//Determines what the min and max gross pay is
//---------------------------------------------------------------------------------------------------------
void min_max_gross(double gross_pay, int employees, double &min_gross, double &max_gross)
{
    if (employees == 0){
        max_gross = gross_pay;
        min_gross = gross_pay;
    }
    else if (gross_pay > max_gross)
        max_gross = gross_pay;
    else if (gross_pay < min_gross)
        min_gross = gross_pay;
}
