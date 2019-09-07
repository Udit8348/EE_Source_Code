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
std::string outputDIR = "ee_data_9_6_19_10__31";	//file directory

std::string outputPATH = "/usd/" + outputDIR + std::to_string(independent) + "_" + std::to_string(trialNumber) + ".csv";
std::string outputDATA = "";
std::string outputTIME = "";
ADIEncoder encoder (1, 2, false);
ADIDigitalIn startBtn (3);

void redoCollection() {
	trialNumber = 0;
}

void incrementIndepentVariable(){
	independent++;
	trialNumber = 0;
	encoder.reset();
	a = 0;
	b = 0;
	outputDATA = "";
	outputTIME = "";
	outputPATH = "/usd/" + outputDIR + std::to_string(independent) + "_" + std::to_string(trialNumber) + ".csv";

}

void saveTrial(){
	lcd::clear_line(0);
	lcd::clear_line(1);
	lcd::set_text(0, "Test Saved");
	outputFILE.open (outputPATH);
	outputFILE << outputDATA + "\n";
	outputFILE << outputTIME + "\n";
	outputFILE.close();
}

void viewLiveValues(void* param){
	while(1){
		lcd::print(6,"%d", encoder.get_value());
		lcd::print(7,"%d", startBtn.get_value());

		if(encoder.get_value() < -40){
				lcd::print(3, "%s", outputDATA);
				lcd::print(4, "%s", outputTIME);
				lcd::print(5, "%s", outputPATH);
			}else{
				lcd::clear_line(3);
				lcd::clear_line(4);
				lcd::clear_line(5);

			}

		delay(10);
	}
}


void opcontrol() {
	lcd::register_btn0_cb(incrementIndepentVariable);	//left button
	lcd::register_btn1_cb(redoCollection);	//middle button
	lcd::register_btn2_cb(saveTrial);
	encoder.reset();
	Task debug(viewLiveValues, NULL);


	while (true) {
	/*
if(startBtn.get_value()){
			if(!b){
				outputTIME+= std::to_string(millis());
				b = 1;
			}else{
				outputTIME+= "," + std::to_string(millis());
			}

			if(!a){
				outputDATA += std::to_string(1);
				a = 1;
			}else{
				outputDATA += "," + std::to_string(1);
			}
		}
*/
// Test Prodecure starts here
		while(startBtn.get_value() == 0){
			lcd::set_text(0, "Pull Back Arm");
			delay(2);
		}

		//reset test variables
		a = 0;
		b = 0;
		count = 0;
		bHasSettled = false;

		while(startBtn.get_value()){
			lcd::set_text(0, "Release Arm");
			delay(2);
		}

		startTime = millis();

		while(!bHasSettled){

			//Log time
			if(!b){
				outputTIME+= std::to_string(millis() - startTime);
				b = 1;
			}else{
				outputTIME+= "," + std::to_string(millis() - startTime);
			}

			//log data
			if(!a){
				outputDATA += std::to_string(encoder.get_value());
				a = 1;
			}else{
				outputDATA += "," + std::to_string(encoder.get_value());
			}

			//check if settled
			if(abs(encoder.get_value()) < 3){
				count++;
			}else{
				count = 0;
			}

			//complete test when its considered settled (% or just a const?)
			if(count > 10){
				bHasSettled = true;
			}
			delay(50);
		}

		lcd::set_text(0, "Test Complete");
		lcd::set_text(1, "Please Save");	//save trial by pressing right button

		//reset position to where it has stopped
		encoder.reset();

		while(1){
			delay(2);
		}

		delay(10);	// 10ms delay
	}
}
