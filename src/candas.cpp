#include <string>
#include <vector>
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <cmath>
#include <cstdio>
#include <sstream>
#include <algorithm>
using namespace std;
/*CarInfo class keeps department's name and elapsed time  for the car in this department*/
class CarInfo {
public:
    string name;
    int time;
    CarInfo* next;
    CarInfo(string name);
};
CarInfo::CarInfo(string name) {
    this->name = name;
    this->time = 0;
    this->next = NULL;
}
/*Car class keeps the car's information*/
class Car {
public:
    string Car_Name = "candas";
    string Car_ID;
    int Start_Time;
    int Finish_Time = -1;
    CarInfo* carInfo = NULL;
    Car(string name, string id, int start);
};
Car::Car(string name, string id, int start){
    this->Car_Name = name;
    this->Car_ID = id;
    this->Start_Time = start;
}
/*Department class keeps the department's information and methods*/
class Department {
public:
    string Department_Name = "candas";
    int Processing_Time = -1;
    int time_elapsed = 0;
    Car *Processing_Car = NULL;
    Department *next;
    vector<Car*> Processed_Cars;
    int isEmpty();
    Car* proces(string dept_name);
    void removeCar(vector<Car*> carsToRemove);
    string currentProcessing();
    string processedCars();
};
int Department::isEmpty(){
    if(Processing_Car != NULL) {
        return 0;
    }
    return 1;
}
/*This function returns department's statements in a string */
string Department::currentProcessing(){
    string dep="|Report for Department \""+Department_Name+"\"|";
    string temp;
    int var;
    for (var = 0; var <dep.length(); ++var) {
        temp +="-";
    }
    temp +="\n"+dep+"\n";
    for (var = 0; var <dep.length(); ++var) {
        temp +="-";
    }
    temp +="\n";


    if(Processing_Car != NULL)
        return temp + "I am currently processing " + Processing_Car->Car_Name + " "+ Processing_Car->Car_ID;
    else
        return temp + Department_Name + " is now free.";
}
/*This function returns the processed cars in a string */
string Department::processedCars(){
    if(Processed_Cars.size() == 0)
        return "";
    string processedCars = "Processed Cars\n";
    int var;
    for (var = 0; var < Processed_Cars.size(); ++var) {
        processedCars += to_string(var + 1) +". "+Processed_Cars[var]->Car_Name + " " + Processed_Cars[var]->Car_ID + "\n";
    }
    return processedCars;
}
/*This function processes the car which in this department just one time */
Car* Department::proces(string dept_name) {
    if(this->Processing_Car == NULL)
        return NULL;
    this->time_elapsed++;
    CarInfo* info = this->Processing_Car->carInfo;
    while(info != NULL) {
        if(info->name == dept_name && info->time < this->Processing_Time ) {
            info->time++;
            break;
        }
        info = info->next;
    }
    if(Processing_Time <= time_elapsed) {
        return this->Processing_Car;
    }
    return NULL;
}
/*This function removes the car which is ready to send out and refreshes the department*/
void Department::removeCar(vector<Car*> carsToRemove) {
    int var =0;
    for (var = 0; var < carsToRemove.size(); ++var) {
        if(this->Processing_Car !=NULL && carsToRemove[var]->Car_ID == this->Processing_Car->Car_ID) {
            this->Processed_Cars.push_back(this->Processing_Car);
            this->Processing_Car = NULL;
            this->time_elapsed = 0;
        }
    }
}
/*FactoryDepartment class keeps the manager department's infromation and methods*/
class FactoryDepartment {

public:
    string dept_name;
    FactoryDepartment *next_department= NULL;
    Department *department = NULL;
    int checkStatusAfterProcess();
    vector<Car*> processAll(int emptySpace);
    void addCarsToDepartments(vector<Car*> carsToAdd);
    int FindMin(vector<Car*> carList);
};
/*This function add the cars in sub-departments for each opening*/
void FactoryDepartment::addCarsToDepartments(vector<Car*> carsToAdd) {
    Department* dept = this->department;
    int i = 0;
    while(dept != NULL) {
        if(i== carsToAdd.size())
            break;
        if(dept->isEmpty() == 1) {

            dept->Processing_Car = carsToAdd[i];
            dept->time_elapsed = 0;
            i++;
        }
        dept = dept->next;
    }
}
/*This function determines opening for each sub-department and returns the number of empty spaces*/
int FactoryDepartment::checkStatusAfterProcess() {
    int emptySpace = 0;
    Department* temp = this->department;
    while(temp!=NULL) {
        emptySpace += temp->isEmpty();
        temp = temp->next;
    }
    return emptySpace;
}
/*This function processes all of the departments for just one cycle*/
vector<Car*> FactoryDepartment::processAll(int emptySpace) {
    Department* dept = this->department;
    vector<Car*> carsToGo;


    while(dept != NULL) {
        Car* temp = dept->proces(this->dept_name);
        if(temp != NULL) {

            carsToGo.push_back(temp);
        }

        dept = dept->next;
    }


    if(carsToGo.size() <= emptySpace) {
        dept = this->department;
        while(dept != NULL) {
            dept->removeCar(carsToGo);
            dept = dept->next;
        }
        return carsToGo;
    } else {
        vector<Car*> carsToRemove;
        int var;
        for (var = 0; var < emptySpace; ++var) {
            int temp = FindMin(carsToGo);
            carsToRemove.push_back(carsToGo[temp]);

            carsToGo.erase(carsToGo.begin() + temp);
        }
        dept = this->department;
        while(dept != NULL) {
            dept->removeCar(carsToRemove);
            dept = dept->next;
        }
        return carsToRemove;
    }

}
/*This function determines the car which have priority to get in the factory */
int FactoryDepartment::FindMin(vector<Car*> carList) {
    int var;
    int min;
    int minIndex;
    min = carList[0]->Start_Time;
    minIndex = 0;
    for (var = 0; var < carList.size(); ++var) {
        if(carList[var]->Start_Time < min) {
            min = carList[var]->Start_Time;
            minIndex = var;
        }
    }
    return minIndex;
}

