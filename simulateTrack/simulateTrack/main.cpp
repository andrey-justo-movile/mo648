//
//  main.cpp
//  simulateTrack
//
//  Created by Andrey Justo on 11/27/15.
//  Copyright © 2015 Andrey Justo. All rights reserved.
//

/*
 Segundo exercício: Fazer uma simulação com trace de dados como ordenador de eventos (chegada de pacote), plotar gráfico tamanho médio da fila x utilização. Entregar relatório com código fonte e gráfico e explicação sobre o simulador e resultados encontrados. Os registros dos arquivos contém a hora de chegada do pacotes em segundos e o tamanho do pacote.
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <math.h>
#include <stdio.h>
#include "gnuplot_i.hpp"
#include <vector>

using namespace std;

const string INPUT_FILE = "input.txt";
const string RESULT_FILE = "result.dat";
const string RESULT_IMAGE = "result.jpeg";
const int BYTES_NUMBER = 8;

vector<string>* split(string source) {
  vector<string> *splittedStrings = new vector<string>;
  istringstream buf(source);
  istream_iterator<std::string> beg(buf), end;
  
  std::vector<std::string> tokens(beg, end); // done!
  
  for(auto& s: tokens) {
    splittedStrings->push_back(s);
  }

  return splittedStrings;
}

double calculateDelay(vector<double> timeline, vector<double> works, double &deliveredPackets, int index, double queueWork) {
  double currentDelay = timeline[0];
  double currentTime = timeline[index];
  
  for (int i = 0; i <= index; i++) {
    if (currentDelay < timeline[i]) {
      currentDelay = timeline[i] + works[i] / queueWork;
    } else {
      currentDelay += works[i] / queueWork;
    }
    
    if (currentTime > currentDelay) {
      deliveredPackets = i + 1;
    }
  }
  
  return currentDelay - currentTime;
}

double distExpNumberProb(double lambda, double x) {
  if (x < 0 || lambda <= 0) {
    return 0;
  }
  
  return lambda * (exp(-lambda*x));
}

double generateQueueWork(int argc, const char * argv[]) {
  if (argc < 2) {
    double lambda = (double)rand()/(double)INT16_MAX;
    double x = (double)rand()/(double)INT16_MAX;
    return distExpNumberProb(lambda, x) * BYTES_NUMBER;
    //return distExpNumberProb(100, 0.005) * BYTES_NUMBER;
  }
  
  return distExpNumberProb(atof(argv[1]), atof(argv[2])) * BYTES_NUMBER;
}

int main(int argc, const char * argv[]) {

  double queueWorkByMicroseconds = generateQueueWork(argc, argv) * 1000000;
  
  cout << "Current Queue Work " << queueWorkByMicroseconds;

  vector<double> *times = new vector<double>();
  vector<double> *works = new vector<double>();
  ifstream inputFile;
  inputFile.open(INPUT_FILE);
  string line = "";
  cout.precision(30);
  
  ofstream resultFile(RESULT_FILE);
  resultFile << "# X Y \n";
  Gnuplot gnuplot;
  gnuplot.set_terminal_std("jpeg");
  gnuplot.showonscreen();

  double currentTime = 0.0;
  double currentWork = 0.0;
  while (inputFile >> currentTime >> currentWork) {
    //getline(inputFile,line);
    
    // X and Y
    times->push_back(currentTime);
    works->push_back(currentWork);    
  }
  
  inputFile.close();
  
  // new variables
  double averageUseOfQueue[times->size()];
  double averageSizeOfQueue[times->size()];

  // calculate averages
  double busyTime = 0.0;
  double deliveredPackets = 0;
  double timeSpent = 0;
  
  for (int k = 0; k < times->size(); k++) {
    calculateDelay(*times, *works, deliveredPackets, k, queueWorkByMicroseconds);

    timeSpent = works->at(k) / (queueWorkByMicroseconds);
    
    if ((busyTime + timeSpent) < times->at(k)) {
      busyTime += timeSpent;
    } else {
      busyTime = times->at(k) - times->at(0);
    }
    
    if (times->at(k) != 0) {
      // calculate average use of queue
      averageSizeOfQueue[k] = (k + 1.0) - deliveredPackets;
      // calculate average queue service
      averageUseOfQueue[k] = busyTime / times->at(k);
    } else {
      averageUseOfQueue[k] = 0;
      averageSizeOfQueue[k] = 0;
    }
    
    cout << averageSizeOfQueue[k] << " " << averageUseOfQueue[k] <<  "\n";
    resultFile << averageSizeOfQueue[k] << " " << averageUseOfQueue[k] <<  "\n";
  }
  
  gnuplot << "set terminal jpeg";
  gnuplot << "set format y \"%f\"";
  gnuplot << "set format x \"%f\"";
  gnuplot << "set output \"" + RESULT_IMAGE + "\"";
  gnuplot << "plot \"" + RESULT_FILE + "\" using 1:2 title \"Tamanho médio x Utilização\" with lines \n";
  
  resultFile.close();
  
  return 0;
}
