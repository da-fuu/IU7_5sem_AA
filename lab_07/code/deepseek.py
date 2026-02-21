import os
import re
import sys

from PyPDF2 import PdfReader


def check_pdf_structure(file_path):
    """
    Ищет подразделы в разделах ВВЕДЕНИЕ/ЗАКЛЮЧЕНИЕ.
    Проверяет, что после числа и пробела идет строго заглавная буква.
    """
    # (?m) - многострочный режим
    # ^\s*(?i:ВВЕДЕНИЕ|ЗАКЛЮЧЕНИЕ)\s*$ - заголовок (регистр не важен)
    # [\s\S]*? - любой текст между
    # ^\s*\d+\.\d+ - число.число в начале строки
    # [^\S\r\n]+ - только горизонтальные пробелы (не перенос строки!)
    # [А-ЯЁA-Z] - строго заглавная буква
    pattern = re.compile(
        r"(?m)^\s*(?i:ВВЕДЕНИЕ|ЗАКЛЮЧЕНИЕ)\s*$[\s\S]*?^\s*\d+\.\d+[^\S\r\n]+[А-ЯЁA-Z]"
    )

    results = []

    try:
        reader = PdfReader(file_path)
        for page_index, page in enumerate(reader.pages):
            text = page.extract_text()
            if not text:
                continue

            for match in pattern.finditer(text):
                full_match_text = match.group(0).strip()

                # Находим позицию начала подразделения внутри найденного куска текста
                # Чтобы точно указать строку, где встретилось "1.1 Название"
                error_match = re.search(
                    r"^\s*\d+\.\d+[^\S\r\n]+[А-ЯЁA-Z]", full_match_text, re.MULTILINE
                )

                if error_match:
                    start_pos_in_page = match.start() + error_match.start()
                    line_number = text[:start_pos_in_page].count("\n") + 1
                    error_line = error_match.group(0).strip()

                    results.append((error_line, (page_index + 1, line_number)))

    except Exception as e:
        return False, f"Ошибка при обработке файла: {e}"

    return len(results) > 0, results


def main():
    if len(sys.argv) < 2:
        path = input("Введите путь к PDF-файлу: ").strip('"')
    else:
        path = sys.argv[1]

    if not os.path.exists(path):
        print(f"Файл '{path}' не найден.")
        return

    is_invalid, details = check_pdf_structure(path)

    if isinstance(details, str):
        print(details)
        return

    if is_invalid:
        print(
            f"\n[!] ОБНАРУЖЕНО НАРУШЕНИЕ: В Введении или Заключении найдены подразделы."
        )
        for error_text, coords in details:
            print(f"Стр {coords[0]}, Строка {coords[1]}: Найдено '{error_text}'")
    else:
        print(
            "\n[OK] Ошибок не обнаружено. Числа внутри предложений (например, '1.2 раза') проигнорированы."
        )


if __name__ == "__main__":
    main()