FactoryDepartment *Factory = NULL;
/*This function updates the carinfo of all the cars for one cycle */
void initCarInfo(vector<Car*> carList) {
    int var;
    FactoryDepartment* temp;
    for (var = 0; var < carList.size(); ++var) {
        temp = Factory;
        while(temp != NULL) {
            CarInfo* tempInfo = new CarInfo(temp->dept_name);
            if(carList[var]->carInfo == NULL) {
                carList[var]->carInfo = tempInfo;
                temp = temp->next_department;
                continue;
            }
            CarInfo* tempInfo1 = carList[var]->carInfo;
            while(tempInfo1->next != NULL){
                tempInfo1 = tempInfo1->next;
            }
            tempInfo1->next = tempInfo;
            temp = temp->next_department;
        }
    }
}
/*This function takes an integer value and returns as a string type*/
string Conversation(int x) { /*This function convert integer to string and return this string*/
    stringstream ss;
    ss << x;
    string str = ss.str();
    return str;
}
/*This function takes a double type value and return the rounded to the number of decDigits decimal places as a string type*/
string prd(const double x, const int decDigits) {
    stringstream ss;
    ss << fixed;
    ss.precision(decDigits);
    ss << x;
    return ss.str();
}
/*This function takes the file name, read the file's informations, saves in a vector as a string type and returns the vector   */
std::vector<std::string> take_inputs(string file_name) {
    ifstream myfile(file_name);
    string line;
    std::vector<std::string> inputs;
    int i = 0;
    if (myfile.is_open()) {/*open the file*/
        while (getline(myfile, line)) {/*If there is a line add this line the vector*/
            inputs.push_back(line);
            i++;
        }
    }
    myfile.close();/*close the file*/
    return inputs;/*return the vector*/
}
/* this function take a string line and split it form " ". Part of lines will keep in a vector and this vector return*/
std::vector<std::string> str_split(string line) {
    int i;
    std::vector<std::string> words;
    int j = 0;
    for (i = 0; i < line.length(); i++) {
        if (line[i] == ' ') {
            if (j == 0)
                words.push_back(line.substr(j, i - j));
            else
                words.push_back(line.substr(j + 1, i - j - 1));
            j = i;
        }
        if (i == line.length() - 1) {
            words.push_back(line.substr(j + 1, i - j));
        }
    }
    return words;
}

