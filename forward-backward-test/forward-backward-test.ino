// Программа реализует движение модели вперёд на 25см, 
// после чего останавливает модель на 2 секунды
// и далее запускает движение модели в течение 50см.

// Подключаем библиотеку для работы с сервоприводом модели
#include <Servo.h>

// Пин, к которому подключен сервопривод модели
#define SERVO_PIN       9
// Пин для управления скоростью мотора модели
#define PWM_MOTOR       6
// Пин, к которому подключен энкодер мотора
#define ENCODER_PIN     3
// Диаметр колеса модели в см
#define WHEEL_DIAMETER  10.0
// Количество импульсов на 1 оборот колеса
#define ENCODER_PULSES  20

// Расстояние пройденное моделью в см
float distance = 0.0,
// Формула для расчёта количества импульсов энкодера на 1 см
// pulses_cm = ENCODER_PULSES / (pi * WHEEL_DIAMETER), где:
// ENCODER_PULSES - количество импульсов энкодера на один оборот колеса
// pi - число пи
// WHEEL_DIAMETER - диаметр колеса модели
// pulses_cm - количество импульсов энкодера на 1 см проезда модели
  pulses_cm = (float)ENCODER_PULSES / ((float)3.14 * (float)WHEEL_DIAMETER);
// Скорость движения модели. Значение скорости модели может
// быть 0 до 255
int speed = 120,
// Количество импульсов энкодера
  encoder_pos = 0;
// Флаг, определяющий в первый раз выполняется ли код
bool start = true;
// Создаём экземпляр класса для управления сервоприводом
Servo myservo;

void setup() {
  // Указываем, к какому порту подключен сервопривод
  myservo.attach(SERVO_PIN);
  // Устанавливаем тип пина мотора, как выход.
  // То есть на этот пин необходимо подавать напряжения для работы мотора
  pinMode(PWM_MOTOR, OUTPUT); 
  // Устанавливаем, то что при срабатывании энкодера вызывалась функция upd_encoder
  // Устанавливаем тип пина энкодера, как вход
  // С этого пина необходимо считывать напряжение
  pinMode(ENCODER_PIN, INPUT);
  // Когда энкодер срабатывает, он отправляет напряжение на порт, к которому подключён
  // Чтобы ловить это напряжение, необходимо использовать аппаратное прерывание
  // Прерывание позволяет ловить изменение напряжение на порту и говорить об этом программе
  // Чтобы активировать прерывание, необходимо использовать функцию attachInterrupt
  attachInterrupt(1,                            // Номер прерывания
                  upd_encoder,                  // Функция, вызываемая прерыванием
                                                // То есть, когда напряжение на порту измениться он вызовет эту функцию
                                                // Функция должна быть без параметров и не возвращать значений
                  FALLING);                     // Прерывание вызывается только при смене значения на порту с HIGH на LOW
  // Более подробно об этой функции можете прочитать на http://arduino.ru/Reference/AttachInterrupt

  // Устанавливаем качалку сервопривода в центральное положение
  // То есть передние колёса модели стоят прямо
  myservo.write(90);
  // Запускаем движение модели со скоростью содержащийся в переменной speed
  analogWrite(PWM_MOTOR, speed);
}

void loop() {
  if(start) {
    // Ждём пока, машинка не проедет 25см
    while(distance < 25);
    // Останавливаем движение модели
    analogWrite(PWM_MOTOR, 0);
    // Ждём 2 секунду
    delay(2000);
    // Запускаем движение модели
    analogWrite(PWM_MOTOR, speed);

    // Ждём пока, машинка не проедет 75см
    while(distance < 75);
    // Останавливаем движение модели
    analogWrite(PWM_MOTOR, 0);
    // Ждём 2 секунду
    delay(2000);
    // Запускаем движение модели
    analogWrite(PWM_MOTOR, speed);

    start = false;
  }
}

void upd_encoder() {
  // Увеличиваем счётчик количества импульсов энкодера на единицу
  encoder_pos++;

  // Формула для расчёта пройденного расстояния в см
  // distance = encoder_pos / pulses_cm, где:
  // encoder_pos - количество импульсов энкодера, за время работы программы
  // pulses_cm - количество импульсов энкодера на 1 см проезда модели
  // distance - пройденное расстояние в см
  distance = (float)encoder_pos / pulses_cm;
}
