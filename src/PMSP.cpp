#include "../include/PMSP.h"

Pmsp::Pmsp(std::string fileName, int algorithm) {
  switch (algorithm)
  {
  case 0:
    algorithm_ = std::make_shared<Greedy>();
    break;
  
  default:
    throw std::string("No algorithm given");
    break;
  }
  z_ = 0;
  loadData(fileName);
}

Pmsp::~Pmsp() {
  //delete algorithm_;
}

void Pmsp::loadData(std::string fileName) {
  std::ifstream file;
  file.open(fileName);
  if (!file.is_open()) {
    throw std::string("Cannot open file\n");
  }
  std::string aux, inputString;
  file >> aux >> nTasks;
  file >> aux >> mMachines;
  S.resize(mMachines);
  file >> aux; //Lectura de Pi:U[1-99]
  int tmpNumber;
  for (int i = 0; i < nTasks; i++) {
    file >> tmpNumber;
    Task task(i + 1, tmpNumber);
    taskVector.push_back(task);
  }
    
  setupTime.resize(nTasks + 1);
  for(int i = 0; i < nTasks + 1; i++) {
    setupTime[i].resize(nTasks + 1);
  }
  file >> aux;
  for(int i = 0; i <= nTasks; i++) {
    for(int j = 0; j <= nTasks; j++) {
      file >> tmpNumber;
      setupTime[i][j] = tmpNumber;
      //std::cout << setupTime[i][j] << " ";
    }
    //std::cout << std::endl;
  }
}

std::vector<Machine>& Pmsp::getS() {
  return S;
}

std::vector<Task>& Pmsp::getTasks() {
  return taskVector;
}

std::vector<std::vector<int>> Pmsp::getSetupTime() {
  return setupTime;
}

bool Pmsp::allVisited() {
  for (int i = 0; i < taskVector.size(); i++) {
    if (taskVector[i].getVisited() == false) {
      return false;
    }
  }
  return true;
}

int Pmsp::getM() {
  return mMachines;
}

void Pmsp::computeSolution() {
  S = algorithm_->computePmspSolution(*this);
}

void Pmsp::printSolution() {
  std::cout << "Los resultados son: \n"; 
  for (int i = 0; i < S.size(); i++) {
    std::cout << "Maquina nº" << i + 1 << ": {";
    for (int j = 0; j < S[i].getTasks().size(); j++) {
      std::cout << S[i].getTasks().at(j).getId() << " ";
    }
    std::cout << "}\n";
  }
  std::cout << "El valor de z es: " << getZ() << "\n";
}

int Pmsp::getZ() {
  for (int i = 0; i < S.size(); i++) {
    z_ += S[i].getTct();
  }
  return z_;
}