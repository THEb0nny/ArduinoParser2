#include <SoftwareSerial.h>
#include <StringUtils.h> // Библиотека работа со строками

#define BTN_LED_PIN 13

bool btnLedState = false;
 
void setup()
{
  Serial.begin(115200); // Открываем последовательный порт
  Serial.setTimeout(5); // Позволяет задать время ожидания данных
  pinMode(BTN_LED_PIN, OUTPUT); // Настраиваем пин светодиода кнопки на вывод
  Serial.println();
}
 
void loop()
{
  SerialInputPrecessing(true); // Считываем по Serial значения
}

// Парсинг значений из Serial и установка значений
void SerialInputPrecessing(bool debug) {
  if (Serial.available() > 2) { // Если что-то прислали
    String inputString = Serial.readString();
    inputString.trim(); // Удаление лишние символы с начала и конца строки
    inputString.replace(" ", ""); // Удалить пробел между ключем и значением
    Text myStr(inputString);
    // Serial.println("InputString: " + myStr.toString());

    TextParser outerTextParser(myStr, ';'); // Внешний разделитель строки
    while (outerTextParser.parse()) {
      // Serial.println(outerTextParser);
      TextParser innerTextParser(outerTextParser, ':'); // Внутренний разделитель строки
      Text key, value;
      while (innerTextParser.parse()) {
        if (innerTextParser.index() == 0) key = innerTextParser.get();
        else value = innerTextParser.get();
      }
      // Serial.println(key + ": " + value);
      // Что делать если найдены нужные ключи
      if (key == "led") {
        btnLedState = value.toInt();
        digitalWrite(BTN_LED_PIN, btnLedState);
      } else { // Какое-то другое сообщение просто выводим
        Serial.println(inputString);
      }
    }
    // Serial.println();
  }
}