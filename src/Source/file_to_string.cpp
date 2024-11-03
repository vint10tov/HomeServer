#include "file_to_string.hpp"

FileToString::FileToString(const std::string& filename) {

  // Получаем информацию о файле
  if (stat(filename.c_str(), &file_stat) != 0) {
    flag_error = false;
    Logger::error_log("FileToString: Ошибка при получении информации о файле: " + filename);
    return;
  }

  file_buffer.resize(file_stat.st_size);

  std::ifstream file(filename, std::ios::binary);
    
  if (!file) {
    flag_error = false;
    Logger::error_log("FileToString: Не удалось открыть файл: " + filename);
    return;
  }
   
  // Читаем содержимое файла в строку
  if (!file.read(&file_buffer[0], file_stat.st_size)) {
    Logger::error_log("FileToString: Ошибка при чтении файла: " + filename);
    flag_error = false;
  }
  
  file.close();
}