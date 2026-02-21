import os
import re
import sys

from PyPDF2 import PdfReader


def check_pdf_structure_errors(file_path):
    """
    Проверяет PDF на наличие подразделов в разделах ВВЕДЕНИЕ и ЗАКЛЮЧЕНИЕ.
    Учитывает регистр заголовков и проверяет, что предполагаемый подраздел
    действительно является заголовком (начинается с заглавной буквы).
    """

    # (?mi) - флаги (multiline + case insensitive)
    # ^\s*(?:ВВЕДЕНИЕ|ЗАКЛЮЧЕНИЕ)\s*$ - строка, содержащая только заголовок раздела
    # [\s\S]*? - любой текст далее
    # ^\s*\d+\.\d+\.?\s+[А-ЯЁA-Z] - новая строка, начинающаяся с "Число.Число",
    # за которым обязательно следует Заглавная буква (признак заголовка).
    regex_pattern = (
        r"(?mi)^\s*(?:ВВЕДЕНИЕ|ЗАКЛЮЧЕНИЕ)\s*$[\s\S]*?^\s*\d+\.\d+\.?\s+[А-ЯЁA-Z]"
    )

    errors = []

    try:
        reader = PdfReader(file_path)
        for page_idx, page in enumerate(reader.pages, start=1):
            text = page.extract_text()
            if not text:
                continue

            # Поиск вхождения
            match = re.search(regex_pattern, text)

            if match:
                # Нам нужно найти именно ту часть, которая вызвала ошибку (подраздел)
                # match.group() вернет огромный кусок текста от ВВЕДЕНИЯ до ошибки.
                # Поэтому мы найдем последнюю строку в этом совпадении, которая похожа на подраздел.

                full_match = match.group()

                # Ищем внутри совпадения конкретную строку с ошибкой (1.1 ...)
                # Это нужно для красивого вывода, так как match захватит весь текст от заголовка
                error_pattern = r"^\s*\d+\.\d+\.?\s+[А-ЯЁA-Z]"
                sub_matches = list(re.finditer(error_pattern, full_match, re.MULTILINE))

                if sub_matches:
                    # Берем последнее совпадение (оно и стриггерило конец regex)
                    last_sub_match = sub_matches[-1]
                    matched_str = last_sub_match.group().strip()

                    # Вычисляем глобальное смещение для точного определения строки
                    global_start = match.start() + last_sub_match.start()
                    line_number = text[:global_start].count("\n") + 1

                    errors.append((matched_str, (page_idx, line_number)))

    except Exception as e:
        print(f"Ошибка при обработке: {e}")
        return (False, [])

    return (len(errors) > 0, errors)


# --- Блок запуска для проверки ---
if __name__ == "__main__":
    print("=== Проверка структуры PDF ===")
    if len(sys.argv) > 1:
        fpath = sys.argv[1]
    else:
        fpath = input("Введите путь к файлу: ").strip().strip('"')

    if os.path.exists(fpath):
        is_error, details = check_pdf_structure_errors(fpath)
        if is_error:
            print("\n[!] НАЙДЕНЫ ОШИБКИ:")
            for txt, (pg, ln) in details:
                print(
                    f"   Стр {pg}, строка {ln}: найдет подраздел '{txt}' в запрещенной секции."
                )
        else:
            print("\n[OK] Ошибок не найдено.")
    else:
        print("Файл не найден.")
