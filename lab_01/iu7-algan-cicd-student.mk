# Обязательно в реализации
# - Цели НЕ менять
# - Зависимости и сценарии поменять на необходимые


# Собрать на сервере И положить на проверку pdf отчёта
ready/report.pdf:
	mkdir -p ./ready
	$(RM) ./report/report.pdf
	cd ./report && make
	cp ./report/report.pdf $@


# Сборка и запуск модульных тестов прямо на сервере
ready/stud-unit-test-report.json:
	mkdir -p ./ready
	cd ./code && make unit
	cp ./code/out/stud-unit-test-report.json $@

# Очистка
.PHONY: clean
clean:
	cd ./code && make clean
