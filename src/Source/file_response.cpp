#include "file_response.hpp"

FileResponse::FileResponse(const std::string& filename) {

  // Получаем информацию о файле
  if (stat(filename.c_str(), &file_stat) != 0) {
    str_error = "Ошибка при получении информации о файле: " + filename;
    flag_error = false;
    return;
  }

  file_buffer.resize(file_stat.st_size);

  std::ifstream file(filename, std::ios::binary);
    
  if (!file) {
    str_error = "Не удалось открыть файл: " + filename;
    flag_error = false;
    return;
  }
   
  // Читаем содержимое файла в строку
  if (!file.read(&file_buffer[0], file_stat.st_size)) {
    str_error = "Ошибка при чтении файла: " + filename;
    flag_error = false;
  }

  file.close();
}