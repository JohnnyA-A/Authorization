#include <windows.h>
#include <ctime>
#include <io.h>
#include <fstream>
#include <iostream>
#include <string>
using namespace std;

int actionId = 0;                     // Id действия (0 - авторизация впервые, 1 - повторная авторизация, 2 - превышен лимит авторизации)
int numberAuth = 5;                   // Количество максимальных авторизаций
int curr_numberAuth = 4;              // Количество оставшихся авторизаций
string path;                          // Путь к файлу с зарегистрированными именами
const string fileName = "auth-base";  //Имя файла с зарегистрированными именами
void getPath();
void autorization();
int nameVerification(string&);
bool DirIsExist(const TCHAR*);
void footer();

int main() {
  cout << '\n'<< "Autorization v1.0.\n\n";
  getPath();
  autorization();
  footer();
  return 0;
}

// Путь к файлу с зарегистрированными именами
void getPath() {
  char *userName = getenv("username");// Получение имени пользователя
  DWORD size = 256;
  path = string("C:\\Users\\") + string(userName) + string("\\AppData\\Roaming\\");
}

// Функция афторизации
void autorization() {
  cout << "Enter your name: ";
  string enterName;                    // Вводимое имя
  getline(cin, enterName);             // Ввод имени
  if (enterName.empty()){
    cout << "Please, enter not empty name\n";
    return;
  }
  actionId = nameVerification(enterName);         // Проверка на лимит авторизации
  if (actionId == 0)  // Успешная авторизация
    cout << "\nAuthorization was successful\n"
         << "Hello " << enterName << "!\n"
         << "Remaining authorizations: " << curr_numberAuth << "\n";
  else if (actionId == 1) // Имя уже было
    cout << "This name already authirizated\n";
  else 
    cout << "\nThe authorization limit has been exceeded\n"
         << "You can buy the premium version for $300\n";
}

// Функция проверки имени на лимит авторизации
int nameVerification(string& enterName) {
  string readName[100];  // Имя считываемое из файла с зарегистированными именами
  fstream authNames;
  if (access((path + fileName).c_str(), 0) == 1){  // создаём файл, если его нет
    authNames.open(path + fileName, ios::out);
    authNames.close();
  }
  int i = 0; // номер строки в файле
  authNames.open(path + fileName, ios::binary | ios::in | ios::out | ios::app);  // Файл с зарегистрированными именами
  getline(authNames, readName[0]);
  while (!readName[i].empty()){  // читаем файл до конца
    
    if (readName[i] == enterName){
      authNames.close();  // если имя уже есть, заканчиваем
      return 1;
    }
    i++;
    getline(authNames, readName[i]);
  }
  
  if (i < numberAuth){  // если количество строк меньше максимального вводим новое имя
      authNames.clear();
      authNames << enterName <<'\n';
      curr_numberAuth = 4 - i;
      authNames.close();
      return 0;
  }
  else{
    authNames.close();
    return 2;
  }

  return 0;
}

// Футер программы
void footer() {
  cout << '\n'
       << "The program is completed\n\n";
  system("PAUSE");
  exit(0);
}