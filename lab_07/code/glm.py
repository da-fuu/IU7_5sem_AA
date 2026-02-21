import os
import re
import sys

from PyPDF2 import PdfReader


def check_structure_errors(pdf_path):
    """
    Ищет подразделы внутри ВВЕДЕНИЯ/ЗАКЛЮЧЕНИЯ.
    Возвращает координаты строки с подразделом (X.X).
    """
    # Регулярное выражение с захватом строки подраздела в группу 1
    regex_pattern = (
        r"(?sm)^\s*(?i:ВВЕДЕНИЕ|ЗАКЛЮЧЕНИЕ)\s*$.*?(^\s*\d+\.\d+\s*[А-ЯЁA-Z].*)$"
    )

    found_errors = []

    if not os.path.exists(pdf_path):
        return (False, [])

    try:
        reader = PdfReader(pdf_path)
        for page_num, page in enumerate(reader.pages, start=1):
            text = page.extract_text()
            if not text:
                continue

            # Используем finditer для нахождения всех ошибочных подразделов на странице
            for match in re.finditer(regex_pattern, text):
                # Текст самого подраздела (группа 1)
                error_line_text = match.group(1).strip()

                # Позиция начала подраздела в тексте страницы
                start_pos_of_error = match.start(1)

                # Вычисляем номер строки, где находится подраздел
                line_num = text[:start_pos_of_error].count("\n") + 1

                found_errors.append((error_line_text, (page_num, line_num)))

    except Exception as e:
        print(f"Ошибка при чтении PDF: {e}")
        return (False, [])

    return (len(found_errors) > 0, found_errors)


def main():
    if len(sys.argv) < 2:
        print("Запуск: python script.py <путь_к_файлу.pdf>")
        return

    file_path = sys.argv[1]
    is_error, details = check_structure_errors(file_path)

    print(f"Наличие ошибки: {is_error}")
    if is_error:
        print("Список ошибок (Строка подраздела, (Страница, Номер строки)):")
        for error in details:
            print(error)


if __name__ == "__main__":
    main()
