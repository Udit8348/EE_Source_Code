#include "main.h"
#include <string>
#include <fstream>
#include <iostream>
using namespace pros;

int a, b = 0;
int count = 0;
bool bHasSettled = false;
int independent = 0;
int trialNumber = 0;
int startTime = 0;
std::ofstream outputFILE;
std::string outputDIR = "ee_data_sept_7_05_";	//file directory
std::string outputPATH = "/usd/" + outputDIR + std::to_string(independent) + "_" + std::to_string(trialNumber) + ".csv";
std::string outputDATA = "";
std::string outputTIME = "";
ADIEncoder encoder (1, 2, false);
ADIDigitalIn startBtn (3);

void resetEncoder(){
	encoder.reset();
}

void saveTrial(){
	lcd::clear_line(0);
	lcd::clear_line(1);
	//Update path
	outputPATH = "/usd/" + outputDIR + std::to_string(independent) + "_" + std::to_string(trialNumber) + ".csv";
	outputFILE.open (outputPATH);
	outputFILE << outputTIME + "\n";	//x-axis data
	outputFILE << outputDATA + "\n";	//y-aixs data
	outputFILE.close();
	lcd::set_text(0, "Test Saved");
}

void viewLiveValues(void* param){
	encoder.reset();
	while(1){
		//Always display all sensor values
		lcd::print(6,"%d", encoder.get_value());
		lcd::print(7,"%d", startBtn.get_value());

		//Display all output information when the arm is pushed forward 90 degrees
		if(encoder.get_value() < -90){
			lcd::print(3, "%s", outputDATA);
			lcd::print(4, "%s", outputTIME);
			lcd::print(5, "%s", outputPATH);
		}else{
			lcd::clear_line(3); lcd::clear_line(4); lcd::clear_line(5);
		}
		delay(10);
	}
}


void opcontrol() {
	lcd::register_btn2_cb(saveTrial);	//right button
	lcd::register_btn0_cb(resetEncoder);
	Task debug(viewLiveValues, NULL);
	delay(50);

	//Does Test Prodecure 5 Times for 5 Different Masses
	for (int j; j < 5; j++){
		//keep track of independent variable
		independent = j;
	for (int i = 0; i < 5; i++) {
		//keep track of trial number
		trialNumber = i;

		//Wait for arm to be pulled back to the button
		while(startBtn.get_value() == 0){
			lcd::set_text(0, "Pull Back Arm");
			delay(2);
		}

		//Resets test variables
		a = 0; b = 0; count = 0; bHasSettled = false;

		//Waits for arm to be released from the button
		while(startBtn.get_value()){
			lcd::set_text(0, "Release Arm");
			delay(2);
		}
		startTime = millis();

		//Completes data collection until the arm has settled
		while(!bHasSettled){
			lcd::set_text(0, "Testing...");
			//Log time
			if(!a){
				outputTIME+= std::to_string(millis() - startTime); a = 1;
			}else{
				outputTIME+= "," + std::to_string(millis() - startTime);
			}

			//log data
			if(!b){
				outputDATA += std::to_string(encoder.get_value()); b = 1;
			}else{
				outputDATA += "," + std::to_string(encoder.get_value());
			}

			//counts how long the arm remains within the accepted settled zone
			if(abs(encoder.get_value()) < 9){
				count++;
			}else{
				count = 0;
			}

			//determines that the test is complete when it has remained settled for 10 counts
			if(count > 10){
				bHasSettled = true;
			}
			//samples data every 50 ms (20 Hz)
			delay(50);
		}

		encoder.reset();
		lcd::print(0, "IDV %d, Trial %d Complete", j+1, i + 1);
		delay(500);
		saveTrial();
	}//end of trials for loop

	lcd::print(0, "All IDV %d Trials Complete", independent + 1);
	delay(2000);
}//end of independent variable for loop

//lcd::clear();
lcd::set_text(0, "TESTING COMPLETE :)");
}
