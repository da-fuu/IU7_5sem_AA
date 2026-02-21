# Обязательно в реализации
# - Цели НЕ менять
# - Зависимости и сценарии поменять на необходимые

TEX_SRCS := $(wildcard ./report/*.tex)

# Собрать на сервере И положить на проверку pdf отчёта
ready/report.pdf : report/report.pdf | ready
	cp $< $@

report/report.pdf : $(TEX_SRCS)
	make -C ./report


# Сборка и запуск модульных тестов прямо на сервере
ready/stud-unit-test-report.json : code/out/stud-unit-test-report.json | ready
	cp $< $@

code/out/stud-unit-test-report.json : 
	make -C ./code unit



ready :
	mkdir -p ./ready


# Очистка
.PHONY: clean
clean:
	make -C ./code clean
	make -C ./report clean
