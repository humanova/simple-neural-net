// 2018 Emir Erbasan (humanova)
// MIT License, see LICENSE for more detail

#include <iostream>
#include <math.h>
#include <cstdlib>
#include <time.h>
#include <string>
#include <fstream>

#define DEFAULT_TRAIN_COUNT 10003
#define DEFAULT_NUM_HIDDEN 6
#define DEFAULT_LEARNING_RATE 1.4142135
#define DEFAULT_MOMENTUM 0.50
#define DEFAULT_PRINT_STATS 0
#define DEFAULT_TAKE_INPUT 0

using namespace std;


//=====================================
//-----Settings and file reading-------
//=====================================

struct Settings{
    int num_Hidden;
    long int trainLoopCount;
    float learningRate;
    float momentum;
    int PrintStats;
    int takeInput;
};

Settings getDefaultSettings(){
    Settings default_settings;
    default_settings.trainLoopCount = DEFAULT_TRAIN_COUNT;
    default_settings.num_Hidden     = DEFAULT_NUM_HIDDEN;
    default_settings.learningRate   = DEFAULT_LEARNING_RATE;
    default_settings.momentum       = DEFAULT_MOMENTUM;
    default_settings.PrintStats     = DEFAULT_PRINT_STATS;
    default_settings.takeInput      = DEFAULT_TAKE_INPUT;

    
    return default_settings; 
}

Settings getSettingsFromFile(){

    ifstream file("settings.txt");
    if(!file){
        cout<<"Error while opening settings.txt...\nUsing default settings...\n\n";
        return getDefaultSettings();
    }

    string line;
    string comment_prefix("//");
    string empty_prefix("\n");
    string::size_type sz;

    Settings file_settings;

    int a = 0;
    try{
        while(getline(file, line))
        {   
            if(!line.compare(0, comment_prefix.size(), comment_prefix) ||
             line == "" ||
             line == " "){
                continue;
            }

            if(a ==0) 
            { 
                long int check = stoi(line,&sz);
                if(check < 10)
                {
                    cout<<"Train loop count cannot be lower than 10... (Default value set)\n";
                    file_settings.trainLoopCount = getDefaultSettings().trainLoopCount;
                }
                else
                    file_settings.trainLoopCount = check;
            }

            else if(a == 1) 
            {   
                int check = stoi(line,&sz);
                if(check < 3){
                    cout<<"Hidden Neuron count cannot be lower than 3... (Default value set)\n";
                    file_settings.num_Hidden = getDefaultSettings().num_Hidden;
                }
                else
                    file_settings.num_Hidden = check;
            }           
            else if(a == 2)
            {   
                float check = strtof((line).c_str(),0);
                if(check < 1.0)
                {
                    cout<<"Learning rate cannot be lower than 1... (Default value set)\n";
                    file_settings.learningRate = getDefaultSettings().learningRate;
                }
                else
                    file_settings.learningRate = check;
            }
            else if(a == 3) 
            { 
                float check = strtof((line).c_str(), 0);
                if(check <= 0.01 && check >= 0.0)
                {
                    cout<<"Momentum cannot be lower than 0.01 and higher than 0.9... (Default value set)\n";
                    file_settings.momentum = getDefaultSettings().momentum;
                }
                else
                    file_settings.momentum = check;
            }
            else if(a == 4)
            {
                int check = stoi(line,&sz); 
                if(check != 0 && check != 1)
                {
                    cout<<"PrintStats cannot be different than 1 or 0... (Default value set)\n";
                    file_settings.PrintStats = getDefaultSettings().PrintStats;
                }
                else
                    file_settings.PrintStats = check;
            }
            else if (a == 5)
            {   
                int check = stoi(line,&sz); 
                if(check != 0 && check != 1)
                {
                    cout<<"TakeInput cannot be different than 1 or 0... (Default value set)\n";
                    file_settings.takeInput = getDefaultSettings().takeInput;
                }
                else
                    file_settings.takeInput = check;

                break;
            }

            a++;
        }

        cout << "Settings are imported from settings.txt\n\n";
        return file_settings;
    }
    catch(const std::exception &exc){
        cerr << exc.what();
        cout<<"Error while getting settings from settings.txt...\nUsing default settings...\n\n";
        return getDefaultSettings();
    }

}


void printCurrentSettings(int X,int H,int Y,float lRate,float mo,int loop){
    cout << "[Neural Net Settings]\nX = "<<X<<"  H = "<<H <<"  Y = "<<Y <<endl;
	cout << "Learning Rate = " << lRate<<"   "<<"\nMomentum = "<<mo<<"\n";
    cout << "Train Loop Count = " << loop << "\n\n";
}

//I had to use 'system' since i haven't created
//python plotter as a subprocess, these 2 programs are
//seperated (their connection is rmseErrorShort.txt)
//.exe writes it, and plotter script reads it and uses it as
//plot data 
void closePlotter(){
    system("taskkill /F /T /IM pythonw.exe");
}

//=====================================
//------Math and other functions-------
//=====================================

//to see expected output
double calcXOR(double input1, double input2){ 
    if(input1 != input2){
        return 1;
    }
    else{
        return 0;
    }
}

//our good-gold sigmoid function 
//used as activation function 
double sigmoid(double x){
    return 1/(1+exp(-x));
}

//sigmoid derivative
double sigmoidTurev(double x){
    return exp(-x)/(pow(1+exp(-x), 2));

}

//randomize a num between 0 and 1
double random(){
    return rand() / double(RAND_MAX);
}

void randWeight(double arr[], int x){      

    for(int i = 0; i< x; i++){
        arr[i] = random();
    }
    
}

//calculates neuron output
//every neuron (except input) uses this func to
//calculate it's output
double calcOutput(double in, double bias){
    return sigmoid(in + bias);

}

//never used this one, it's not ideal
//if you are developing something that 
//errorRate matters
double stepFunction(double x){

    if( x >= 0.91){
        return 1.0;
    }

    if( x <= 0.09){
        return 0;
    }

    if ( x >= 0.41 && x <= 0.59){
        return 0.5;
    }

}

//not used
void transpose(double arr[], const int count, double transposedArr[]) {      //tersine cevir

    double swap;

    for(int i = 0; i < count/2; i++){
        swap = arr[i];
        transposedArr[i] = arr[count - i - 1];
        transposedArr[count - i - 1] = swap;   
    }
    if(count % 2 != 0){
        transposedArr[count/2] = arr[count/2];
    }


}

double calcError(double output, double expected){
    return (output - expected);
}

double clearArray(double arr[],int count){
    for(int i = 0; i < count; i++){
        arr[i] = 0; 
    }
}

double calcRMSE(double err1, double err2, double err3, double err4){
    return sqrt( ((err1 * err1) + (err2 * err2) + (err3 * err3) + (err4 * err4)) / 4);
}

double calcRMSE2(double err1, double err2, double err3, double err4,double err5, double err6, double err7){
    return sqrt( ((err1 * err1) + (err2 * err2) + (err3 * err3) + (err4 * err4) + (err5 * err5) + (err6 * err6) + (err7 * err7)) / 7);
}
