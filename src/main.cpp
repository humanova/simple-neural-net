// Emir Erbasan 08.08.18
// github.com/humanova

#include <iostream>
#include <fstream>
#include <Windows.h>
#include <tchar.h>

#include "neural-net.h"

using namespace std;


bool done = false;
//console control handler for catching close event
BOOL ctrl_handler(DWORD event)
{
    if (event == CTRL_CLOSE_EVENT) {
        done = true;
        closePlotter();  //close python plotter
        return 0;
    }
    return FALSE;
}

int main(){
	//Setting up console handler to close both plotter and this application
	SetConsoleCtrlHandler((PHANDLER_ROUTINE)(ctrl_handler), TRUE);

	srand(time(0));

	//Setting up output .txt files
	ofstream weightFile;
	ofstream debugFile;
	ofstream errorFile;
	ofstream errorFile2;

	weightFile.open("Neural-Net-Settings\\Weights.txt", ios::out | ios::trunc);
	debugFile.open("Neural-Net-Settings\\Debug.txt", ios::out | ios::trunc);
	errorFile.open("Neural-Net-Settings\\rmseError.txt", ios::out | ios::trunc);
	errorFile2.open("Neural-Net-Settings\\rmseErrorShort(percent).txt", ios::out | ios::trunc);

	//Run error plotter python script
	ShellExecute(NULL, _T("open"), _T("run_plotter.bat"), NULL, NULL, SW_SHOWNORMAL);
	
	//get settings from settings.txt. if couldn't, use default ones 
	//default = (hidden_num = 6, learning rate = sqrt(2), momentum = 0.50)
	Settings settings;
	settings = getSettingsFromFile();

	//set 0 to disable all "cout"s (excluding the last one and user inputs) and speed up the net
	//it has a big impact on the net speed
	int PRINT_STATS = settings.PrintStats; 
	
	//xor operator training data & answer data
	double trainingData[4][2] = { { 1, 0 }, { 1, 1 }, { 0, 1 }, { 0, 0 }};
	double answerData[4] = { 1, 0 , 1, 0};

	const int
	 	numInput = 2,
	 	numHidden = 6,
	 	numOutput = 1;

	const int
		X = numInput,
 		H = numHidden,
		Y = numOutput,
		W1 = X * H,
		W2 = H * Y;

	double input[X];
	double answer[Y];

	double sqrErrLoss;
	double error[4];
	double errorRMSE;

	double learningRate = settings.learningRate;	//changeable (1 - ...)  (default : 1.4142135)
	double momentum = settings.momentum;			//changeable (0 - 0.9) stay below 0.90 (default : 0.50)

	double biasH = 1;
	double biasY = 1;

	double hiddenInput[H][X];
	double hiddenInputSum[H];
	double hiddenOutput[H];
	double outputInput[Y][H];
	double outputInputSum[Y];
	double output[Y];

	double Weight1[W1];
	double Weight2[W2];
	//double biasHWeight[H];
	//double biasYWeight[Y];

	double hiddenDerivative[H];
	double outputDerivative[Y];

	double gradientWeight1[W1];
	double gradientWeight2[W2];
	//double gradientBiasYWeight[Y];
	//double gradientBiasHWeight[H];

	double prevWeight1Update[W1];
	double prevWeight2Update[W2];
	//double prevBiasYUpdate[Y];
	//double prevBiasHUpdate[H];

	int epoch=0;
	int loop= settings.trainLoopCount;
	int trainLoop=0;
	int denemeModu;	//when epoch hits loop-1, it takes 4 inputs from user,sets denemeModu to 1,
					// to disable back propagation (disables updating weights)

	randWeight(Weight1,W1);
	randWeight(Weight2,W2);
	//randWeight(biasHWeight,H);
	//randWeight(biasYWeight,Y);

	clearArray(prevWeight1Update,W1);
	clearArray(prevWeight2Update,W2);
	//clearArray(prevBiasYUpdate,Y);
	//clearArray(prevBiasHUpdate,H);


	printCurrentSettings(X,H,Y,learningRate,momentum,loop);
	cout<< "== Training Started ==" <<endl;
	while(epoch < loop){
		for(trainLoop = 0; trainLoop < 4; trainLoop++ ){	//load training data
			input[0] = trainingData[trainLoop][0];
			input[1] = trainingData[trainLoop][1];
			answer[0] = answerData[trainLoop];

			if(epoch == loop-1){
				if(!settings.takeInput == 0){

					cout<<"\n\n Input 1 : "; cin>>input[0];
					cout<<"\n Input 2 : "; cin>>input[1];
			
					denemeModu = 1;
				}
			}

			clearArray(hiddenInputSum,H);
			clearArray(outputInputSum,Y);

		// =============== FORWARD PROPAGATION ===============//
		//sum hidden layer inputs  [ XW1 ]
		//activate hidden layer sums with sigmoid function [ f(XW1 + bias) ]
		//sum output layer inputs [ HW2 ]
		//activate output layer sums with sigmoid function [ f(HW2 + bias) ]

			for(int a = 0; a < H; a++){								   	//sum hiddenInput * weight1
				for(int i = 0; i < X ; i++){
					hiddenInput[a][i] = input[i];
					hiddenInputSum[a] += hiddenInput[a][i] * Weight1[i + a*X];
				}
			}

			for(int a = 0; a < H; a++){									//activate hiddenInputSum
				hiddenOutput[a] = calcOutput(hiddenInputSum[a], biasH);
			}

			for(int a = 0; a < Y; a++){									//sum outputInput * weight2
				for(int i = 0; i < H ; i++){
					outputInput[a][i] = hiddenOutput[i];
					outputInputSum[a] += outputInput[a][i] * Weight2[i + a*H];
				}
			}

			for(int a = 0; a < Y; a++){									//activate outputInputSum (output)
				output[a] = calcOutput(outputInputSum[a], biasY);
			}

		// ================= END OF FORWARD PROP ==================//
			
			
			//Print current status
			for(int a = 0; a < Y; a++){
				if(denemeModu != 1){
					error[trainLoop] = calcError(output[a],answer[a]);
					if (PRINT_STATS == 1){
						cout<<"GEN [ "<< epoch +1 << " ]\n";
						cout<<"expected : "<<answer[a]<<" | ";
						printf("output : %.5f \n",output[a]);
						printf("error : %.5f \n",fabs(error[trainLoop]));
					}
				}
				else{
					printf("\n output : %.5f \n\n",output[a]);
				}
			}
			


		// ================ BACK PROPAGATION ================== //
		//calculate derivatives
		//calculate gradients
		//calculate new weights
		//update weights

			// ======== DERIVATIVES  ======== //
			//note about sigmoidTurev() : Turev means Derivative in Turkish, I am very lazy to fix it

			//calculate derivatives
			if(denemeModu!=1){
				for(int i = 0; i < Y; i++){		//calc output derivatives
					outputDerivative[i] = -calcError(output[i],answer[i]) * sigmoidTurev(outputInputSum[i]);
				}

				for(int a = 0; a < Y; a++){
					for(int i = 0; i < H; i++){		//calc hidden derivatives
						hiddenDerivative[H * a + i] = sigmoidTurev(hiddenInputSum[i]) * Weight2[i] * outputDerivative[a];
					}
				}

			// ======== GRADIENTS ======== //

				//calculate gradients
				for(int a = 0; a < H; a++){		//hidden neuron gradients
					for(int i = 0; i < X; i++){
						gradientWeight1[X * a + i] = sigmoid(input[i]) * hiddenDerivative[a];
					}
				}

				for(int a = 0; a < Y; a++){		//output neuron gradients
					for(int i = 0; i < H; i++){
						gradientWeight2[H * a + i] = hiddenOutput[i] * outputDerivative[a];
					}
				}

				/*
				for(int a = 0; a < H; a++){		//hidden bias gradients
					gradientBiasHWeight[a] = sigmoid(biasH) * hiddenDerivative[a];
				}

				for(int a = 0; a < Y; a++){		//output bias gradients
					gradientBiasYWeight[a] = sigmoid(biasY) * outputDerivative[a];
				}*/

			// ========= UPDATE WEIGHTS ========= //

				for(int i = 0; i < W1; i++){	//update weight1 (input - hidden W)
					Weight1[i] += (learningRate * gradientWeight1[i]) + (momentum * prevWeight1Update[i]);
					prevWeight1Update[i] = (learningRate * gradientWeight1[i]) + (momentum * prevWeight1Update[i]);
				}

				for(int i = 0; i < W2; i++){	//update weight2 (hidden - output W)
					Weight2[i] += (learningRate * gradientWeight2[i]) + (momentum * prevWeight2Update[i]);
					prevWeight2Update[i] = (learningRate * gradientWeight2[i]) + (momentum * prevWeight2Update[i]);
				}

				/*
				for(int i = 0; i < H; i++){		//update hidden bias
					biasHWeight[i] += (learningRate * gradientBiasHWeight[i]) + (momentum * prevBiasHUpdate[i]);
					prevBiasHUpdate[i] = (learningRate * gradientBiasHWeight[i]) + (momentum * prevBiasHUpdate[i]);
				}

				for(int i = 0; i < Y; i++){		//update output bias
					biasYWeight[i] += (learningRate * gradientBiasYWeight[i]) + (momentum * prevBiasYUpdate[i]);
					prevBiasYUpdate[i] = (learningRate * gradientBiasYWeight[i]) + (momentum * prevBiasYUpdate[i]);
				}*/
			}
		// =============== END OF BACK PROP ===============//
  		}

	//========== SAVE ERROR DATA =========//
		
		if(denemeModu != 1){
			errorRMSE = calcRMSE(error[0],error[1],error[2],error[3]);
			errorFile << epoch << "  " << errorRMSE << endl;

			if(epoch == 0 || epoch % 10 == 0){
				errorFile2.open("Neural-Net-Settings\\rmseErrorShort(percent).txt", ios::out | ios::app);

				errorFile2 <<epoch<<" "<< errorRMSE * 100 << endl;
				
				if(loop - 3 == epoch){
					errorFile2 << "-end-";
				}
				errorFile2.close();
			}

			if(PRINT_STATS == 1){
				cout<<"\n RMSE ERROR = "<<errorRMSE;
				cout<<"\n======================="<<endl;
			}
		}

		else{
			cout<<"\nLast RMSE Error = "<<errorRMSE<<endl;
		}

  		epoch++;
	}
	cout<< "== Training Complete ==\n"<<endl;

	//========== SAVE WEIGHTS AND OTHER DATA ===========//

	weightFile<< "BiasH = "<<biasH<<endl<<"BiasY = "<<biasY<<"\n\n";

	debugFile << "[Neural Net Settings]\nX = "<<X<<"  H = "<<H <<"  Y = "<<Y <<endl;
	debugFile<< "learningRate = "<<learningRate<<"   "<<"\nmomentum = "<<momentum<<endl;
	debugFile<<"\n---------- input layer ----------\n"<<endl;

	for(int a = 0; a < X; a++){
		debugFile<< "Input ["<< a <<"] = "<< input[a]<<endl;

	}

	debugFile<<"\n---------- hidden layer ----------"<<endl;
	weightFile<<"\n---------- input - hidden layer ----------\n"<<endl;
	for(int a = 0; a < H; a++){
		debugFile<< "\n\nHidden Input Sum ["<< a<<"] = "<< hiddenInputSum[a]<<"\n==================\n";
		for(int i = 0; i < X ; i++){
			debugFile<< "Hidden Input ["<< a <<" " << i << "] = "<< hiddenInput[a][i]<<endl;
			weightFile<<"Weight1 for hidden/input "<<a <<"/"<< i<< "  : "<<Weight1[i + a*X]<<endl;
		}
		weightFile<< "\n==================\n";
	}

	for(int a = 0; a < H; a++){
		debugFile << "\nHiddenOutput ["<<a<<"] = "<< hiddenOutput[a];
	}
	debugFile<<"\n\n---------- output layer ----------"<<endl;
	weightFile<<"\n---------- hidden - output layer ----------"<<endl;
	for(int a = 0; a < Y; a++){
		debugFile<< "\nOutput Input Sum ["<< a<<"] = "<< outputInputSum[a]<<"\n===================\n";
		for(int i = 0; i < H; i++){
			debugFile<< "Output Input ["<< a <<" " << i << "] = "<< outputInput[a][i]<<endl;
			weightFile<<"\nWeight2 for output/hidden "<<a <<"/"<< i<< "  : "<<Weight2[i + a*H];
		}
		weightFile<< "\n==================\n";
	}

	for(int a = 0; a < Y; a++){
		debugFile << "\n\nExpected = "<<calcXOR(input[0],input[1]) <<" | "<<" Output ["<<a<<"] = "<< output[a]<<"\n\n";
	}
	cout << "Neural-Net Data has been saved to the Neural-Net-Settings folder.\n\n";
	//========== END OF SAVE DATA ===========//

	system("pause");
	return 0;
}
