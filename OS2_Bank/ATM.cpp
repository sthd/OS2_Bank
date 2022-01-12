//
//  ATM.cpp
//  OS2_Bank
//
//  Created by Elior on 07/01/2022.
//  Copyright © 2022 Elior. All rights reserved.
//

#include "ATM.hpp"


/*
  pre = "pre error, num arg=" + std::to_string(num_arg) + "  I am atm: " + std::to_string(ATM);
  cout << pre << endl;
  pr =" I am atm: " + std::to_string(ATM) + "   reading line:" + std::to_string(li) + "   num_arg: " + std::to_string(num_arg) + "  arg1: " + std::to_string( *args[1] ) + "  arg2: " + std::to_string( *args[2] );
  pr =" I am atm: " + std::to_string(ATM) + "   reading line:" + std::to_string(li) + "   num_arg: " + std::to_string(num_arg) + "  arg1: " + std::to_string(stoi(args[1])) + "  arg2: " + std::to_string(stoi(args[2]));
 
  if (num_arg > 1)
      cout << pr << endl;
 
 //cout << "I am atm: " << ATM << " perform line: " << li << " args are: " << stoi(args[1]) << " " << stoi(args[2]) << endl;
 */













/*
void testOpenAccounts(){
    Lloyds.openAccount(12, 1234, 1111, 23);
    Lloyds.openAccount(12, 900, 1111, 23);
    Lloyds.openAccount(12, 2004, 1111, 23);
    Lloyds.openAccount(12, 1870, 1111, 23);
    Lloyds.openAccount(12, 123, 1111, 23);
    Lloyds.openAccount(12, 50004, 1111, 23);
}
*/

/*
std::vector<std::string> readFileToVector(const std::string& filename){
    std::ifstream source;
    source.open(filename);
    std::vector<std::string> lines;
    std::string line;
    while (std::getline(source, line))
    {
        lines.push_back(line);
    }
    return lines;
}
*/

//char lineSize[MAX_LINE_SIZE];

//std::string filename = "atm1.txt" ;



/*
cout << "ATM1 reading instrucions: " << endl;
readMe(1, argv[2]);
cout << endl;
cout << "ATM2 reading instrucions: " << endl;
readMe(2, argv[3]);
cout << endl;
cout << "ATM3 reading instrucions: " << endl;
readMe(3, argv[4]);
cout << "Finished reading" << endl;
*/


/*
 while(1 && num_arg!= 0){
       fgets(lineSize, MAX_LINE_SIZE, stdin); //lineSize is our variable
       strcpy(cmdString, lineSize);
       cmdString[strlen(lineSize)-1]='\0';
       char* cmd;
       char* args[MAX_ARG];
       num_arg = 0;
       cmd = strtok(cmdString, delimiters);
       if (cmd == NULL)
           return 0;
       args[0] = cmd;
       for (i=1; i<MAX_ARG; i++){
           args[i] = strtok(NULL, delimiters);
           if (args[i] != NULL)
               num_arg++;
       }
       cout << "command is: " << cmd << " num args: " << num_arg << " now the parameters are:  ";
       for (int j=1; j<num_arg+1; j++){
           cout << args[j] << " ";
       }
       lineSize[0] = '\0';
       cmdString[0] ='\0';
       cout << endl;
   }
 */



/*
 Lloyds.openAccount(12, 1234, 1111, 23);
 Lloyds.openAccount(12, 1234, 1111, 0);
 
 //Lloyds.closeAccount(13, 1234, 1110);
 //Lloyds.closeAccount(14, 1234, 1111);
 Lloyds.moneyTransfer(12, 1321, 2222, 3214, 30);  // no source
 Lloyds.moneyTransfer(12, 1234, 2222, 3214, 30); // no password
 Lloyds.moneyTransfer(12, 1234, 1111, 3214, 30); // no money
 Lloyds.moneyTransfer(12, 1234, 1111, 3214, 10); // no target
 Lloyds.openAccount(12, 3214, 1111, 25);
 
 Lloyds.moneyTransfer(12, 1234, 1111, 3214, 10); // should work
 
 cout << "Pause " << endl;

 printf("\033[2J");
 printf("\033[1;1H");
 for (std::vector<Account>::iterator it=Lloyds.accountVector.begin(); it !=Lloyds.accountVector.end(); ++it){
     cout << it->getID() << endl;
 }
 sleep(3);
 printf("\033[2J");
 printf("\033[1;1H");
 cout << " mahakti" << endl;
 for (std::vector<Account>::iterator it=Lloyds.accountVector.begin(); it !=Lloyds.accountVector.end(); ++it){
     cout << it->getID() << endl;
 }
 
 //char nextPwd[MAX_LINE_SIZE] ="";
 //char oldPwd[MAX_LINE_SIZE] ="";
 //char tmpPwd[MAX_LINE_SIZE] ="";
 //char dash[] = "-";
 */


    
    //ifstream
    //getline(input, )
        
    
    
    /*
     int ExeCmd(char* lineSize, char* cmdString){
        char* cmd;
        char* args[MAX_ARG];
        //char pwd[MAX_LINE_SIZE];
        //char* delimiters = " \t\n";
        int i = 0, num_arg = 0;
        bool illegal_cmd = false; // illegal command
            cmd = strtok(lineSize, delimiters);
        if (cmd == NULL)
            return 0;
        args[0] = cmd;
        for (i=1; i<MAX_ARG; i++)
        {
            args[i] = strtok(NULL, delimiters);
            if (args[i] != NULL)
                num_arg++;
     
        }
     */
    
    

    
    //join threads
    
    
    //free memory + last prints
    
