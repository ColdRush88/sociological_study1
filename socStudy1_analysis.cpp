#include <iostream>
#include <string>
#include <fstream>
#include "/home/user/job/squire3/routine/extract.hpp"
#include <gsl/gsl_cdf.h>
using namespace std;

void soc1_analysis(string file) {
	ifstream inputRead(file.c_str(),ifstream::in);
	string buffer;
	vector <string> data;
	while(inputRead.good()) {
		getline(inputRead,buffer);
		if(buffer.size()<1) {
			continue;
		}
		vector <string> line=extract2(buffer,"	");
		for(int i=0;i<line.size();i++) {
			data.push_back(line[i]);
		}
	}
	inputRead.close();
	vector <int> counts(3,0);
	for(int i=0;i<data.size();i++) {
		if(data[i][0]=='0') {
			counts[0]++;
		}
		if(data[i][0]=='1') {
			counts[1]++;
		}
		if(data[i][0]=='2') {
			counts[2]++;
		}
	}
	int negNeutCount=counts[0]+counts[2];
	int supNeutCount=counts[1]+counts[2];
	unsigned int n=data.size();
	//Evaluate proportion of neg+neut
	double p;
	bool haveLowerLimit=false;
	bool haveUpperLimit=false;
	double lowerLimit_negNeut,upperLimit_negNeut;
	for(int i=0;i<100;i++) {
		p=0.01*(double)(i+1);
		double pValue=gsl_cdf_binomial_P(negNeutCount,p,n);
		if(pValue<0.95 && !haveLowerLimit) {
			lowerLimit_negNeut=p;
			haveLowerLimit=true;
		}
		if(pValue<0.05 && !haveUpperLimit) {
			upperLimit_negNeut=p;
			haveUpperLimit=true;
		}
	}
	haveLowerLimit=false;
	haveUpperLimit=false;
	//Evaluate proportion of sup+neut
	double lowerLimit_supNeut,upperLimit_supNeut;
	for(int i=0;i<100;i++) {
		p=0.01*(double)(i+1);
		double pValue=gsl_cdf_binomial_P(supNeutCount,p,n);
		if(pValue<0.95 && !haveLowerLimit) {
			lowerLimit_supNeut=p;
			haveLowerLimit=true;
		}
		if(pValue<0.05 && !haveUpperLimit) {
			upperLimit_supNeut=p;
			haveUpperLimit=true;
		}
	}
	//Evaluate proportion of sup/neg excluding neut
	n=counts[0]+counts[1];
	haveLowerLimit=false;
	haveUpperLimit=false;
	double lowerLimit_neg,upperLimit_neg;
	for(int i=0;i<100;i++) {
		p=0.01*(double)(i+1);
		double pValue=gsl_cdf_binomial_P(counts[0],p,n);
		if(pValue<0.95 && !haveLowerLimit) {
			lowerLimit_neg=p;
			haveLowerLimit=true;
		}
		if(pValue<0.05 && !haveUpperLimit) {
			upperLimit_neg=p;
			haveUpperLimit=true;
		}
	}
	//Evaluate proportion of negToAll 
	n=counts[0]+counts[1]+counts[2];
	haveLowerLimit=false;
	haveUpperLimit=false;
	double lowerLimit_negToAll,upperLimit_negToAll;
	for(int i=0;i<100;i++) {
		p=0.01*(double)(i+1);
		double pValue=gsl_cdf_binomial_P(counts[0],p,n);
		if(pValue<0.95 && !haveLowerLimit) {
			lowerLimit_negToAll=p;
			haveLowerLimit=true;
		}
		if(pValue<0.05 && !haveUpperLimit) {
			upperLimit_negToAll=p;
			haveUpperLimit=true;
		}
	}
	//Evaluate proportion of suppToAll
	n=counts[0]+counts[1]+counts[2];
	haveLowerLimit=false;
	haveUpperLimit=false;
	double lowerLimit_supToAll,upperLimit_supToAll;
	for(int i=0;i<100;i++) {
		p=0.01*(double)(i+1);
		double pValue=gsl_cdf_binomial_P(counts[1],p,n);
		if(pValue<0.95 && !haveLowerLimit) {
			lowerLimit_supToAll=p;
			haveLowerLimit=true;
		}
		if(pValue<0.05 && !haveUpperLimit) {
			upperLimit_supToAll=p;
			haveUpperLimit=true;
		}
	}
	//////////////////////////////////////////
	//Output
	cout<<"95% доверительный интервал для/confidence interval for (0+2)/(0+1+2): "<<lowerLimit_negNeut<<" - "<<upperLimit_negNeut<<endl;
	cout<<"95% доверительный интервал для/confidence interval for (1+2)/(0+1+2): "<<lowerLimit_supNeut<<" - "<<upperLimit_supNeut<<endl;
	cout<<"95% доверительный интервал для/confidence interval for (0/(0+1+2): "<<lowerLimit_negToAll<<" - "<<upperLimit_negToAll<<endl;
	cout<<"95% доверительный интервал для/confidence interval for 1/(0+1+2): "<<lowerLimit_supToAll<<" - "<<upperLimit_supToAll<<endl;
	cout<<"95% доверительный интервал для/confidence interval for 0/(0+1): "<<lowerLimit_neg<<" - "<<upperLimit_neg<<endl;
}

int main(int argc,char*argv[]) {
	soc1_analysis(argv[1]);
}
