#include <iostream>
#include <sstream>
#include <vector>
#include <nlohmann/json.hpp>
#include <cpr/cpr.h>

using namespace std;

const string API_URL{"https://ftapi.pythonanywhere.com"};

string translate(string lang, string text)
{
  cpr::Response response = cpr::Get(cpr::Url{API_URL + "/translate"},
                                    cpr::Parameters{{"dl", lang}, {"text", text}});

  if (response.status_code != 200)
    return "Ошибка при переводе. Вы указали неверный ключ языка.";

  // Парсинг JSON-строки
  nlohmann::json jsonData = nlohmann::json::parse(response.text);
  return jsonData["destination-text"];
}

void printLangs()
{
  cpr::Response response = cpr::Get(cpr::Url{API_URL + "/languages"});
  // Парсинг JSON-строки
  nlohmann::json jsonData = nlohmann::json::parse(response.text);

  // Проход по всем ключам и значениям
  for (auto &[key, value] : jsonData.items())
  {
    string valueTranslated = translate("ru", value.get<string>());
    std::cout << "Ключ: " << key << ", Язык: " << valueTranslated << std::endl;
  }
}

int main(int argc, char *argv[])
{
  std::string langValue, text;

  // Проходим по всем аргументам командной строки
  for (int i = 1; i < argc; ++i)
  {
    std::string arg = argv[i];

    // Проверяем, является ли аргумент --lang=<VALUE>
    if (arg.find("--lang=") == 0)
    {
      // Извлекаем значение
      langValue = arg.substr(7); // 7 - длина строки "--lang="
      break;
    }
    else if (arg.find("--list") == 0)
    {
      printLangs();
      return 0;
    }
    else if (arg.find("--help") == 0)
    {
      cout << "Пример использования:\n"
           << "--lang=<VALUE> - ключ языка перевода (для списка возможных ключей языков используйте команду --list)\n"
           << "--list - выводит список всех доступных ключей языков\n"
           << "--help - выводит это сообщение\n";
      return 0;
    }
  }

  if (langValue.empty())
  {
    cout << "Введите ключ языка перевода (для списка возможных ключей языков используйте команду --list)\nКлюч: ";
    getline(cin, langValue);
  }
  cout << "Введите текст для перевода: ";
  getline(cin, text);
  cout << "\nПеревод: " << translate(langValue, text) << endl;
  return 0;
}