/*This function takes the department's name, the number of the same departments, the processing time, then creates the department and linked each other in linkedlist's format.*/
void Add_Department(string Number, string Dept_Name, string time) {
    int number_of_dept = stoi(Number);

    cout<<"Department "<<Dept_Name<<" has been created."<<endl;
    FactoryDepartment *factory_department = new FactoryDepartment();
    factory_department->dept_name = Dept_Name;
    for (int i = 0; i < number_of_dept; ++i){
        Department* dept = new Department();
        dept->Processing_Time = stoi(time);
        dept->Department_Name = Dept_Name+ " " + Conversation(i+1);
        if(i == 0) {
            factory_department->department = dept;
            dept->next = NULL;
            continue;
        }
        Department *deptP = factory_department->department;
        while(deptP->next != NULL ) {
            deptP = deptP->next;
        }
        deptP->next = dept;
    }
    if(Factory == NULL) {
        Factory = factory_department;
        return;
    }
    FactoryDepartment* fact = Factory;
    factory_department->next_department = fact;
    Factory = factory_department;

}
/*This function prints all of the departments of the factory in the determined format.*/
void Print_Factory() {
    int space = 0;
    FactoryDepartment *f = Factory;
    vector<string> elements;
    while(f != NULL) {
        string output = "";
        Department *d = f->department;
        while(d!=NULL) {
            output += d->Department_Name + " ";
            d = d->next;
        }
        elements.push_back(output);
        f = f->next_department;
    }
    reverse(elements.begin(), elements.end());
    unsigned int i,j;
    for (i = 0; i < elements.size(); i++) {
    	elements[i]="- "+elements[i];
        cout<< elements[i] <<endl;
        space += elements[i].length()-1;
        if(i!=elements.size()-1){
        for(j=0;j<space;j++){
            cout<<" ";
        }
        }
    }
}
/*This function process all factory for just one cycle.(T)*/
void ProcessEverything() {
    vector<Car*> carsToGo= Factory->processAll(999999);
    FactoryDepartment* manager = Factory->next_department;
    int emptySpaces = Factory->checkStatusAfterProcess();
    FactoryDepartment* prev = Factory;
    while(manager != NULL) {
        carsToGo = manager->processAll(emptySpaces);

        prev->addCarsToDepartments(carsToGo);
        emptySpaces = manager->checkStatusAfterProcess();

        prev = prev->next_department;
        manager = manager->next_department;
    }


}
/*This function adds a new car in the factory.*/
vector<Car*> addCarsToFactory(vector<Car*> CarList, int t) {
    FactoryDepartment* dept = Factory;
    while(dept->next_department != NULL) {
        dept = dept->next_department;
    }
    int emptySpaces = dept->checkStatusAfterProcess();
    int var;
    vector<Car*> carsToAdd;
    int count = 0;
    for (var = 0; var < CarList.size(); ++var) {
        if(count == emptySpaces)
            break;
        else if(CarList[var]->Start_Time <= t) {
            carsToAdd.push_back(CarList[var]);
            count++;
        }
    }
    vector<Car*> newList;
    for (var = 0; var < CarList.size(); ++var) {

        int control = 0;
        for (int var1 = 0; var1 < carsToAdd.size(); ++var1) {
            if(CarList[var]->Car_ID == carsToAdd[var1]->Car_ID)
                control = 1;
        }
        if(control == 0) {
            newList.push_back(CarList[var]);
        }
    }
    dept->addCarsToDepartments(carsToAdd);


    return newList;
}
/*This function finds the number of maximum cycle(T) for factory */
int FindMaxTime(vector<string> Reports){
	int max=0;
	int var;
	vector<string> words;
	for (var = 0; var < Reports.size(); ++var) {
		words=str_split(Reports[var]);
		if(stoi(words[1])>max){
			max=stoi(words[1]);
		}
	}
	return max;
}
/*This function returns  department's statement and all the processed cars as a string type for each department */
string ReportForDepartments(){
	string output="";
    FactoryDepartment* temp = Factory;
    while (temp != NULL) {
        Department* dep = temp->department;
        while (dep != NULL) {
        	output += dep->currentProcessing()+"\n";
        	output += dep->processedCars();
            dep = dep->next;
        }

        temp = temp->next_department;
    }
    return output;

}
/*This function returns car's statement as a string type for each car*/
string ReportForCars(vector<Car*> CarList, int totalTimeForProcess){
	string output="";
	for (int i = 0; i < CarList.size(); ++i)
	    {
	    	int var;
	        CarInfo* c = CarList[i]->carInfo;
	        vector<string> list;
	        string car="|Report for "+CarList[i]->Car_Name+" "+CarList[i]->Car_ID+" |";
	        for (var = 0;  var < car.length(); ++ var) {
	        	output +="-";
			}
	        output += "\n"+car+"\n";
			for (var = 0; var < car.length(); ++var) {
				output += "-";
			}
        	output +="\n";
	        int time_elapsed = 0;
	        while(c!= NULL) {
	            list.push_back(c->name + ":" + Conversation(c->time) + ", ");
	            time_elapsed += c->time;
	            c= c->next;
	        }
	        reverse(list.begin(), list.end());
	        for (int i = 0; i < list.size(); ++i)
	        {
	            output += list[i];
	        }
	        float asd = (float)(time_elapsed*100)/totalTimeForProcess;
	        asd=floorf(asd * 100) / 100;
	        string percent=prd(asd,2);
	        output += "| Start Time: " + Conversation(CarList[i]->Start_Time) + " | Complete:";
	        output += percent+"%";
	        if(time_elapsed==totalTimeForProcess) {
	            output += " | Complete\n";
	        } else {
	            output += " | Not complete\n";
	        }


	}
	return output;
}
/*This function returns car's statement as a string type for just one specific car*/
string ReportForOneCar(string carName,string CarID, vector<Car*> CarList, int totalTimeForProcess){
	string output="";
	for (int i = 0; i < CarList.size(); ++i)
	    {
		if(CarList[i]->Car_ID==CarID){
	    	int var;
	        CarInfo* c = CarList[i]->carInfo;
	        vector<string> list;
	        string car="|Report for "+CarList[i]->Car_Name+" "+CarList[i]->Car_ID+" |";
	        for (var = 0;  var < car.length(); ++ var) {
	        	output +="-";
			}
	        output += "\n"+car+"\n";
			for (var = 0; var < car.length(); ++var) {
				output += "-";
			}
        	output +="\n";
	        int time_elapsed = 0;
	        while(c!= NULL) {
	            list.push_back(c->name + ":" + Conversation(c->time) + ", ");
	            time_elapsed += c->time;
	            c= c->next;
	        }
	        reverse(list.begin(), list.end());
	        for (int i = 0; i < list.size(); ++i)
	        {
	            output += list[i];
	        }
	        float asd = (float)(time_elapsed*100)/totalTimeForProcess;
	        asd=floorf(asd * 100) / 100;
	        string percent=prd(asd,2);
	        output += "| Start Time: " + Conversation(CarList[i]->Start_Time) + " | Complete:";
	        output += percent+"%";
	        if(time_elapsed==totalTimeForProcess) {
	            output += " | Complete\n";
	        } else {
	            output += " | Not complete\n";
	        }
	        return output;
	    }

	}
	return output;
}
/*This function returns all output lines for report commands in a vector as a string type*/
vector<string> Report(vector<string> Reports, vector<string> outputs, int t, vector<Car*> CarList, int totalTimeForProcess){
    int var;
    vector<string> words;
    for (var = 0; var < Reports.size(); ++var) {
        words=str_split(Reports[var]);
        if(stoi(words[1]) == t || stoi(words[1]) == t+1){
            if(stoi(words[1]) == t && words.size() == 3 && (words[2]=="Cars" || words[2]=="Cars\r")){
            	string output="\nCommand: "+words[0]+" "+words[2]+" "+words[1]+"\n";
            	output += ReportForCars(CarList, totalTimeForProcess);
            	outputs.push_back(output);
            }
            else if(stoi(words[1]) == t+1 &&words.size() == 3 && (words[2]=="Departments" || words[2]=="Departments\r")){
            	string output="\nCommand: "+words[0]+" "+words[2]+" "+words[1]+"\n";
            	output+=ReportForDepartments();
            	outputs.push_back(output);

            }
            else if(stoi(words[1]) == t && words.size() == 4){
            	string output="\nCommand: "+words[0]+" "+words[2]+" "+words[1]+" "+words[3]+"\n";
            	output +=ReportForOneCar(words[2],words[3],CarList,totalTimeForProcess);
            	outputs.push_back(output);

            }

        }
    }
    return outputs;
}
/*This function prints all the results for report commands*/
void PrintReport(vector<string> Reports,vector<string> outputs){
	int i,j;
    vector<string> words;
	for(i=0;i<Reports.size();i++){
		words=str_split(Reports[i]);
		for(j=0;j<outputs.size();j++){
			if(words.size() == 3){
				if(outputs[j].find(words[0]+" "+words[2]+" "+words[1]) != std::string::npos){
				cout<<outputs[j];
				}
			}
			else if(words.size() == 4){
				if(outputs[j].find(words[0]+" "+words[2]+" "+words[1]+" "+words[3]) != std::string::npos){
					cout<<outputs[j];
				}
			}
		}
	}

}

