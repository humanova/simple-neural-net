//Emir Erbasan 07/06/18 
//github.com/humanova

#include <iostream>
#include <math.h>
#include <cstdlib>
#include <time.h>

using namespace std;

//to see expected output
double calcXOR(double input1, double input2){ 
    if(input1 != input2){
        return 1;
    }
    else{
        return 0;
    }
}

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

double calcOutput(double in, double bias){
    return sigmoid(in + bias);

}

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


