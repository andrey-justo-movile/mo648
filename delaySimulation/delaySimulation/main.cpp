//
//  main.cpp
//  delaySimulation
//
//  Created by Andrey Justo on 10/4/15.
//  Copyright © 2015 Andrey Justo. All rights reserved.
//

/*
 First Exercise:
 Description in Portuguese:
 Primeiro exercício: Fazer um programa de simulação e coletar dados para plotar um gráfico retardo médio x utilização. A utilização é a razão entre a taxa de chegada e a taxa de serviço do servidor. Assuma que o intervalo entre chegada de pacotes é exponencialmente distribuído, bem como o tamanho dos pacotes. Entregar relatório com gráfico, código fonte e explicação como derivou intervalo de confiança e qual foi o critério adotado para eliminar o transiente da simulação.
 */

#include <iostream>
#include <fstream>
#include <math.h>
#include <stdio.h>
#include "gnuplot_i.hpp"


const std::string RESULT_FILE = "result.dat";
const std::string RESULT_IMAGE = "result.jpeg";

void withInputs(double &lambda, double &x, double &lambdaWork, double &xWork) {
  
  std::cout << "Digite um valor para Lambda:";
  
  std::cin >> lambda;
  
  std::cout << "Digite um valor inicial para x:";
  
  std::cin >> x;
  
  std::cout << "Digite um valor inicial para o trabalho médio de cada client:";
  
  std::cin >> lambdaWork;
  
  std::cout << "Digite um valor inicial para o lambda do trabalho médio de cada client:";
  
  std::cin >> xWork;
  
}

double generateX() {
  return rand();
}

double distExpNumber(double lambda, double x) {
  if (x < 0 || lambda <= 0) {
    return 0;
  }
  
  return (exp(-lambda*x));
}


double distExpNumberProb(double lambda, double x) {
  if (x < 0 || lambda <= 0) {
    return 0;
  }
  
  return lambda * (exp(-lambda*x));
}

double distExpNumberAc(double lambda, double x) {
  if (x < 0.0 || lambda < 0.0) {
    return 0;
  }
  
  return 1 - (exp(-lambda*x));
}

double calculateDelay(double timeline[], double works[], double &deliveredPackets, int index) {
  double currentDelay = timeline[0];
  double currentTime = timeline[index];
  
  for (int i = 0; i <= index; i++) {
    if (currentDelay < timeline[i]) {
      currentDelay = timeline[i] + works[i];
    } else {
      currentDelay += works[i];
    }
    
    if (currentTime > currentDelay) {
      deliveredPackets = i + 1;
    }
  }
  
  return currentDelay - currentTime;
}

int main(int argc, const char * argv[]) {
  
  int maxIterations = 100;
  
  double lambda = 0, x = 0, xWork = 0, lambdaWork = 0;
  double times[maxIterations], works[maxIterations];
  double averageDelay[maxIterations], delays[maxIterations];
  double averageUseOfQueue[maxIterations];
  
  // output file
  std::ofstream resultFile(RESULT_FILE);
  resultFile << "# X Y \n";
  Gnuplot gnuplot;
  gnuplot.set_terminal_std("jpeg");
  gnuplot.showonscreen();
  
  std::cout << "# Excercise one - MO 648" << "\n";
  std::cout.precision(30);
  
  if (argc < 4) {
    withInputs(lambda, x, lambdaWork, xWork);
  } else {
    lambda = atof(argv[1]);
    x = atof(argv[2]);
    lambdaWork = atof(argv[3]);
    xWork = atof(argv[4]);
  }
  
  std::cout << "# " << lambda << " - " << x << " - " << lambdaWork << " - " << xWork << "\n";
  
  double currentTime = distExpNumber(lambda, x);
  double currentWork = distExpNumber(lambdaWork, xWork);
  // generate exponencial packet time arrival and size of packets
  for (int i = 0; i < maxIterations; i++) {            
    if (i == 0) {
      times[i] = currentTime;
    } else {
      times[i] = times[i - 1] + currentTime;
    }
    
    works[i] = currentWork;
    xWork = currentWork;
    x = currentTime;
    
    currentTime = distExpNumberProb(lambda, x);
    currentWork = distExpNumberProb(lambdaWork, xWork);
    std::cout << "TIMES " << times[i] << " " << works[i] <<  "\n";
  }
  // calculate averages
  double busyTime = 0.0;
  double deliveredPackets = 0.0;
  
  for (int i = 0; i < maxIterations; i++) {  
    busyTime = times[i] - times[0];
    
    // calculate average queue service
    averageUseOfQueue[i] = busyTime / times[i];
    
    // calculate average delay for each packet that arrives to the queue
    delays[i] = calculateDelay(times, works, deliveredPackets, i);
    
    double sumOfDelays = 0.0;
    for (int j = 0; j <= i; j++) {
      sumOfDelays += delays[j];
    }
    
    if (i != 0) {
      averageDelay[i] = sumOfDelays / (i + 1);
    } else {
      averageDelay[i] = sumOfDelays;
    }
    
    std::cout << averageDelay[i] << " " << averageUseOfQueue[i] <<  "\n";
    resultFile << averageDelay[i] << " " << averageUseOfQueue[i] <<  "\n";
  }

  gnuplot << "set terminal jpeg";
  gnuplot << "set format y \"%f\"";
  gnuplot << "set format x \"%f\"";
  gnuplot << "set output \"" + RESULT_IMAGE + "\"";
  gnuplot << "plot \"" + RESULT_FILE + "\" using 1:2 title \"Retardo médio x Utilização\" with lines \n";
  
  resultFile.close();
  
  return 0;
}