int main(int argc, char *argv[]) {
	string inputFile=argv[1];
    vector<std::string> inputs = take_inputs(inputFile);
    int i;
    vector<Car*> CarList;
    vector<string> Reports ;
    int totalTimeForProcess = 0;
    for (i = 0; i < inputs.size(); i++) {
        if (inputs[i].find("AddDept") != std::string::npos) {
            std::vector<std::string> words = str_split(inputs[i]);
            Add_Department(words[1], words[2], words[3]);/*Add the departments in the linkedlist*/
            totalTimeForProcess += stoi(words[3]);/*determine total time for process*/
        } else if (inputs[i].find("Produce") != std::string::npos) {
            std::vector<std::string> words = str_split(inputs[i]);
            Car* c = new Car(words[2], words[3], stoi(words[1]));
            CarList.push_back(c);/*Take all car information*/
        } else if (inputs[i].find("Report") != std::string::npos) {
            Reports.push_back(inputs[i]);/*Take all report commands*/
        } else if (inputs[i].find("PrintFactory") != std::string::npos) {
            Print_Factory();/*Print all factory*/
        }
    }
    initCarInfo(CarList);/*initialized all cars*/
    vector<Car*> newList;/*Copy of list of cars*/
    for (int i = 0; i < CarList.size(); ++i)
    {
        newList.push_back(CarList[i]);
    }
    int maxTime=FindMaxTime(Reports)+2;/*number of total cycle */
    int time= 1;
    vector<string> outputs;/*output lines*/
    while(time<maxTime) {
        ProcessEverything();/*Process all factory for just one time*/
        CarList = addCarsToFactory(CarList, time);/*Add cars to in the factory*/
        outputs=Report(Reports,outputs,time-1, newList, totalTimeForProcess);/*Check for report commands and take output lines*/
        time++;/*increase time (T)*/
    }
    PrintReport(Reports,outputs);/*Print all outputs for report commands*/

    return 0;
}
